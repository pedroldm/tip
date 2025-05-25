#ifndef ILS_HPP
#define ILS_HPP

#include <algorithm>
#include <chrono>
#include <numeric>
#include <random>
#include <set>
#include <unordered_set>
#include <vector>

#include "InOut.hpp"
#include "Instance.hpp"
#include "VND.hpp"

class ILS {
   public:
    Instance instance;
    InOut io;
    VND vnd;

    std::vector<int> bestSolution;
    int minFx;
    std::vector<std::pair<int, int>> searchPairs;
    std::vector<std::pair<int, int>> reverseSearchPairs;

    ILS(Instance& instance, InOut& io, VND& vnd);
    std::vector<int> createInitialSolution();
    std::vector<int> disturb(std::vector<int>& solution);
    size_t vectorHash(const std::vector<int>& vec);
    void run();
};

#endif  // ILS_HPP
