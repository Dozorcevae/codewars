#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include "Graph.h"

static int fails = 0;

#define CHECK_TRUE(expr) do { \
    if (!(expr)) { \
        std::cout << "[FAIL] " #expr "\n"; \
        ++fails; \
    } else { \
        std::cout << "[ OK ] " #expr "\n"; \
    } \
} while(0)

#define CHECK_EQ(a,b) do { \
    auto _a = (a); \
    auto _b = (b); \
    if (!((_a) == (_b))) { \
        std::cout << "[FAIL] " #a " == " #b " (got " << _a << ", expected " << _b << ")\n"; \
        ++fails; \
    } else { \
        std::cout << "[ OK ] " #a " == " #b "\n"; \
    } \
} while(0)

#define CHECK_THROWS(stmt) do { \
    bool thrown = false; \
    try { (stmt); } catch (...) { thrown = true; } \
    if (!thrown) { \
        std::cout << "[FAIL] expected throw: " #stmt "\n"; \
        ++fails; \
    } else { \
        std::cout << "[ OK ] throws: " #stmt "\n"; \
    } \
} while(0)

int main() {
    using W = int;

    std::cout << "=== AUTOTEST: Variant 8 (Directed, L-graph, Dijkstra radius) ===\n";

    // --- Тест 1: базовая инициализация ---
    Graph<W> g(4, Graph<W>::Type::Directed, Graph<W>::Rep::AdjList);
    CHECK_EQ(g.V(), 4);
    CHECK_EQ(g.E(), 0);

    // --- Тест 2: Insert / Edge / запрет параллельных / запрет петель ---
    CHECK_TRUE(g.Insert(1, 0) == true);
    CHECK_TRUE(g.Insert(2, 0) == true);
    CHECK_TRUE(g.Insert(3, 0) == true);
    CHECK_EQ(g.E(), 3);

    CHECK_TRUE(g.Edge(2, 0) == true);
    CHECK_TRUE(g.Edge(0, 2) == false);

    CHECK_TRUE(g.Insert(2, 0) == false); // параллельное запрещено
    CHECK_EQ(g.E(), 3);

    CHECK_THROWS(g.Insert(2, 2)); // петля запрещена
    CHECK_EQ(g.E(), 3);

    // --- Тест 3: SetEdge ---
    CHECK_TRUE(g.SetEdge(1, 0, 2) == true);
    CHECK_TRUE(g.SetEdge(2, 0, 5) == true);
    CHECK_TRUE(g.SetEdge(3, 0, 3) == true);

    CHECK_TRUE(g.SetEdge(0, 1, 7) == false); // ребра нет
    CHECK_THROWS(g.SetEdge(1, 0, -1));       // отрицательный вес запрещен

    // --- Тест 4: Task() (ожидаем R=5, center=0, путь 2->0) ---
    auto r = g.Task();
    CHECK_TRUE(r.ok == true);
    CHECK_EQ(r.radius, 5);
    CHECK_EQ(r.center, 0);
    CHECK_EQ(r.from, 2);

    CHECK_EQ(static_cast<int>(r.path.size()), 2);
    CHECK_EQ(r.path[0], 2);
    CHECK_EQ(r.path[1], 0);

    // --- Тест 5: SaveDot ---
    const std::string dotfile = "test_graph.dot";
    CHECK_TRUE(g.SaveDot(dotfile, &r) == true);

    std::ifstream fin(dotfile);
    CHECK_TRUE(fin.good() == true);
    fin.close();

    // --- Тест 6: Delete ---
    CHECK_TRUE(g.Delete(2, 0) == true);
    CHECK_TRUE(g.Edge(2, 0) == false);
    CHECK_EQ(g.E(), 2);

    CHECK_TRUE(g.Delete(2, 0) == false); // уже удалено

    // --- Тест 7: случай ok=0 ---
    Graph<W> g2(3, Graph<W>::Type::Directed, Graph<W>::Rep::AdjList);
    CHECK_TRUE(g2.Insert(0, 1) == true);
    auto r2 = g2.Task();
    CHECK_TRUE(r2.ok == false);

    std::cout << "\n=== RESULT ===\n";
    if (fails == 0) {
        std::cout << "ALL TESTS PASSED\n";
        return 0;
    }
    std::cout << "FAILED: " << fails << "\n";
    return 1;
}

