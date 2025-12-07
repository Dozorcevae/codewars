#include "StudentList.h"
#include "Utils.h"
#include <iostream>


void addStudent(StudentList& list) {
    char lastName[MAX_LAST_NAME];
    char birthDate[MAX_DATE];
    char enrollmentDate[MAX_DATE];
    char expulsionDate[MAX_DATE];
    char address[MAX_ADDRESS];
    char group[MAX_GROUP];

    inputString("Введите фамилию: ", lastName, MAX_LAST_NAME);
    inputDate("Введите дату рождения (ДД.ММ.ГГГГ): ", birthDate);
    inputDate("Введите дату поступления (ДД.ММ.ГГГГ): ", enrollmentDate);
    inputDate("Введите дату отчисления (ДД.ММ.ГГГГ): ", expulsionDate);
    inputString("Введите адрес: ", address, MAX_ADDRESS);
    inputString("Введите группу: ", group, MAX_GROUP);

    Student student(lastName, birthDate, enrollmentDate, expulsionDate, address, group);
    list.add(student);
    std::cout << "Студент успешно добавлен.\n";
}

void deleteStudent(StudentList& list) {
    int index;
    std::cout << "Введите индекс студента для удаления: ";
    std::cin >> index;
    if (list.remove(index)) {
        std::cout << "Студент успешно удален.\n";
    } else {
        std::cerr << "Ошибка: индекс вне диапазона.\n";
    }
}

void displayStudents(const StudentList& list) {
    list.displayAll();
}

void searchStudent(const StudentList& list) {
    int criterion;
    char pattern[100];

    std::cout << "Выберите критерий поиска:\n";
    std::cout << "1. Фамилия\n";
    std::cout << "2. Группа\n";
    std::cout << "3. Дата поступления\n";
    std::cout << "4. Дата отчисления\n";
    std::cout << "Введите номер критерия: ";
    std::cin >> criterion;

    if (criterion < 1 || criterion > 4) {
        std::cerr << "Ошибка: неверный критерий.\n";
        return;
    }

    std::cout << "Введите шаблон для поиска: ";
    std::cin.ignore(); // Очистка ввода
    std::cin.getline(pattern, sizeof(pattern));

    list.search(pattern, criterion);
}

void saveToFile(const StudentList& list) {
    char filename[100];
    inputString("Введите имя файла для сохранения: ", filename, 100);
    list.saveToFile(filename);
    std::cout << "Данные успешно сохранены в файл.\n";
}

void loadFromFile(StudentList& list) {
    char filename[100];
    inputString("Введите имя файла для загрузки: ", filename, 100);
    list.loadFromFile(filename);
    std::cout << "Данные успешно загружены из файла.\n";
}

void sortStudents(StudentList& list) {
    list.sortByLastName();
    std::cout << "Список успешно отсортирован по фамилии.\n";
}

int main() {
    StudentList list;
    int choice;

    while (true) {
        std::cout << "\n--- Меню ---\n";
        std::cout << "1. Добавить студента\n";
        std::cout << "2. Удалить студента\n";
        std::cout << "3. Показать всех студентов\n";
        std::cout << "4. Поиск студента\n";
        std::cout << "5. Сортировать студентов по фамилии\n";
        std::cout << "6. Сохранить список студентов в файл\n";
        std::cout << "7. Загрузить список студентов из файла\n";
        std::cout << "8. Выйти\n";
        std::cout << "Введите ваш выбор: ";
        std::cin >> choice;

        switch (choice) {
            case 1:
                addStudent(list);
                break;
            case 2:
                deleteStudent(list);
                break;
            case 3:
                displayStudents(list);
                break;
            case 4:
                searchStudent(list);
                break;
            case 5:
                sortStudents(list);
                break;
            case 6:
                saveToFile(list);
                break;
            case 7:
                loadFromFile(list);
                break;
            case 8:
                std::cout << "Выход из программы.\n";
                return 0;
            default:
                std::cerr << "Ошибка: неверный выбор.\n";
        }
    }
}
