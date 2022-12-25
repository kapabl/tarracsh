#ifndef TARRACSH_FS_UTILS_H
#define TARRACSH_FS_UTILS_H
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>


namespace org::kapa::tarracsh::fsUtils {


inline bool isClassfile(const std::filesystem::path &path) {
    auto const result = path.extension() == ".class";
    return result;
}

inline bool isJar(const std::filesystem::path &path) {
    auto const result = path.extension() == ".jar";
    return result;
}

inline bool isJMod(const std::filesystem::path &path) {
    auto const result = path.extension() == ".jmod";
    return result;
}


inline auto getLastWriteTimestamp(const std::string &filename) {
    const auto lastWriteTime = std::filesystem::last_write_time(filename);
    const auto result = std::chrono::duration_cast<std::chrono::microseconds>(lastWriteTime.time_since_epoch()).count();
    return result;
}

inline void backupPrevFile(const std::string &filename) {
    if (!std::filesystem::exists(filename)) {
        return;
    }
    const auto prevFilename = std::filesystem::path(filename + ".prev");
    std::filesystem::remove(prevFilename);

    std::filesystem::rename(
        std::filesystem::path(filename),
        prevFilename);
}

inline auto writeLines(const std::string &filename, const std::vector<std::string> &lines) {
    std::ofstream file;
    std::string str;
    file.open(filename);
    for (auto &line : lines) {
        file << line << std::endl;
    }
}

inline auto stdoutFile(const std::string &filename) {

    std::ifstream file;
    file.open(filename, std::ios::in);

    if (file) {
        std::string line;
        while (!file.eof()) {
            std::getline(file, line);
            std::cout << line << std::endl;
        }
    }
}

inline auto stdoutLines(const std::vector<std::string> &lines) {

    for (auto &line : lines) {
        std::cout << line << std::endl;
    }
}

inline void ensureDir(const std::filesystem::path &dir) {
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir);
    }
}
}


#endif
