#include "IOReport.hpp"

#include <cstdlib>
#include <iostream>

#include "json.hpp"

using json = nlohmann::json;

void IOReport::bestFitness() {
    std::cout << (int)this->algorithm->getBestFitness();
}

void IOReport::printIOReport() {
    json report;

    report["brkgaParameters"] = {
        {"filepath", this->filepath},
        {"chromosomeSize", this->n},
        {"populationSize", this->p},
        {"eliteFraction", this->pe},
        {"mutationFraction", this->originalPm},
        {"inheritanceProbability", this->rhoe},
        {"numberOfPopulations", this->K},
        {"numberOfThreads", this->MAXT},
        {"exchangeInterval", this->X_INTVL},
        {"numberOfExchanges", this->X_NUMBER},
        {"maximumGenerations", this->MAX_GENS},
        {"randomNumberGeneratorSeed", this->rngSeed},
        {"currentGeneration", this->generation},
        {"maxIterations", this->maxIterations},
        {"useVND", this->useVND},
        {"irace", this->irace},
        {"timeLimit", this->timeLimit},
        {"lsCoveragePercentage", this->lsCoveragePercentage},
        {"lsEliteApplicationPercentage", this->lsEliteApplicationPercentage},
        {"lsNonEliteApplicationPercentage", this->lsNonEliteApplicationPercentage},
        {"lastPm", this->algorithm->getPm()},
        {"pmIncreaseFactor", this->PM_INCREASE_FACTOR},
        {"maxGensWithoutImprovement", this->MAX_GENS_WITHOUT_IMPROVEMENT},
        {"gensWithoutImprovement", this->GENS_WITHOUT_IMPROVEMENT}
    };

    // INSTANCE
    report["instance"] = {
        {"tools", this->decoder.tools},
        {"slots", this->decoder.slots},
        {"soaCost", this->decoder.SOACost},
        {"soaSolution", this->decoder.SOASolution},
        {"instance", this->decoder.instance}
    };

    // TOP INDIVIDUALS
    json topIndividuals;
    const unsigned bound = std::min(p, unsigned(10));
    for (unsigned i = 0; i < K; ++i) {
        json population;
        for (unsigned j = 0; j < bound; ++j) {
            population.push_back({
                {"rank", j},
                {"fitness", this->algorithm->getPopulation(i).getFitness(j)}
            });
        }
        topIndividuals["population"] = population;
    }
    report["topIndividuals"] = topIndividuals;

    // CONVERGENCE REPORT
    json convergenceReport;
    for (const auto& info : this->algorithm->convergenceInfo) {
        convergenceReport.push_back({
            {"generation", info.first},
            {"bestFitness", info.second}
        });
    }
    report["convergenceReport"] = convergenceReport;

    // LOCAL SEARCH REPORT
    report["localSearchReport"] = {
        {"swap", this->algorithm->lsImprovements[0]},
        {"twoOpt", this->algorithm->lsImprovements[1]},
        {"reinsertion", this->algorithm->lsImprovements[2]}
    };

    // EXECUTION TIME REPORT
    std::chrono::duration<double> duration = this->finishTime - this->startTime;
    report["executionTimeReport"] = {
        {"durationSeconds", duration.count()}
    };

    // RESULT
    std::vector<int> bestSolution = this->decoder.outputDecode(this->algorithm->getBestChromosome());
    report["result"] = {
        {"bestSolution", bestSolution},
        {"minFx", this->algorithm->getBestFitness()}
    };

    // Print the JSON report
    std::cout << report.dump(4) << std::endl;  // Pretty-print with 4 spaces indentation
}

void IOReport::run() {
    this->startTime = std::chrono::high_resolution_clock::now();
    const auto maxDuration = std::chrono::minutes(this->timeLimit);
    do {
        this->algorithm->evolve(this->useVND);
        if(this->algorithm->registerConvergence(this->generation)) {
            this->GENS_WITHOUT_IMPROVEMENT = 0;
            this->pm = this->originalPm;
            this->algorithm->setPm((unsigned)this->p * this->originalPm);
        } else {
            this->GENS_WITHOUT_IMPROVEMENT++;
            this->pm = this->pm * (1 + this->PM_INCREASE_FACTOR);
            this->algorithm->setPm((unsigned)std::min(this->p * this->pm, static_cast<double>(this->p) / 2));
            if (this->GENS_WITHOUT_IMPROVEMENT >= this->MAX_GENS_WITHOUT_IMPROVEMENT) {
                break;
            }
        }
        if ((++this->generation) % this->X_INTVL == 0) {
            this->algorithm->exchangeElite(
                this->X_NUMBER);
        }
        if (std::chrono::high_resolution_clock::now() - this->startTime >
            maxDuration)
            break;
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
        else if (arg.find("--pm=") == 0) {
            this->pm = std::atof(arg.substr(5).c_str());
            this->originalPm = this->pm;
        }
        else if (arg.find("--rhoe=") == 0)
            this->rhoe = std::atof(arg.substr(7).c_str());
        else if (arg.find("--K=") == 0)
            this->K = std::atoi(arg.substr(4).c_str());
        else if (arg.find("--timeLimit=") == 0)
            this->timeLimit = std::atoi(arg.substr(12).c_str());
        else if (arg.find("--maxIterations=") == 0)
            this->maxIterations = std::atoi(arg.substr(16).c_str());
        else if (arg.find("--MAXT=") == 0)
            this->MAXT = std::atoi(arg.substr(7).c_str());
        else if (arg.find("--PM_INCREASE_FACTOR=") == 0)
            this->PM_INCREASE_FACTOR = std::atof(arg.substr(21).c_str());
        else if (arg.find("--lsCoveragePercentage=") == 0)
            this->lsCoveragePercentage = std::atof(arg.substr(23).c_str());
        else if (arg.find("--lsEliteApplicationPercentage=") == 0)
            this->lsEliteApplicationPercentage =
                std::atof(arg.substr(31).c_str());
        else if (arg.find("--lsNonEliteApplicationPercentage=") == 0)
            this->lsNonEliteApplicationPercentage =
                std::atof(arg.substr(33).c_str());
        else if (arg.find("--X_INTVL=") == 0)
            this->X_INTVL = std::atoi(arg.substr(10).c_str());
        else if (arg.find("--X_NUMBER=") == 0)
            this->X_NUMBER = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--MAX_GENS=") == 0)
            this->MAX_GENS = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--MAX_GENS_WITHOUT_IMPROVEMENT=") == 0)
            this->MAX_GENS_WITHOUT_IMPROVEMENT = std::atoi(arg.substr(31).c_str());
        else if (arg.find("--rngSeed=") == 0)
            this->rngSeed = std::strtoul(arg.substr(10).c_str(), nullptr, 10);
        else if (arg.find("--irace=") == 0) {
            std::string value = arg.substr(8);
            this->irace = (value == "true");
        } else if (arg.find("--useVND=") == 0) {
            std::string value = arg.substr(9);
            this->useVND = (value == "true");
        }
    }

    if (this->filepath.empty()) {
        std::cerr << "Error: Missing mandatory parameter --filepath=<path>\n";
        exit(EXIT_FAILURE);
    }

    this->rng = new MTRand(this->rngSeed);
    this->decoder = SampleDecoder(this->filepath);
    this->n = this->decoder.tools;
    this->vnd = VND(this->decoder, this->maxIterations);
    this->algorithm = new BRKGA<SampleDecoder, MTRand>(
        this->n, this->p, this->pe, this->pm, this->rhoe,
        this->lsCoveragePercentage, this->lsEliteApplicationPercentage,
        this->lsNonEliteApplicationPercentage, this->decoder, *this->rng,
        this->vnd, this->K, this->MAXT);
}