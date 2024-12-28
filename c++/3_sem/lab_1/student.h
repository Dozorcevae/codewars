#ifndef STUDENT_H
#define STUDENT_H

class Student {
private:
    char* surname;  // динамическая память для фамилии
    int age;
    float avg_grade;

public:
    // Конструктор по умолчанию
    Student();

    // Конструктор с параметрами
    Student(const char* s, int a, float avg);

    // Конструктор копирования
    Student(const Student& other);

    // Деструктор
    ~Student();

    // Методы для изменения данных
    void setSurname(const char* s);
    void setAge(int a);
    void setAvgGrade(float avg);

    // Метод для вывода данных
    void display() const;

    // Перегрузка оператора сложения
    Student operator+(const Student& other);
};

#endif // STUDENT_H
