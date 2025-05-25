#include "Instance.hpp"

Instance::Instance(const std::string& stringPath) {
    std::ifstream inputFile(stringPath);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + stringPath);
    }

    inputFile >> tools;
    inputFile >> slots;

    HS_RSSolution.resize(slots);
    for (int i = 0; i < this->slots; i++) {
        std::string value;
        inputFile >> value;
        HS_RSSolution[i] = (value == "x") ? -1 : stoi(value) - 1;
    }
    inputFile >> this->HS_RSCost;

    frequencyMatrix.resize(tools, std::vector<int>(tools));
    for (int i = 0; i < tools; ++i) {
        for (int j = 0; j < tools; ++j) {
            inputFile >> frequencyMatrix[i][j];
        }
    }

    inputFile.close();
}

int Instance::evaluate(const std::vector<int>& solution){
    std::unordered_map<int, int> toolIndex;

    for (int i = 0; i < this->tools; i++) {
        toolIndex[solution[i]] = i;
    }

    double totalCost = 0;

    for (int i = 0; i < this->tools; i++) {
        for (int j = i + 1; j < this->tools; j++) {
            if (!this->frequencyMatrix[i][j]) continue;
            int dist = abs(toolIndex[i] - toolIndex[j]);
            int circularDist = std::min(dist, (this->tools - dist) + (this->slots - this->tools));
            totalCost += this->frequencyMatrix[i][j] * circularDist;
        }
    }

    return totalCost;
}