#ifndef TARRACSH_DIGEST_UTILS_H
#define TARRACSH_DIGEST_UTILS_H

#include <string>
#include <set>
#include <locale>
#include <format>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>
#include "StringUtils.h"


namespace org::kapa::tarracsh::digestUtils {




inline std::vector<unsigned char> md5(const std::string &value) {

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream << value;
    stream.close();
    auto result = md5.digest();
    return result;
}

inline std::vector<unsigned char> md5(const char *bytes, const int length) {

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream.write(bytes, length);
    stream.close();
    auto result = md5.digest();
    return result;
}

inline std::vector<unsigned char> md5(const std::wstring &value) {
    const auto utf8 = stringUtils::utf16ToUtf8(value);
    auto result = md5(utf8);
    return result;
}

inline std::string digestToString(Poco::DigestEngine::Digest &digest) {
    auto result = std::string(reinterpret_cast<char *>(&*digest.begin()), digest.size());
    return result;
}

inline Poco::DigestEngine::Digest bytesToDigest(const unsigned char *bytes, int length) {
    Poco::DigestEngine::Digest result;
    for (int i = 0; i < length; i++) {
        result.push_back(bytes[i]);
    }
    return result;
}
inline std::string md5AsString(const std::wstring &value) {
    auto digest = md5(value);
    auto result = digestToString(digest);
    return result;
}

inline std::string md5AsString(const std::string &value) {
    auto digest = md5(value);
    auto result = digestToString(digest);
    return result;
}

inline std::string md5SetAsString(const std::set<std::string> &md5Set) {
    const std::string delim;
    const std::string methodsMd5 = stringUtils::join(md5Set, delim);
    auto result = md5AsString(methodsMd5);
    return result;
}

}


#endif
