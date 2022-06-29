
#include <cassert>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <inttypes.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <vector>

using namespace std;
using namespace std::filesystem;

#include "includes/classfile_constants.h"
#include "ClassFileStructure.h"
#include "ConstantPool.h"
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

    return 0;
}