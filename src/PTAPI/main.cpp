#include "include/PT.h"
#include "TIPPermutation.hpp"

int main(int argc, char* argv[]) {
    TIPPermutation* prob = new TIPPermutation("/home/pedro/Pessoal/MSc/tip/instances/sko/sko-100.txt");
    PT<solTIP> algo(0.01, 50, 15, 800, 4000, 1, 3, 400);
    solTIP sol = algo.start(15, prob);
    std::cout << sol.evalSol;
}