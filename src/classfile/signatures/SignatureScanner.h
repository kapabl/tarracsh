//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_DESCRIPTORSCANNER_H
#define TARRASH_DESCRIPTORSCANNER_H

#include <string>

namespace org::kapa::tarrash::signatures {

class SignatureScanner {

public:
    explicit SignatureScanner(std::wstring signatureString) : _signatureString(std::move(signatureString)) {}

    explicit SignatureScanner(const SignatureScanner &other)
        : _signatureString(other._signatureString),
          _position(other._position) {
    }

    SignatureScanner(SignatureScanner &&other) noexcept
        : _signatureString(other._signatureString),
          _position(other._position) {
    }

    SignatureScanner & operator=(const SignatureScanner &other) {
        if (this == &other)
            return *this;
        _signatureString = other._signatureString;
        _position = other._position;
        return *this;
    }

    SignatureScanner & operator=(SignatureScanner &&other) noexcept {
        if (this == &other)
            return *this;
        _signatureString = other._signatureString;
        _position = other._position;
        return *this;
    }

    wchar_t getNextChar() {
        if (_position + 1 >= _signatureString.size()) return 0;

        _position++;

        const auto result = _signatureString[_position];

        return result;
    }

    void step() {
        if (_position + 1 >= _signatureString.size()) return;
        _position++;
    }

    [[nodiscard]] wchar_t currentChar() const {
        if (_position >= _signatureString.size()) return 0;
        const auto result = _signatureString[_position];
        return result;
    }

    [[nodiscard]] std::wstring getSignatureString() const { return _signatureString; }

    [[nodiscard]] bool isEOF() const { return _position >= _signatureString.size(); }

    [[nodiscard]] unsigned int getPosition() const { return _position; }
    void reset(const unsigned int position ) { _position = position; }

private:
    std::wstring _signatureString;
    unsigned int _position = 0x0ffffffff;
};

}

// namespace org::kapa::tarrash

#endif // TARRASH_DESCRIPTORSCANNER_H
