#include "hash_table_oa.h"
#include <iostream>
#include <vector>
#include <numeric>
#include <iomanip>
#include <random>
#include <fstream>

using std::cout;
using std::endl;
using std::string;

// Экспериментальная оценка качества хеш-функции по критерию χ².
// По МУ: объем выборки N = 20*m (m — емкость таблицы), усреднить по нескольким экспериментам.

static double chi_square_for_counts(const std::vector<size_t>& cnt)
{
    const size_t m = cnt.size();
    const double N = static_cast<double>(std::accumulate(cnt.begin(), cnt.end(), size_t{0}));
    const double E = N / static_cast<double>(m);

    double chi2 = 0.0;
    for (size_t i = 0; i < m; ++i)
    {
        const double diff = static_cast<double>(cnt[i]) - E;
        chi2 += (diff * diff) / E;
    }
    return chi2;
}

int main()
{
    // max_items=500 => m=1000, N = 20*m = 20000
    const size_t max_items = 500; // значит m=1000 (в твоей реализации m = 2 * max_items)
    HashTableOA<string, int, CyrillicUpperHorner> ht(max_items);

    const size_t m = ht.Capacity();
    const size_t N = 20 * m;

    const int experiments = 20;
    std::mt19937_64 rng(123456789ULL);

    // (опционально) CSV для графика χ² по экспериментам
    std::ofstream chi_csv("chi2_series.csv");
    chi_csv << "exp,chi2\n";

    double sum = 0.0;

    for (int e = 1; e <= experiments; ++e)
    {
        std::vector<size_t> cnt(m, 0);

        for (size_t i = 0; i < N; ++i)
        {
            // Ключи: заглавные кириллицы, длина 5..25
            const string k = CyrillicUpperHorner::gen_key(rng, 5, 25);
            const size_t h = ht.HashIndexForKey(k);
            cnt[h]++;
        }

        const double chi2 = chi_square_for_counts(cnt);
        sum += chi2;

        cout << "Эксперимент " << e << ": chi^2 = " << std::fixed << std::setprecision(2) << chi2 << endl;
        chi_csv << e << "," << std::fixed << std::setprecision(6) << chi2 << "\n";

        // (опционально) сохранить распределение f_i для 1-го эксперимента (для столбчатого графика)
        if (e == 1)
        {
            std::ofstream dist("dist_e1.csv");
            dist << "i,fi\n";
            for (size_t i = 0; i < m; ++i)
                dist << i << "," << cnt[i] << "\n";
        }
    }

    const double avg = sum / experiments;
    cout << "\nСреднее chi^2 по " << experiments << " экспериментам: "
         << std::fixed << std::setprecision(2) << avg << endl;

    return 0;
}
