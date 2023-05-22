#include "App.h"



#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma clang diagnostic ignored "-Wpragma-pack"
#endif

extern "C" {
__attribute__((weak)) void __tcmalloc();
}

#ifdef __clang__
#pragma clang diagnostic pop
#endif


#ifdef _MSC_VER
    // #ifndef _DEBUG
    #pragma comment(linker, "/include:__tcmalloc")
//    #pragma comment(lib, "libtcmalloc_minimal.lib")
    // #endif
#endif

using namespace kapa::tarracsh;
using namespace app;

/**
 *
 */
int main(int argc, char *argv[]) {
    const auto result = App::run(argc, argv);
    return result;
}   
