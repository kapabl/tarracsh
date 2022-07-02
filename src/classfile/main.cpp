#include <fstream>
#include <iostream>

using namespace std;
using namespace std::filesystem;

#include "ClassFileParser.h"

using namespace org::kapa::tarrash;

int main(int argc, char *argv[]) {
    cout << "Tarrash...." << endl;
    cout << endl;
    if (argc < 1) {
        cout << "Expecting class file" << endl;
        return 1;
    }

    ClassFileParser classFileParser(argv[1]);
    classFileParser.output();
#ifdef _DEBUG
    cin.get();
#endif 

    return 0;
}