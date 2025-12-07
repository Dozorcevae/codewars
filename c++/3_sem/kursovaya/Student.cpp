#include "Student.h"
#include <cstring>
#include <cstdlib>

// Конструктор по умолчанию
Student::Student() {
    lastName = new (std::nothrow) char[1];
    if (lastName) {
        lastName[0] = '\0';
    } else {
        std::cerr << "Ошибка выделения памяти для фамилии.\n";
        exit(EXIT_FAILURE);
    }
    strcpy(birthDate, "00.00.0000");
    strcpy(enrollmentDate, "00.00.0000");
    strcpy(expulsionDate, "00.00.0000");
    strcpy(address, "No data");
    strcpy(group, "No data");
}

// Конструктор с параметрами
Student::Student(const char* ln, const char* bd, const char* ed, const char* exd, const char* addr, const char* grp) {
    lastName = new (std::nothrow) char[strlen(ln) + 1];
    if (lastName) {
        strcpy(lastName, ln);
    } else {
        std::cerr << "Ошибка выделения памяти для фамилии.\n";
        exit(EXIT_FAILURE);
    }
    strcpy(birthDate, bd);
    strcpy(enrollmentDate, ed);
    strcpy(expulsionDate, exd);
    strcpy(address, addr);
    strcpy(group, grp);
}

// Копирующий конструктор
Student::Student(const Student& s) {
    if (s.lastName) {
        lastName = new (std::nothrow) char[strlen(s.lastName) + 1];
        if (lastName) {
            strcpy(lastName, s.lastName);
        } else {
            std::cerr << "Ошибка выделения памяти при копировании фамилии.\n";
            exit(EXIT_FAILURE);
        }
    } else {
        lastName = nullptr;
    }
    strcpy(birthDate, s.birthDate);
    strcpy(enrollmentDate, s.enrollmentDate);
    strcpy(expulsionDate, s.expulsionDate);
    strcpy(address, s.address);
    strcpy(group, s.group);
}

// Оператор присваивания
Student& Student::operator=(const Student& s) {
    if (this != &s) {
        // Освобождаем предыдущую память
        delete[] lastName;

        if (s.lastName) {
            lastName = new (std::nothrow) char[strlen(s.lastName) + 1];
            if (lastName) {
                strcpy(lastName, s.lastName);
            } else {
                std::cerr << "Ошибка выделения памяти при присваивании фамилии.\n";
                exit(EXIT_FAILURE);
            }
        } else {
            lastName = nullptr;
        }
        strcpy(birthDate, s.birthDate);
        strcpy(enrollmentDate, s.enrollmentDate);
        strcpy(expulsionDate, s.expulsionDate);
        strcpy(address, s.address);
        strcpy(group, s.group);
    }
    return *this;
}

// Деструктор
Student::~Student() {
    delete[] lastName;
}

// Метод для вывода информации о студенте
void Student::display() const {
    std::cout << "Фамилия: " << (lastName ? lastName : "None") << "\n"
              << "Дата рождения: " << birthDate << "\n"
              << "Дата поступления: " << enrollmentDate << "\n"
              << "Дата отчисления: " << expulsionDate << "\n"
              << "Адрес: " << address << "\n"
              << "Группа: " << group << "\n";
}

// Метод для сохранения в бинарный файл
void Student::saveToFile(std::ofstream& ofs) const {
    size_t len = lastName ? strlen(lastName) + 1 : 1;
    ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
    if (lastName) {
        ofs.write(lastName, len);
    } else {
        char empty = '\0';
        ofs.write(&empty, 1);
    }
    ofs.write(birthDate, sizeof(birthDate));
    ofs.write(enrollmentDate, sizeof(enrollmentDate));
    ofs.write(expulsionDate, sizeof(expulsionDate));
    ofs.write(address, sizeof(address));
    ofs.write(group, sizeof(group));
}

// Метод для загрузки из бинарного файла
void Student::loadFromFile(std::ifstream& ifs) {
    size_t len;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
    delete[] lastName;
    if (len > 1) {
        lastName = new (std::nothrow) char[len];
        if (lastName) {
            ifs.read(lastName, len);
        } else {
            std::cerr << "Ошибка выделения памяти при загрузке фамилии.\n";
            exit(EXIT_FAILURE);
        }
    } else {
        lastName = nullptr;
        ifs.ignore(1); // Игнорируем символ '\0'
    }
    ifs.read(birthDate, sizeof(birthDate));
    ifs.read(enrollmentDate, sizeof(enrollmentDate));
    ifs.read(expulsionDate, sizeof(expulsionDate));
    ifs.read(address, sizeof(address));
    ifs.read(group, sizeof(group));
}
