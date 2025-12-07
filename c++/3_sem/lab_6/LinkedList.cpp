
#include "LinkedList.hpp"

// Деструктор освобождает память всех узлов
LinkedList::~LinkedList() {
    ListNode* current = head;
    while (current) {
        ListNode* next = current->next;
        delete current->data; // Удаление данных узла
        delete current; // Удаление самого узла
        current = next;
    }
}

// Метод добавляет узел в конец списка
void LinkedList::addNode(Node* node) {
    if (!node) {
        throw ListException("Cannot add a null node.");
    }

    ListNode* newNode = new(std::nothrow) ListNode(node);
    if (!newNode) {
        throw std::runtime_error("Memory allocation failed.");
    }

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

// Метод удаляет узел по идентификатору
void LinkedList::removeNode(int id) {
    ListNode* current = head;
    ListNode* previous = nullptr;

    // Ищем узел с указанным идентификатором
    while (current && current->data->getId() != id) {
        previous = current;
        current = current->next;
    }

    if (!current) {
        throw ListException("Node with the given ID not found.");
    }

    if (previous) {
        previous->next = current->next; // Удаляем узел из середины или конца
    } else {
        head = current->next; // Удаляем голову списка
    }

    delete current->data; // Удаляем данные узла
    delete current; // Удаляем сам узел
}

// Метод ищет узел по идентификатору
Node* LinkedList::findNode(int id) const {
    ListNode* current = head;
    while (current) {
        if (current->data->getId() == id) {
            return current->data; // Возвращаем найденный узел
        }
        current = current->next;
    }
    throw ListException("Node not found.");
}

// Метод вставляет узел на заданную позицию
void LinkedList::insertNode(int position, Node* node) {
    if (position < 0) {
        throw ListException("Position cannot be negative.");
    }

    ListNode* newNode = new(std::nothrow) ListNode(node);
    if (!newNode) {
        throw std::runtime_error("Memory allocation failed.");
    }

    if (position == 0) {
        newNode->next = head;
        head = newNode; // Новый узел становится головой
        return;
    }

    ListNode* current = head;
    for (int i = 0; i < position - 1 && current; ++i) {
        current = current->next; // Ищем нужную позицию
    }

    if (!current) {
        delete newNode;
        throw ListException("Position out of bounds.");
    }

    newNode->next = current->next;
    current->next = newNode; // Вставляем новый узел
}

// Метод отображает все узлы списка
void LinkedList::displayList() const {
    ListNode* current = head;
    while (current) {
        current->data->display(); // Выводим данные узла
        current = current->next;
    }
}
