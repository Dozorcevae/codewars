#include "queue_tester.hpp"
#include "utilities.hpp"
#include <iostream>
#include <chrono>

void testQueue(const std::vector<int>& data, const std::string& logFilename) {
    std::queue<int> myQueue;

    std::cout << "Testing queue operations:\n";

    // Замер времени добавления в очередь
    auto start = std::chrono::high_resolution_clock::now();
    for (int value : data) {
        myQueue.push(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::string logData = "Queue - Time to push elements: " +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
    std::cout << logData << std::endl;
    logResults(logFilename, logData);

    // Замер времени извлечения из очереди
    start = std::chrono::high_resolution_clock::now();
    while (!myQueue.empty()) {
        myQueue.pop();
    }
    end = std::chrono::high_resolution_clock::now();
    logData = "Queue - Time to pop elements: " +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
    std::cout << logData << std::endl;
    logResults(logFilename, logData);
}
