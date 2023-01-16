// ReSharper disable CppClangTidyClangDiagnosticUnusedValue
#include "Terminal.h"
#include "RuleFuncs.h"

using namespace kapa::tarracsh::domain::classfile::signature;
using namespace std;

Terminal::Terminal()
    : Rule() {
}


Terminal::Terminal(wstring value)
    : Rule(), _value(std::move(value)) {

    SET_RULE_NAME2(this, format(L"terminal({})", _value));
}

Terminal::Terminal(const wchar_t *value)
    : Rule(), _value(value) {
    SET_RULE_NAME2(this, format(L"terminal({})", _value));
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

bool Terminal::match(SignatureScanner &scanner, std::wstring &value) {
    bool result = false;
    DEBUG_RULE(this);
    result = terminalMatch(scanner);
    if (result) {
        if ( _capture ) {
            value = _value;
        }
        //TODO match the followBy

    }
    return result;

}

std::wstring Terminal::getValue() { return _value; }
