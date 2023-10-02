#include "Results.h"

#include <iostream>
#include <fmt/format.h>
#include <fmt/chrono.h>

#define OOF_IMPL
#include <oof.h>


using namespace kapa::tarracsh::domain::stats;


Results::Results(Options& options): options(options) {
    report = std::make_unique<report::Report>(*this);
    profileData = std::make_unique<profiler::ProfileData>(*this);
}

void Results::print() const {

    const auto result = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::steady_clock::now() - lastPrint);

    if (result.count() < 500) return;

    lastPrint = std::chrono::steady_clock::now();
    printProgress();

}

void Results::printProgress() const {

//    if (standaloneClassfiles.count + jarfiles.classfiles.count == lastClassfileCountPrint ||
//        jarfiles.count == lastJarCountPrint) return;

    lastClassfileCountPrint = standaloneClassfiles.count + jarfiles.classfiles.count;
    lastJarCountPrint = jarfiles.count;

    // if ( progressRow == - 1 ) {
    //     
    // }
    if ( progressStarted ) {
        std::cout << oof::move_up(1);
    }
    std::cout << oof::hposition(0);
          
    std::cout << fmt::format("classfiles: {}, jars: {}\n", lastClassfileCountPrint, lastJarCountPrint );
    std::cout << std::flush;
    progressStarted = true;
}

void Results::printAll() const {

    std::cout << std::endl << std::endl;
    std::cout << "classfiles:" << std::endl << std::right
        << std::setw(10) << "No"
        << std::setw(10) << "Ok"
        << std::setw(10) << "Error"
        << std::endl
        << std::setw(10) << standaloneClassfiles.count
        << std::setw(10) << standaloneClassfiles.parsedCount
        << std::setw(10) << standaloneClassfiles.errors
        << std::endl;

    std::cout << "jars:" << std::endl << std::right
        << std::setw(10) << "No"
        << std::setw(10) << "Ok"
        << std::setw(10) << "Error"
        << std::endl
        << std::setw(10) << jarfiles.count
        << std::setw(10) << jarfiles.parsedCount
        << std::setw(10) << jarfiles.errors
        << std::endl;

    if (options.isPublicDigest) {

        std::cout << "standalone classfile.taskResult." << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << standaloneClassfiles.taskResult.count
            << std::setw(10) << standaloneClassfiles.taskResult.newFile
            << std::setw(10) << standaloneClassfiles.taskResult.same
            << std::setw(10) << standaloneClassfiles.taskResult.differentDigest
            << std::setw(10) << standaloneClassfiles.taskResult.unchangedCount
            << std::endl;

        std::cout << "jar.taskResult." << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << jarfiles.taskResult.count
            << std::setw(10) << jarfiles.taskResult.newFile
            << std::setw(10) << jarfiles.taskResult.same
            << std::setw(10) << jarfiles.taskResult.differentDigest
            << std::setw(10) << jarfiles.taskResult.unchangedCount
            << std::endl;

        std::cout << "classfile inside jars.taskResult." << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << jarfiles.classfiles.taskResult.count
            << std::setw(10) << jarfiles.classfiles.taskResult.newFile
            << std::setw(10) << jarfiles.classfiles.taskResult.same
            << std::setw(10) << jarfiles.classfiles.taskResult.differentDigest
            << std::setw(10) << jarfiles.classfiles.taskResult.unchangedCount
            << std::endl;
    }

    long long totalClassfiles = standaloneClassfiles.count + jarfiles.classfileCount;
    const auto totalTime = profileData->analyzerTime.count();
    std::cout << std::endl;
    log->writeln(fmt::format(".class files: {}, in jars: {}, total: {}",
        static_cast<uint32_t>(standaloneClassfiles.count),
        static_cast<uint32_t>(jarfiles.classfileCount),
        totalClassfiles), true);

    log->writeln(fmt::format("speed: {:.2f} classfile/s",
        1000.0 * totalClassfiles / totalTime), true);
    log->writeln(fmt::format("total time: {}", profileData->analyzerTime), true);
    log->writeln(fmt::format("output dir: {}", options.outputDir), true);

    std::cout << "\r" << std::flush;
}

