//
//  LinkedList.hpp
//  code
//
//  Created by Елена Дозорцева on 22.12.2024.
//

#ifndef LinkedList_hpp
#define LinkedList_hpp
#include "Node.hpp"

// Исключение для работы с LinkedList
class ListException : public std::exception {
private:
    std::string message;

public:
    // Конструктор, принимающий сообщение об ошибке
    explicit ListException(const std::string& msg) : message(msg) {}

    // Метод возвращает сообщение об ошибке
    const char* what() const noexcept override { return message.c_str(); }
};

// Класс LinkedList
class LinkedList {
private:
    struct ListNode {
        Node* data; // Указатель на данные узла
        ListNode* next; // Указатель на следующий узел

        // Конструктор узла списка
        ListNode(Node* node) : data(node), next(nullptr) {}
    };

    ListNode* head; // Указатель на голову списка

public:
    // Конструктор инициализирует пустой список
    LinkedList() : head(nullptr) {}

    // Деструктор удаляет все узлы списка
    ~LinkedList();

    // Метод для добавления узла в конец списка
    void addNode(Node* node);

    // Метод для удаления узла по идентификатору
    void removeNode(int id);

    // Метод для поиска узла по идентификатору
    Node* findNode(int id) const;

    // Метод для вставки узла на определённую позицию
    void insertNode(int position, Node* node);

    // Метод для отображения всех узлов списка
    void displayList() const;
};

#endif /* LinkedList_hpp */
