#include <iostream>
#include "Student.hpp"

int main(){
    
    Student student1("Nevsky", 30, 4.5);
        Student student2;
    
    // Демонстрация ввода/вывода
        std::cout << "Initial data for student1:\n" << student1;

        std::cout << "\nEnter data for student2:\n";
        std::cin >> student2;

        std::cout << "\nUpdated data for student2:\n" << student2;

        // Работа с файлами
        const char* textFile = "student.txt";
        const char* binaryFile = "student.bin";

        student1.saveToTxt(textFile);
        student1.saveToBinary(binaryFile);

        Student student3, student4;
    student3.loadFromTxt(textFile);
        student4.loadFronBinaryFile(binaryFile);

        std::cout << "\nData loaded from text file:\n" << student3;
        std::cout << "\nData loaded from binary file:\n" << student4;

        return 0;
}
