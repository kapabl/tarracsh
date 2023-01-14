#include "App.h"



// #ifndef _DEBUG
#pragma comment(linker, "/include:__tcmalloc")
#pragma comment(lib, "libtcmalloc_minimal.lib")
// #endif


using namespace kapa::tarracsh;
using namespace app;

/**
 *
 */
int main(int argc, char *argv[]) {
    const auto result = App::run(argc, argv);
    return result;
}   
