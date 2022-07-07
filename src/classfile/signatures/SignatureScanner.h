//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_DESCRIPTORSCANNER_H
#define TARRASH_DESCRIPTORSCANNER_H

#include <string>

namespace org::kapa::tarrash::signatures {

class SignatureScanner {

public:
    explicit SignatureScanner(std::wstring fieldDescriptor) : _fieldDescriptor(std::move(fieldDescriptor)) {}

    SignatureScanner(const SignatureScanner &other)
        : _fieldDescriptor(other._fieldDescriptor),
          _position(other._position) {
    }

    SignatureScanner(SignatureScanner &&other) noexcept
        : _fieldDescriptor(other._fieldDescriptor),
          _position(other._position) {
    }

    SignatureScanner & operator=(const SignatureScanner &other) {
        if (this == &other)
            return *this;
        _fieldDescriptor = other._fieldDescriptor;
        _position = other._position;
        return *this;
    }

    SignatureScanner & operator=(SignatureScanner &&other) noexcept {
        if (this == &other)
            return *this;
        _fieldDescriptor = other._fieldDescriptor;
        _position = other._position;
        return *this;
    }

    wchar_t getNextChar() {
        if (_position + 1 >= _fieldDescriptor.size()) return 0;

        _position++;

        const auto result = _fieldDescriptor[_position];

        return result;
    }

    void step() {
        if (_position + 1 >= _fieldDescriptor.size()) return;
        _position++;
    }

    wchar_t currentChar() const {
        if (_position >= _fieldDescriptor.size()) return 0;
        const auto result = _fieldDescriptor[_position];
        return result;
    }

    std::wstring getFieldDescriptor() const { return _fieldDescriptor; }

private:
    std::wstring _fieldDescriptor;
    unsigned int _position = 0x0ffffffff;
};

}

// namespace org::kapa::tarrash

#endif // TARRASH_DESCRIPTORSCANNER_H
