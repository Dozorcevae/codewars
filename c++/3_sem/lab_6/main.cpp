#include "Node.hpp"
#include "LinkedList.hpp"
#include <iostream>

int main() {
    try {
        LinkedList list;

        // Добавление узлов
        list.addNode(new DerivedNode1(1, "Node A"));
        list.addNode(new DerivedNode2(2, 3.14));
        list.addNode(new DerivedNode1(3, "Node B"));

        std::cout << "Initial list:" << std::endl;
        list.displayList();

        // Вставка узла
        list.insertNode(1, new DerivedNode2(4, 2.71));
        std::cout << "After insertion:" << std::endl;
        list.displayList();

        // Удаление узла
        list.removeNode(2);
        std::cout << "After removal of ID 2:" << std::endl;
        list.displayList();

        // Поиск узла
        try {
            Node* found = list.findNode(5);
            if (found) {
                std::cout << "Found node:" << std::endl;
                found->display();
            }
        } catch (const ListException& ex) {
            std::cerr << "Error: " << ex.what() << std::endl;
        }

    } catch (const std::exception& ex) {
        std::cerr << "Unhandled exception: " << ex.what() << std::endl;
    }

    return 0;
}
