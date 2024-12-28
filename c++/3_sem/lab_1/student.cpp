#include "student.hpp"
#include <iostream>
#include <cstring>

// Конструктор по умолчанию
Student::Student() : surname(nullptr), age(0), avg_grade(0.0) {}

// Конструктор с параметрами
Student::Student(const char* s, int a, float avg) {
    surname = new char[strlen(s) + 1]; // выделяем память для фамилии
    strcpy(surname, s);  // копируем фамилию
    age = a;
    avg_grade = avg;
}

// Конструктор копирования
Student::Student(const Student& other) {
    surname = new char[strlen(other.surname) + 1];
    strcpy(surname, other.surname);
    age = other.age;
    avg_grade = other.avg_grade;
}

// Деструктор
Student::~Student() {
    delete[] surname;  // освобождаем память для фамилии
}

// Метод для изменения фамилии
void Student::setSurname(const char* s) {
    delete[] surname;  // освобождаем старую память
    surname = new char[strlen(s) + 1];
    strcpy(surname, s);
}

// Метод для изменения возраста
void Student::setAge(int a) {
    age = a;
}

// Метод для изменения среднего балла
void Student::setAvgGrade(float avg) {
    avg_grade = avg;
}

// Метод для вывода данных студента
void Student::display() const {
    std::cout << "Surname: " << (surname ? surname : "No surname") << std::endl;
    std::cout << "Age: " << age << std::endl;
    std::cout << "Average Grade: " << avg_grade << std::endl;
}

// Перегрузка оператора сложения (средний балл)
Student Student::operator+(const Student& other) {
    return Student(surname, age, (avg_grade + other.avg_grade) / 2);
}
