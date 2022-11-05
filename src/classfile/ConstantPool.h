#ifndef TARRACSH_CONSTANT_POOL_H
#define TARRACSH_CONSTANT_POOL_H


#include <cstring>

#include "ClassFileStructures.h"

namespace org::kapa::tarracsh {

class ConstantPool {
public:
    ConstantPool();
    ~ConstantPool();
    void relocate();
    ConstantPoolRecord &operator[](const u2 index) const;
    void setCount(const u2 count) { _count = count; }
    [[nodiscard]] u2 getCount() const { return _count; }

    // template <typename T>
    // void add(T &data, const int size) {
    //
    //     auto reservedSpace = size;
    //     while (reservedSpace > 0) {
    //         _buffer.push_back(0);
    //         --reservedSpace;
    //     }
    //     const auto start = &*(_buffer.end() - size);
    //     memcpy(start, &data, size);
    //
    // }
    template <typename T>
    void add(T &data, const int size) {

        if (size + _position >= _size) {
            _size <<= 1;
            _buffer = static_cast<u1 *>(realloc(_buffer, _size));
        }

        memcpy(&_buffer[_position], &data, size);
        _position += size;

    }

    template <typename T>
    void addRecord(T &data) {
        addRecord(data, sizeof(data));
    }

    template <typename T>
    void addRecord(T &data, int size) {
        _constantPoolIndex.push_back(
            reinterpret_cast<ConstantPoolRecord *>(_position) // NOLINT(performance-no-int-to-ptr)
            );
        add(data, size);
    }

    [[nodiscard]] std::wstring getClassInfoName(const u2 classInfoIndex) const;
    [[nodiscard]] std::wstring getClassname(const u2 nameIndex) const;
    [[nodiscard]] std::wstring getString(const u2 stringIndex, const bool withEscaped = false) const;
    [[nodiscard]] std::wstring getTypeString(const u2 stringIndex) const;
    [[nodiscard]] std::wstring getConstantValueString(const u2 constantIndex) const;

    void print() const;
    void addEmptyIndex();
    static void init();
    static std::string tagToString(ConstantPoolTag tag);
    static std::string refKindToString(MethodHandleSubtypes tag);

private:
    [[maybe_unused]] u2 _count{};
    // std::vector<u1> _buffer;
    u1 *_buffer;
    uint64_t _size{1024 * 1024};
    uint64_t _position{0};
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
