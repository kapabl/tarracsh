//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_STRINGUTILS_H
#define TARRASH_STRINGUTILS_H

#include <string>
#include <numeric>
#include <algorithm>
#include <locale>
#include <codecvt>

using namespace std;

/**
 *
 * TODO use ICU library
 */

namespace org::kapa::tarrash::stringUtils {

inline string toLower( const string& data ) {
    string result;
    for( auto& element: data ) {
        result.push_back( tolower( element ) );
    }
    return result;
}

inline wstring toLower( const wstring& data ) {
    wstring result;
    for( auto& element: data ) {
        result.push_back( towlower( element ) );
    }
    return result;
}

inline wstring u162wstring(const std::u16string &str) {
    wstring_convert<std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>,
                    wchar_t>
        conv;

    wstring result =
        conv.from_bytes(reinterpret_cast<const char *>(&str[0]),
                        reinterpret_cast<const char *>(&str[0] + str.size()));

    return result;
}

inline wstring utf82wstring(const char *source) {
    wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> convert;
    u16string u16s = convert.from_bytes(source);
    wstring result = u162wstring(u16s);
    return result;
}

inline string join(const vector<std::string> &strings, string delim) {
    if (strings.empty()) {
        return std::string();
    }

    auto result =
        accumulate(strings.begin() + 1, strings.end(), strings[0],
                   [&delim]( const string& x, const string& y) { return x + delim + y; });
    return result;
}

template<typename T> T join(const vector<T> &parts, T delim) {
    if (parts.empty()) {
        return T();
    }

    auto result = accumulate(parts.begin() + 1, parts.end(), parts[0],
                             [&delim](const T& x, const T& y) { return x + delim + y; });
    return result;
}

inline unsigned short swapShort(unsigned short value) {
    const unsigned short result = value >> 8 | value << 8;
    return result;
}

inline unsigned int swapLong(unsigned int value) {
    const unsigned int result = value >> 24 | (value << 8 & 0x00FF0000) | (value >> 8 & 0x0000FF00) | value << 24;
    return result;
}

}



#endif // TARRASH_STRINGUTILS_H
