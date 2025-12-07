#ifndef CIRCULAR_LIST_HPP
#define CIRCULAR_LIST_HPP

#include <iostream>
#include <stdexcept>

// Узел односвязного списка
// Класс, представляющий отдельный элемент списка, содержащий данные и указатель на следующий элемент
template<typename T>
class Node {
public:
    T data; // Значение элемента списка
    Node* next; // Указатель на следующий элемент списка

    // Конструктор узла
    Node(T value, Node* nextNode = nullptr) : data(value), next(nextNode) {}
};

// Односвязный список с сохранением упорядоченности
// Класс, реализующий односвязный список с методами для упорядоченной вставки и других операций
template<typename T>
class OrderedSinglyLinkedList {
private:
    Node<T>* head; // Указатель на первый элемент списка
    int size; // Количество элементов в списке

    // Метод для проверки порядка элементов (по умолчанию сравнение <)
    // Может быть переопределен для сложных типов данных
    bool compare(const T& a, const T& b) const {
        return a < b;
    }

public:
    // Конструктор: создает пустой список
    OrderedSinglyLinkedList() : head(nullptr), size(0) {}

    // Деструктор: очищает список
    ~OrderedSinglyLinkedList() { clear(); }

    // Вставка элемента с сохранением упорядоченности
    void insertOrdered(T value);

    // Вставка элемента по заданному индексу
    void insertAt(int index, T value);

    // Удаление элемента по индексу
    void remove(int index);

    // Получение элемента по индексу
    T get(int index) const;

    // Вывод всех элементов списка
    void display() const;

    // Очистка списка
    void clear();

    // Поиск индекса элемента по его значению
    int find(const T& value) const;
};

// Вставка с сохранением упорядоченности
// Добавляет элемент в нужное место списка так, чтобы порядок не нарушался
template<typename T>
void OrderedSinglyLinkedList<T>::insertOrdered(T value) {
    Node<T>* newNode = new Node<T>(value);

    // Если список пуст или новый элемент меньше первого, вставляем в начало
    if (!head || compare(value, head->data)) {
        newNode->next = head;
        head = newNode;
    } else {
        // Иначе ищем позицию для вставки
        Node<T>* current = head;
        while (current->next && !compare(value, current->next->data)) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    ++size; // Увеличиваем размер списка
}

// Вставка по индексу
// Добавляет элемент на заданную позицию в списке
template<typename T>
void OrderedSinglyLinkedList<T>::insertAt(int index, T value) {
    if (index < 0 || index > size) throw std::out_of_range("Index out of range");
    Node<T>* newNode = new Node<T>(value);

    // Если индекс 0, вставляем в начало
    if (index == 0) {
        newNode->next = head;
        head = newNode;
    } else {
        // Иначе проходим до нужной позиции
        Node<T>* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
    }
    ++size; // Увеличиваем размер списка
}

// Удаление по индексу
// Удаляет элемент на указанной позиции в списке
template<typename T>
void OrderedSinglyLinkedList<T>::remove(int index) {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");

    Node<T>* toDelete;
    // Если индекс 0, удаляем первый элемент
    if (index == 0) {
        toDelete = head;
        head = head->next;
    } else {
        // Иначе проходим до нужной позиции
        Node<T>* current = head;
        for (int i = 0; i < index - 1; ++i) {
            current = current->next;
        }
        toDelete = current->next;
        current->next = toDelete->next;
    }
    delete toDelete; // Освобождаем память
    --size; // Уменьшаем размер списка
}

// Получение элемента по индексу
// Возвращает значение элемента на указанной позиции
template<typename T>
T OrderedSinglyLinkedList<T>::get(int index) const {
    if (index < 0 || index >= size) throw std::out_of_range("Index out of range");

    Node<T>* current = head;
    for (int i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

// Вывод списка
// Отображает все элементы списка в порядке их хранения
template<typename T>
void OrderedSinglyLinkedList<T>::display() const {
    Node<T>* current = head;
    while (current) {
        std::cout << current->data << " ";
        current = current->next;
    }
    std::cout << std::endl;
}

// Очистка списка
// Удаляет все элементы из списка
template<typename T>
void OrderedSinglyLinkedList<T>::clear() {
    while (head) {
        Node<T>* toDelete = head;
        head = head->next;
        delete toDelete;
    }
    size = 0;
}

// Поиск элемента
// Возвращает индекс первого элемента с заданным значением или -1, если не найден
template<typename T>
int OrderedSinglyLinkedList<T>::find(const T& value) const {
    Node<T>* current = head;
    int index = 0;
    while (current) {
        if (current->data == value) return index;
        current = current->next;
        ++index;
    }
    return -1;
}

#endif // CIRCULAR_LIST_HPP
