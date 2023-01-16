#include "DigestReport.h"

#include <iostream>

#include "Results.h"

using namespace kapa::tarracsh::domain::stats::report;


DigestReport::DigestReport(Results &results)
    : results(results),
      jarfiles(results.jarfiles),
      classfiles(results.standaloneClassfiles),
      options(results.options) {
}

void DigestReport::asNewJar(const std::string &filename) {
    ++jarfiles.digest.newFile;
    if (options.doDiffReport) {
        asNewFile(filename);
    }
}

void DigestReport::asNewFile(const std::string &filename) {
    std::unique_lock lock(mutex);
    FileResult fileResult;
    fileResult.isNew = true;
    fileResult.isSamePublicDigest = false;
    fileResult.filename = filename;
    jarResults.push_back(fileResult);
}

void DigestReport::asNewClassfile(const std::string &filename, const std::string &strongClassname) {
    ++classfiles.digest.newFile;
    if (options.doDiffReport) {
        asNewFile(filename);
        asNewClass(strongClassname);
    }
}

void DigestReport::asModifiedFile(const std::string &filename, const bool isSamePublicDigest) {
    std::unique_lock lock(mutex);
    FileResult fileResult;
    fileResult.filename = filename;
    fileResult.isNew = false;
    fileResult.isModified = true;
    fileResult.isSamePublicDigest = isSamePublicDigest;
    jarResults.push_back(fileResult);
}

void DigestReport::asModifiedJar(const std::string &filename, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++jarfiles.digest.same;
    } else {
        ++jarfiles.digest.differentDigest;
    }

    if (options.doDiffReport) {
        asModifiedFile(filename, isSamePublicDigest);
    }
}

void DigestReport::asModifiedClassfile(const std::string &filename, const bool isSamePublicDigest, const std::string& strongClassname) {
    if (isSamePublicDigest) {
        ++classfiles.digest.same;
    } else {
        ++classfiles.digest.differentDigest;
    }

    if (options.doDiffReport) {
        asModifiedFile(filename, isSamePublicDigest);
        asModifiedClass(strongClassname, isSamePublicDigest);
    }

}


void DigestReport::asUnchangedFile(const std::string &filename) {
    std::unique_lock lock(mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = true;
    fileResult.filename = filename;
    jarResults.push_back(fileResult);
}

void DigestReport::asFailedFile(const std::string &filename) {
    std::unique_lock lock(mutex);
    FileResult fileResult;
    fileResult.isNew = false;
    fileResult.isModified = false;
    fileResult.isSamePublicDigest = false;
    fileResult.failed = true;
    fileResult.filename = filename;
    jarResults.push_back(fileResult);
}

void DigestReport::asUnchangedJar(const std::string &filename) {
    ++jarfiles.digest.unchangedCount;

    if (options.doDiffReport) {
        asUnchangedFile(filename);
    }

   
}

void DigestReport::asUnchangedClassfile(const std::string &filename) {
    ++classfiles.digest.unchangedCount;

    if (options.doDiffReport) {
        asUnchangedFile(filename);
    }
}

void DigestReport::asFailedJar(const std::string &filename) {
    ++jarfiles.errors;
    if (options.doDiffReport) {
        asFailedFile(filename);
    }
}

void DigestReport::asFailedClassfile(const std::string &filename) {
    ++classfiles.errors;
    if (options.doDiffReport) {
        asFailedFile(filename);
    }
}

void DigestReport::asNewClass(const std::string &strongClassname) {
    std::unique_lock lock(mutex);
    ClassResult classfileResult;
    classfileResult.isNew = true;
    classfileResult.isSamePublicDigest = false;
    classfileResult.strongClassname = strongClassname;
    classfileResults.push_back(classfileResult);
}

void DigestReport::asNewJarClass(const std::string &strongClassname) {
    ++results.jarfiles.classfiles.digest.newFile;

    if (!options.doDiffReport) {
        return;
    }

    asNewClass(strongClassname);

}

void DigestReport::asUnchangedClass(const std::string &strongClassname) {
    std::unique_lock lock(mutex);
    ClassResult classfileResult;
    classfileResult.strongClassname = strongClassname;
    classfileResult.isNew = false;
    classfileResult.isModified = false;
    classfileResult.isSamePublicDigest = true;
    classfileResults.push_back(classfileResult);
}

void DigestReport::asUnchangedJarClass(const std::string &strongClassname) {
    ++results.jarfiles.classfiles.digest.unchangedCount;

    if (options.doDiffReport) {
        asUnchangedClass(strongClassname);;
    }

    
}

void DigestReport::asModifiedClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    std::unique_lock lock(mutex);
    ClassResult classfileResult;
    classfileResult.strongClassname = strongClassname;
    classfileResult.isModified = true;
    classfileResult.isSamePublicDigest = isSamePublicDigest;
    classfileResults.push_back(classfileResult);
}

void DigestReport::asModifiedJarClass(const std::string &strongClassname, const bool isSamePublicDigest) {
    if (isSamePublicDigest) {
        ++results.jarfiles.classfiles.digest.same;
    } else {
        ++results.jarfiles.classfiles.digest.differentDigest;
    }

    if (options.doDiffReport) {
        asModifiedClass(strongClassname, isSamePublicDigest);;
    }
    
}

void DigestReport::print() const {
    if (options.verbose) {
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

    if (options.verbose) {
        std::cout << "Files:" << std::endl;
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
        std::cout << "Classes:" << std::endl;
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

        std::cout << classfileResult.strongClassname << "\t" << flags << std::endl;
    }
}
