#include "Terminal.h"

using namespace org::kapa::tarrash::signatures;
using namespace std;


Terminal::Terminal(wstring value)
    : Rule(), _value(std::move(value)) {
}

Terminal::Terminal(const wchar_t *value)
    : Rule(), _value(value) {
}


Terminal::Terminal(const Terminal &other) = default;


Terminal::Terminal(Terminal &&other) noexcept
    : Rule(std::move(other)),
      _value(other._value) {
}

Terminal &Terminal::operator=(const Terminal &other) {
    if (this == &other)
        return *this;
    Rule::operator =(other);
    _value = other._value;
    return *this;
}

Terminal &Terminal::operator=(Terminal &&other) noexcept {
    if (this == &other)
        return *this;

    _value = other._value;
    Rule::operator =(std::move(other));
    return *this;
}

// bool Terminal::match(SignatureScanner &scanner) {
//
//     const auto scannerPosition = scanner.getPosition();
//     auto position = 0u;
//     while (position < _value.length() && !scanner.isEOF()) {
//         if (_value[position] != scanner.getNextChar()) break;
//         position++;
//     }
//     const auto result = position == _value.length();
//     if (!result) {
//         scanner.reset(scannerPosition);
//     }
//     return false;
// }

std::wstring Terminal::getValue() { return _value; }
