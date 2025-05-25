#include "InOut.hpp"
#include <iostream>
#include <cstdlib>
#include <string>
#include <stdexcept>

InOut::InOut(int argc, char* argv[]) {
    if (argc < 7) {
        throw std::invalid_argument("Usage: <program> --filePath=<instancePath> --maxIterations=<maxIterations> --maxTime=<maxTime> --maxIterationsWithoutImprovement=<maxIterationsWithoutImprovement> --maxThreads=<maxThreads> --alpha=<alpha>");
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg.find("--filePath=") == 0) {
            instancePath = arg.substr(11);
        } else if (arg.find("--maxIterations=") == 0) {
            maxIterations = std::stoi(arg.substr(16));
        } else if (arg.find("--maxTime=") == 0) {
            maxTime = std::stoi(arg.substr(10));
        } else if (arg.find("--maxThreads=") == 0) {
            maxThreads = std::stoi(arg.substr(13));
        } else if (arg.find("--maxIterationsWithoutImprovement=") == 0) {
            maxIterationsWithoutImprovement = std::stoi(arg.substr(34));
        } else if (arg.find("--lsCoverage=") == 0) {
            lsCoverage = std::stof(arg.substr(13));
        } else if (arg.find("--alpha=") == 0) {
            alpha = std::stod(arg.substr(8));
        } else {
            throw std::invalid_argument("Unknown argument: " + arg);
        }
    }
}