#ifndef TARRACSH_DIGEST_UTILS_H
#define TARRACSH_DIGEST_UTILS_H

#include <string>
#include <set>
#include <locale>
#include <format>


#define SHA_256_DIGEST_LENGTH 40
#define DIGEST_LENGTH SHA_256_DIGEST_LENGTH
#include <sodium/crypto_hash_sha256.h>


#include "../infrastructure/string/StringUtils.h"

namespace kapa::tarracsh::domain::digestUtils {

typedef std::vector<unsigned char> DigestVector;

struct DigestBuffer : std::vector<unsigned char> {

    DigestBuffer()
        : std::vector<unsigned char>() {
    }

    DigestBuffer(size_t size)
        : std::vector<unsigned char>(size) {
    }

    template <typename T>
    DigestBuffer &append(const T &value) {
        std::ranges::copy(value, std::back_inserter(*this));
        return *this;
    }

    template <typename T>
    DigestBuffer &append(const T &value, int size) {
        std::copy_n(value, DIGEST_LENGTH, std::back_inserter(*this));
        return *this;
    }

    DigestBuffer &append(const unsigned short &value) {
        push_back(static_cast<char>(value & 0x0ff));
        push_back(static_cast<char>(value >> 8));
        return *this;
    }
};
inline DigestVector digest(const char *bytes, const int length) {
    DigestVector result(DIGEST_LENGTH);
    crypto_hash_sha256(
        &*result.begin(),
        reinterpret_cast<const unsigned char *>(bytes),
        length);
    return result;
}
// inline DigestVector digest(const DigestBuffer &buffer) {
//     auto result = digest(reinterpret_cast<const char *>(&*buffer.begin()), buffer.size());
//     return result;
// }

inline DigestVector digest(const std::vector<unsigned char> &buffer) {
    if (buffer.empty()) {
        return DigestVector(DIGEST_LENGTH);
    }
    auto result = digest(reinterpret_cast<const char *>(&*buffer.begin()), buffer.size());
    return result;
}

inline DigestVector digest(const std::vector<char> &buffer) {
    auto result = digest(&*buffer.begin(), buffer.size());
    return result;
}

inline DigestVector digestSet(const std::set<DigestVector> &digestSet) {

    if (digestSet.empty()) {
        return DigestVector(DIGEST_LENGTH);
    }
    DigestBuffer buffer(DIGEST_LENGTH * digestSet.size());
    auto index = 0u;
    for (auto &digest : digestSet) {
        for (const auto byte : digest) {
            buffer[index] = byte;
            index++;
        }
        // buffer.append(digest);
    }
    auto result = digest(buffer);
    return result;
}

[[nodiscard]] inline std::string getStrongClassname(const char* filename, const char* jvmClassname) {
    std::string result(filename);

    if (jvmClassname != nullptr && jvmClassname[0] != 0) {
        result += "@";
        result += jvmClassname;
    }

    return result;
}

}


#endif
