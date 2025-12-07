
#include "Node.hpp"
#include "LinkedList.hpp"
#include <iostream>

int main() {
    LinkedList list;

    // Добавление объектов различных типов
    list.addNode(new DerivedNode1(1, "Node A"));
    list.addNode(new DerivedNode2(2, 3.14));
    list.addNode(new DerivedNode1(3, "Node B"));

    // Демонстрация отображения
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
    Node* found = list.findNode(3);
    if (found) {
        std::cout << "Found node:" << std::endl;
        found->display();
    } else {
        std::cout << "Node not found." << std::endl;
    }

    return 0;
}
