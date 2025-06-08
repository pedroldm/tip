#include "TIPPermutation.hpp"


TIPPermutation::TIPPermutation(std::string filename) {
    std::ifstream inputFile(filename);
    if (!inputFile.is_open()) {
        throw std::runtime_error("Unable to open file: " + filename);
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

solTIP TIPPermutation::construction() {
    solTIP ss;
    ss.sol.resize(tools);
    std::iota(ss.sol.begin(), ss.sol.end(), 0);

    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::shuffle(ss.sol.begin(), ss.sol.end(), mersenne_engine);

    return ss;
}

double TIPPermutation::evaluate(solTIP s){
    std::unordered_map<int, int> toolIndex;

    for (int i = 0; i < this->tools; i++) {
        toolIndex[s.sol[i]] = i;
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

solTIP TIPPermutation::neighbor(solTIP sol) {
    solTIP newS = sol;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(0, this->tools - 1);

//    int iterations = static_cast<int>(this->alpha * this->tools);

    int index = dist(gen);
    int newIndex = dist(gen);
    std::swap(newS.sol[index], newS.sol[newIndex]);

//    int index = dist(gen);
//    int newIndex = dist(gen);
//    int element = newS.sol[index];
//    newS.sol.erase(newS.sol.begin() + index);
//    if (newIndex >= index) {
//        newIndex = std::min(newIndex, static_cast<int>(newS.sol.size()));
//    }
//    newS.sol.insert(newS.sol.begin() + newIndex, element);

    return newS;
}

