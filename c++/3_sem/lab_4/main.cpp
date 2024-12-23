#include "Student.hpp"
#include "Undergraduate.hpp"
#include "Postgraduate.hpp"
#include <iostream>
#include <vector>

int main() {
    // Создание объектов базового и производных классов
    Student base("Ivanov", 20, 4.5);
    Undergraduate undergrad("Petrov", 19, 3.8, "Computer Science");
    Postgraduate postgrad("Sidorov", 24, 4.2, "Artificial Intelligence");

    // Демонстрация полиморфизма через базовый указатель
    std::vector<Student*> students = {&base, &undergrad, &postgrad};

    for (const auto* student : students) {
        student->displayDetails();
        std::cout << "------------------\n";
    }

    // Демонстрация копирования объектов
    Undergraduate undergradCopy = undergrad;
    undergradCopy.setMajor("Mathematics");
    undergradCopy.displayDetails();

    return 0;
}
