#include <iostream>
#include "ProfileData.h"


#include "../../utils/FilesystemUtils.h"

using namespace org::kapa::tarracsh::stats::profiler;


ProfileData::ProfileData(Results &results): results(results) {
}

void ProfileData::output(const Options &options) const {
    const std::string filename(options.outputDir + "/profiler.txt");
    if (std::filesystem::exists(filename)) {
        std::filesystem::remove(filename);
    }
    std::vector<std::string> lines;
    std::ofstream file(options.outputDir + "/profiler.txt");
    lines.push_back(std::format("Analyzer total time: {}", analyzerTime));
    lines.push_back(std::format("Analyzer init: {}", initAnalyzer));
    lines.push_back(std::format("Init Db: {}", initDb));
    lines.push_back(std::format("Write Digest Db: {}", writeDigestDb));
    lines.push_back(std::format("Write Call Graph Db: {}", writeCallGraphDb));
    fsUtils::writeLines(filename, lines);
    if ( options.printProfiler ) {
        std::cout << std::endl;
        fsUtils::stdoutFile(filename);
    }
}

