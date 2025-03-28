#ifndef POPULATION_H
#define POPULATION_H

#include <vector>
#include <algorithm>
#include <exception>
#include <stdexcept>
#include "../Chromosome.hpp"

class Population {
    template<class Decoder, class RNG>
    friend class BRKGA;

public:
    unsigned getN() const;  // Size of each chromosome
    unsigned getP() const;  // Size of population

    double getBestFitness() const;
    double getFitness(unsigned i) const;
    const Chromosome& getChromosome(unsigned i) const;

private:
    Population(const Population& other);
    Population(unsigned n, unsigned p);
    ~Population();

    std::vector<Chromosome> population;
    std::vector<std::pair<double, unsigned>> fitness;

    void sortFitness();
    void setFitness(unsigned i, double f);
    Chromosome& getChromosome(unsigned i);

    double& operator()(unsigned i, unsigned j);
    Chromosome& operator()(unsigned i);
};

#endif
