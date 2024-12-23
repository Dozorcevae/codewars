
#ifndef Student_hpp
#define Student_hpp

#include <iostream>
#include <cstring>

// Базовый класс Student, который представляет основную информацию о студенте
class Student {
protected:
    char* surname; // Фамилия студента
    int age;       // Возраст студента
    float avg_grade; // Средний балл студента

public:
    // Конструкторы и деструктор
    Student(); // Конструктор по умолчанию
    Student(const char* s, int a, float avg); // Конструктор с параметрами
    Student(const Student& other); // Конструктор копирования
    virtual ~Student(); // Виртуальный деструктор

    // Установка данных студента
    void setStudent(const char* s, int a, float avg);
    // Виртуальный метод для отображения данных студента
    virtual void displayDetails() const;

    // Дружественные операторы ввода/вывода
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    friend std::istream& operator>>(std::istream& is, Student& student);
};

#endif // STUDENT_HPP
