#include "JarParser.h"

#include <utility>

using namespace org::kapa::tarrash::jar;
using namespace std;

JarParser::JarParser(string fileName, string classPath)
    : _fileName(move(fileName)), _classPath(move(classPath)) {

    //TODO unzip and process each file
}

void JarParser::run() {
    //TODO
}
