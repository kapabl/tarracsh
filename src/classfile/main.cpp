#include <fstream>
#include <iostream>

#include "JarAnalyzer.h"
#include "ClassFileAnalyzer.h"
#include "tarrash.h"
#include "includes/CLI11.hpp"

using namespace std;
using namespace std::filesystem;
using namespace org::kapa::tarrash;

int main(int argc, char *argv[]) {

    CLI::App app("Tarrash");

    app.set_version_flag("-v,--version", "version " TARRASH_VERSION);

    std::string classFile;
    const auto classfileOption = app.add_option("--classfile", classFile, " Input class file");

    std::string jarFile;
    const auto jarOption = app.add_option("--jar", jarFile, "Input jar file")->excludes( classfileOption );
    classfileOption->excludes(jarOption);

    std::string classPath;
    app.add_flag("-c,--classpath", classPath, "Class paths to look into.");

    std::string outputDir = "output";
    app.add_flag("--output", outputDir, "Output directory, default './output'");

    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError &e) {
        return app.exit(e);
    }

    if (!classfileOption->empty()) {
        ClassFileAnalyzer classFileParser(classFile, classPath);
        classFileParser.run();
        classFileParser.output();
    } else if (!jarOption->empty()) {
        jar::JarAnalyzer jarParser(jarFile, classPath);
        jarParser.run();
     }

#ifdef _DEBUG
    cin.get();
#endif 

    return 0;
}