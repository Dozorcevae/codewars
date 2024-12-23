#include "queue_tester.hpp"
#include "set_tester.hpp"
#include "utilities.hpp"
#include <vector>
#include <iostream>

int main() {
    const int DATA_SIZE = 10000;
    const int VALUE_RANGE = 100000;

    // Генерация случайных данных
    std::vector<int> randomData(DATA_SIZE);
    for (int& num : randomData) {
        num = generateRandomNumber(1, VALUE_RANGE);
    }

    std::string logFilename = "lab8_results.log";

    // Тестирование очереди
    testQueue(randomData, logFilename);

    // Тестирование множества
    testSet(randomData, logFilename);

    std::cout << "Results have been logged to " << logFilename << std::endl;
    return 0;
}
