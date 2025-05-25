#ifndef INSTANCE_HPP
#define INSTANCE_HPP

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <iostream>
#include <unordered_map>

class Instance {
public:
    std::string instancePath;
    int tools;
    int slots;
    int HS_RSCost;
    std::vector<int> HS_RSSolution;
    std::vector<std::vector<int>> frequencyMatrix;

    Instance(const std::string& instancePath);
    int evaluate(const std::vector<int>& solution);
};

#endif // INSTANCE_HPP