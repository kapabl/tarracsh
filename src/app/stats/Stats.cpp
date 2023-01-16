#include "Stats.h"

#include <iostream>


// #include "../app/TarracshApp.h"

using namespace kapa::tarracsh::app::stats;


Results::Results(Options& options): options(options) {
    report = std::make_unique<report::DigestReport>(*this);
    profileData = std::make_unique<profiler::ProfileData>(*this);
}

void Results::print() const {

    const auto result = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - lastPrint);

    if (result.count() < 500) return;

    lastPrint = std::chrono::high_resolution_clock::now();
    forcePrint();

}

void Results::forcePrint() const {
    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    std::cout << "\r";

    std::cout << "classfiles: " <<
        standaloneClassfiles.count + jarfiles.classfiles.count <<
        " jars: " << jarfiles.count;

    std::cout << std::flush;
}

void Results::printAll() {

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

        std::cout << "standalone classfile digest:" << std::endl << std::right
            << std::setw(10) << "No"
            << std::setw(10) << "New"
            << std::setw(10) << "Same"
            << std::setw(10) << "Diff"
            << std::setw(10) << "Unchanged"
            << std::endl
            << std::setw(10) << standaloneClassfiles.digest.count
            << std::setw(10) << standaloneClassfiles.digest.newFile
            << std::setw(10) << standaloneClassfiles.digest.same
            << std::setw(10) << standaloneClassfiles.digest.differentDigest
            << std::setw(10) << standaloneClassfiles.digest.unchangedCount
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

    long long totalClassfiles = standaloneClassfiles.count + jarfiles.classfileCount;
    const auto totalTime = profileData->analyzerTime.count();
    std::cout << std::endl;
    log->writeln(std::format("classfiles: {:L}", totalClassfiles), true);
    log->writeln(std::format("speed: {:.2f} classfile/s",
        1000.0 * totalClassfiles / totalTime), true);
    log->writeln(std::format("total time: {}", profileData->analyzerTime), true);

    std::cout << "\r" << std::flush;
}
