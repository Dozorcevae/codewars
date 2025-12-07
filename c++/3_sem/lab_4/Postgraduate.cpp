
#include "Postgraduate.hpp"
// Конструктор по умолчанию
Postgraduate::Postgraduate() : Student(), research_topic(nullptr) {}

// Конструктор с параметрами
Postgraduate::Postgraduate(const char* s, int a, float avg, const char* topic) : Student(s, a, avg) {
    research_topic = new char[strlen(topic) + 1];
    strcpy(research_topic, topic);
}

// Конструктор копирования
Postgraduate::Postgraduate(const Postgraduate& other) : Student(other) {
    research_topic = new char[strlen(other.research_topic) + 1];
    strcpy(research_topic, other.research_topic);
}

// Деструктор
Postgraduate::~Postgraduate() {
    delete[] research_topic;
}

// Установка темы исследования
void Postgraduate::setResearchTopic(const char* topic) {
    delete[] research_topic;
    research_topic = new char[strlen(topic) + 1];
    strcpy(research_topic, topic);
}

// Переопределённый метод для отображения информации о магистранте
void Postgraduate::displayDetails() const {
    Student::displayDetails();
    std::cout << "Research Topic: " << (research_topic ? research_topic : "No research topic") << std::endl;
}
