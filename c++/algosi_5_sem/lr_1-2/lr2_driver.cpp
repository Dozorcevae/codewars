#include "hash_table_oa.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <random>
#include <fstream>
#include <iomanip>
#include <cmath>

using std::cout;
using std::endl;
using std::string;

// ============================================================
// ЛР-2: тестирование трудоёмкости хеш-таблицы (открытая адресация)
// Вариант 8: ключи - заглавная кириллица, k->k' (Горнер),
// хеширование k' - мультипликативное, коллизии - линейное зондирование.
//
// Метрика трудоёмкости операций: число проб (зондирований),
// возвращаемое ht.CountProbe() после каждой операции.
//
// Драйвер измеряет среднее число проб для операций:
//   - Search (поиск)
//   - Insert (вставка)
//   - Delete (удаление)
// в зависимости от коэффициента заполнения alpha = n/m.
//
// Параметры (можно менять ниже):
//   max_items        - предельное число элементов (в конструкторе m=2*max_items)
//   ops_per_trial    - число операций в одном прогоне для заданного alpha
//   trials_per_alpha - число прогонов (усреднение)
//   miss_prob        - доля "промахов" (неуспешных операций) для каждого типа
//   alpha_values     - список alpha, для которых строится зависимость.
//
// Вывод:
//   - печать таблицы в консоль
//   - файл lr2_results.csv (разделитель ';')
// ============================================================

enum class OpType { Search = 0, Insert = 1, Delete = 2 };

struct OpStats
{
    long double sum_probes = 0.0L;
    std::uint64_t count = 0;

    long double sum_probes_hit = 0.0L;
    std::uint64_t count_hit = 0;

    long double sum_probes_miss = 0.0L;
    std::uint64_t count_miss = 0;

    void add(std::uint64_t probes, bool is_miss)
    {
        sum_probes += static_cast<long double>(probes);
        count++;
        if (is_miss)
        {
            sum_probes_miss += static_cast<long double>(probes);
            count_miss++;
        }
        else
        {
            sum_probes_hit += static_cast<long double>(probes);
            count_hit++;
        }
    }

    long double avg() const { return count ? (sum_probes / static_cast<long double>(count)) : 0.0L; }
    long double avg_hit() const { return count_hit ? (sum_probes_hit / static_cast<long double>(count_hit)) : 0.0L; }
    long double avg_miss() const { return count_miss ? (sum_probes_miss / static_cast<long double>(count_miss)) : 0.0L; }
};

struct ResultRow
{
    double alpha = 0.0;
    double avg_search = 0.0;
    double avg_insert = 0.0;
    double avg_delete = 0.0;

    double avg_search_hit = 0.0;
    double avg_search_miss = 0.0;

    double avg_insert_hit = 0.0;
    double avg_insert_miss = 0.0;

    double avg_delete_hit = 0.0;
    double avg_delete_miss = 0.0;

    std::size_t m = 0;
    std::size_t target_n = 0;
    double miss_prob = 0.0;
    std::uint64_t ops = 0;
    int trials = 0;
};

static string gen_absent_key(std::mt19937_64& rng,
                            const std::unordered_set<string>& present,
                            size_t min_len = 5,
                            size_t max_len = 25)
{
    for (;;)
    {
        string k = CyrillicUpperHorner::gen_key(rng, min_len, max_len);
        if (present.find(k) == present.end()) return k;
    }
}

static void fill_to_target(HashTableOA<string, int, CyrillicUpperHorner>& ht,
                           std::vector<string>& keys,
                           std::unordered_set<string>& present,
                           std::mt19937_64& rng,
                           std::size_t target_n)
{
    while (ht.Size() < target_n)
    {
        string k = gen_absent_key(rng, present);
        if (ht.Insert(k, 1))
        {
            keys.push_back(k);
            present.insert(k);
        }
    }
}

static void fix_size_back(HashTableOA<string, int, CyrillicUpperHorner>& ht,
                          std::vector<string>& keys,
                          std::unordered_set<string>& present,
                          std::mt19937_64& rng,
                          std::size_t target_n)
{
    // Доводим размер точно до target_n, НЕ учитывая эти операции в статистике.
    while (ht.Size() < target_n)
    {
        string k = gen_absent_key(rng, present);
        if (ht.Insert(k, 1))
        {
            keys.push_back(k);
            present.insert(k);
        }
    }
    while (ht.Size() > target_n && !keys.empty())
    {
        std::uniform_int_distribution<std::size_t> pick(0, keys.size() - 1);
        std::size_t idx = pick(rng);
        const string k = keys[idx];
        if (ht.Delete(k))
        {
            present.erase(k);
            keys[idx] = keys.back();
            keys.pop_back();
        }
    }
}

static ResultRow run_for_alpha(double alpha,
                               std::size_t max_items,
                               std::uint64_t ops_per_trial,
                               int trials_per_alpha,
                               double miss_prob,
                               std::uint64_t master_seed)
{
    // Накапливаем суммарные статистики по всем trials, потом делим.
    OpStats s_search, s_insert, s_delete;

    for (int t = 0; t < trials_per_alpha; ++t)
    {
        std::mt19937_64 rng(master_seed + static_cast<std::uint64_t>(t) * 0x9E3779B97F4A7C15ULL);

        HashTableOA<string, int, CyrillicUpperHorner> ht(max_items);
        const std::size_t m = ht.Capacity();
        const std::size_t target_n = static_cast<std::size_t>(std::llround(alpha * static_cast<double>(m)));

        std::vector<string> keys;
        keys.reserve(target_n + 16);
        std::unordered_set<string> present;
        present.reserve(target_n * 2 + 64);

        fill_to_target(ht, keys, present, rng, target_n);

        std::uniform_int_distribution<int> op_dist(0, 2); // Search/Insert/Delete равновероятно
        std::bernoulli_distribution miss_dist(miss_prob); // промахи

        for (std::uint64_t op = 0; op < ops_per_trial; ++op)
        {
            const OpType type = static_cast<OpType>(op_dist(rng));
            const bool is_miss = miss_dist(rng);

            if (type == OpType::Search)
            {
                if (is_miss)
                {
                    const string k = gen_absent_key(rng, present);
                    try { (void)ht.Search(k); }
                    catch (int) { /* ожидаемо */ }
                    s_search.add(ht.CountProbe(), true);
                }
                else
                {
                    if (keys.empty())
                        fill_to_target(ht, keys, present, rng, target_n);

                    std::uniform_int_distribution<std::size_t> pick(0, keys.size() - 1);
                    const string& k = keys[pick(rng)];
                    try { (void)ht.Search(k); }
                    catch (int) { /* на всякий */ }
                    s_search.add(ht.CountProbe(), false);
                }
            }
            else if (type == OpType::Insert)
            {
                if (is_miss)
                {
                    // неуспешная вставка: вставить уже существующий ключ
                    if (keys.empty())
                        fill_to_target(ht, keys, present, rng, target_n);

                    std::uniform_int_distribution<std::size_t> pick(0, keys.size() - 1);
                    const string& k = keys[pick(rng)];
                    (void)ht.Insert(k, 1); // false
                    s_insert.add(ht.CountProbe(), true);
                }
                else
                {
                    // успешная вставка: новый ключ
                    const string k = gen_absent_key(rng, present);
                    const bool ok = ht.Insert(k, 1);
                    s_insert.add(ht.CountProbe(), false);
                    if (ok)
                    {
                        keys.push_back(k);
                        present.insert(k);
                    }
                }
            }
            else // Delete
            {
                if (is_miss)
                {
                    // неуспешное удаление: удалить отсутствующий ключ
                    const string k = gen_absent_key(rng, present);
                    (void)ht.Delete(k); // false
                    s_delete.add(ht.CountProbe(), true);
                }
                else
                {
                    // успешное удаление: удалить существующий ключ
                    if (keys.empty())
                        fill_to_target(ht, keys, present, rng, target_n);

                    std::uniform_int_distribution<std::size_t> pick(0, keys.size() - 1);
                    const std::size_t idx = pick(rng);
                    const string k = keys[idx];
                    const bool ok = ht.Delete(k);
                    s_delete.add(ht.CountProbe(), false);
                    if (ok)
                    {
                        present.erase(k);
                        keys[idx] = keys.back();
                        keys.pop_back();
                    }
                }
            }
        }

        // По МУ: в начале и в конце теста alpha не должна "уплывать".
        // Возвращаем размер к target_n (эти операции НЕ считаем в статистике).
        fix_size_back(ht, keys, present, rng, target_n);
    }

    // m и target_n одинаковы во всех trials
    HashTableOA<string, int, CyrillicUpperHorner> tmp(max_items);
    const std::size_t m = tmp.Capacity();
    const std::size_t target_n = static_cast<std::size_t>(std::llround(alpha * static_cast<double>(m)));

    ResultRow row;
    row.alpha = alpha;

    row.avg_search = static_cast<double>(s_search.avg());
    row.avg_insert = static_cast<double>(s_insert.avg());
    row.avg_delete = static_cast<double>(s_delete.avg());

    row.avg_search_hit = static_cast<double>(s_search.avg_hit());
    row.avg_search_miss = static_cast<double>(s_search.avg_miss());

    row.avg_insert_hit = static_cast<double>(s_insert.avg_hit());
    row.avg_insert_miss = static_cast<double>(s_insert.avg_miss());

    row.avg_delete_hit = static_cast<double>(s_delete.avg_hit());
    row.avg_delete_miss = static_cast<double>(s_delete.avg_miss());

    row.m = m;
    row.target_n = target_n;
    row.miss_prob = miss_prob;
    row.ops = ops_per_trial;
    row.trials = trials_per_alpha;
    return row;
}

int main()
{
    // ---- Параметры тестирования ----
    const std::size_t max_items = 500;          // как в ЛР-1 (m=1000)
    const std::uint64_t ops_per_trial = 200000; // операций на один alpha
    const int trials_per_alpha = 10;            // прогонов для усреднения
    const double miss_prob = 0.50;              // доля промахов

    // Для открытой адресации обычно 0.1..0.9
    const std::vector<double> alpha_values = {
        0.10, 0.20, 0.30, 0.40, 0.50, 0.60, 0.70, 0.80, 0.90
    };

    const std::uint64_t master_seed = 123456789ULL;

    // ---- CSV ----
    std::ofstream csv("lr2_results.csv");
    csv << "alpha;m;target_n;ops;trials;miss_prob;"
           "avg_search;avg_insert;avg_delete;"
           "avg_search_hit;avg_search_miss;"
           "avg_insert_hit;avg_insert_miss;"
           "avg_delete_hit;avg_delete_miss\n";

    cout << "ЛР-2: Тестирование трудоёмкости хеш-таблицы (открытая адресация)\n";
    cout << "max_items=" << max_items
         << ", m=" << HashTableOA<string,int,CyrillicUpperHorner>(max_items).Capacity()
         << ", ops_per_trial=" << ops_per_trial
         << ", trials=" << trials_per_alpha
         << ", miss_prob=" << miss_prob << "\n\n";

    cout << std::left
         << std::setw(6) << "alpha"
         << std::setw(10) << "S"
         << std::setw(10) << "I"
         << std::setw(10) << "D"
         << "(ср. число проб)" << "\n";

    cout << std::fixed << std::setprecision(3);

    for (double alpha : alpha_values)
    {
        ResultRow row = run_for_alpha(alpha, max_items, ops_per_trial, trials_per_alpha, miss_prob, master_seed);

        // консоль
        cout << std::setw(6) << row.alpha
             << std::setw(10) << row.avg_search
             << std::setw(10) << row.avg_insert
             << std::setw(10) << row.avg_delete
             << "\n";

        // CSV (';' чтобы Numbers/Excel в русской локали не путались)
        csv << std::fixed << std::setprecision(2)
            << row.alpha << ';'
            << row.m << ';'
            << row.target_n << ';'
            << row.ops << ';'
            << row.trials << ';'
            << row.miss_prob << ';'
            << std::setprecision(6)
            << row.avg_search << ';'
            << row.avg_insert << ';'
            << row.avg_delete << ';'
            << row.avg_search_hit << ';'
            << row.avg_search_miss << ';'
            << row.avg_insert_hit << ';'
            << row.avg_insert_miss << ';'
            << row.avg_delete_hit << ';'
            << row.avg_delete_miss
            << "\n";
    }

    cout << "\nГотово. Результаты сохранены в lr2_results.csv (разделитель ';').\n";
    cout << "Его можно открыть в Numbers/Excel и построить графики: Y=S/I/D, X=alpha.\n";

    return 0;
}

