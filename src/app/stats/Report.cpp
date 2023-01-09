#include "Report.h"
#include "Stats.h"
#include "../App.h"

using namespace org::kapa::tarracsh;
using namespace app;
using namespace stats::report;


Report::Report(Results& results): results(results), options(results.options) {
}

void Report::asNew(const std::string &filename) {
    ++results.jarfiles.digest.newFile;
    if (!options.doDiffReport) {
        return;
    }

    std::unique_lock lock(mutex);
    JarResult jarResult;
    jarResult.isNew = true;
    jarResult.isSamePublicDigest = false;
    jarResult.filename = filename;
    jarResults.push_back(jarResult);

}

void Report::asModified(const std::string &filename, bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++results.jarfiles.digest.same;
    } else {
        ++results.jarfiles.digest.differentDigest;
    }

    if (!options.doDiffReport) {
        return;
    }


    std::unique_lock lock(mutex);
    JarResult jarResult;
    jarResult.filename = filename;
    jarResult.isNew = false;
    jarResult.isModified = true;
    jarResult.isSamePublicDigest = isSamePublicDigest;
    jarResults.push_back(jarResult);
}


void Report::asUnchanged(const std::string &filename) {
    ++results.jarfiles.digest.unchangedCount;

    if (!options.doDiffReport) {
        return;
    }

    std::unique_lock lock(mutex);
    JarResult jarResult;
    jarResult.isNew = false;
    jarResult.isModified = false;
    jarResult.isSamePublicDigest = true;
    jarResult.filename = filename;
    jarResults.push_back(jarResult);
}

void Report::asNewClass(const std::string &fullClassname) {
    ++results.jarfiles.classfiles.digest.newFile;

    if (!options.doDiffReport) {
        return;
    }

    std::unique_lock lock(mutex);
    ClassfileResult classfileResult;
    classfileResult.isNew = true;
    classfileResult.isSamePublicDigest = false;
    classfileResult.filename = fullClassname;
    classfileResults.push_back(classfileResult);

}

void Report::asUnchangedClass(const std::string &fullClassname) {
    ++results.jarfiles.classfiles.digest.unchangedCount;

    if (!options.doDiffReport) {
        return;
    }

    std::unique_lock lock(mutex);
    ClassfileResult classfileResult;
    classfileResult.filename = fullClassname;
    classfileResult.isNew = false;
    classfileResult.isModified = false;
    classfileResult.isSamePublicDigest = true;
    classfileResults.push_back(classfileResult);
}

void Report::asModifiedClass(const std::string &fullClassname, bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++results.jarfiles.classfiles.digest.same;
    } else {
        ++results.jarfiles.classfiles.digest.differentDigest;
    }

    if (!options.doDiffReport) {
        return;
    }

    std::unique_lock lock(mutex);
    ClassfileResult classfileResult;
    classfileResult.filename = fullClassname;
    classfileResult.isModified = true;
    classfileResult.isSamePublicDigest = isSamePublicDigest;
    classfileResults.push_back(classfileResult);
}

void Report::print() const {
    if (options.verbose) {
        std::cout << R"legend(Legend:
    N - New file
    C - Changed
    U - Unchanged,
    S - Same Public Digest
    D - Different Public Digest)legend"
            << std::endl
            << std::endl;
    }

    if (options.verbose) {
        std::cout << "Jars:" << std::endl;
    }
    for (auto &jarResult : jarResults) {
        std::string flags;
        if (jarResult.isNew) {
            flags.push_back('N');
        } else {
            if (jarResult.isModified) {
                flags.push_back('C');
            } else {
                flags.push_back('U');
            }

            if (jarResult.isSamePublicDigest) {
                flags.push_back('S');
            } else {
                flags.push_back('D');
            }
        }

        std::cout << jarResult.filename << "\t" << flags << std::endl;
    }

    std::cout << std::endl;
    if (options.verbose) {
        std::cout << "Class-files:" << std::endl;
    }
    for (auto &classfileResult : classfileResults) {
        std::string flags;
        if (classfileResult.isNew) {
            flags.push_back('N');
        } else {
            if (classfileResult.isModified) {
                flags.push_back('C');
            } else {
                flags.push_back('U');
            }

            if (classfileResult.isSamePublicDigest) {
                flags.push_back('S');
            } else {
                flags.push_back('D');
            }
        }

        std::cout << classfileResult.filename << "\t" << flags << std::endl;
    }
}
