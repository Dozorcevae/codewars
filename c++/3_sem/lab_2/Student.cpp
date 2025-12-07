
#include "Student.hpp"
#include <iostream>
#include <cstring>

//конструктор по умолчанию
Student::Student() : surname(nullptr), age(0), avg_grade(0.0) {}

//конструктор с параметрами
Student::Student(const char* s, int a, float avg){
    surname = new char[strlen(s) + 1];
    strcpy(surname, s);
    age = a;
    avg_grade = avg;
}

//конструктор копирования
Student::Student(const Student& other){
    surname = new char(strlen(other.surname) + 1);
    strcpy(surname, other.surname);
    age = other.age;
    avg_grade = other.avg_grade;
}

//деструктор
Student::~Student(){
    delete[] surname;
}

// метод для изменения фамилии
void Student::SetSurname(const char *s){
    delete [] surname;               //освобождаем память для того чтобы записать новую фамилию
    surname = new char[strlen(s) + 1];
    strcpy(surname, s);
}

//метод для измененя возраста
void Student::SetAge(const int a){
    age = a;
}

//метод для изменения среднего балла
void Student::SetAvgGrade(const float avg){
    avg_grade = avg;
}

//метод вывода на экран
void Student::display() const{
    std::cout << "Surname : " << (surname ? surname : "No surname") << std::endl; //проверка: если surname не содержит данных, выведет "No surname"
    std::cout << "Age : " << age << std::endl;
    std::cout << "Average grade : " << avg_grade << std::endl;
    std::cout<<"-----------------------------------------------"<<std::endl;
}

//перегрузка оператора сложения
Student Student::operator+(const Student& other){
    return Student(surname, age, (avg_grade + other.avg_grade)/2);
}

//перегрузка оператора присваивания
Student&Student::operator=(const Student &other){
    if (this == &other) return *this; //защита от самоприсваивания
    
    delete[] surname; //освобождаем память чтобы поменять значение фамилии
    surname = new char[strlen(other.surname) + 1];
    strcpy(surname, other.surname);
    
    age = other.age;
    
    avg_grade = other.avg_grade;
    
    return *this;
}

//префиксный инкремент
Student& Student::operator++(){
    ++age;
    return *this;
}

//постфиксный инкремент
Student Student::operator++(int){
    Student cpy(*this); //Создает временный объект cpy, копируя текущий объект (*this).
    age++;
    return cpy;
}

//преведение к типу  float
Student::operator float() const{
    return avg_grade;
}

//Дружественная функция вычитания
Student operator-(const Student& a, const Student& b){
    
    return Student(a.surname, a.age - b.age, a.avg_grade - b.avg_grade);
}
