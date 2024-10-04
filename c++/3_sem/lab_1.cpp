#include <iostream>
#include <cstring>

class Student {
private:
    char *surname;  // фамилия студента
    int age;        // возраст студента
    float avgGrade; // средний балл

    // Метод для копирования строки
    char *copyString(const char *str) {
        if (str == nullptr) {
            return nullptr;
        }
        char *newStr = new char[strlen(str) + 1];
        strcpy(newStr, str);
        return newStr;
    }

public:
    // Конструктор по умолчанию
    Student() : surname(nullptr), age(0), avgGrade(0.0f) {
    }

    // Конструктор с параметрами
    Student(const char *surname, int age, float avgGrade) 
        : age(age), avgGrade(avgGrade) {
        this->surname = copyString(surname);
    }

    // Конструктор копирования
    Student(const Student &other) {
        surname = copyString(other.surname);
        age = other.age;
        avgGrade = other.avgGrade;
    }

    // Оператор присваивания
    Student &operator=(const Student &other) {
        if (this == &other) {
            return *this;
        }

        // Освобождение старой памяти
        delete[] surname;

        // Копирование данных
        surname = copyString(other.surname);
        age = other.age;
        avgGrade = other.avgGrade;

        return *this;
    }

    // Оператор сложения для сложения средних баллов двух студентов
    Student operator+(const Student &other) const {
        char *newSurname = new char[strlen(this->surname) + strlen(other.surname) + 4];
        strcpy(newSurname, this->surname);
        strcat(newSurname, " & ");
        strcat(newSurname, other.surname);

        Student newStudent(newSurname, this->age, (this->avgGrade + other.avgGrade) / 2.0f);
        delete[] newSurname;
        return newStudent;
    }

    // Метод для изменения данных студента
    void setStudent(const char *surname, int age, float avgGrade) {
        delete[] this->surname;

        this->surname = copyString(surname);
        this->age = age;
        this->avgGrade = avgGrade;
    }

    // Метод для вывода информации о студенте
    void display() const {
        std::cout << "Surname: " << (surname ? surname : "Unknown") << std::endl;
        std::cout << "Age: " << age << std::endl;
        std::cout << "Average Grade: " << avgGrade << std::endl;
    }

    // Деструктор
    ~Student() {
        delete[] surname;
    }
};

int main() {
    // Создание объектов класса Student
    Student student1("Ivanov", 20, 4.5f);
    Student student2("Petrov", 22, 4.8f);

    // Вывод информации о студентах
    std::cout << "Student 1:" << std::endl;
    student1.display();

    std::cout << "\nStudent 2:" << std::endl;
    student2.display();

    // Сложение двух объектов(студентов)
    Student combinedStudent = student1 + student2;
    std::cout << "\nCombined Student:" << std::endl;
    combinedStudent.display();

    return 0;
}
