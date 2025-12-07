#include <iostream>
#include <cstring>

class Student {
private:
    char *surname;   // выделение памяти под фамилию
    int age;         // Возраст 
    float avgGrade;  // Средний балл 

    // вспомогательный метод для выделения памяти и копирования строки
    void allocateAndCopy(const char *source) {
        if (source) {
            surname = new char[strlen(source) + 1];
            strcpy(surname, source);
        } else {
            surname = nullptr;
        }
    }

public:
    // Конструктор по умолчанию
    Student() : surname(nullptr), age(18), avgGrade(0.0f) {
        // Инициализация фамилии пустой строкой
        allocateAndCopy("NoSurname");
    }

    // Конструктор с параметрами
    Student(const char *newSurname, int newAge, float newAvgGrade)
        : age(newAge), avgGrade(newAvgGrade) {
        allocateAndCopy(newSurname);
    }

    // Конструктор копирования
    Student(const Student &other) : age(other.age), avgGrade(other.avgGrade) {
        allocateAndCopy(other.surname);
    }

    // Оператор присваивания
    Student &operator=(const Student &other) {
        if (this != &other) {
            // Освобождаем память перед новым присваиванием
            delete[] surname;
            age = other.age;
            avgGrade = other.avgGrade;
            allocateAndCopy(other.surname);
        }
        return *this;
    }

    // Оператор сложения студентов (средний балл - среднее арифметическое)
    Student operator+(const Student &other) const {
        char *combinedSurname = new char[strlen(this->surname) + strlen(other.surname) + 3];
        strcpy(combinedSurname, this->surname);
        strcat(combinedSurname, " & ");
        strcat(combinedSurname, other.surname);

        // Возраст остаётся как возраст первого студента
        Student combinedStudent(combinedSurname, this->age, (this->avgGrade + other.avgGrade) / 2);
        delete[] combinedSurname;

        return combinedStudent;
    }

    // Метод для изменения данных студента
    void updateStudent(const char *newSurname, int newAge, float newAvgGrade) {
        delete[] surname;  // Освобождаем предыдущую память
        allocateAndCopy(newSurname);
        age = newAge;
        avgGrade = newAvgGrade;
    }

    // Метод для вывода информации о студенте
    void displayInfo() const {
        std::cout << "Фамилия: " << (surname ? surname : "Unknown") << std::endl;
        std::cout << "Возраст: " << age << std::endl;
        std::cout << "Средний балл: " << avgGrade << std::endl;
    }

    // Деструктор
    ~Student() {
        delete[] surname;  // Освобождаем динамически выделенную память
    }
};

int main() {
    // Создание объектов класса Student с использованием разных конструкторов
    Student student1("Sidorov", 19, 4.3f);
    Student student2("Ivanova", 21, 4.7f);
    Student student3;  // Использование конструктора по умолчанию

    // Вывод информации о студентах
    std::cout << "Информация о студентах:" << std::endl;
    student1.displayInfo();
    student2.displayInfo();
    student3.displayInfo();

    // Обновление данных студента
    student3.updateStudent("Petrov", 20, 3.9f);
    std::cout << "\nОбновлённая информация о студенте 3:" << std::endl;
    student3.displayInfo();

    // Сложение двух студентов
    Student combinedStudent = student1 + student2;
    std::cout << "\nОбъединённый студент:" << std::endl;
    combinedStudent.displayInfo();

    return 0;
}
