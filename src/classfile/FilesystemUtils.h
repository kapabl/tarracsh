#ifndef TARRACSH_FS_UTILS_H
#define TARRACSH_FS_UTILS_H
#include <string>
#include <filesystem>
#include <chrono>



namespace org::kapa::tarracsh::fsUtils {
    
inline auto getLastWriteTimestamp( const std::string& filename) {
    const auto lastWriteTime = std::filesystem::last_write_time(filename);
    const auto result = std::chrono::duration_cast<std::chrono::microseconds>(lastWriteTime.time_since_epoch()).count();
    return result;
}

inline void backupPrevFile(const std::string& filename) {
    if (!std::filesystem::exists(filename)) {
        return;
    }
    const auto prevFilename = std::filesystem::path(filename + ".prev");
    std::filesystem::remove(prevFilename);

    std::filesystem::rename(
        std::filesystem::path(filename),
        prevFilename);
}

}


#endif
