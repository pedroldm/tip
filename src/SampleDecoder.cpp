/*
 * SampleDecoder.cpp
 *
 * For more information, see SampleDecoder.h
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
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
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

#include "SampleDecoder.hpp"

SampleDecoder::SampleDecoder() {}

SampleDecoder::~SampleDecoder() {}

SampleDecoder::SampleDecoder(int tools, int slots, std::vector<int> solution, std::vector<int> SOASolution,
                             std::vector<std::vector<int>> instance, int SOACost)
    : tools(tools),
      slots(slots),
      SOACost(SOACost),
      SOASolution(SOASolution),
      instance(move(instance)) {}

SampleDecoder::SampleDecoder(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file) {
        std::cerr << "Error: Could not open file " << filepath << std::endl;
        exit(1);
    }

    file >> this->tools >> this->slots;

    this->SOASolution.resize(this->slots);
    for (int i = 0; i < this->slots; i++) {
        std::string value;
        file >> value;
        SOASolution[i] = (value == "x") ? -1 : stoi(value) - 1;
    }

    file >> this->SOACost;

    this->instance.resize(this->tools, std::vector<int>(this->tools));
    for (int i = 0; i < this->tools; i++) {
        for (int j = 0; j < this->tools; ++j) {
            file >> this->instance[i][j];
        }
    }
}

double SampleDecoder::fitness(const std::vector<int> solution) const {
    std::unordered_map<int, int> toolIndex;

    for (int i = 0; i < this->tools; i++) {
        toolIndex[solution[i]] = i;
    }

    double totalCost = 0;

    for (int i = 0; i < this->tools; i++) {
        for (int j = i + 1; j < this->tools; j++) {
            if (!this->instance[i][j]) continue;
            int dist = abs(toolIndex[i] - toolIndex[j]);
            int circularDist = std::min(dist, (this->tools - dist) + (this->slots - this->tools));
            totalCost += this->instance[i][j] * circularDist;
        }
    }

    return totalCost;
}

Chromosome SampleDecoder::encode(const std::vector<int>& solution, Chromosome& originalChromosome) const {
    std::sort(originalChromosome.chromosome.begin(), originalChromosome.chromosome.end());

    Chromosome newChromosome(std::vector<double>(this->tools));
    for(size_t i = 0; i < this->tools; i++) {
        newChromosome.chromosome[solution[i]] = originalChromosome.chromosome[i];
    }

    return newChromosome;
}

double SampleDecoder::decode(const Chromosome& chromosome) const {
    std::vector<std::tuple<double, int>> chromosomeIndex(this->tools);
    for (int i = 0; i < this->tools; ++i) {
        chromosomeIndex[i] = std::make_tuple(chromosome.chromosome[i], i);
    }
    std::sort(chromosomeIndex.begin(), chromosomeIndex.end());

    std::vector<int> decodedSolution(this->tools);
    for (int i = 0; i < this->tools; ++i) {
        decodedSolution[i] = std::get<1>(chromosomeIndex[i]);
    }

    return this->fitness(decodedSolution);
}

std::vector<int> SampleDecoder::outputDecode(const Chromosome& chromosome) {
    std::unordered_map<double, std::vector<int>> valueToIndicesMap;
    Chromosome sortedChromosome(chromosome.chromosome);
    
    std::sort(sortedChromosome.chromosome.begin(), sortedChromosome.chromosome.end());
    
    std::vector<int> decodedSolution(this->tools);

    for (size_t i = 0; i < chromosome.chromosome.size(); ++i) {
        valueToIndicesMap[chromosome.chromosome[i]].push_back(i);
    }

    for (size_t i = 0; i < this->tools; ++i) {
        double value = sortedChromosome.chromosome[i];
        int index = valueToIndicesMap[value].back();
        valueToIndicesMap[value].pop_back();
        decodedSolution[index] = i;
    }

    return decodedSolution;
}
