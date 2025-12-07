
#include <iostream>
#include "Student.hpp"

int main(){
    //создание студентов
    Student student1("Dozortseva", 20, 3.8);
    Student student2("Nevsky", 30, 4.7);
    
    //вывод студентов
    std::cout<<"Student 1:\n ";
    student1.display();
    
    std::cout<<"Student 2:\n";
    student2.display();
    
    //сложение студентов
    Student student3 = student2 + student1;
    std::cout<<"\n Average of student 1 and student 2 : \n";
    student3.display();
    
    //присваивание
    Student student4;
    student4 = student1;
    std::cout<<"\n tudent 4 (after assignment from Student 1): \n";
    
    //префиксный инкремент
    ++student1;
    std::cout<<"\n Student 1 after prefix increment \n";
    student1.display();
    
    //постфиксный инкремент
    student1++;
    std::cout<<"\n Student 1 after postfix increment \n";
    student1.display();
    
    //приведение к float
    float grade = static_cast<float>(student1);
    std::cout << "\nStudent 1's average grade as float: " << grade << std::endl;

    //вычитание студентов
    Student student5 = student2 - student1;
    std::cout << "\nStudent 5 (Student 2 - Student 1):\n";
    student5.display();
    return 0;
}
