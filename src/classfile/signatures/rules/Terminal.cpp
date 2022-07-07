#include "Terminal.h"

using namespace org::kapa::tarrash::signatures;


Terminal::Terminal(const std::wstring value, SignatureScanner &scanner)
    : Rule(scanner), _value(value) {
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

bool Terminal::match() {
    auto result = true;
    //TODO
    return result;
}
