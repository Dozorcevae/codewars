//
//  Postgraduate.hpp
//  code
//
//  Created by Елена Дозорцева on 20.12.2024.
//

#ifndef Postgraduate_hpp
#define Postgraduate_hpp

#include <stdio.h>
#include "Student.hpp"

// Производный класс Postgraduate, который добавляет информацию о теме исследования
class Postgraduate : public Student {
private:
    char* research_topic; // Тема исследования

public:
    // Конструкторы и деструктор
    Postgraduate(); // Конструктор по умолчанию
    Postgraduate(const char* s, int a, float avg, const char* topic); // Конструктор с параметрами
    Postgraduate(const Postgraduate& other); // Конструктор копирования
    ~Postgraduate(); // Деструктор

    // Установка темы исследования
    void setResearchTopic(const char* topic);
    // Переопределённый виртуальный метод для отображения информации о студенте
    void displayDetails() const override;
};

#endif /* Postgraduate_hpp */
