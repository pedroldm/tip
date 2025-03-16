/**
 * api-usage.cpp
 *
 * Driver class with a simple example of how to instantiate and use the BRKGA API.
 * See SampleDecoder.h for details on the decoder's implementation.
 *
 * Created on : Nov 17, 2011 by rtoso
 * Authors    : Rodrigo Franco Toso <rtoso@cs.rutgers.edu>
 *              Mauricio G.C. Resende <mgcr@research.att.com>
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2018
 * Rodrigo Franco Toso (rfrancotoso@gmail.com) and
 * Mauricio G.C. Resende
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */

#include <algorithm>
#include <iostream>

#include "SampleDecoder.h"
#include "brkgaAPI/BRKGA.h"
#include "brkgaAPI/MTRand.h"

int main(int argc, char* argv[]) {
    std::cout << "Welcome to the BRKGA API sample driver.\nFinding a (heuristic) minimizer for "
              << " f(x) = sum_i (x_i * i) where x \\in [0,1)^n." << std::endl;

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

    if (argc < 2) {
        std::cerr << "Error: Missing mandatory parameter --filepath=<path>\n";
        return 1;
    }

    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        if (arg.find("--filepath=") == 0)
            filepath = arg.substr(11);
        else if (arg.find("--p=") == 0)
            p = std::atoi(arg.substr(4).c_str());
        else if (arg.find("--pe=") == 0)
            pe = std::atof(arg.substr(5).c_str());
        else if (arg.find("--pm=") == 0)
            pm = std::atof(arg.substr(5).c_str());
        else if (arg.find("--rhoe=") == 0)
            rhoe = std::atof(arg.substr(7).c_str());
        else if (arg.find("--K=") == 0)
            K = std::atoi(arg.substr(4).c_str());
        else if (arg.find("--MAXT=") == 0)
            MAXT = std::atoi(arg.substr(7).c_str());
        else if (arg.find("--X_INTVL=") == 0)
            X_INTVL = std::atoi(arg.substr(10).c_str());
        else if (arg.find("--X_NUMBER=") == 0)
            X_NUMBER = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--MAX_GENS=") == 0)
            MAX_GENS = std::atoi(arg.substr(11).c_str());
        else if (arg.find("--rngSeed=") == 0)
            rngSeed = std::strtoul(arg.substr(10).c_str(), nullptr, 10);
    }

    // Ensure the filepath is provided
    if (filepath.empty()) {
        std::cerr << "Error: Missing mandatory parameter --filepath=<path>\n";
        return 1;
    }

    MTRand rng(rngSeed);              // initialize the random number generator
    SampleDecoder decoder(filepath);  // initialize the decoder
    n = decoder.slots;
    BRKGA<SampleDecoder, MTRand> algorithm(n, p, pe, pm, rhoe, decoder, rng, K, MAXT);

    std::cout << "Running for " << MAX_GENS << " generations..." << std::endl;
    do {
        algorithm.evolve();  // evolve the population for one generation

        if ((++generation) % X_INTVL == 0) {
            algorithm.exchangeElite(X_NUMBER);  // exchange top individuals
        }
    } while (generation < MAX_GENS);

    // print the fitness of the top 10 individuals of each population:
    std::cout << "Fitness of the top 10 individuals of each population:" << std::endl;
    const unsigned bound = std::min(p, unsigned(10));  // makes sure we have 10 individuals
    for (unsigned i = 0; i < K; ++i) {
        std::cout << "Population #" << i << ":" << std::endl;
        for (unsigned j = 0; j < bound; ++j) {
            std::cout << "\t" << j << ") "
                      << algorithm.getPopulation(i).getFitness(j) << std::endl;
        }
    }

    std::cout << "Best solution found has objective value = "
              << algorithm.getBestFitness() << std::endl;

    return 0;
}