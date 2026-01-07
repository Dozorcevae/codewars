#pragma once
#include <vector>
#include <queue>
#include <limits>
#include <algorithm>
#include <fstream>
#include <type_traits>
#include <string>

// АТД "Граф" (шаблон по типу веса ребра W)
template <class W>
class Graph final {
    static_assert(std::is_arithmetic_v<W>, "Тип веса W должен быть числовым");

public:
    enum class Type { Directed = 1, Undirected = 2 };
    enum class Rep  { AdjList  = 1, AdjMatrix  = 2 }; // по варианту используем AdjList

    // Результат Task() (вариант 8)
    struct TaskResult {
        bool ok = false;          // 1 - найден радиус/путь, 0 - радиус не определен
        W radius{};               // радиус R
        int center = -1;          // центр (вершина с минимальным входящим эксцентриситетом)
        int from = -1;            // вершина-источник пути радиуса
        std::vector<int> path;    // путь: from -> ... -> center
    };

    // Конструктор пустого графа для заданных числа вершин, типа и формы представления
    Graph(int vertices, Type type, Rep rep)
        : n_(vertices), type_(type), rep_(rep), adj_(static_cast<size_t>(vertices)) {
        if (n_ < 0) n_ = 0;
        // По варианту 8: взвешенный орграф + L-граф
        // Но конструктор "по МУ" принимает параметры — поэтому просто проверяем, что выбрано правильно
        if (type_ != Type::Directed || rep_ != Rep::AdjList) {
            // Можно было бы поддерживать и другие режимы, но по варианту это не требуется
            throw std::runtime_error("Для варианта 8 требуется: Directed + L(AdjList)");
        }
    }

    // V() - опрос числа вершин
    int V() const noexcept { return n_; }

    // E() - опрос числа ребер
    int E() const noexcept { return m_; }

    // Edge(v1,v2) - опрос наличия ребра
    bool Edge(int v1, int v2) const {
        checkVertex_(v1); checkVertex_(v2);
        return edgeIndex_(v1, v2) != -1;
    }

    // Insert(v1,v2) - вставка ребра (вес по умолчанию = 1)
    // Возврат: true если вставлено, false если ребро уже есть (параллельные запрещены)
    bool Insert(int v1, int v2) {
        checkVertex_(v1); checkVertex_(v2);
        if (v1 == v2) throw std::runtime_error("Петли запрещены");
        if (Edge(v1, v2)) return false;

        adj_[static_cast<size_t>(v1)].push_back(Arc{v2, static_cast<W>(1)});
        ++m_;
        return true;
    }

    // Delete(v1,v2) - удаление ребра
    bool Delete(int v1, int v2) {
        checkVertex_(v1); checkVertex_(v2);
        if (v1 == v2) throw std::runtime_error("Петли запрещены");

        auto &lst = adj_[static_cast<size_t>(v1)];
        for (size_t i = 0; i < lst.size(); ++i) {
            if (lst[i].to == v2) {
                lst.erase(lst.begin() + static_cast<long>(i));
                --m_;
                return true;
            }
        }
        return false;
    }

    // SetEdge(v1,v2,data) - задание параметров ребра (вес)
    bool SetEdge(int v1, int v2, W w) {
        checkVertex_(v1); checkVertex_(v2);
        if (v1 == v2) throw std::runtime_error("Петли запрещены");
        if (w < static_cast<W>(0)) throw std::runtime_error("Вес должен быть >= 0 (Дейкстра)");

        auto &lst = adj_[static_cast<size_t>(v1)];
        for (auto &e : lst) {
            if (e.to == v2) {
                e.w = w;
                return true;
            }
        }
        return false; // ребра нет
    }

    // Show() - вывод структуры графа на экран (L-граф)
    void Show(std::ostream &out) const {
        for (int v = 0; v < n_; ++v) {
            out << v << ":";
            for (const auto &e : adj_[static_cast<size_t>(v)]) {
                out << " (" << e.to << "," << e.w << ")";
            }
            out << "\n";
        }
    }

    // Task() - вариант 8:
    // Радиус = min_v max_u dist(u,v) (минимальный входящий эксцентриситет)
    // Путь = кратчайший путь from -> ... -> center, где dist(from,center) = радиусу
    TaskResult Task() const {
        using Dist = long double;
        const Dist INF = std::numeric_limits<Dist>::infinity();

        std::vector<Dist> ecc_in(static_cast<size_t>(n_), 0.0L);
        std::vector<bool> ok(static_cast<size_t>(n_), true);
        std::vector<int> far_source(static_cast<size_t>(n_), -1);

        // 1) Для каждого источника s запускаем Дейкстру и обновляем ecc_in[v]
        for (int s = 0; s < n_; ++s) {
            auto [dist, _parent] = dijkstra_(s);

            for (int v = 0; v < n_; ++v) {
                if (dist[static_cast<size_t>(v)] == INF) {
                    ok[static_cast<size_t>(v)] = false; // v недостижима хотя бы из одного s
                } else if (ok[static_cast<size_t>(v)] && dist[static_cast<size_t>(v)] > ecc_in[static_cast<size_t>(v)]) {
                    ecc_in[static_cast<size_t>(v)] = dist[static_cast<size_t>(v)];
                    far_source[static_cast<size_t>(v)] = s;
                }
            }
        }

        // 2) Находим center с минимальным ecc_in среди ok==true
        Dist best = INF;
        int center = -1;
        for (int v = 0; v < n_; ++v) {
            if (!ok[static_cast<size_t>(v)]) continue;
            if (ecc_in[static_cast<size_t>(v)] < best) {
                best = ecc_in[static_cast<size_t>(v)];
                center = v;
            }
        }

        TaskResult res;
        if (center == -1) {
            res.ok = false; // радиус не определен (нет вершины, достижимой из всех остальных)
            return res;
        }

        res.ok = true;
        res.center = center;
        res.from = far_source[static_cast<size_t>(center)];
        res.radius = static_cast<W>(best);

        // 3) Восстанавливаем путь from -> ... -> center (по parent из Дейкстры)
        auto [dist2, parent2] = dijkstra_(res.from);
        if (dist2[static_cast<size_t>(center)] == INF) {
            // теоретически не должно случиться, но на всякий случай
            res.ok = false;
            res.center = -1;
            res.from = -1;
            res.path.clear();
            return res;
        }

        std::vector<int> rev;
        for (int v = center; v != -1; v = parent2[static_cast<size_t>(v)]) {
            rev.push_back(v);
            if (v == res.from) break;
        }
        std::reverse(rev.begin(), rev.end());
        res.path = std::move(rev);

        return res;
    }

    // Сохранить DOT (для визуализации)
    // Если передан результат Task(), путь подсвечивается
    bool SaveDot(const std::string &filename, const TaskResult *highlight = nullptr) const {
        std::ofstream f(filename);
        if (!f) return false;

        // собираем ребра пути для подсветки
        std::vector<std::pair<int,int>> hi;
        if (highlight && highlight->ok && highlight->path.size() >= 2) {
            for (size_t i = 1; i < highlight->path.size(); ++i) {
                hi.push_back({highlight->path[i-1], highlight->path[i]});
            }
        }

        auto isHi = [&](int a, int b) -> bool {
            for (auto &p : hi) if (p.first == a && p.second == b) return true;
            return false;
        };

        f << "digraph G {\n";
        f << "  rankdir=LR;\n";
        f << "  node [shape=circle];\n";
        for (int v = 0; v < n_; ++v) f << "  " << v << ";\n";

        for (int v = 0; v < n_; ++v) {
            for (const auto &e : adj_[static_cast<size_t>(v)]) {
                f << "  " << v << " -> " << e.to << " [label=\"" << e.w << "\"";
                if (isHi(v, e.to)) f << ", color=red, penwidth=2";
                f << "];\n";
            }
        }
        f << "}\n";
        return true;
    }

private:
    struct Arc { int to; W w; };

    int n_ = 0;   // вершины
    int m_ = 0;   // ребра
    Type type_;
    Rep rep_;
    std::vector<std::vector<Arc>> adj_;

    void checkVertex_(int v) const {
        if (v < 0 || v >= n_) throw std::runtime_error("Неверный номер вершины");
    }

    int edgeIndex_(int v1, int v2) const {
        const auto &lst = adj_[static_cast<size_t>(v1)];
        for (size_t i = 0; i < lst.size(); ++i) {
            if (lst[i].to == v2) return static_cast<int>(i);
        }
        return -1;
    }

    // Дейкстра от источника s (неотрицательные веса)
    std::pair<std::vector<long double>, std::vector<int>> dijkstra_(int s) const {
        using Dist = long double;
        const Dist INF = std::numeric_limits<Dist>::infinity();

        std::vector<Dist> dist(static_cast<size_t>(n_), INF);
        std::vector<int> parent(static_cast<size_t>(n_), -1);

        struct Node {
            Dist d;
            int v;
            bool operator>(const Node &o) const { return d > o.d; }
        };
        std::priority_queue<Node, std::vector<Node>, std::greater<Node>> pq;

        dist[static_cast<size_t>(s)] = 0.0L;
        pq.push(Node{0.0L, s});

        while (!pq.empty()) {
            Node cur = pq.top(); pq.pop();
            if (cur.d != dist[static_cast<size_t>(cur.v)]) continue;

            for (const auto &e : adj_[static_cast<size_t>(cur.v)]) {
                Dist w = static_cast<Dist>(e.w);
                Dist nd = cur.d + w;
                if (nd < dist[static_cast<size_t>(e.to)]) {
                    dist[static_cast<size_t>(e.to)] = nd;
                    parent[static_cast<size_t>(e.to)] = cur.v;
                    pq.push(Node{nd, e.to});
                }
            }
        }
        return {std::move(dist), std::move(parent)};
    }
};
