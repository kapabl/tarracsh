#include "DigestReport.h"
#include "../digest/DigestUtils.h"

#include <iostream>

#include "Results.h"

using namespace kapa::tarracsh::domain::stats::report;


DigestReport::DigestReport(Results &results)
    : _results(results),
      _jarfiles(results.jarfiles),
      _classfiles(results.standaloneClassfiles),
      _options(results.options) {
}

void DigestReport::asNewJar(const std::string &filename) {
    ++_jarfiles.digest.newFile;
    if (_options.digest.isDiff) {
        addNewFile(filename);
    }
}

void DigestReport::addNewFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = true;
    fileResult.isSamePublicDigest = false;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void DigestReport::asNewClassfile(const std::string &strongClassname) {
    ++_classfiles.digest.newFile;
    if (_options.digest.isDiff) {
        const auto parts = digestUtils::splitStrongClassname(strongClassname);
        addNewFile(parts[0]);
        addNewClass(strongClassname);
    }
}

void DigestReport::addModifiedFile(const std::string &filename, const bool isSamePublicDigest) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.filename = filename;
    fileResult.isNew = false;
    fileResult.isModified = true;
    fileResult.isSamePublicDigest = isSamePublicDigest;
    _fileResults.push_back(fileResult);
}

void DigestReport::asModifiedJar(const std::string &filename, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++_jarfiles.digest.same;
    } else {
        ++_jarfiles.digest.differentDigest;
    }

    if (_options.digest.isDiff) {
        addModifiedFile(filename, isSamePublicDigest);
    }
}

void DigestReport::asModifiedClassfile(const bool isSamePublicDigest,
                                       const std::string &strongClassname) {
    if (isSamePublicDigest) {
        ++_classfiles.digest.same;
    } else {
        ++_classfiles.digest.differentDigest;
    }

    if (_options.digest.isDiff) {
        const auto parts = digestUtils::splitStrongClassname(strongClassname);
        addModifiedFile(parts[0], isSamePublicDigest);
        addModifiedClass(strongClassname, isSamePublicDigest);
    }

}


void DigestReport::addUnchangedFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = true;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void DigestReport::addFailedFile(const std::string &filename) {
    std::unique_lock lock(_mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = false;
    fileResult.failed = true;
    fileResult.filename = filename;
    _fileResults.push_back(fileResult);
}

void DigestReport::asUnchangedJar(const std::string &filename) {
    ++_jarfiles.digest.unchangedCount;

    if (_options.digest.isDiff) {
        addUnchangedFile(filename);
    }

}

void DigestReport::asUnchangedClassfile(const std::string &filename) {
    ++_classfiles.digest.unchangedCount;

    if (_options.digest.isDiff) {
        addUnchangedFile(filename);
    }
}

void DigestReport::asFailedJar(const std::string &filename) {
    ++_jarfiles.errors;
    if (_options.digest.isDiff) {
        addFailedFile(filename);
    }
}

void DigestReport::asFailedClassfile(const std::string &filename) {
    ++_classfiles.errors;
    if (_options.digest.isDiff) {
        addFailedFile(filename);
    }
}

void DigestReport::addNewClass( const std::string &strongClassname) {
    std::unique_lock lock(_mutex);
    ClassResult classfileResult;
    classfileResult.isNew = true;
    classfileResult.isSamePublicDigest = false;
    classfileResult.strongClassname = strongClassname;
    _classResults.push_back(classfileResult);
}

void DigestReport::asNewJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.digest.newFile;

    if (_options.digest.isDiff) {
        addNewClass(strongClassname);
    }
}

void DigestReport::addUnchangedClass(const std::string &strongClassname) {
    std::unique_lock lock(_mutex);
    ClassResult classfileResult;
    //classfileResult.filename = filename;
    classfileResult.strongClassname = strongClassname;
    classfileResult.isNew = false;
    classfileResult.isModified = false;
    classfileResult.isSamePublicDigest = true;
    _classResults.push_back(classfileResult);
}

void DigestReport::asUnchangedJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.digest.unchangedCount;

    if (_options.digest.isDiff) {
        addUnchangedClass(strongClassname);
    }

}

void DigestReport::asFailedJarClass(const std::string &strongClassname) {
    ++_results.jarfiles.classfiles.errors;


    if (_options.digest.isDiff) {
        addUnchangedClass(strongClassname);
    }
}

void DigestReport::addModifiedClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    std::unique_lock lock(_mutex);
    ClassResult classResult;
    classResult.strongClassname = strongClassname;
    classResult.isModified = true;
    classResult.isSamePublicDigest = isSamePublicDigest;
    _classResults.push_back(classResult);
}

void DigestReport::asModifiedJarClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++_results.jarfiles.classfiles.digest.same;
    } else {
        ++_results.jarfiles.classfiles.digest.differentDigest;
    }

    if (_options.digest.isDiff) {
        addModifiedClass(strongClassname, isSamePublicDigest);
    }

}

void DigestReport::print() const {
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
