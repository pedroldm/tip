#include "IOReport.hpp"

#include <cstdlib>
#include <iostream>
#include "json.hpp"

using json = nlohmann::json;

void IOReport::bestFitness() {
    std::cout << (int) this->algorithm->getBestFitness();
}

void IOReport::printIOReport() {
    json report;

    // BRKGA PARAMETERS
    report["BRKGA_PARAMETERS"] = {
        {"Filepath", this->filepath},
        {"Chromosome_size", this->n},
        {"Population_size", this->p},
        {"Elite_fraction", this->pe},
        {"Mutation_fraction", this->pm},
        {"Inheritance_probability", this->rhoe},
        {"Number_of_populations", this->K},
        {"Number_of_threads", this->MAXT},
        {"Exchange_interval", this->X_INTVL},
        {"Number_of_exchanges", this->X_NUMBER},
        {"Maximum_generations", this->MAX_GENS},
        {"Random_number_generator_seed", this->rngSeed},
        {"Current_generation", this->generation}};

    // INSTANCE
    report["INSTANCE"] = {
        {"Tools", this->decoder.tools},
        {"Slots", this->decoder.slots},
        {"SOA_Cost", this->decoder.SOACost},
        {"SOA_Solution", this->decoder.SOASolution},
        {"Instance", this->decoder.instance}};

    // TOP INDIVIDUALS
    json topIndividuals;
    const unsigned bound = std::min(p, unsigned(10));  // Limit to top 10 individuals
    for (unsigned i = 0; i < K; ++i) {
        json population;
        for (unsigned j = 0; j < bound; ++j) {
            population.push_back({{"rank", j},
                                  {"fitness", this->algorithm->getPopulation(i).getFitness(j)}});
        }
        topIndividuals["Population_" + std::to_string(i)] = population;
    }
    report["TOP_INDIVIDUALS"] = topIndividuals;

    // CONVERGENCE REPORT
    json convergenceReport;
    for (const auto& info : this->algorithm->convergenceInfo) {
        convergenceReport.push_back({{"generation", info.first},
                                     {"best_fitness", info.second}});
    }
    report["CONVERGENCE_REPORT"] = convergenceReport;

    // EXECUTION TIME REPORT
    std::chrono::duration<double> duration = this->finishTime - this->startTime;
    report["EXECUTION_TIME_REPORT"] = {
        {"duration_seconds", duration.count()}};

    // RESULT
    std::vector<int> bestSolution = this->decoder.outputDecode(this->algorithm->getBestChromosome());
    report["RESULT"] = {
        {"Best_Solution", bestSolution},
        {"Min_f(x)", this->algorithm->getBestFitness()}};

    // Print the JSON report
    std::cout << report.dump(4) << std::endl;  // Pretty-print with 4 spaces indentation
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
        else if (arg.find("--irace=") == 0) {
            std::string value = arg.substr(8);
            this->irace = (value == "true");
        }
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