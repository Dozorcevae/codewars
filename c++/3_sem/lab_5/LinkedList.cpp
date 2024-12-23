

#include "LinkedList.hpp"
// Деструктор: удаление всех узлов списка
LinkedList::~LinkedList() {
    ListNode* current = head;
    while (current) {
        ListNode* next = current->next;
        delete current->data; // Удаляем объект данных
        delete current; // Удаляем узел
        current = next;
    }
}

// Добавление узла в конец списка
void LinkedList::addNode(Node* node) {
    ListNode* newNode = new ListNode(node);
    if (!head) {
        head = newNode; // Если список пуст, новый узел становится головой
    } else {
        ListNode* current = head;
        while (current->next) {
            current = current->next; // Проходим до конца списка
        }
        current->next = newNode; // Добавляем новый узел
    }
}

// Удаление узла по идентификатору
void LinkedList::removeNode(int id) {
    ListNode* current = head;
    ListNode* previous = nullptr;

    // Поиск узла с заданным ID
    while (current && current->data->getId() != id) {
        previous = current;
        current = current->next;
    }

    // Если узел не найден
    if (!current) {
        std::cout << "Node with ID " << id << " not found." << std::endl;
        return;
    }

    // Удаление узла
    if (previous) {
        previous->next = current->next;
    } else {
        head = current->next; // Удаляем голову списка
    }

    delete current->data; // Удаляем данные
    delete current; // Удаляем узел
}

// Поиск узла по идентификатору
Node* LinkedList::findNode(int id) const {
    ListNode* current = head;
    while (current) {
        if (current->data->getId() == id) {
            return current->data; // Возвращаем найденный узел
        }
        current = current->next;
    }
    return nullptr; // Узел не найден
}

// Вставка узла на заданную позицию
void LinkedList::insertNode(int position, Node* node) {
    ListNode* newNode = new ListNode(node);
    if (position == 0) {
        newNode->next = head;
        head = newNode; // Новый узел становится головой
        return;
    }

    ListNode* current = head;
    for (int i = 0; i < position - 1 && current; ++i) {
        current = current->next; // Переход к нужной позиции
    }

    if (!current) {
        std::cout << "Position out of bounds." << std::endl;
        delete newNode;
        return;
    }

    newNode->next = current->next;
    current->next = newNode; // Вставляем узел
}

// Отображение всех узлов списка
void LinkedList::displayList() const {
    ListNode* current = head;
    while (current) {
        current->data->display(); // Вывод информации об узле
        current = current->next;
    }
}
