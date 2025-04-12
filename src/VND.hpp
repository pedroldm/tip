#ifndef VND_HPP
#define VND_HPP

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "SampleDecoder.hpp"

class VND {
   public:
    int maxIterations;
    SampleDecoder sampleDecoder;

    VND();
    VND(SampleDecoder sampleDecoder, int maxIterations);
    ~VND();

    std::tuple<Chromosome, std::vector<int>> VNDSearch(Chromosome& chromosome,
                         std::vector<std::pair<int, int>>& searchPairs, std::vector<std::pair<int, int>>& reverseSearchPairs,
                         int pairsToConsider);
    bool TwoOPTLocalSearch(std::vector<int>& solution, double& currentBest,
                           std::vector<std::pair<int, int>> searchPairs,
                           int pairsToConsider);
    bool swapLocalSearch(std::vector<int>& solution, double& currentBest,
                         std::vector<std::pair<int, int>> searchPairs,
                         int pairsToConsider);
    bool reinsertionLocalSearch(std::vector<int>& solution, double& currentBest,
                                std::vector<std::pair<int, int>> searchPairs, std::vector<std::pair<int, int>>& reverseSearchPairs,
                                int pairsToConsider);
};

#endif  // VND_HPP
