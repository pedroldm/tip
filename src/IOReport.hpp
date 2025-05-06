#ifndef IOREPORT_HPP
#define IOREPORT_HPP

#include "SampleDecoder.hpp"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include "VND.hpp"
#include <chrono>
#include <cmath>

class IOReport {
    public:
        unsigned n;                 // tamanho do cromossomo
        unsigned p = 100;           // tamanho da população
        double pe = 0.10;           // fração de elite
        double pm = 0.10;           // fração de mutantes
        double originalPm = 0.10;
        double rhoe = 0.70;         // probabilidade de herança
        unsigned K = 3;             // número de populações
        unsigned MAXT = 1;          // número de threads
        unsigned X_INTVL = 100;     // intervalo de troca
        unsigned X_NUMBER = 2;      // número de trocas
        unsigned MAX_GENS = 1000;   // número máximo de gerações
        unsigned MAX_GENS_WITHOUT_IMPROVEMENT = 1000; // número máximo de gerações sem melhoria
        unsigned GENS_WITHOUT_IMPROVEMENT = 0;
        float PM_INCREASE_FACTOR = 0.0; // fator de aumento da fração de mutantes
        long unsigned rngSeed = 0;  // semente do gerador de números aleatórios
        unsigned generation = 0;
        int maxIterations = 1000;
        bool useVND = true;
        bool irace = false;
        short timeLimit = 120;
        double lsCoveragePercentage = 1.0;
        double lsEliteApplicationPercentage = 1.0;
        double lsNonEliteApplicationPercentage = 0.05;
        std::string filepath;

        BRKGA<SampleDecoder, MTRand>* algorithm;
        MTRand* rng;
        SampleDecoder decoder;
        VND vnd;

        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> finishTime;

        IOReport(int argc, char* argv[]);
        ~IOReport() = default;

        void printIOReport();
        void run();
        void bestFitness();
};

#endif  // IOREPORT_HPP