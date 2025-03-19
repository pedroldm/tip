#include "IOReport.h"

#include <cstdlib>
#include <iostream>

void IOReport::printIOReport() {
    std::cout << "# ---------------- BRKGA PARAMETERS" << std::endl;
    std::cout << "Filepath: " << this->filepath << std::endl;
    std::cout << "Chromosome size (n): " << this->n << std::endl;
    std::cout << "Population size (p): " << this->p << std::endl;
    std::cout << "Elite fraction (pe): " << this->pe << std::endl;
    std::cout << "Mutation fraction (pm): " << this->pm << std::endl;
    std::cout << "Inheritance probability (rhoe): " << this->rhoe << std::endl;
    std::cout << "Number of populations (K): " << this->K << std::endl;
    std::cout << "Number of threads (MAXT): " << this->MAXT << std::endl;
    std::cout << "Exchange interval (X_INTVL): " << this->X_INTVL << std::endl;
    std::cout << "Number of exchanges (X_NUMBER): " << this->X_NUMBER << std::endl;
    std::cout << "Maximum generations (MAX_GENS): " << this->MAX_GENS << std::endl;
    std::cout << "Random number generator seed (rngSeed): " << this->rngSeed << std::endl;
    std::cout << "Current generation: " << this->generation << std::endl << std::endl;

    std::cout << "# ---------------- INSTANCE" << std::endl;
    std::cout << "Tools: " << this->decoder.tools << std::endl;
    std::cout << "Slots: " << this->decoder.slots << std::endl;
    std::cout << "SOA Cost: " << this->decoder.SOACost << std::endl;
    std::cout << "SOA Solution: ";
    for (int i = 0; i < this->decoder.SOASolution.size(); ++i) {
        std::cout << (this->decoder.SOASolution[i] == -1 ? std::string("X") : std::to_string(this->decoder.SOASolution[i]));
        if (i < this->decoder.SOASolution.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Instance: " << std::endl;
    for (size_t i = 0; i < this->decoder.instance.size(); ++i) {
        for (size_t j = 0; j < this->decoder.instance[i].size(); ++j) {
            std::cout << this->decoder.instance[i][j] << "\t";  // Using tab to separate values
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    std::cout << "# ---------------- TOP INDIVIDUALS" << std::endl;
    const unsigned bound = std::min(p, unsigned(10));  // makes sure we have 10 individuals
    for (unsigned i = 0; i < K; ++i) {
        std::cout << "Population #" << i << ":" << std::endl;
        for (unsigned j = 0; j < bound; ++j) {
            std::cout << "\t" << j << ") "
                      << this->algorithm->getPopulation(i).getFitness(j)
                      << std::endl;
        }
    }

    std::cout << std::endl <<  "# ---------------- CONVERGENCE REPORT" << std::endl;
    for (const auto& info : this->algorithm->convergenceInfo) {
        std::cout << "Generation:" << "\t" <<  info.first
                  << "\t" << "Best Fitness:" << "\t" << info.second << std::endl;
    }

    std::cout << std::endl <<  "# ---------------- EXECUTION TIME REPORT" << std::endl;
    std::chrono::duration<double> duration = this->finishTime - this->startTime;
    std::cout << duration.count() << " seconds." << std::endl;

    std::cout << std::endl <<  "# ---------------- RESULT" << std::endl;
    std::vector<int> bestSolution = this->decoder.outputDecode(this->algorithm->getBestChromosome());
    std::cout << "Best Solution: ";
    for (int i = 0; i < bestSolution.size(); ++i) {
        std::cout << (bestSolution[i] == -1 ? std::string("X") : std::to_string(bestSolution[i]));
        if (i < bestSolution.size() - 1) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;
    std::cout << "Min f(x): " << this->algorithm->getBestFitness() << std::endl;
}

void IOReport::run() {
    this->startTime = std::chrono::high_resolution_clock::now();
    do {
        this->algorithm->evolve();  // evolve the population for one generation
        this->algorithm->registerConvergence(this->generation);
        if ((++this->generation) % this->X_INTVL == 0) {
            this->algorithm->exchangeElite(
                this->X_NUMBER);  // exchange top individuals
        }
    } while (this->generation < this->MAX_GENS);
    this->finishTime = std::chrono::high_resolution_clock::now();
}

IOReport::IOReport(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Error: Missing mandatory parameter --filepath=<path>\n";
        exit(EXIT_FAILURE);
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--filepath=") == 0)
            this->filepath = arg.substr(11);
        else if (arg.find("--p=") == 0)
            this->p = std::atoi(arg.substr(4).c_str());
        else if (arg.find("--pe=") == 0)
            this->pe = std::atof(arg.substr(5).c_str());
        else if (arg.find("--pm=") == 0)
            this->pm = std::atof(arg.substr(5).c_str());
        else if (arg.find("--rhoe=") == 0)
            this->rhoe = std::atof(arg.substr(7).c_str());
        else if (arg.find("--K=") == 0)
            this->K = std::atoi(arg.substr(4).c_str());
        else if (arg.find("--MAXT=") == 0)
            this->MAXT = std::atoi(arg.substr(7).c_str());
        else if (arg.find("--X_INTVL=") == 0)
            this->X_INTVL = std::atoi(arg.substr(10).c_str());
        else if (arg.find("--X_NUMBER=") == 0)
            this->X_NUMBER = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--MAX_GENS=") == 0)
            this->MAX_GENS = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--rngSeed=") == 0)
            this->rngSeed = std::strtoul(arg.substr(10).c_str(), nullptr, 10);
    }

    if (this->filepath.empty()) {
        std::cerr << "Error: Missing mandatory parameter --filepath=<path>\n";
        exit(EXIT_FAILURE);
    }

    this->rng = new MTRand(this->rngSeed);
    this->decoder = SampleDecoder(this->filepath);
    this->n = this->decoder.slots;
    this->algorithm = new BRKGA<SampleDecoder, MTRand>(
        this->n, this->p, this->pe, this->pm, this->rhoe, this->decoder,
        *this->rng, this->K, this->MAXT);
}