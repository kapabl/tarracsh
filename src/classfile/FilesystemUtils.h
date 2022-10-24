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

}


#endif
