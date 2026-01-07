#include <iostream>
#include <string>
#include "Graph.h"

static void printMenu() {
    std::cout << "\nМЕНЮ:\n";
    std::cout << "1) V()                 - число вершин\n";
    std::cout << "2) E()                 - число ребер\n";
    std::cout << "3) Insert(v1,v2)        - вставить ребро v1->v2 (вес=1)\n";
    std::cout << "4) Delete(v1,v2)        - удалить ребро v1->v2\n";
    std::cout << "5) Edge(v1,v2)          - есть ли ребро v1->v2\n";
    std::cout << "6) SetEdge(v1,v2,w)     - задать вес ребра v1->v2\n";
    std::cout << "7) Task()               - вариант 8 (радиус + путь)\n";
    std::cout << "8) Show()               - показать структуру L-графа\n";
    std::cout << "9) Save DOT (graph.dot) - сохранить DOT (подсветит путь Task)\n";
    std::cout << "0) Выход\n";
    std::cout << ">> ";
}

int main() {
    using W = int; // можешь поменять на double, если нужны дробные веса

    int V;
    std::cout << "Введите число вершин V = ";
    std::cin >> V;

    int typeIn;
    std::cout << "Тип графа (1 - ориентированный, 2 - неориентированный): ";
    std::cin >> typeIn;

    int repIn;
    std::cout << "Представление (1 - L(списки смежности), 2 - M(матрица)): ";
    std::cin >> repIn;

    try {
        auto type = static_cast<typename Graph<W>::Type>(typeIn);
        auto rep  = static_cast<typename Graph<W>::Rep>(repIn);

        Graph<W> g(V, type, rep);
        typename Graph<W>::TaskResult last; // запомним последний результат Task()

        while (true) {
            printMenu();
            int cmd;
            std::cin >> cmd;
            if (!std::cin) break;
            if (cmd == 0) break;

            try {
                if (cmd == 1) {
                    std::cout << g.V() << "\n";
                } else if (cmd == 2) {
                    std::cout << g.E() << "\n";
                } else if (cmd == 3) {
                    int v1, v2;
                    std::cout << "Введите v1 v2: ";
                    std::cin >> v1 >> v2;
                    std::cout << (g.Insert(v1, v2) ? 1 : 0) << "\n";
                } else if (cmd == 4) {
                    int v1, v2;
                    std::cout << "Введите v1 v2: ";
                    std::cin >> v1 >> v2;
                    std::cout << (g.Delete(v1, v2) ? 1 : 0) << "\n";
                } else if (cmd == 5) {
                    int v1, v2;
                    std::cout << "Введите v1 v2: ";
                    std::cin >> v1 >> v2;
                    std::cout << (g.Edge(v1, v2) ? 1 : 0) << "\n";
                } else if (cmd == 6) {
                    int v1, v2;
                    W w;
                    std::cout << "Введите v1 v2 w: ";
                    std::cin >> v1 >> v2 >> w;
                    std::cout << (g.SetEdge(v1, v2, w) ? 1 : 0) << "\n";
                } else if (cmd == 7) {
                    last = g.Task();
                    // вывод "сырой", чтобы удобно сверять:
                    std::cout << (last.ok ? 1 : 0) << "\n";
                    if (last.ok) {
                        std::cout << last.radius << "\n";
                        std::cout << last.center << "\n";
                        std::cout << last.from << "\n";
                        std::cout << last.path.size() << "\n";
                        for (size_t i = 0; i < last.path.size(); ++i) {
                            if (i) std::cout << " ";
                            std::cout << last.path[i];
                        }
                        std::cout << "\n";
                    }
                } else if (cmd == 8) {
                    g.Show(std::cout);
                } else if (cmd == 9) {
                    const std::string filename = "graph.dot";
                    const typename Graph<W>::TaskResult *p = (last.ok ? &last : nullptr);
                    std::cout << (g.SaveDot(filename, p) ? 1 : 0) << "\n";
                    std::cout << "Файл сохранен: " << filename << "\n";
                } else {
                    std::cout << "Неизвестная команда\n";
                }
            } catch (const std::exception &ex) {
                // ошибки операций АТД (не падаем)
                std::cout << "Ошибка: " << ex.what() << "\n";
            }
        }

    } catch (const std::exception &ex) {
        std::cout << "Ошибка инициализации: " << ex.what() << "\n";
        return 1;
    }

    return 0;
}
