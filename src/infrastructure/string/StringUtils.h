#ifndef TARRACSH_STRINGUTILS_H
#define TARRACSH_STRINGUTILS_H

#include <cctype>
#include <string>
#include <numeric>
#include <locale>
#include <codecvt>
#include <format>
#include <vector>

namespace kapa::infrastructure::string::stringUtils {


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

inline std::u16string utf82u32string(const char* source, const bool withEscape = false) {
    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t> utf16conv;
    std::u16string u16s = utf16conv.from_bytes(source);

    std::u16string result;

    if (withEscape) {
        for (const auto wchar : u16s) {
            if (wchar < 256 && std::isgraph(wchar)) {
                result.push_back(wchar);
            }
            else {
                auto formatted = std::format(L"{:#06x}", static_cast<int>(wchar));
                formatted.erase(0, 2);
                auto unicodeCode = L"\\u" + formatted;
                result.append((char16_t*)unicodeCode.c_str());
            }
        }
    }
    else {
        result = u16s;
    }
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

inline std::string pathToClassname(std::string path) {
    std::ranges::replace(path, '/', '.');
    return path;
}
inline std::string bytesToHexString(const unsigned char *bytes, int length) {
    std::string result;
    for (int i = 0; i < length; i++) {
        result += std::format("{:02x}", bytes[i]);
    }
    return result;
}

}


#endif
