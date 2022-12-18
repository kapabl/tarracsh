#include "../app/Stats.h"

using namespace org::kapa::tarracsh::stats;


void Report::asNew(const std::string &filename) {
    ++results.jarfiles.digest.newFile;
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
    std::unique_lock lock(mutex);
    ClassfileResult classfileResult;
    classfileResult.isNew = true;
    classfileResult.isSamePublicDigest = false;
    classfileResult.filename = fullClassname;
    classfileResults.push_back(classfileResult);

}

void Report::asUnchangedClass(const std::string &fullClassname) {
    ++results.jarfiles.classfiles.digest.unchangedCount;
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

    std::unique_lock lock(mutex);
    ClassfileResult classfileResult;
    classfileResult.filename = fullClassname;
    classfileResult.isModified = true;
    classfileResult.isSamePublicDigest = isSamePublicDigest;
    classfileResults.push_back(classfileResult);
}

void Report::print() {
    std::cout << "Report:" << std::endl;
    std::cout << "Legend: N - New file, C/U - Change/Unchanged, S/D - Same/Different public digest"
        << std::endl
        << std::endl;

    std::cout << "Jars:" << std::endl;
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
    std::cout << "Class-files:" << std::endl;
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

Results::Results()
    : report(*this) {
}

void Results::print(const Options &options) const {
    // cout << "\033[2K";

    const auto result = std::chrono::duration_cast<std::chrono::milliseconds>(
        std::chrono::high_resolution_clock::now() - lastPrint);

    if (result.count() < 500) return;

    lastPrint = std::chrono::high_resolution_clock::now();

    printf("\033[2J");
    printf("\033[%d;%dH", 0, 0);

    std::cout << "\r";

    std::cout << "classfiles: " <<
        classfiles.count + jarfiles.classfiles.count <<
        " jars: " << jarfiles.count;

    std::cout << std::flush;

}

void Results::printAll(const Options &options) const {

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

    if (options.isPublicDigest) {

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

    std::cout << std::endl << std::format("total classfiles: {}", classfiles.count +
                                                                  jarfiles.classfileCount) << std::endl;

    std::cout << "\r" << std::flush;
}
