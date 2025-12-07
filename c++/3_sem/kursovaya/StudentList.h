#ifndef STUDENTLIST_H
#define STUDENTLIST_H

#include "Student.h"

// Класс Node представляет узел двусвязного списка
class Node {
public:
    Student data; // Данные студента
    Node* next;   // Указатель на следующий узел
    Node* prev;   // Указатель на предыдущий узел

    // Конструктор узла с инициализацией данных
    Node(const Student& s);
};

// Класс StudentList реализует двусвязный список студентов
class StudentList {
private:
    Node* head; // Указатель на первый элемент списка
    Node* tail; // Указатель на последний элемент списка
    int size;   // Текущий размер списка

public:
    // Конструктор и деструктор
    StudentList();
    ~StudentList();

    // Методы управления списком
    void clear();
    void add(const Student& s);
    bool insert(int index, const Student& s);
    bool remove(int index);
    Student* extract(int index) const;
    void sortByLastName();
    void search(const char* pattern, int criterion) const;
    void saveToFile(const char* filename) const;
    void loadFromFile(const char* filename);
    void displayAll() const;
    int getSize() const;
};

#endif // STUDENTLIST_H