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

    const auto exitCode = TarracshApp::run(argc, argv);
      
#if defined(_DEBUG) || true
    cin.get();
#endif


    return exitCode;
}
