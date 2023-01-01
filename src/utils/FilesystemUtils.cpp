#ifndef TARRACSH_FS_UTILS_H
#define TARRACSH_FS_UTILS_H
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iostream>


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

inline auto readFileContent(const std::string &filename) -> std::string {
    const std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto result = buffer.str();
    return result;
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


inline std::filesystem::path classnameToPath(const std::string &classname) {
#ifdef _WIN32
    std::istringstream f(classname);
    std::string part;
    std::filesystem::path result;
    while (getline(f, part, '/')) {
        if (result.empty()) {
            result = part;
        } else {
            result = result / part;
        }
    }
#else
    result = classname;
#endif
    return result;

}

#ifdef _WIN32
inline void ensureDir(const std::filesystem::path &dir) {
    const auto win32Dir = "\\\\?\\" + dir.string();
    if (!std::filesystem::exists(win32Dir)) {
        std::filesystem::create_directories(win32Dir);
    }
}
#else
inline void ensureDir(const std::filesystem::path &dir) {
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir.string());
    }
}
#endif

}


#endif
