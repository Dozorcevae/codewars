
#include "utilities.hpp"
#include <random>
#include <fstream>
#include <iostream>

int generateRandomNumber(int min, int max) {
    static std::mt19937 generator(std::random_device{}());
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
}

void logResults(const std::string& filename, const std::string& data) {
    std::ofstream file(filename, std::ios::app);
    if (file.is_open()) {
        file << data << std::endl;
    } else {
        std::cerr << "Error: Unable to open file " << filename << std::endl;
    }
}
