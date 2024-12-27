#include "Student.hpp"

// Конструктор по умолчанию
Student::Student() : surname(nullptr), age(0), avg_grade(0.0f) {}

// Конструктор с параметрами
Student::Student(const char* s, int a, float avg) : age(a), avg_grade(avg) {
    surname = new char[strlen(s) + 1];
    strcpy(surname, s);
}

// Конструктор копирования
Student::Student(const Student& other) : age(other.age), avg_grade(other.avg_grade) {
    surname = new char[strlen(other.surname) + 1];
    strcpy(surname, other.surname);
}

// Виртуальный деструктор
Student::~Student() {
    delete[] surname;
}

// Установка данных студента
void Student::setStudent(const char* s, int a, float avg) {
    delete[] surname;
    // TODO: !!!!!!!!!!!! check nullptr meaning
    if (s && strlen(s)>0) {
        surname = new char[strlen(s)+1];
        strcpy(surname,s);
    }else {
        surname = new char[1];
        surname[0] = '\0';
    }
    age = a;
    avg_grade = avg;
}

// Виртуальный метод для отображения данных студента
void Student::displayDetails() const {
    std::cout << "Surname: " << (surname ? surname : "No surname") << "\n";
    std::cout << "Age: " << age << "\n";
    std::cout << "Average Grade: " << avg_grade << std::endl;
}

// Оператор вывода данных о студенте
std::ostream& operator<<(std::ostream& os, const Student& student) {
    os << "Surname: " << (student.surname ? student.surname : "No surname")
       << "\nAge: " << student.age
       << "\nAverage Grade: " << student.avg_grade;
    return os;
}

// Оператор ввода данных о студенте
std::istream& operator>>(std::istream& is, Student& student) {
    char buffer[256];
    std::cout << "Enter surname: ";
    is.ignore();
    is.getline(buffer, 256);
    student.setStudent(buffer, student.age, student.avg_grade);

    std::cout << "Enter age: ";
    is >> student.age;

    std::cout << "Enter average grade: ";
    is >> student.avg_grade;

    return is;
}
