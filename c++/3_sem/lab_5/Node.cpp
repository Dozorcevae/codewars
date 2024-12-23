
#include "Node.hpp"

// Класс для управления односвязным списком
class LinkedList {
private:
    // Внутренний узел списка
    struct ListNode {
        Node* data; // Указатель на узел данных
        ListNode* next; // Указатель на следующий элемент списка

        // Конструктор внутреннего узла
        ListNode(Node* node) : data(node), next(nullptr) {}
    };

    ListNode* head; // Указатель на голову списка

public:
    // Конструктор инициализирует пустой список
    LinkedList() : head(nullptr) {}

    // Деструктор удаляет все узлы списка
    ~LinkedList();

    // Добавление узла в конец списка
    void addNode(Node* node);

    // Удаление узла по ID
    void removeNode(int id);

    // Поиск узла по ID
    Node* findNode(int id) const;

    // Вставка узла на заданную позицию
    void insertNode(int position, Node* node);

    // Отображение всех узлов списка
    void displayList() const;
};
