#ifndef KAPA_FS_UTILS_H
#define KAPA_FS_UTILS_H
#include <string>
#include <filesystem>
#include <vector>


namespace kapa::infrastructure::filesystem::utils {

//TODO remove tarracsh specific functions
bool isClassfile(const std::filesystem::path& path);
bool isJar(const std::filesystem::path& path);
bool isJMod(const std::filesystem::path& path);

std::filesystem::path getUserHomeDir();
long long getLastWriteTimestamp(const std::string& filename);
void backupPrevFile(const std::string& filename);
void writeLines(const std::string& filename, const std::vector<std::string>& lines);
auto readFileContent(const std::string& filename) -> std::string;
void stdoutFile(const std::string& filename);
void stdoutLines(const std::vector<std::string>& lines);
std::filesystem::path classnameToPath(const std::string& classname);
void ensureDir(const std::filesystem::path& dir);


}


#endif
