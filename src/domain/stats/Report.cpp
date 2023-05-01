#include "Report.h"

#include <cassert>

#include "domain/Utils.h"
#include "domain/digest/DigestUtils.h"

#include <iostream>

#include "Results.h"

using namespace kapa::tarracsh::domain::stats::report;

using kapa::tarracsh::domain::BaseOptions;
using kapa::tarracsh::domain::stats::TaskResult;


Report::Report(Results &results)
    : _results(results),
      _jarfiles(results.jarfiles),
      _classfiles(results.standaloneClassfiles),
      _options(results.options) {
}

BaseOptions& Report::getBaseOptions() const {
    if ( _options.isCallGraph) {
        return _options.callGraph;
    } else if ( _options.isPublicDigest ) {
        return _options.digest;
    } else {
        return _options.parse;
    }
}


void Report::asNewJar(const std::string &filename) {
    ++_jarfiles.taskResult.newFile;
    if (getBaseOptions().isDiff) {
        addNewFile(filename);
    }
}

void Report::addNewFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = true;
    fileResult.isSamePublicDigest = false;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void Report::asNewClassfile(const std::string &strongClassname) {
    ++_classfiles.taskResult.newFile;
    if (getBaseOptions().isDiff) {
        const auto parts = utils::splitStrongClassname(strongClassname);
        addNewFile(parts[0]);
        addNewClass(strongClassname);
    }
}

void Report::addModifiedFile(const std::string &filename, const bool isSamePublicDigest) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.filename = filename;
    fileResult.isNew = false;
    fileResult.isModified = true;
    fileResult.isSamePublicDigest = isSamePublicDigest;
    _fileResults.push_back(fileResult);
}

void Report::asModifiedJar(const std::string &filename, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++_jarfiles.taskResult.same;
    } else {
        ++_jarfiles.taskResult.differentDigest;
    }

    if (getBaseOptions().isDiff) {
        addModifiedFile(filename, isSamePublicDigest);
    }
}

void Report::asModifiedClassfile(const bool isSamePublicDigest,
                                       const std::string &strongClassname) {
    if (isSamePublicDigest) {
        ++_classfiles.taskResult.same;
    } else {
        ++_classfiles.taskResult.differentDigest;
    }

    if (getBaseOptions().isDiff) {
        const auto parts = utils::splitStrongClassname(strongClassname);
        addModifiedFile(parts[0], isSamePublicDigest);
        addModifiedClass(strongClassname, isSamePublicDigest);
    }

}


void Report::addUnchangedFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = true;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void Report::addFailedFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = false;
    fileResult.failed = true;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void Report::asUnchangedJar(const std::string &filename) {
    ++_jarfiles.taskResult.unchangedCount;

    if (getBaseOptions().isDiff) {
        addUnchangedFile(filename);
    }

}

void Report::asUnchangedClassfile(const std::string &filename) {
    ++_classfiles.taskResult.unchangedCount;

    if (getBaseOptions().isDiff) {
        addUnchangedFile(filename);
    }
}

void Report::asFailedJar(const std::string &filename) {
    ++_jarfiles.errors;
    if (getBaseOptions().isDiff) {
        addFailedFile(filename);
    }
}

void Report::asFailedClassfile(const std::string &filename) {
    ++_classfiles.errors;
    if (getBaseOptions().isDiff) {
        addFailedFile(filename);
    }
}

void Report::addNewClass( const std::string &strongClassname) {
    std::unique_lock lock(_mutex);
    ClassResult classfileResult;
    classfileResult.isNew = true;
    classfileResult.isSamePublicDigest = false;
    classfileResult.strongClassname = strongClassname;
    _classResults.push_back(classfileResult);
}

void Report::asNewJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.taskResult.newFile;

    if (getBaseOptions().isDiff) {
        addNewClass(strongClassname);
    }
}

void Report::addUnchangedClass(const std::string &strongClassname) {
    std::unique_lock lock(_mutex);
    ClassResult classfileResult;
    //classfileResult.filename = filename;
    classfileResult.strongClassname = strongClassname;
    classfileResult.isNew = false;
    classfileResult.isModified = false;
    classfileResult.isSamePublicDigest = true;
    _classResults.push_back(classfileResult);
}

void Report::asUnchangedJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.taskResult.unchangedCount;

    if (getBaseOptions().isDiff) {
        addUnchangedClass(strongClassname);
    }

}

void Report::asFailedJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.errors;


    if (getBaseOptions().isDiff) {
        addUnchangedClass(strongClassname);
    }
}

void Report::addModifiedClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    std::unique_lock lock(_mutex);
    ClassResult classResult;
    classResult.strongClassname = strongClassname;
    classResult.isModified = true;
    classResult.isSamePublicDigest = isSamePublicDigest;
    _classResults.push_back(classResult);
}

void Report::asModifiedJarClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++_results.jarfiles.classfiles.taskResult.same;
    } else {
        ++_results.jarfiles.classfiles.taskResult.differentDigest;
    }

    if (getBaseOptions().isDiff) {
        addModifiedClass(strongClassname, isSamePublicDigest);
    }

}

void Report::print() const {
    std::cout << std::endl;
    if (_options.verbose) {
        std::cout << R"legend(Legend:
    N - New file
    C - Changed
    U - Unchanged,
    S - Same Public Digest
    D - Different Public Digest
    E - File Error)legend"
            << std::endl
            << std::endl;
    }

    std::cout << "Files:" << std::endl;
    
    for (auto &fileResult : _fileResults) {
        if (!fileResult.isNew && !fileResult.isModified) continue;
        std::string flags;
        if (fileResult.isNew) {
            flags.push_back('N');
        } else {
            if (fileResult.isModified) {
                flags.push_back('C');
            } else {
                flags.push_back('U');
            }

            if (fileResult.isSamePublicDigest) {
                flags.push_back('S');
            } else {
                flags.push_back('D');
            }
        }

        std::cout << std::format("{} {}", fileResult.filename, flags ) << std::endl;
    }

    std::cout << std::endl;
    std::cout << "Classes:" << std::endl;

    for (auto &classResult : _classResults) {
        if (!classResult.isNew && !classResult.isModified) continue;

        std::string flags;

        if (classResult.isNew) {
            flags.push_back('N');
        } else {
            if (classResult.isModified) {
                flags.push_back('C');
            } else {
                flags.push_back('U');
            }

            if (classResult.isSamePublicDigest) {
                flags.push_back('S');
            } else {
                flags.push_back('D');
            }
        }
        std::cout << std::format("{} {}", classResult.strongClassname, flags) << std::endl;
    }
    std::cout << std::endl;
}
