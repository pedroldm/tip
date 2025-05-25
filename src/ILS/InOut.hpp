// filepath: /home/pedro/Pessoal/MSc/tip/src/ILS/InOut.hpp
#ifndef IO_HPP
#define IO_HPP

#include <string>
#include <unordered_map>
#include <stdexcept>

class InOut {
public:
    std::string instancePath;
    int maxIterations;
    int maxTime;
    int maxIterationsWithoutImprovement;
    int maxThreads;
    float lsCoverage;
    double alpha;

    InOut(int argc, char* argv[]);
};

#endif // IO_HPP