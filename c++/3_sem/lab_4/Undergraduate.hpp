//
//  Undergraduate.hpp
//  code
//
//  Created by Елена Дозорцева on 20.12.2024.
//

#ifndef Undergraduate_hpp
#define Undergraduate_hpp
#include "Student.hpp"
#include <stdio.h>


// Производный класс Undergraduate, который добавляет информацию о специальности
class Undergraduate : public Student {
private:
    char* major; // Специальность студента

public:
    // Конструкторы и деструктор
    Undergraduate(); // Конструктор по умолчанию
    Undergraduate(const char* s, int a, float avg, const char* m); // Конструктор с параметрами
    Undergraduate(const Undergraduate& other); // Конструктор копирования
    ~Undergraduate(); // Деструктор

    // Установка специальности студента
    void setMajor(const char* m);
    // Переопределённый виртуальный метод для отображения информации о студенте
    void displayDetails() const override;
};

#endif /* Undergraduate_hpp */
