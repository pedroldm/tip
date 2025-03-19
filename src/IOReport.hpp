#include "SampleDecoder.hpp"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"
#include <chrono>

class IOReport {
    public:
        unsigned n;                 // tamanho do cromossomo
        unsigned p = 100;           // tamanho da população
        double pe = 0.10;           // fração de elite
        double pm = 0.10;           // fração de mutantes
        double rhoe = 0.70;         // probabilidade de herança
        unsigned K = 3;             // número de populações
        unsigned MAXT = 1;          // número de threads
        unsigned X_INTVL = 100;     // intervalo de troca
        unsigned X_NUMBER = 2;      // número de trocas
        unsigned MAX_GENS = 1000;   // número máximo de gerações
        long unsigned rngSeed = 0;  // semente do gerador de números aleatórios
        unsigned generation = 0;
        std::string filepath;

        BRKGA<SampleDecoder, MTRand>* algorithm;
        MTRand* rng;
        SampleDecoder decoder;

        std::chrono::time_point<std::chrono::high_resolution_clock> startTime;
        std::chrono::time_point<std::chrono::high_resolution_clock> finishTime;

        IOReport(int argc, char* argv[]);
        ~IOReport() = default;

        void printIOReport();
        void run();
};