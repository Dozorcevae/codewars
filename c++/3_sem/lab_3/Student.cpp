#include "Student.hpp"
#include <iostream>
#include <cstring>

//конструктор по умолчанию
Student::Student() : surname(nullptr), age(0), avg_grade(0.0) {
}

//конструктор с параметрами
Student::Student(const char *s, int a, float avg) {
    surname = new char[strlen(s) + 1];
    strcpy(surname, s);
    age = a;
    avg_grade = avg;
}

//конструктор копирования
// Student::Student(const Student &other) {
//     //Вместо new char() используй new char[]
//     surname = new char[strlen(other.surname) + 1];
//     strcpy(surname, other.surname);
//     age = other.age;
//     avg_grade = other.avg_grade;
// }
//Обрабатывай исключения доступными способами на nullptr
Student::Student(const Student &other) {
    if (other.surname) {
        surname = new char[strlen(other.surname) + 1];
        strcpy(surname, other.surname);
    } else {
        surname = new char[1];
        surname[0] = '\0';
    }
    age = other.age;
    avg_grade = other.avg_grade;
}

//деструктор
Student::~Student() {
    // А если я копирую объект вручную ????
    // Сколько вызовов деструктора будет в таком случае ????
    // delete[] surname;
    if (surname) {
        delete[] surname;
        surname = nullptr;
    }
}

//оператор вывода
std::ostream &operator<<(std::ostream &os, const Student &student) {
    if (!os) {
        std::cerr << "Error: Stream is not in a valid state." << std::endl;
        return os;
    }
    //Внятно форматируй вывод информации
    os << "Surname: " << (student.surname ? student.surname : "Unknown")
            << "\nAge: " << student.age
            << "\nAverage Grade: " << student.avg_grade << "\n";
    return os;
}

//оператор ввода
// std::istream &operator>>(std::istream &is, Student &student) {
//     //буфер для хранения фамилии на 256 символов
//     char buffer[256];
//     //ввод фамилии
//     std::cout << "Enter surname: ";
//     //недостаточная проверка и очистка потока после предыдушего ввода данных
//     //is.ignore(); //пропускаем первый символ строки оставшийся в потоке
//     is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
//     is.getline(buffer, 256); // считываем строку, ограниченную буфером
//     student.setStudent(buffer, student.age, student.avg_grade);
//     //ввод возраста
//     std::cout << "Enter age: ";
//     is >> student.age;
//     //ввод среднего балла
//     std::cout << "Enter average grade: ";
//     is >> student.avg_grade;
//     return is; //возвращаем поток ввода для цепочки вызовов
// }

// Все поля устанавливаются через setStudent для унифицированной логики.
// Устранена смешанная логика установки полей (часть через метод, часть напрямую).
// Улучшена обработка ошибок ввода через централизованный метод.
std::istream& operator>>(std::istream& is, Student& student) {
    char buffer[256];
    std::cout << "Enter surname: ";
    is.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    is.getline(buffer, 256);
    int age;
    float avg_grade;
    std::cout << "Enter age: ";
    is >> age;
    std::cout << "Enter average grade: ";
    is >> avg_grade;
    student.setStudent(buffer, age, avg_grade);
    return is;
}


void Student::setStudent(const char* s, int a, float avg) {
    // это dev строка
    // std::cout << "Setting surname to: " << (s ? s : "nullptr") << std::endl;
    delete[] surname;
    if (s && strlen(s) > 0) {
        surname = new char[strlen(s) + 1];
        strcpy(surname, s);
    } else {
        surname = new char[1];
        surname[0] = '\0';
    }
    age = a;
    avg_grade = avg;
}



//сохранение в текстовый файл
void Student::saveToTxt(const char *_filename) const {
    //открываем файл для записи в текстовом формате
    std::ofstream ofs(_filename);
    //если файл не открылся, нужно вывести ошибку
    if (!ofs) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }

    //построчно записываем данные в файл
    ofs << surname << "\n" << age << "\n" << avg_grade << "\n";
    ofs.close(); //закрываем файл после записи
}

//загрузка из текстового файла
void Student::loadFromTxt(const char *_filename) {
    std::ifstream ifs(_filename);
    if (!ifs) {
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }
    /*char buffer[256]; //буфер для чтения строки из файла
    ifs.getline(buffer, 256); //читаем фамилию из файла
    setStudent(buffer, 0, 0); //устонавливаем фамилию в объект студента
    ifs >> age >> avg_grade; //читаем возраст и средний возраст
    ifs.close(); // закрываем файл*/

    //Input_Lag  для присвоения данных setStudent
    char buffer[256];
    int age;
    float avg_grade;
    ifs.getline(buffer, 256);
    ifs >> age >> avg_grade;
    setStudent(buffer, age, avg_grade);
}

//сохранение в бинарный файл
void Student::saveToBinary(const char *_filename) const {
    // Открываем файл для записи в бинарном формате
    std::ofstream ofs(_filename, std::ios::binary);
    if (!ofs) {
        // Если файл не открылся, выводим сообщение об ошибке
        std::cerr << "Error: Unable to open file for writing." << std::endl;
        return;
    }
    // Вычисляем длину строки фамилии
    size_t len = strlen(surname);
    // Сначала записываем длину строки
    ofs.write(reinterpret_cast<const char *>(&len), sizeof(len));
    // Затем записываем саму строку фамилии
    ofs.write(surname, len);
    // Записываем возраст и средний балл
    ofs.write(reinterpret_cast<const char *>(&age), sizeof(age));
    ofs.write(reinterpret_cast<const char *>(&avg_grade), sizeof(avg_grade));
    // Закрываем файл после записи
    ofs.close();
}

// Загрузка из бинарного файла

//loadFronBinaryFile -> loadFromBinaryFile
// void Student::loadFromBinaryFile(const char *filename) {
//     // Открываем файл для чтения в бинарном формате
//     std::ifstream ifs(filename, std::ios::binary);
//     //ужастная проверка чтения данных
//     // if (!ifs) {
//     //     // Если файл не открылся, выводим сообщение об ошибке
//     //     std::cerr << "Error: Unable to open file for reading." << std::endl;
//     //     return;
//     // }
//     if (!ifs.read(reinterpret_cast<char *>(&len), sizeof(len))) {
//         std::cerr << "Error: Unable to read data from binary file." << std::endl;
//         return;
//     }
//
//     // Читаем длину строки фамилии
//     size_t len;
//     ifs.read(reinterpret_cast<char *>(&len), sizeof(len));
//     // Выделяем новую память для фамилии
//     delete[] surname;
//     surname = new char[len + 1];
//     // Читаем строку фамилии из файла
//     ifs.read(surname, len);
//     // Завершаем строку нулевым символом
//     surname[len] = '\0';
//     // Читаем возраст и средний балл
//     ifs.read(reinterpret_cast<char *>(&age), sizeof(age));
//     ifs.read(reinterpret_cast<char *>(&avg_grade), sizeof(avg_grade));
//     // Закрываем файл после чтения
//     ifs.close();
// }

// Добавлена проверка открытия файла и чтения данных.
// Реализована очистка памяти при ошибках чтения
// Улучшены сообщения об ошибках
void Student::loadFromBinaryFile(const char *_filename) {
    std::ifstream ifs(_filename, std::ios::binary);
    if (!ifs) {
        std::cerr << "_Error: unableToOpenFileForReading." << std::endl;
        return;
    }
    size_t len;
    if (!ifs.read(reinterpret_cast<char *>(&len), sizeof(len))) {
        std::cerr << "_Error: unableToReadDataFromBinaryFile." << std::endl;
        return;
    }
    delete[] surname;
    surname = new char[len + 1];
    if (!ifs.read(surname, len)) {
        std::cerr << "_Error: unableToReadDataFromBinaryFile." << std::endl;
        delete[] surname;
        surname = nullptr;
        return;
    }
    surname[len] = '\0';
    if (!ifs.read(reinterpret_cast<char *>(&age), sizeof(age)) ||
        !ifs.read(reinterpret_cast<char *>(&avg_grade), sizeof(avg_grade))) {
        std::cerr << "_Error: unableToReadDataFromBinaryFile." << std::endl;
        return;
    }
    ifs.close();
}
