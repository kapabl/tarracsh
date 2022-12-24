#include <fstream>
#include <iostream>
#include "TarracshApp.h"


using namespace std;
using namespace std::filesystem;
using namespace org::kapa::tarracsh;

/**
 *
 */
int main(int argc, char *argv[]) {

    const auto result = TarracshApp::run(argc, argv);
    return result;
}   
