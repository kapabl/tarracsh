#include <cassert>
#ifndef TARRACSH_FS_UTILS_H
#define TARRACSH_FS_UTILS_H
#include <string>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iostream>
#include <cstdlib>
#include <vector>
#include "Utils.h"

namespace kapa::infrastructure::filesystem::utils {

std::filesystem::path getUserHomeDir() {
    auto result = std::filesystem::path(getenv("HOME"));
    return result;
}

bool isClassfile(const std::filesystem::path &path) {
    auto const result = path.extension() == ".class";
    return result;
}

bool isJar(const std::filesystem::path &path) {
    auto const result = path.extension() == ".jar";
    return result;
}

bool isJMod(const std::filesystem::path &path) {
    auto const result = path.extension() == ".jmod";
    return result;
}

long long getLastWriteTimestamp(const std::string &filename) {
    const auto lastWriteTime = std::filesystem::last_write_time(filename);
    const auto result = std::chrono::duration_cast<std::chrono::microseconds>(lastWriteTime.time_since_epoch()).count();
    return result;
}

void backupPrevFile(const std::string &filename) {
    if (!std::filesystem::exists(filename)) {
        return;
    }
    const auto prevFilename = std::filesystem::path(filename + ".prev");
    std::filesystem::remove(prevFilename);

    std::filesystem::rename(std::filesystem::path(filename), prevFilename);
}

void writeLines(const std::string &filename, const std::vector<std::string> &lines) {
    std::ofstream file;
    std::string str;
    file.open(filename);
    for (auto &line : lines) {
        file << line << std::endl;
    }
}

auto readFileContent(const std::string &filename) -> std::string {
    const std::ifstream file(filename);
    std::stringstream buffer;
    buffer << file.rdbuf();
    auto result = buffer.str();
    return result;
}

void stdoutFile(const std::string &filename) {
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

void stdoutLines(const std::vector<std::string> &lines) {

    for (auto &line : lines) {
        std::cout << line << std::endl;
    }
}

std::filesystem::path classnameToPath(const std::string &classname) {
    std::filesystem::path result;
    return classname;
}

void ensureDir(const std::filesystem::path &dir) {
    if (!std::filesystem::exists(dir)) {
        std::filesystem::create_directories(dir.string());
    }
}

} // namespace kapa::infrastructure::filesystem::utils

#endif
