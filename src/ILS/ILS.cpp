#include "ILS.hpp"

#include "VND.hpp"

ILS::ILS(Instance& instance, InOut& io, VND& vnd)
    : instance(instance), io(io), vnd(vnd) {
    for (int i = 0; i < instance.tools; i++) {
        for (int j = i + 1; j < instance.tools; j++) {
            searchPairs.emplace_back(i, j);
            reverseSearchPairs.emplace_back(j, i);
        }
    }
}

void ILS::run() {
    std::vector<int> solution = createInitialSolution();
    int minFx = instance.evaluate(solution);
    int currentBest = minFx;
    bestSolution = solution;
    int currIteration = 0;

    auto startTime = std::chrono::steady_clock::now();

    std::unordered_set<size_t> tabuHashSet;
    tabuHashSet.insert(vectorHash(solution));

    while (currIteration < io.maxIterations) {
        auto currentTime = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTime - startTime;
        if (elapsed.count() / 60.0 >= io.maxTime) {
            break;
        }

        std::shuffle(searchPairs.begin(), searchPairs.end(),
                  std::mt19937(std::random_device()()));
        std::shuffle(reverseSearchPairs.begin(), reverseSearchPairs.end(),
                  std::mt19937(std::random_device()()));

        std::vector<std::tuple<int, std::vector<int>>> neighborhood(
            io.maxThreads);

#ifdef _OPENMP
#pragma omp parallel for num_threads(io.maxThreads)
#endif
        for (int i = 0; i < io.maxThreads; i++) {
            std::vector<int> newSolution = disturb(solution);
            auto [newCost, improvedSolution, localSearchImprovements] =
                vnd.run(newSolution, searchPairs, reverseSearchPairs,
                        (int)instance.tools * io.lsCoverage);

            neighborhood[i] = std::make_tuple(newCost, improvedSolution);
        }

        std::sort(neighborhood.begin(), neighborhood.end(),
                  [](const auto& a, const auto& b) {
                      return std::get<0>(a) < std::get<0>(b);
                  });

        for (const auto& [neighborCost, neighborSolution] : neighborhood) {
            size_t hash = vectorHash(neighborSolution);
            if (tabuHashSet.find(hash) == tabuHashSet.end()) {
                solution = neighborSolution;
                currentBest = neighborCost;
                tabuHashSet.insert(hash);
                break;
            }
        }

        std::cout << "Iteração [" << currIteration
                  << "] : " << std::get<0>(neighborhood[0]) << std::endl;

        if (currentBest < instance.evaluate(bestSolution)) {
            std::cout << "Novo melhor [" << currIteration
                      << "] : " << currentBest << std::endl;
            bestSolution = solution;
            minFx = currentBest;
        }

        currIteration++;
    }
}

std::vector<int> ILS::disturb(std::vector<int>& solution) {
    std::vector<int> newSolution = solution;

    for (int i = 0; i < (int)io.alpha * instance.tools; i++) {
        int index = rand() % instance.tools;
        int newIndex = rand() % instance.tools;
        std::swap(newSolution[index], newSolution[newIndex]);
    }

    for (int i = 0; i < static_cast<int>(io.alpha * instance.tools); i++) {
        int index = rand() % instance.tools;
        int newIndex = rand() % (instance.tools - 1);
        int element = newSolution[index];
        newSolution.erase(newSolution.begin() + index);
        newSolution.insert(newSolution.begin() + newIndex, element);
    }

    return newSolution;
}

size_t ILS::vectorHash(const std::vector<int>& vec) {
    size_t seed = vec.size();
    for (int i : vec) {
        seed ^= std::hash<int>{}(i) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }
    return seed;
}

std::vector<int> ILS::createInitialSolution() {
    std::vector<int> solution(instance.tools);
    std::iota(solution.begin(), solution.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(solution.begin(), solution.end(), g);
    return solution;
}