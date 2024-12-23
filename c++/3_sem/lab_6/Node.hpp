//
//  Node.hpp
//  code
//
//  Created by Елена Дозорцева on 22.12.2024.
//

#ifndef Node_hpp
#define Node_hpp

#include <iostream>
#include <string>
#include <stdexcept> // Для исключений

// Базовый класс Node
class Node {
protected:
    int id; // Идентификатор узла

public:
    // Конструктор, инициализирующий идентификатор узла
    // Проверяет, что ID не отрицательный
    Node(int id) {
        if (id < 0) {
            throw std::invalid_argument("ID cannot be negative.");
        }
        this->id = id;
    }

    // Виртуальный деструктор для правильного удаления производных объектов
    virtual ~Node() {}

    // Виртуальный метод для отображения информации об узле
    virtual void display() const {
        std::cout << "Node ID: " << id << std::endl;
    }

    // Метод для получения идентификатора узла
    int getId() const { return id; }
};

// Производный класс 1
class DerivedNode1 : public Node {
private:
    std::string name; // Имя узла

public:
    // Конструктор, принимающий ID и имя узла
    DerivedNode1(int id, const std::string& name) : Node(id), name(name) {}

    // Переопределённый метод для отображения информации о DerivedNode1
    void display() const override {
        std::cout << "DerivedNode1 [ID: " << id << ", Name: " << name << "]" << std::endl;
    }
};

// Производный класс 2
class DerivedNode2 : public Node {
private:
    double value; // Значение узла

public:
    // Конструктор, принимающий ID и значение узла
    DerivedNode2(int id, double value) : Node(id), value(value) {}

    // Переопределённый метод для отображения информации о DerivedNode2
    void display() const override {
        std::cout << "DerivedNode2 [ID: " << id << ", Value: " << value << "]" << std::endl;
    }
};


#endif /* Node_hpp */
