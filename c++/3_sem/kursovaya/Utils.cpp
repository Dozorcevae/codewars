#include "Utils.h"
#include <cstring>
#include <iostream>
#include <cstdlib>

// Функция для проверки корректности даты в формате ДД.ММ.ГГГГ
bool isValidDate(const char* date) {
    if (strlen(date) != 10)
        return false;
    if (date[2] != '.' || date[5] != '.')
        return false;
    // Проверка, что все остальные символы - цифры
    for (int i = 0; i < 10; ++i) {
        if (i == 2 || i == 5)
            continue;
        if (!isdigit(date[i]))
            return false;
    }
    // Дополнительная проверка диапазона значений
    int day = (date[0] - '0') * 10 + (date[1] - '0');
    int month = (date[3] - '0') * 10 + (date[4] - '0');
    int year = (date[6] - '0') * 1000 + (date[7] - '0') * 100 + (date[8] - '0') * 10 + (date[9] - '0');

    if (day < 1 || day > 31)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (year < 1900 || year > 2100)
        return false;
    return true;
}

// Функция для ввода строки с проверкой длины
void inputString(const char* prompt, char* buffer, int maxLength) {
    while (true) {
        std::cout << prompt;
        std::cin.ignore(); // Очистка буфера ввода от предыдущих символов новой строки
        std::cin.getline(buffer, maxLength);
        if (strlen(buffer) == 0) {
            std::cout << "Поле не может быть пустым. Попробуйте снова.\n";
        } else {
            break;
        }
    }
}

// Функция для ввода даты с валидацией
void inputDate(const char* prompt, char* buffer) {
    while (true) {
        std::cout << prompt;
        std::cin >> buffer;
        if (isValidDate(buffer)) {
            break;
        } else {
            std::cout << "Некорректный формат даты. Используйте ДД.ММ.ГГГГ.\n";
        }
    }
}
