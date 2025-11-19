#include "interfaces/tarracsh/TarracshCli.h"



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


using namespace kapa::tarracsh;
using namespace app;

/**
 *
 */
int main(int argc, char *argv[]) {
    const auto result = TarracshCli::run(argc, argv);
    return result;
}   
