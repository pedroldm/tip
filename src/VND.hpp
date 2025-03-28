#ifndef VND_HPP
#define VND_HPP

#include <algorithm>
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <unordered_map>

#include "SampleDecoder.hpp"

class VND {
    public:
        int maxIterations;
        SampleDecoder sampleDecoder;

        VND();
        VND(SampleDecoder sampleDecoder, int maxIterations);
        ~VND();

        Chromosome VNDSearch(Chromosome& chromosome);
        bool TwoOPTLocalSearch(std::vector<int>& solution, double& currentBest);
        bool swapLocalSearch(std::vector<int>& solution, double& currentBest);
        bool reinsertionLocalSearch(std::vector<int>& solution, double& currentBest);
};

#endif  // VND_HPP
