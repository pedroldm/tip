#ifndef TIPPermutation_HPP
#define TIPPermutation_HPP

#include <algorithm>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <random>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include "TIP.hpp"
#include "include/Problem.h"

class TIPPermutation : public Problem<solTIP> {
   public:
    int tools;
    int slots;
    int HS_RSCost;
    double alpha = 0.05;
    std::vector<int> HS_RSSolution;
    std::vector<std::vector<int>> frequencyMatrix;

    TIPPermutation(std::string filename);
    solTIP construction();
    solTIP neighbor(solTIP sol);
    double evaluate(solTIP sol);
};

#endif