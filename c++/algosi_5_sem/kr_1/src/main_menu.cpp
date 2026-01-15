#include <iostream>
#include "/Users/elenadozorceva/codewars/c++/algosi_5_sem/kr_1/include/bst.hpp"

// функция для show(): печать с отступами underscore
static void emitKey(const int& key, int level) {
    for (int i = 0; i < level; ++i) std::cout << "_";
    std::cout << key << "\n";
}

int main() {
    BST<int,int> tree;

    while (true) {
        std::cout
            << "\n1  Insert(k d)\n"
            << "2  Delete(k)\n"
            << "3  Find(k)\n"
            << "4  Size()\n"
            << "5  Empty()\n"
            << "6  Clear()\n"
            << "7  Traverse Lt->Rt->t (print keys)\n"
            << "8  Predecessor(k)\n"
            << "9  Show()\n"
            << "10 Iterate (keys min->max)\n"
            << "0  Exit\n"
            << "> ";

        int cmd;
        if (!(std::cin >> cmd)) return 0;
        if (cmd == 0) break;

        switch (cmd) {
            case 1: {
                int k, d; std::cin >> k >> d;
                std::cout << tree.insert(k, d);
                break;
            }
            case 2: {
                int k; std::cin >> k;
                std::cout << tree.erase(k);
                break;
            }
            case 3: {
                int k; std::cin >> k;
                int d = 0;
                bool ok = tree.find(k, d);
                std::cout << ok;
                if (ok) std::cout << " " << d;
                break;
            }
            case 4: std::cout << tree.size(); break;
            case 5: std::cout << tree.empty(); break;
            case 6: tree.clear(); std::cout << tree.size(); break;
            case 7: {
                tree.traverse_postorder([](const int& k, const int&){ std::cout << k << " "; });
                std::cout << "\n";
                break;
            }
            case 8: {
                int k; std::cin >> k;
                int pk, pd;
                bool ok = tree.predecessor(k, pk, pd);
                std::cout << ok;
                if (ok) std::cout << " " << pk << " " << pd;
                break;
            }
            case 9: tree.show(emitKey); break;
            case 10: {
                for (auto it = tree.begin(); it != tree.end(); ++it)
                    std::cout << it.key() << " ";
                std::cout << "\n";
                break;
            }
            default: break;
        }
    }
}
