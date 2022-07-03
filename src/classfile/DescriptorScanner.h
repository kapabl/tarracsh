//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_DESCRIPTORSCANNER_H
#define TARRASH_DESCRIPTORSCANNER_H

namespace org::kapa::tarrash {

class DescriptorScanner {

public:
    explicit DescriptorScanner(const std::wstring &fieldDescriptor) : _fieldDescriptor(fieldDescriptor) {}

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
    const std::wstring &_fieldDescriptor;
    unsigned int _position = 0x0ffffffff;
};

}

// namespace org::kapa::tarrash

#endif // TARRASH_DESCRIPTORSCANNER_H
