#include "set_tester.hpp"
#include "utilities.hpp"
#include <iostream>
#include <chrono>

void testSet(const std::vector<int>& data, const std::string& logFilename) {
    std::set<int> mySet;

    std::cout << "\nTesting set operations:\n";

    // Замер времени добавления в множество
    auto start = std::chrono::high_resolution_clock::now();
    for (int value : data) {
        mySet.insert(value);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::string logData = "Set - Time to insert elements: " +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
    std::cout << logData << std::endl;
    logResults(logFilename, logData);

    // Замер времени поиска в множестве
    start = std::chrono::high_resolution_clock::now();
    for (int value : data) {
        mySet.find(value);
    }
    end = std::chrono::high_resolution_clock::now();
    logData = "Set - Time to find elements: " +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
    std::cout << logData << std::endl;
    logResults(logFilename, logData);

    // Замер времени удаления из множества
    start = std::chrono::high_resolution_clock::now();
    for (int value : data) {
        mySet.erase(value);
    }
    end = std::chrono::high_resolution_clock::now();
    logData = "Set - Time to erase elements: " +
        std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count()) + " ms";
    std::cout << logData << std::endl;
    logResults(logFilename, logData);
}
