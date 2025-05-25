#ifndef VND_HPP
#define VND_HPP

#include <algorithm>
#include <string>
#include <vector>

#include "Instance.hpp"
#include "InOut.hpp"

class VND {
   public:
    Instance instance;
    InOut io;

    VND(Instance& instance, InOut& io);
    std::tuple<int, std::vector<int>, std::vector<int>> run(
        const std::vector<int>& initialSolution,
        std::vector<std::pair<int, int>>& searchPairs,
        std::vector<std::pair<int, int>>& reverseSearchPairs,
        int pairsToConsider);
    bool swapLocalSearch(std::vector<int>& solution, int& currentBest,
                         std::vector<std::pair<int, int>> searchPairs,
                         int pairsToConsider);
    bool TwoOPTLocalSearch(std::vector<int>& solution, int& currentBest,
                           std::vector<std::pair<int, int>> searchPairs,
                           int pairsToConsider);
    bool reinsertionLocalSearch(
        std::vector<int>& solution, int& currentBest,
        std::vector<std::pair<int, int>> searchPairs,
        std::vector<std::pair<int, int>>& reverseSearchPairs,
        int pairsToConsider);
};

#endif