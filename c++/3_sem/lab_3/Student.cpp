
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

//оператор вывода
std::ostream& operator<<(std::ostream& os, const Student& student){
    if(!os){
        std::cerr << "Error: Stream is not in a valid state." << std::endl;
        return os;
    }
    os << "Surname: " << (student.surname ? student.surname : "Unknown") << "Age: " << student.age << "\n" << "Average Grade: " << student.avg_grade << "\n";
    return os;
}

//оператор ввода 
std::istream& operator>>(std::istream& is, Student& student){
    //буфер для хранения фамилии на 256 символов
    char buffer[256];
    
    //ввод фамилии
    std::cout<< "Enter surname: ";
    is.ignore(); //пропускаем первый символ строки оставшийся в потоке
    is.getline(buffer, 256); // считываем строку, ограниченную буфером
    student.setStudent(buffer, student.age, student.avg_grade);
    
    //ввод возраста
    std::cout<<"Enter age: ";
    is >> student.age;
    
    //ввод среднего балла
    std::cout << "Enter average grade: ";
    is >> student.avg_grade;
    
    return is; //возвращаем поток ввода для цепочки вызовов
}

void Student::setStudent(const char* s, int a, float avg) {
    // Удаляем текущую память, если есть
    delete[] surname;

    // Проверяем, не является ли входная строка пустой
    if (!s) {
        surname = nullptr;
    } else {
        // Копируем новую строку
        surname = new char[strlen(s) + 1];
        strcpy(surname, s);
    }

    // Устанавливаем возраст и средний балл
    age = a;
    avg_grade = avg;
}


//сохранение в текстовый файл
void Student::saveToTxt(const char *filename) const{
    //открываем файл для записи в текстовом формате
    std::ofstream ofs(filename);
    //если файл не открылся, нужно вывести ошибку
    if (!ofs){
        std::cerr << "Error: Unable to open file for reading." << std::endl;
            return;
    }
    
    //построчно записываем данные в файл
    ofs << surname << "\n" << age << "\n" << avg_grade << "\n";
    ofs.close(); //закрываем файл после записи
}

//загрузка из текстового файла
void Student::loadFromTxt(const char *filename){
    std::ifstream ifs(filename);
    if (!ifs){
        std::cerr << "Error: Unable to open file for reading." << std::endl;
            return;
    }
    char buffer[256]; //буфер для чтения строки из файла
    ifs.getline(buffer, 256); //читаем фамилию из файла
    setStudent(buffer, 0, 0); //устонавливаем фамилию в объект студента
    ifs >> age >> avg_grade; //читаем возраст и средний возраст
    ifs.close(); // закрываем файл
}

//сохранение в бинарный файл
void Student::saveToBinary(const char *filename) const {
    // Открываем файл для записи в бинарном формате
        std::ofstream ofs(filename, std::ios::binary);
        if (!ofs) {
            // Если файл не открылся, выводим сообщение об ошибке
            std::cerr << "Error: Unable to open file for writing." << std::endl;
            return;
        }
        // Вычисляем длину строки фамилии
        size_t len = strlen(surname);
        // Сначала записываем длину строки
        ofs.write(reinterpret_cast<const char*>(&len), sizeof(len));
        // Затем записываем саму строку фамилии
        ofs.write(surname, len);
        // Записываем возраст и средний балл
        ofs.write(reinterpret_cast<const char*>(&age), sizeof(age));
        ofs.write(reinterpret_cast<const char*>(&avg_grade), sizeof(avg_grade));
        // Закрываем файл после записи
        ofs.close();
}
// Загрузка из бинарного файла
void Student::loadFronBinaryFile(const char *filename) {
    // Открываем файл для чтения в бинарном формате
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        // Если файл не открылся, выводим сообщение об ошибке
        std::cerr << "Error: Unable to open file for reading." << std::endl;
        return;
    }
    // Читаем длину строки фамилии
    size_t len;
    ifs.read(reinterpret_cast<char*>(&len), sizeof(len));
    // Выделяем новую память для фамилии
    delete[] surname;
    surname = new char[len + 1];
    // Читаем строку фамилии из файла
    ifs.read(surname, len);
    // Завершаем строку нулевым символом
    surname[len] = '\0';
    // Читаем возраст и средний балл
    ifs.read(reinterpret_cast<char*>(&age), sizeof(age));
    ifs.read(reinterpret_cast<char*>(&avg_grade), sizeof(avg_grade));
    // Закрываем файл после чтения
    ifs.close();
}


