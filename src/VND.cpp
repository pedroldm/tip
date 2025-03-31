#include "VND.hpp"

VND::VND(SampleDecoder sampleDecoder, int maxIterations)
    : sampleDecoder(sampleDecoder), maxIterations(maxIterations) {}

VND::VND() {}

VND::~VND() {}

Chromosome VND::VNDSearch(Chromosome& chromosome, std::vector<std::pair<int, int>>& searchPairs, int pairsToConsider) {
    std::vector<int> solution = this->sampleDecoder.outputDecode(chromosome);
    double currentBest = this->sampleDecoder.fitness(solution);

    int iteration = 0;
    int currIteration = 0;
    while (currIteration < this->maxIterations && iteration < 3) {
        currIteration += 1;
        switch(iteration) {
            case 0:
                this->swapLocalSearch(solution, currentBest, searchPairs, pairsToConsider) ? iteration = 0 : iteration += 1;
                break;
            case 1:
                this->TwoOPTLocalSearch(solution, currentBest, searchPairs, pairsToConsider) ? iteration = 0 : iteration += 1;
                break;
            case 2:
                this->reinsertionLocalSearch(solution, currentBest, searchPairs, pairsToConsider) ? iteration = 0 : iteration += 1;
                break;
        }
    }

    return this->sampleDecoder.encode(solution, chromosome);
}

bool VND::swapLocalSearch(std::vector<int>& solution, double& currentBest, std::vector<std::pair<int, int>> searchPairs, int pairsToConsider) {
    for (int k = 0; k < pairsToConsider; k++) {
        int i = searchPairs[k].first;
        int j = searchPairs[k].second;
        std::swap(solution[i], solution[j]);
        double newFitness = this->sampleDecoder.fitness(solution);
        if (newFitness < currentBest) {
            currentBest = newFitness;
            return true;
        } else {
            std::swap(solution[i], solution[j]);
        }
    }
    return false;
}

bool VND::TwoOPTLocalSearch(std::vector<int>& solution, double& currentBest, std::vector<std::pair<int, int>> searchPairs, int pairsToConsider) {
    for (int k = 0; k < pairsToConsider; k++) {
        int i = searchPairs[k].first;
        int j = searchPairs[k].second;
        std::reverse(solution.begin() + i, solution.begin() + j);
        double newFitness = this->sampleDecoder.fitness(solution);
        if (newFitness < currentBest) {
            currentBest = newFitness;
            return true;
        } else {
            std::reverse(solution.begin() + i, solution.begin() + j);
        }
    }
    return false;
}

bool VND::reinsertionLocalSearch(std::vector<int>& solution, double& currentBest, std::vector<std::pair<int, int>> searchPairs, int pairsToConsider) {
    for (int k = 0; k < pairsToConsider; k++) {
        int i = searchPairs[k].first;
        int j = searchPairs[k].second;
        if (i == j) continue;
        int tool = solution[i];
        solution.erase(solution.begin() + i);
        solution.insert(solution.begin() + j, tool);
        double newFitness = this->sampleDecoder.fitness(solution);
        if (newFitness < currentBest) {
            currentBest = newFitness;
            return true;
        } else {
            solution.erase(solution.begin() + j);
            solution.insert(solution.begin() + i, tool);
        }
    }
    return false;
}