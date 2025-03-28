#ifndef CHROMOSOME_HPP
#define CHROMOSOME_HPP

#include <vector>

class Chromosome {
   public:
    std::vector<double> chromosome;
    bool refined = false;

    Chromosome(std::vector<double> chromosome);
    Chromosome();
    ~Chromosome();
};

#endif