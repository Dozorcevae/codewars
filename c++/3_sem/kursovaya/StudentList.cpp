#include "StudentList.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

// Конструктор Node
Node::Node(const Student& s) : data(s), next(nullptr), prev(nullptr) {}

// Конструктор по умолчанию
StudentList::StudentList() : head(nullptr), tail(nullptr), size(0) {}

// Деструктор очищает список
StudentList::~StudentList() {
    clear();
}

// Метод для очистки списка
void StudentList::clear() {
    Node* current = head;
    while (current) {
        Node* tmp = current;
        current = current->next;
        delete tmp;
    }
    head = tail = nullptr;
    size = 0;
}

// Метод для добавления студента в конец списка
void StudentList::add(const Student& s) {
    Node* newNode = new (std::nothrow) Node(s);
    if (!newNode) {
        std::cerr << "Ошибка выделения памяти для нового узла.\n";
        return;
    }

    if (!head) { // Если список пуст
        head = tail = newNode;
    } else { // Добавляем в конец
        tail->next = newNode;
        newNode->prev = tail;
        tail = newNode;
    }
    size++;
}

// Метод для вставки студента по индексу
bool StudentList::insert(int index, const Student& s) {
    if (index < 0 || index > size) {
        std::cerr << "Индекс вне диапазона.\n";
        return false;
    }

    if (index == size) { // Вставка в конец
        add(s);
        return true;
    }

    Node* newNode = new (std::nothrow) Node(s);
    if (!newNode) {
        std::cerr << "Ошибка выделения памяти для нового узла.\n";
        return false;
    }

    if (index == 0) { // Вставка в начало
        newNode->next = head;
        head->prev = newNode;
        head = newNode;
    } else { // Вставка в середину
        Node* current = head;
        for (int i = 0; i < index; ++i)
            current = current->next;
        newNode->prev = current->prev;
        newNode->next = current;
        current->prev->next = newNode;
        current->prev = newNode;
    }
    size++;
    return true;
}

// Метод для удаления студента по индексу
bool StudentList::remove(int index) {
    if (index < 0 || index >= size) {
        std::cerr << "Индекс вне диапазона.\n";
        return false;
    }

    Node* toDelete;
    if (index == 0) { // Удаление первого элемента
        toDelete = head;
        head = head->next;
        if (head)
            head->prev = nullptr;
        else
            tail = nullptr;
    } else if (index == size - 1) { // Удаление последнего элемента
        toDelete = tail;
        tail = tail->prev;
        tail->next = nullptr;
    } else { // Удаление элемента из середины
        toDelete = head;
        for (int i = 0; i < index; ++i)
            toDelete = toDelete->next;
        toDelete->prev->next = toDelete->next;
        toDelete->next->prev = toDelete->prev;
    }
    delete toDelete;
    size--;
    return true;
}

// Метод для извлечения студента по индексу
Student* StudentList::extract(int index) const {
    if (index < 0 || index >= size) {
        std::cerr << "Индекс вне диапазона.\n";
        return nullptr;
    }
    Node* current = head;
    for (int i = 0; i < index; ++i)
        current = current->next;
    return &(current->data);
}

// Метод для сортировки списка по фамилии (метод вставки)
void StudentList::sortByLastName() {
    if (!head || !head->next)
        return;

    Node* sorted = nullptr; // Начало отсортированного списка

    Node* current = head;
    while (current) {
        Node* next = current->next;

        // Вставка текущего узла в отсортированный список
        if (!sorted || strcmp(current->data.lastName, sorted->data.lastName) < 0) {
            current->next = sorted;
            if (sorted)
                sorted->prev = current;
            current->prev = nullptr;
            sorted = current;
        } else {
            Node* temp = sorted;
            while (temp->next && strcmp(current->data.lastName, temp->next->data.lastName) > 0)
                temp = temp->next;
            current->next = temp->next;
            if (temp->next)
                temp->next->prev = current;
            temp->next = current;
            current->prev = temp;
        }

        current = next;
    }

    // Обновляем head и tail
    head = sorted;
    tail = head;
    while (tail->next)
        tail = tail->next;
}

// Метод для поиска студентов по различным критериям
void StudentList::search(const char* pattern, int criterion) const {
    // Критерии поиска:
    // 1 - Фамилия
    // 2 - Группа
    // 3 - Дата поступления
    // 4 - Дата отчисления

    Node* current = head;
    bool found = false;
    while (current) {
        bool match = false;
        switch (criterion) {
            case 1:
                if (strstr(current->data.lastName, pattern))
                    match = true;
                break;
            case 2:
                if (strstr(current->data.group, pattern))
                    match = true;
                break;
            case 3:
                if (strstr(current->data.enrollmentDate, pattern))
                    match = true;
                break;
            case 4:
                if (strstr(current->data.expulsionDate, pattern))
                    match = true;
                break;
            default:
                std::cerr << "Неверный критерий поиска.\n";
                return;
        }

        if (match) {
            current->data.display();
            std::cout << "--------------------------\n";
            found = true;
        }
        current = current->next;
    }

    if (!found)
        std::cout << "Совпадений не найдено.\n";
}

// Метод для сохранения списка в бинарный файл
void StudentList::saveToFile(const char* filename) const {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        std::cerr << "Ошибка открытия файла для записи.\n";
        return;
    }
    ofs.write(reinterpret_cast<const char*>(&size), sizeof(size));
    if (!ofs) {
        std::cerr << "Ошибка записи размера списка в файл.\n";
        ofs.close();
        return;
    }

    Node* current = head;
    while (current) {
        current->data.saveToFile(ofs);
        if (!ofs) {
            std::cerr << "Ошибка записи данных студента в файл.\n";
            ofs.close();
            return;
        }
        current = current->next;
    }
    ofs.close();
    std::cout << "Данные успешно сохранены в файл \"" << filename << "\".\n";
}

// Метод для загрузки списка из бинарного файла
void StudentList::loadFromFile(const char* filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "Ошибка открытия файла для чтения.\n";
        return;
    }

    int fileSize;
    ifs.read(reinterpret_cast<char*>(&fileSize), sizeof(fileSize));
    if (!ifs) {
        std::cerr << "Ошибка чтения размера списка из файла.\n";
        ifs.close();
        return;
    }

    clear(); // Очищаем текущий список

    for (int i = 0; i < fileSize; ++i) {
        Student s;
        s.loadFromFile(ifs);
        if (!ifs) {
            std::cerr << "Ошибка чтения данных студента из файла.\n";
            ifs.close();
            return;
        }
        add(s);
    }
    ifs.close();
    std::cout << "Данные успешно загружены из файла \"" << filename << "\".\n";
}

// Метод для вывода всех студентов в списке
void StudentList::displayAll() const {
    if (!head) {
        std::cout << "Список студентов пуст.\n";
        return;
    }

    Node* current = head;
    int index = 0;
    while (current) {
        std::cout << "Студент #" << index++ << ":\n";
        current->data.display();
        std::cout << "--------------------------\n";
        current = current->next;
    }
}

// Метод для получения текущего размера списка
int StudentList::getSize() const {
    return size;
}