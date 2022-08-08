#ifndef TARRACSH_CONSTANT_POOL_H
#define TARRACSH_CONSTANT_POOL_H


#include <cassert>
#include <cstring>

#include "DescriptorParser.h"
#include "ClassFileStructures.h"

namespace org::kapa::tarracsh {

class ConstantPool {
public:
    ConstantPool() {
        _buffer.reserve(1048576);
    }

    void relocate();

    ConstantPoolRecord &operator[](const u2 index) const {
        return getEntry(index);
    }

    void setCount(const u2 count) { _count = count; }
    [[nodiscard]] u2 getCount() const { return _count; }

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

    [[nodiscard]] std::wstring getString(const u2 stringIndex, const bool withEscaped = false) const {
        auto name = getEntry(stringIndex).utf8Info.getValue(withEscaped);
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
                result = std::to_wstring(constantPoolRecord.floatInfo.getFloat());
                break;

            case JVM_CONSTANT_Long:
                result = std::to_wstring(constantPoolRecord.longInfo.getLongLong());
                break;

            case JVM_CONSTANT_Double:
                result = std::to_wstring(constantPoolRecord.doubleInfo.getDouble());
                break;

            case JVM_CONSTANT_Utf8:
                result = L"\"" + constantPoolRecord.utf8Info.getValue(true) + L"\"";
                break;

            case JVM_CONSTANT_String:
                result = getString(constantPoolRecord.stringInfo.stringIndex);
                break;

            case JVM_CONSTANT_Class:
                result = getClassInfoName(constantPoolRecord.classInfo.nameIndex);
                break;

            case JVM_CONSTANT_Unicode:
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

    void print() const;
    static void init();
    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);

private:
    [[maybe_unused]] u2 _count{};
    std::vector<u1> _buffer;
    static std::vector<std::string> _poolTagToString;
    static std::vector<std::string> _refKindToString;
    
    std::vector<ConstantPoolRecord *> _constantPoolIndex;

    [[nodiscard]] ConstantPoolRecord &getEntry(const u2 index) const;
    void printEntry(const ConstPoolBase &entry, int index) const;
    void printEntry(const Utf8Info &entry, int index) const;
    void printEntry(const StringInfo &entry, int index) const;
    void printEntry(const LongInfo &entry, int index) const;
    void printEntry(const DoubleInfo &entry, int index) const;
    void printEntry(const IntegerInfo &entry, int index) const;
    void printEntry(const FloatInfo &entry, int index) const;
    void printEntry(const ClassInfo &entry, int index) const;
    void printEntry(const MethodrefInfo &entry, int index) const;
    void printEntry(const MethodHandleInfo &entry, int index) const;
    void printEntry(const MethodTypeInfo &entry, int index) const;
    void printEntry(const FieldrefInfo &entry, int index) const;
    void printEntry(const ModuleInfo &entry, int index) const;
    void printRefExtraInfo(const MemberInfo &entry) const;
    void printEntry(const InterfaceMethodrefInfo &entry, int index) const;
    void printEntry(const InvokeDynamicInfo &entry, int index) const;
    void printEntry(const NameAndTypeInfo &entry, int index) const;
    void printEntry(const ConstantPoolRecord &entry, int index) const;

};


}
#endif //TARRACSH_CONSTANT_POOL_H
