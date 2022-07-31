#include "JarAnalyzer.h"

#include <iostream>
#include <utility>

using namespace org::kapa::tarracsh::jar;
using namespace std;

JarAnalyzer::JarAnalyzer(Options options)
    :  _options(std::move(options)) {


}

void JarAnalyzer::run() {
    //TODO unzip and process each file
}

void JarAnalyzer::output() {
    cout << "TODO: Jar File" << _options.jarFile << endl;
}

unsigned int JarAnalyzer::getClassfileCount() {
    return _classfileCount;
}
