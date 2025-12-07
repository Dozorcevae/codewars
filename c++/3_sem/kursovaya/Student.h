#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <fstream>

class Student {
public:
    // Поля класса
    char* lastName;                 // Фамилия
    char birthDate[11];            // Дата рождения (ДД.ММ.ГГГГ)
    char enrollmentDate[11];       // Дата поступления (ДД.ММ.ГГГГ)
    char expulsionDate[11];        // Дата отчисления (ДД.ММ.ГГГГ)
    char address[100];             // Адрес
    char group[10];                // Группа

    // Конструкторы и деструктор
    Student();
    Student(const char* ln, const char* bd, const char* ed, const char* exd, const char* addr, const char* grp);
    Student(const Student& s);
    Student& operator=(const Student& s);
    ~Student();

    // Методы
    void display() const;
    void saveToFile(std::ofstream& ofs) const;
    void loadFromFile(std::ifstream& ifs);
};

#endif // STUDENT_H