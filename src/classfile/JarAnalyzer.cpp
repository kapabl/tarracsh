#include "JarAnalyzer.h"

#include <utility>

using namespace org::kapa::tarrash::jar;
using namespace std;

JarAnalyzer::JarAnalyzer(string fileName, string classPath)
    : _fileName(move(fileName)), _classPath(move(classPath)) {

    //TODO unzip and process each file
}

void JarAnalyzer::run() {
    //TODO
}
