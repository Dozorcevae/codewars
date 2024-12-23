
#ifndef Student_hpp
#define Student_hpp
#include <stdio.h>

class Student{
private:
    char* surname;
    int age;
    float avg_grade;
    
public:
    //Конструкторы и диструктор
    Student();
    Student(const char* s, int a, float avg);
    Student(const Student& other);
    ~Student();
    
    //методы для изменения данных
    void SetSurname(const char* s);
    void SetAge(const int a);
    void SetAvgGrade(const float avg);
    
    //метод для вывода данных
    void display() const;
    
    //перегрузка операций
    Student operator+(const Student& other);
    Student& operator=(const Student& other);
    Student& operator++();
    Student operator++(int);
    
    //приведение к типу
    operator float() const;
    
    //дружественная функция вычитания
    friend Student operator-(const Student& a, const Student& b);
};

#endif /* Student_hpp */
