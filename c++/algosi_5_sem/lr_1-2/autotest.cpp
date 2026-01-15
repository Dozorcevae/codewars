#include "hash_table_oa.h"
#include <iostream>
#include <unordered_map>
#include <cassert>

using std::string;

static void require(bool cond, const char* msg)
{
    if (!cond)
    {
        std::cerr << "[FAIL] " << msg << "\n";
        std::exit(1);
    }
}

int main()
{
    // Маленькая таблица (по МУ для демонстрации): max_items=5 => m=10
    HashTableOA<string, int, CyrillicUpperHorner> ht(5);
    require(ht.Capacity() == 10, "Capacity must be 10 for max_items=5");

    // 1) Некорректный ключ -> E_INVALID_KEY
    try {
        ht.Insert("ABC", 1);
        require(false, "Expected E_INVALID_KEY for latin key");
    } catch (int code) {
        require(code == E_INVALID_KEY, "Wrong error for invalid key");
    }

    // 2) Поиск отсутствующего -> E_NOT_FOUND
    try {
        ht.Search("ААААА");
        require(false, "Expected E_NOT_FOUND for missing key");
    } catch (int code) {
        require(code == E_NOT_FOUND, "Wrong error for missing key");
    }

    // 3) Найти коллизию по h(k) и проверить рост проб
    std::mt19937_64 rng(42);
    std::unordered_map<size_t, string> seen;
    string k1, k2;
    size_t h0 = 0;

    for (int tries = 0; tries < 200000; ++tries)
    {
        string k = CyrillicUpperHorner::gen_key(rng, 5, 25);
        size_t h = ht.HashIndexForKey(k);
        auto it = seen.find(h);
        if (it == seen.end())
        {
            seen.emplace(h, k);
        }
        else
        {
            k1 = it->second;
            k2 = k;
            h0 = h;
            break;
        }
    }

    require(!k1.empty() && !k2.empty(), "Failed to find collision (unexpected)");

    require(ht.Insert(k1, 10), "Insert k1 failed");
    const size_t idx1 = ht.LastIndex();
    require(ht.LastHash() == h0, "LastHash mismatch for k1");

    require(ht.Insert(k2, 20), "Insert k2 failed");
    const size_t probes2 = ht.CountProbe();
    require(probes2 >= 2, "Expected probes>=2 for collided insert");

    // 4) Удалить k1 и убедиться, что deleted-слот можно переиспользовать
    require(ht.Delete(k1), "Delete k1 failed");
    const size_t deleted_idx = ht.LastIndex();
    require(deleted_idx == idx1, "Deleted index should match inserted index of k1");

    // Находим еще один ключ с тем же h0
    string k3;
    for (int tries = 0; tries < 200000; ++tries)
    {
        string k = CyrillicUpperHorner::gen_key(rng, 5, 25);
        if (ht.HashIndexForKey(k) == h0 && k != k2)
        {
            k3 = k;
            break;
        }
    }
    require(!k3.empty(), "Failed to find k3 with same hash");

    require(ht.Insert(k3, 30), "Insert k3 failed");
    require(ht.LastIndex() == deleted_idx, "Expected reuse of deleted slot");

    // 5) Дубликат
    require(!ht.Insert(k2, 999), "Duplicate insert must be false");

    std::cout << "[OK] All autotests passed\n";
    return 0;
}
