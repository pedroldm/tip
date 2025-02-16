#include "Instance.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>

using namespace std;

Instance::Instance(int tools, int slots, vector<int> solution,
                   vector<vector<int>> instance, int SOACost)
    : tools(tools),
      slots(slots),
      SOACost(SOACost),
      solution(move(solution)),
      instance(move(instance)) {}

Instance::Instance(const string& filename) {
    ifstream file(filename);
    if (!file) {
        cerr << "Error: Could not open file " << filename << endl;
        exit(1);
    }

    file >> this->tools >> this->slots;

    this->solution.resize(this->slots);
    for (int i = 0; i < this->slots; i++) {
        string value;
        file >> value;
        solution[i] = (value == "x") ? -1 : stoi(value) - 1;
    }

    file >> this->SOACost;

    this->instance.resize(this->tools, vector<int>(this->tools));
    for (int i = 0; i < this->tools; i++) {
        for (int j = 0; j < this->tools; ++j) {
            file >> this->instance[i][j];
        }
    }
}

int Instance::evaluate() {
    unordered_map<int, int> toolIndex;
    
    for (int i = 0; i < this->slots; i++) {
        if (this->solution[i] == -1) continue;
        toolIndex[this->solution[i]] = i;
    }

    int totalCost = 0;

    for (int i = 0; i < this->tools; i++) {
        for (int j = i + 1; j < this->tools; j++) {
            if (!this->instance[i][j]) continue;
            int dist = abs(toolIndex[i] - toolIndex[j]);
            int circularDist = min(dist, this->slots - dist);
            totalCost += this->instance[i][j] * circularDist;
        }
    }

    return totalCost;
}

void Instance::print() {
    cout << "Tools: " << this->tools << ", Slots: " << this->slots
         << ", SOACost: " << this->SOACost << "\nSolution: ";
    for (int val : this->solution) {
        cout << val << " ";
    }
    cout << "\nSOA Cost : " << this->SOACost;
    cout << "\nInstance Matrix:\n";
    for (const auto& row : this->instance) {
        for (int val : row) {
            cout << val << "\t";
        }
        cout << "\n";
    }
}
