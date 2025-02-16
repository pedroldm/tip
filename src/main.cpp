#include <iostream>
#include "Instance.h"

using namespace std;

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " <instance_filename>" << endl;
        return 1;
    }

    string filename = argv[1];

    Instance instance(filename);
    instance.print();
    cout << "Cost: " << instance.evaluate() << endl;

    return 0;
}
