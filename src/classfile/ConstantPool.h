//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CONSTANTPOOL_H
#define TARRASH_CONSTANTPOOL_H


#include <cstring>

#include "DescriptorParser.h"
#include "ClassFileStructures.h"

namespace org::kapa::tarrash {

class ConstantPool {
public:
    ConstantPool() {
        _buffer.reserve(1048576);
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
        addRecord(data, sizeof(data));
    }

    template <typename T>
    void addRecord(T &data, int size) {
        _constantPoolIndex.push_back(
            reinterpret_cast<ConstantPoolRecord *>(_buffer.size()) // NOLINT(performance-no-int-to-ptr)
            );
        add(data, size);
    }

    [[nodiscard]] std::wstring getClassInfoName(const u2 classInfoIndex) const {
        if (classInfoIndex == 0) return L"";
        const auto &classInfo = getEntry(classInfoIndex).classInfo;
        auto name = getEntry(classInfo.nameIndex).utf8Info.getValueAsClassname();
        return name;
    }

    [[nodiscard]] std::wstring getClassname(const u2 nameIndex) const {
        if (nameIndex == 0) return L"<anonymous>";
        auto name = getEntry(nameIndex).utf8Info.getValueAsClassname();
        return name;
    }

    [[nodiscard]] std::wstring getString(const u2 stringIndex) const {
        auto name = getEntry(stringIndex).utf8Info.getValue();
        return name;
    }

    [[nodiscard]] std::wstring getTypeString(const u2 stringIndex) const {

        const auto type = getString(stringIndex);

        DescriptorParser descriptorParser(type);
        auto &descriptor = descriptorParser.getDescriptor();
        auto result = descriptor.toString();
        return result;
    }


    [[nodiscard]] std::wstring getConstantValueString(const u2 constantIndex) const {
        const auto &constantPoolRecord = getEntry(constantIndex);
        std::wstring result;

        switch (constantPoolRecord.base.tag) {

            case JVM_CONSTANT_Integer:
                result = std::to_wstring(constantPoolRecord.integerInfo.value);
                break;

            case JVM_CONSTANT_Float:
                result = std::to_wstring(constantPoolRecord.floatInfo.value);
                break;

            case JVM_CONSTANT_Long:
                result = std::to_wstring(constantPoolRecord.longInfo.valueUnion.value);
                break;

            case JVM_CONSTANT_Double:
                result = std::to_wstring(constantPoolRecord.doubleInfo.value);
                break;

            case JVM_CONSTANT_Utf8:
                result = L"\"" + constantPoolRecord.utf8Info.getValue(true) + L"\"";
                break;

            case JVM_CONSTANT_String:
                result = getString(constantPoolRecord.stringInfo.stringIndex);
                break;


            case JVM_CONSTANT_Unicode:
            case JVM_CONSTANT_Class:
            case JVM_CONSTANT_Fieldref:
            case JVM_CONSTANT_Methodref:
            case JVM_CONSTANT_InterfaceMethodref:
            case JVM_CONSTANT_NameAndType:
            case JVM_CONSTANT_MethodHandle:
            case JVM_CONSTANT_MethodType:
            case JVM_CONSTANT_Dynamic:
            case JVM_CONSTANT_InvokeDynamic:
            case JVM_CONSTANT_Module:
            case JVM_CONSTANT_Package:
                assert(false);
                break;
        }

        return result;
    }
private:
    [[maybe_unused]] u2 _count{};
    std::vector<u1> _buffer;

    std::vector<ConstantPoolRecord *> _constantPoolIndex;

    [[nodiscard]] ConstantPoolRecord &getEntry(const u2 index) const {
        assert(index - 1 < _constantPoolIndex.size());
        auto &result = *_constantPoolIndex[index - 1];
        return result;
    }

};

}
#endif //TARRASH_CONSTANTPOOL_H
