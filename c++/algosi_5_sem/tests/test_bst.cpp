#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <random>
#include <cassert>
#include "/Users/elenadozorceva/codewars/c++/algosi_5_sem/include/bst.hpp"

static void REQUIRE(bool cond, const char* msg) {
    if (!cond) {
        std::cerr << "[FAIL] " << msg << "\n";
        std::exit(1);
    }
}

static void test_insert_find_update() {
    BST<int,int> t;

    REQUIRE(t.empty(), "tree should start empty");
    REQUIRE(t.insert(5, 50) == true, "insert new key should return true");
    REQUIRE(t.size() == 1, "size after first insert");

    int out = 0;
    REQUIRE(t.find(5, out) == true, "find existing key");
    REQUIRE(out == 50, "data matches");

    // update existing key
    REQUIRE(t.insert(5, 55) == false, "insert existing key updates and returns false");
    REQUIRE(t.size() == 1, "size unchanged after update");
    REQUIRE(t.find(5, out) == true && out == 55, "data updated");
}

static void test_erase_cases() {
    BST<int,int> t;

    // Build tree:
    //        8
    //      /   \
    //     3     10
    //    / \      \
    //   1   6      14
    //      / \     /
    //     4   7   13
    int keys[] = {8,3,10,1,6,14,4,7,13};
    for (int k: keys) t.insert(k, k*10);
    REQUIRE(t.size() == 9, "initial size 9");

    // 1) delete leaf: 7 is leaf
    REQUIRE(t.erase(7) == true, "erase leaf");
    REQUIRE(t.size() == 8, "size after erase leaf");
    int out=0;
    REQUIRE(t.find(7, out) == false, "deleted leaf not found");

    // 2) delete node with one child: 14 has left child 13 (after 7 deleted still true)
    REQUIRE(t.erase(14) == true, "erase node with one child");
    REQUIRE(t.size() == 7, "size after erase one-child");
    REQUIRE(t.find(14, out) == false, "14 not found");

    // 3) delete node with two children: 3 has two children (1 and 6)
    REQUIRE(t.erase(3) == true, "erase node with two children");
    REQUIRE(t.size() == 6, "size after erase two-children");
    REQUIRE(t.find(3, out) == false, "3 not found");
}

static void test_predecessor() {
    BST<int,int> t;
    for (int k: {10,5,15,2,7,12,20}) t.insert(k, k);

    int pk=0, pd=0;
    REQUIRE(t.predecessor(10, pk, pd) == true, "pred exists for 10");
    REQUIRE(pk == 7, "pred(10) == 7");

    REQUIRE(t.predecessor(2, pk, pd) == false, "pred for min key should not exist");
    REQUIRE(t.predecessor(6, pk, pd) == true && pk == 5, "pred(6) == 5");
    REQUIRE(t.predecessor(100, pk, pd) == true && pk == 20, "pred(100) == max");
}

static void test_iterator_sorted() {
    BST<int,int> t;
    std::vector<int> v = {8,3,10,1,6,14,4,7,13};
    for (int k: v) t.insert(k, k);

    std::vector<int> iterKeys;
    for (auto it = t.begin(); it != t.end(); ++it)
        iterKeys.push_back(it.key());

    std::vector<int> expected = {1,3,4,6,7,8,10,13,14};
    REQUIRE(iterKeys == expected, "iterator should produce sorted keys");
}

static void test_postorder_visits_all() {
    BST<int,int> t;
    std::set<int> s;
    for (int k: {5,2,8,1,3,7,9}) { t.insert(k,k); s.insert(k); }

    std::multiset<int> visited;
    t.traverse_postorder([&](const int& k, const int&) {
        visited.insert(k);
    });

    REQUIRE(visited.size() == s.size(), "postorder visited count");
    for (int k: s) REQUIRE(visited.count(k) == 1, "postorder visits each key once");
}

// Strong test: compare with std::map (random operations)
static void test_random_against_map() {
    std::mt19937 rng(12345);
    std::uniform_int_distribution<int> keyDist(0, 200);
    std::uniform_int_distribution<int> opDist(0, 2);

    BST<int,int> t;
    std::map<int,int> m;

    for (int i = 0; i < 2000; ++i) {
        int op = opDist(rng);
        int k = keyDist(rng);
        int d = k * 3 + 1;

        if (op == 0) {
            bool a = t.insert(k, d);
            auto [it, inserted] = m.insert({k, d});
            if (!inserted) it->second = d; // our policy: update
            REQUIRE(a == inserted, "insert return matches map inserted");
        } else if (op == 1) {
            bool a = t.erase(k);
            std::size_t erased = m.erase(k);
            REQUIRE(a == (erased == 1), "erase return matches map");
        } else {
            int out = 0;
            bool a = t.find(k, out);
            auto it = m.find(k);
            bool b = (it != m.end());
            REQUIRE(a == b, "find presence matches map");
            if (a) REQUIRE(out == it->second, "find data matches map");
        }

        REQUIRE(t.size() == m.size(), "size matches map");
    }

    // compare iterator order with map order
    std::vector<int> tKeys;
    for (auto it = t.begin(); it != t.end(); ++it) tKeys.push_back(it.key());

    std::vector<int> mKeys;
    for (auto& kv : m) mKeys.push_back(kv.first);

    REQUIRE(tKeys == mKeys, "iterator order matches map order");
}

int main() {
    test_insert_find_update();
    test_erase_cases();
    test_predecessor();
    test_iterator_sorted();
    test_postorder_visits_all();
    test_random_against_map();

    std::cout << "[OK] All tests passed.\n";
    return 0;
}
