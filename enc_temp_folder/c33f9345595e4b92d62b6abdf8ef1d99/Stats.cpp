#include "Stats.h"

using namespace org::kapa::tarracsh;

void Results::print(const Options & options) {
    // cout << "\033[2K";

    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    std::cout << std::format("classfiles No:\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m",
        classfiles.count,
        classfiles.parsedCount,
        classfiles.errors) << std::endl;

    std::cout << std::format("jars No:\033[36m{}\033[39m|Ok:\033[92m{}\033[39m|Er:\033[91m{}\033[39m",
        jarfiles.count,
        jarfiles.parsedCount,
        jarfiles.errors) << std::endl;

    if (options.generatePublicDigest) {
        std::cout << std::format("classfile digest No:{}|New:{}|Same:{}|Diff:{}|Unchanged:{}",
            classfiles.digest.count,
            classfiles.digest.newFile,
            classfiles.digest.same,
            classfiles.digest.differentDigest,
            classfiles.digest.unchangedCount
        ) << std::endl;

        std::cout << std::format("jar digest\t\t\t\t No:{}\tNew:{}\tSame:{}\tDiff:{}\tUnchanged:{}",
            jarfiles.digest.count,
            jarfiles.digest.newFile,
            jarfiles.digest.same,
            jarfiles.digest.differentDigest,
            jarfiles.digest.unchangedCount
        ) << std::endl;

        std::cout << std::format("classfile digest\t\t No:{}\tNew:{}\tSame:{}\tDiff:{}\tUnchanged:{}",
            jarfiles.classfiles.digest.count,
            jarfiles.classfiles.digest.newFile,
            jarfiles.classfiles.digest.same,
            jarfiles.classfiles.digest.differentDigest,
            jarfiles.classfiles.digest.unchangedCount
        ) << std::endl;
    }
    std::cout << "\r" << std::flush;
}