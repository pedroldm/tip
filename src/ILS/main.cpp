#include <iostream>
#include <stdexcept>
#include <string>
#include <numeric>

#include "InOut.hpp"
#include "Instance.hpp"
#include "VND.hpp"
#include <random>
#include "ILS.hpp"

int main(int argc, char* argv[]) {
    InOut io = InOut(argc, argv);
    Instance instance = Instance(io.instancePath);
    VND vnd = VND(instance, io);
    ILS ils = ILS(instance, io, vnd);
    ils.run();
    std::cout << "Best fitness: " << instance.evaluate(ils.bestSolution) << std::endl;
    std::cout << "HS_RSCost: " << instance.HS_RSCost << std::endl;
    return 0;
}