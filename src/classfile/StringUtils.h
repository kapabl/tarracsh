//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_STRINGUTILS_H
#define TARRASH_STRINGUTILS_H

#include <cctype>
#include <string>
#include <set>
#include <numeric>
#include <locale>
#include <codecvt>
#include <format>
#include <Poco/DigestStream.h>
#include <Poco/MD5Engine.h>

/**
 *
 * TODO use ICU library
 */

namespace org::kapa::tarracsh::stringUtils {

inline std::string toLower(const std::string &data) {
    std::string result;
    for (auto &element : data) {
        result.push_back(static_cast<char>(std::tolower(element)));
    }
    return result;
}

inline std::wstring toLower(const std::wstring &data) {
    std::wstring result;
    for (auto &element : data) {
        result.push_back(towlower(element));
    }
    return result;
}
#pragma warning( push )
#pragma warning( disable: 4996 )
inline std::wstring u162wstring(const std::u16string &str) {

    using CodeCvt = std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>;
    // NOLINT(clang-diagnostic-deprecated-declarations)

    std::wstring_convert<CodeCvt, wchar_t> conv; // NOLINT(clang-diagnostic-deprecated-declarations)

    std::wstring result = conv.from_bytes(reinterpret_cast<const char *>(&str[0]),
                                          reinterpret_cast<const char *>(&str[0] + str.size()));

    return result;
}

inline std::wstring utf82wstring(const char *source, const bool withEscape = false) {

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>
        convert; // NOLINT(clang-diagnostic-deprecated-declarations)

    const std::u16string u16s = convert.from_bytes(source);
    std::wstring result;

    if (withEscape) {
        // std::wstring escapedResult;
        for (const auto wchar : u16s) {
            if (wchar < 256 && std::isgraph(wchar)) {
                result.push_back(wchar);
            } else {
                auto formatted = std::format(L"{:#06x}", static_cast<int>(wchar));
                formatted.erase(0, 2);
                result += L"\\u" + formatted;
            }
        }
        // result = escapedResult;
    } else {
        result = u162wstring(u16s);
    }
    return result;
}

inline wchar_t char2wchar(const char source) {

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>
        convert; // NOLINT(clang-diagnostic-deprecated-declarations)

    const char sourceCString[]{source, 0};

    const std::u16string u16s = convert.from_bytes(sourceCString);
    const std::wstring outString = u162wstring(u16s);

    return outString[0];
}

#pragma warning( pop )
inline std::wstring utf82wstring(const unsigned char *source, bool withEscape = false) {
    return utf82wstring(reinterpret_cast<const char *>(source), withEscape);
}


inline std::string utf16ToUtf8(const std::wstring &utf16Str) {
    std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
    return conv.to_bytes(utf16Str);
}

template <typename T, typename T1>
inline T1 join(const T &parts, T1 delim) {
    T1 result;
    result = accumulate(parts.begin(), parts.end(), result,
                        [&delim](const auto &x, const auto &y) {
                            return x + delim + y;
                        });
    return result;
}


template <typename T> T join(const std::vector<T> &parts, T delim) {
    if (parts.empty()) {
        return T();
    }

    auto result = std::accumulate(
        parts.begin() + 1, parts.end(), parts[0],
        [&delim](const T &x, const T &y) { return x + delim + y; });
    return result;
}

inline unsigned short swapShort(const unsigned short value) {
    const unsigned short result = value >> 8 | value << 8; // NOLINT(clang-diagnostic-implicit-int-conversion)
    return result;
}

inline unsigned int swapLong(const unsigned int value) {
    const unsigned int result = value >> 24 | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | value << 24;
    return result;
}

inline std::vector<unsigned char> md5(const std::string &value) {

    Poco::MD5Engine md5;
    Poco::DigestOutputStream stream(md5);
    stream << value;
    stream.close();
    auto result = md5.digest();
    return result;
}

inline std::vector<unsigned char> md5(const std::wstring &value) {
    const auto utf8 = utf16ToUtf8(value);
    auto result = md5(utf8);
    return result;
}

inline std::string digestToString(Poco::DigestEngine::Digest &digest) {
    auto result = std::string(reinterpret_cast<char *>(&*digest.begin()), digest.size());
    return result;
}

inline std::string md5AsString(const std::wstring &value) {
    auto digest = md5(value);
    auto result = digestToString(digest);
    return result;
}

inline std::string md5AsString(const std::string& value) {
    auto digest = md5(value);
    auto result = digestToString(digest);
    return result;
}

inline std::string md5SetAsString(const std::set<std::string>& md5Set) {
    const std::string delim;
    const std::string methodsMd5 = stringUtils::join(md5Set, delim);
    auto result = stringUtils::md5AsString(methodsMd5);
    return result;
}


}


#endif
