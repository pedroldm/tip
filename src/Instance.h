#ifndef INSTANCE_H
#define INSTANCE_H

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

class Instance {
   public:
    int tools;
    int slots;
    int SOACost;
    vector<int> solution;
    vector<vector<int>> instance;

    Instance(int tools, int slots, vector<int> solution,
             vector<vector<int>> instance, int SOACost = 0);

    Instance(const string& filename);

    void print();
    int evaluate();
};

#endif
