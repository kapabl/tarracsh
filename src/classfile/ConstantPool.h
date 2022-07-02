//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CONSTANTPOOL_H
#define TARRASH_CONSTANTPOOL_H


#include <utility>
#include <cstring>

#include "DescriptorParser.h"
#include "ClassFileStructures.h"

namespace org::kapa::tarrash {

class ConstantPool {

private:
    u2 _count{};
    vector<u1> _buffer;

    vector<ConstantPoolRecord *> _constantPoolIndex;

    ConstantPoolRecord &getEntry(const u2 index) const {
        assert(index - 1 < _constantPoolIndex.size());
        auto &result = *_constantPoolIndex[index - 1];
        return result;
    }

public:
    ConstantPool() {
        _buffer.reserve(1024 * 1024);
    }

    void relocate() {
        const auto baseAddress = reinterpret_cast<u1 *>(_buffer.data());
        for (auto &pConstantPoolRecord : _constantPoolIndex) {
            const auto offset = reinterpret_cast<std::intptr_t>(pConstantPoolRecord);
            pConstantPoolRecord = reinterpret_cast<ConstantPoolRecord *>(baseAddress + offset);
        }
    }

    ConstantPoolRecord &operator[](const u2 index) const {
        return getEntry(index);
    }

    void setCount(const u2 count) { _count = count; }

    template <typename T>
    void add(T &data, const int size) {

        auto reservedSpace = size;
        while (reservedSpace > 0) {
            _buffer.push_back(0);
            --reservedSpace;
        }
        const auto start = &*(_buffer.end() - size);
        memcpy(start, &data, size);

    }


    template <typename T>
    void addRecord(T &data) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, sizeof(data));
    }

    template <typename T>
    void addRecord(T &data, int size) {
        _constantPoolIndex.push_back(reinterpret_cast<ConstantPoolRecord *>(_buffer.size()));
        add(data, size);
    }

    wstring getClassInfoName(const u2 classInfoIndex) const {
        const auto &classInfo = getEntry(classInfoIndex).classInfo;
        auto name = getEntry(classInfo.nameIndex).utf8Info.getValueAsClassname();
        return name;
    }

    wstring getClassname(const u2 nameIndex) const {
        if (nameIndex == 0)
            return L"<anonymous>";
        auto name = getEntry(nameIndex).utf8Info.getValueAsClassname();
        return name;
    }

    wstring getString(const u2 stringIndex) const {
        auto name = getEntry(stringIndex).utf8Info.getValue();
        return name;
    }

    wstring getTypeString(const u2 stringIndex) const {

        const auto type = getString(stringIndex);

        DescriptorParser descriptorParser(type);
        auto &descriptor = descriptorParser.getDescriptor();
        auto result = descriptor.toString();
        return result;
    }


    wstring getConstantValueString(const u2 constantIndex) const {
        const auto &constantPoolRecord = getEntry(constantIndex);
        wstring result;

        switch (constantPoolRecord.base.tag) {

            case JVM_CONSTANT_Integer:
                result = to_wstring(constantPoolRecord.integerInfo.value);
                break;

            case JVM_CONSTANT_Float:
                result = to_wstring(constantPoolRecord.floatInfo.value);
                //auto x = L"\u00a7";
                break;

            case JVM_CONSTANT_Long:
                result = to_wstring(constantPoolRecord.longInfo.value);
                break;

            case JVM_CONSTANT_Double:
                result = to_wstring(constantPoolRecord.doubleInfo.value);
                break;

            case JVM_CONSTANT_Utf8:
                result = L"\"" + constantPoolRecord.utf8Info.getValue(true) + L"\"";
                break;

            case JVM_CONSTANT_String:
                result = getString(constantPoolRecord.stringInfo.stringIndex);
                break;

            default:
                assert(false);
                break;
        }

        return result;
    }


};

}
#endif //TARRASH_CONSTANTPOOL_H
