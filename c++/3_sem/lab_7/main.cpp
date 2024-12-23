#include "CircularList.hpp"

int main() {
    // Создаем упорядоченный список
    OrderedSinglyLinkedList<int> list;

    // Пример вставки элементов с сохранением упорядоченности
    // Элементы вставляются в нужные позиции, чтобы сохранялся порядок
    list.insertOrdered(30);
    list.insertOrdered(10);
    list.insertOrdered(20);
    list.insertOrdered(40);

    std::cout << "List after ordered insertions: ";
    list.display();

    // Пример вставки элемента на определенную позицию
    // Вставляем 25 на позицию 2 (индексация начинается с 0)
    list.insertAt(2, 25);
    std::cout << "List after insertion at index 2: ";
    list.display();

    // Пример удаления элемента
    // Удаляем элемент на позиции 1
    list.remove(1);
    std::cout << "List after removing element at index 1: ";
    list.display();

    // Пример получения элемента по индексу
    // Получаем элемент на позиции 2
    std::cout << "Element at index 2: " << list.get(2) << std::endl;

    // Пример поиска элемента
    // Ищем индекс элемента со значением 40
    int index = list.find(40);
    if (index != -1) {
        std::cout << "Element 40 found at index: " << index << std::endl;
    } else {
        std::cout << "Element 40 not found" << std::endl;
    }

    // Пример очистки списка
    // Удаляем все элементы
    list.clear();
    std::cout << "List after clearing: ";
    list.display();

    return 0;
}
