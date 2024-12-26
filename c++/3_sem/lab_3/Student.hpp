#ifndef Student_hpp
#define Student_hpp
#include <fstream>
#include <limits>

class Student{
private:
    char* surname;
    int age;
    float avg_grade;
    
public:
    //Конструкторы и деструктор
    Student();
    Student(const char* s, int a, float avg);
    Student(const Student& other);
    ~Student();
    
    //методы для изменения данных
    void SetSurname(const char* s);
    //const not use!!!!!!!!!!!!
    void SetAge(int a);
    void SetAvgGrade(float avg);
    
    //метод для вывода данных
    void display() const;

    //метод для установки данных
    void setStudent(const char* s, int a, float avg);
    
    //операторы ввода/вывода
    //оператор для ввода информации в поток (загрузка данных в файл)
    friend std::ostream& operator<<(std::ostream& os, const Student& student);
    //оператор для вывода данных (чтение информации из потока)
    friend std::istream& operator>>(std::istream& is, Student& student);
    
    //функции для работы с файлами (сохранение/выгрузка)
    
    //функции для бинарного файла
    void saveToBinary(const char* filename) const;
    void loadFromBinaryFile(const char* filename);
    //для текстового файла
    void saveToTxt(const char* filename) const;
    void loadFromTxt(const char* filename);
};

#endif /* Student_hpp */
