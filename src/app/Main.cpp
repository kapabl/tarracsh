#include "TarracshApp.h"



// #ifndef _DEBUG
#pragma comment(linker, "/include:__tcmalloc")
#pragma comment(lib, "libtcmalloc_minimal.lib")
// #endif


using namespace org::kapa::tarracsh;

/**
 *
 */
int main(int argc, char *argv[]) {
    //mi_version();
    const auto result = TarracshApp::run(argc, argv);
    return result;
}   
