
#include "Undergraduate.hpp"

// Конструктор по умолчанию
Undergraduate::Undergraduate() : Student(), major(nullptr) {}

// Конструктор с параметрами
Undergraduate::Undergraduate(const char* s, int a, float avg, const char* m) : Student(s, a, avg) {
    major = new char[strlen(m) + 1];
    strcpy(major, m);
}

// Конструктор копирования
Undergraduate::Undergraduate(const Undergraduate& other) : Student(other) {
    major = new char[strlen(other.major) + 1];
    strcpy(major, other.major);
}

// Деструктор
Undergraduate::~Undergraduate() {
    delete[] major;
}

// Установка специальности
void Undergraduate::setMajor(const char* m) {
    delete[] major;
    major = new char[strlen(m) + 1];
    strcpy(major, m);
}

// Переопределённый метод для отображения информации о бакалавре
void Undergraduate::displayDetails() const {
    Student::displayDetails();
    std::cout << "Major: " << (major ? major : "No major") << std::endl;
}
