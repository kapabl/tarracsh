#ifndef TARRACSH_DIGEST_UTILS_H
#define TARRACSH_DIGEST_UTILS_H

#include <string>
#include <set>
#include <locale>
#include <format>


#define MD5_DIGEST_LENGTH 16
#define SHA_256_DIGEST_LENGTH 40

#define DIGEST_LENGTH SHA_256_DIGEST_LENGTH
// #define USE_POCO

#ifdef USE_POCO
    #define DIGEST_LENGTH MD5_DIGEST_LENGTH
    #include <Poco/DigestStream.h>
    #include <Poco/MD5Engine.h>
#else
#define DIGEST_LENGTH SHA_256_DIGEST_LENGTH
#include <sodium/crypto_hash_sha256.h>
#endif


#include "StringUtils.h"

namespace org::kapa::tarracsh::digestUtils {

typedef std::vector<unsigned char> DigestVector;
//typedef std::vector<unsigned char> DigestBuffer;

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

#ifdef USE_POCO
inline DigestVector digest(const char *bytes, const int length) {
    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream.write(bytes, length);
    stream.close();
    auto result = md5.digest();
    return result;
}
#else
inline DigestVector digest(const char *bytes, const int length) {
    DigestVector result(DIGEST_LENGTH);
    crypto_hash_sha256(
        &*result.begin(),
        reinterpret_cast<const unsigned char *>(bytes),
        length);
    return result;
}
#endif

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

}


#endif
