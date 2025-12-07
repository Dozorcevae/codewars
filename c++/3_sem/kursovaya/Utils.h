#ifndef UTILS_H
#define UTILS_H

#include <cctype>

// Константы для максимальной длины строк
const int MAX_LAST_NAME = 50;
const int MAX_DATE = 11;      // Формат: ДД.ММ.ГГГГ
const int MAX_ADDRESS = 100;
const int MAX_GROUP = 10;

// Функции для валидации и ввода данных
bool isValidDate(const char* date);
void inputString(const char* prompt, char* buffer, int maxLength);
void inputDate(const char* prompt, char* buffer);

#endif // UTILS_H