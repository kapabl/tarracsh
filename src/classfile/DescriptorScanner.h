//
// Created by xman on 6/28/2022.
//

#ifndef TARRASH_DESCRIPTORSCANNER_H
#define TARRASH_DESCRIPTORSCANNER_H

namespace org::kapa::tarrash {

class DescriptorScanner {

public:
    DescriptorScanner(const wstring &fieldDescriptor) : _fieldDescriptor(fieldDescriptor) {}

    wchar_t getNextChar() {
        if (_position + 1 >= _fieldDescriptor.size())
            return wchar_t(0);

        _position++;

        auto result = _fieldDescriptor[_position];

        return result;
    }

    void step() {
        if (_position + 1 >= _fieldDescriptor.size())
            return;
        _position++;
    }

    wchar_t currentChar() {
        if (_position >= _fieldDescriptor.size())
            return wchar_t(0);
        auto result = _fieldDescriptor[_position];
        return result;
    }

    wstring getFieldDescriptor() { return _fieldDescriptor; }

private:
    const wstring &_fieldDescriptor;
    int _position = -1;
};

}

// namespace org::kapa::tarrash

#endif // TARRASH_DESCRIPTORSCANNER_H
