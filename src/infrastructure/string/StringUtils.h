#ifndef TARRACSH_STRINGUTILS_H
#define TARRACSH_STRINGUTILS_H

#include <algorithm>
#include <cctype>
#include <string>
#include <numeric>
#include <locale>
#include <codecvt>
#include <fmt/format.h>
#include <vector>


namespace kapa::infrastructure::string::stringUtils {

inline std::string replaceAll(const std::string &value, const std::string &oldSubstring,
                              const std::string &newSubstring) {
    size_t pos = value.find(oldSubstring);
    auto result = value;

    while (pos != std::string::npos) {
        result.replace(pos, oldSubstring.length(), newSubstring);
        pos = result.find(oldSubstring, pos + newSubstring.length());
    }

    return result;
}

inline std::string_view trimView(std::string_view str, const std::string &chars = " \t\n\r") {
    str.remove_prefix(std::min(str.find_first_not_of(chars), str.size()));
    str.remove_suffix(std::min(str.size() - str.find_last_not_of(chars) - 1, str.size()));
    return str;
}

inline std::string trim(const std::string &str, const std::string &chars = " \t\n\r") {
    return std::string(trimView(str, chars));
}


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

inline wchar_t char2wchar(const char source) {

    std::wstring_convert<std::codecvt_utf8_utf16<char16_t>, char16_t>
        convert; // NOLINT(clang-diagnostic-deprecated-declarations)

    const char sourceCString[]{source, 0};

    const std::u16string u16s = convert.from_bytes(sourceCString);
    const std::wstring outString = u162wstring(u16s);

    return outString[0];
}

#pragma warning( pop )

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
        result += fmt::format("{:02x}", bytes[i]);
    }
    return result;
}

inline std::vector<std::string> split(const std::string &value, const std::string &separator) {
    std::vector<std::string> result;
    std::string::size_type start = 0;
    const auto separatorSize = separator.size();
    while (start < value.size()) {
        const auto index = value.find(separator, start);
        if (index != std::string::npos) {
            result.push_back(value.substr(start, index - start));
            start += index + separatorSize;
        } else {
            result.push_back(value.substr(start));
            start = value.size();
        }
    }
    return result;
}

inline std::string sizeToHumanReadable(uint64_t bytes) {
    std::vector<std::string> suffix{"B", "KB", "MB", "GB", "TB"};

    auto i = 0u;
    double doubleBytes = static_cast<double>(bytes);

    if (bytes > 1024) {
        for (i = 0; (bytes / 1024) > 0 && i < suffix.size() - 1; i++, bytes /= 1024) {
            doubleBytes = bytes / 1024.0;
        }
    }

    std::string result = fmt::format("{:.2f} {}", doubleBytes, suffix[i]);

    return result;
}


}


#endif
