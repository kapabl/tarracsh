//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_STRINGUTILS_H
#define TARRASH_STRINGUTILS_H

#include <string>
#include <numeric>
#include <locale>
#include <codecvt>
#include <format>

/**
 *
 * TODO use ICU library
 */

namespace org::kapa::tarrash::stringUtils {

inline string toLower(const string &data) {
    string result;
    for (auto &element : data) {
        result.push_back(static_cast<char>(std::tolower(element)));
    }
    return result;
}

inline wstring toLower(const wstring &data) {
    wstring result;
    for (auto &element : data) {
        result.push_back(towlower(element));
    }
    return result;
}

inline wstring u162wstring(const u16string &str) {


    using CodeCvt = codecvt_utf16<wchar_t, 0x10ffff, little_endian>;  // NOLINT(clang-diagnostic-deprecated-declarations)

    wstring_convert<CodeCvt, wchar_t> conv;  // NOLINT(clang-diagnostic-deprecated-declarations)

    wstring result = conv.from_bytes(reinterpret_cast<const char *>(&str[0]),
                                     reinterpret_cast<const char *>(&str[0] + str.size()));

    return result;
}

inline wstring utf82wstring(const char *source, const bool withEscape = false) {

    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;  // NOLINT(clang-diagnostic-deprecated-declarations)

    const u16string u16s = convert.from_bytes(source);
    wstring result = u162wstring(u16s);

    if (withEscape) {
        wstring escapedResult;
        for (const auto wchar : result) {
            if (isgraph(wchar)) {
                escapedResult.push_back(wchar);
            } else {
                auto formatted = format(L"{:#06x}", static_cast<int>(wchar));
                formatted.erase(0, 2);
                escapedResult += L"\\u" + formatted;
            }
        }
        result = escapedResult;
    }
    return result;
}

inline wstring utf82wstring(const unsigned char *source, bool withEscape = false) {
    return utf82wstring(reinterpret_cast<const char *>(source), withEscape);
}

inline string join(const vector<std::string> &strings, string delim) {
    if (strings.empty()) {
        return {};
    }

    auto result =
        accumulate(strings.begin() + 1, strings.end(), strings[0],
                   [&delim](const string &x, const string &y) { return x + delim + y; });
    return result;
}

template <typename T> T join(const vector<T> &parts, T delim) {
    if (parts.empty()) {
        return T();
    }

    auto result = accumulate(parts.begin() + 1, parts.end(), parts[0],
                             [&delim](const T &x, const T &y) { return x + delim + y; });
    return result;
}

inline unsigned short swapShort(const unsigned short value) {
    const unsigned short result = value >> 8 | value << 8;  // NOLINT(clang-diagnostic-implicit-int-conversion)
    return result;
}

inline unsigned int swapLong(const unsigned int value) {
    const unsigned int result = value >> 24 | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | value << 24;
    return result;
}

}


#endif // TARRASH_STRINGUTILS_H
