
#include <iostream>
#include <iomanip>
#include <string>
#include <filesystem>
#include <cstdint>
#include <vector>
#include <inttypes.h>
#include <cassert>
#include <fstream>


using namespace std;
using namespace std::filesystem;

#include "includes/classfile_constants.h"
#include "./ClassFileParser.h"



int main( int argc, char* argv[] ) {
    cout << "Tarrash" << endl;
    cout << endl;

    ClassFileParser classFileParser("main.class");
    classFileParser.output();


    return 0;
}