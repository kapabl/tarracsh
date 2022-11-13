#include "Stats.h"

using namespace org::kapa::tarracsh;
void Results::print(const Options &options) const {
    // cout << "\033[2K";

    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    std::cout << "classfiles: " <<
        classfiles.count + jarfiles.classfiles.count <<
        " jars: " << jarfiles.count;

    std::cout << "\r" << std::flush;
    
}

void Results::printAll(const Options& options) const {

    std::cout << std::endl << std::endl;
    std::cout << "classfiles:" << std::endl << std::right
        << std::setw(10) << "No"
        << std::setw(10) << "Ok"
        << std::setw(10) << "Error"
        << std::endl
        // ":\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m"
        // << std::setw(10) << std::format("\033[36m{}\033[39m", classfiles.count)
        // << std::setw(10) << std::format("\033[36m{}\033[92m", classfiles.parsedCount )
        // << std::setw(10) << std::format("\033[36m{}\033[31m", classfiles.errors )
        << std::setw(10) << classfiles.count
        << std::setw(10) << classfiles.parsedCount
        << std::setw(10) << classfiles.errors
        << std::endl;

    // std::cout << std::format("jars No:\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m",
    //                          jarfiles.count,
    //                          jarfiles.parsedCount,
    //                          jarfiles.errors) << std::endl;

    std::cout << "jars:" << std::endl << std::right
        << std::setw(10) << "No"
        << std::setw(10) << "Ok"
        << std::setw(10) << "Error"
        << std::endl
        << std::setw(10) << jarfiles.count
        << std::setw(10) << jarfiles.parsedCount
        << std::setw(10) << jarfiles.errors
        << std::endl;

    if (options.generatePublicDigest) {

        std::cout << "standalone classfile digest:" << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << classfiles.digest.count
            << std::setw(10) << classfiles.digest.newFile
            << std::setw(10) << classfiles.digest.same
            << std::setw(10) << classfiles.digest.differentDigest
            << std::setw(10) << classfiles.digest.unchangedCount
            << std::endl;

        std::cout << "jar digest:" << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << jarfiles.digest.count
            << std::setw(10) << jarfiles.digest.newFile
            << std::setw(10) << jarfiles.digest.same
            << std::setw(10) << jarfiles.digest.differentDigest
            << std::setw(10) << jarfiles.digest.unchangedCount
            << std::endl;
        /*
        std::cout << std::format("jar digest\t\t\t\t No:{}\tNew:{}\tSame:{}\tDiff:{}\tUnchanged:{}",
            jarfiles.digest.count,
            jarfiles.digest.newFile,

            jarfiles.digest.same,
            jarfiles.digest.differentDigest,
            jarfiles.digest.unchangedCount
        ) << std::endl;*/

        /*std::cout << std::format("classfile digest\t\t No:{}\tNew:{}\tSame:{}\tDiff:{}\tUnchanged:{}",
            jarfiles.classfiles.digest.count,
            jarfiles.classfiles.digest.newFile,
            jarfiles.classfiles.digest.same,
            jarfiles.classfiles.digest.differentDigest,
            jarfiles.classfiles.digest.unchangedCount
        ) << std::endl;*/

        std::cout << "classfile inside jars digest:" << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << jarfiles.classfiles.digest.count
            << std::setw(10) << jarfiles.classfiles.digest.newFile
            << std::setw(10) << jarfiles.classfiles.digest.same
            << std::setw(10) << jarfiles.classfiles.digest.differentDigest
            << std::setw(10) << jarfiles.classfiles.digest.unchangedCount
            << std::endl;
    }
    std::cout << "\r" << std::flush;
}
