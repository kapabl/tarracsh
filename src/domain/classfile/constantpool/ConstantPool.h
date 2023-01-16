#ifndef TARRACSH_CONSTANT_POOL_H
#define TARRACSH_CONSTANT_POOL_H

#include "../reader/ClassFileReader.h"
#include "ConstpoolStructures.h"

namespace kapa::tarracsh::domain::classfile::constantpool {

class ConstantPool {
public:
    ConstantPool();
    ~ConstantPool();
    // ConstantPoolRecord &operator[](const u2 index) const;
    [[nodiscard]] u2 getPoolSize() const { return static_cast<u2>(_constantPoolIndex.size()); }

    void addRecord(ConstantPoolRecord &data, int size);

    template <typename T>
    void addRecord(T &data) {
        addRecord(reinterpret_cast<ConstantPoolRecord &>(data), sizeof(data));
    }


    [[nodiscard]] std::string getClassInfoName(const u2 classInfoIndex) const;
    [[nodiscard]] std::string getClassname(const u2 nameIndex) const;
    [[nodiscard]] std::string getString(const u2 stringIndex, const bool withEscaped = false) const;
    [[nodiscard]] std::string getTypeString(const u2 stringIndex) const;
    [[nodiscard]] std::string getConstantValueString(const u2 constantIndex) const;
    [[nodiscard]] ConstantPoolRecord& getEntry(const u2 index) const;

    void addEmptyIndex();
    void addUtf8Record(reader::ClassFileReader &reader);

    [[nodiscard]] u2 getNextIndex(u2 index) const;

private:
    [[maybe_unused]] u2 _count{};

    std::vector<ConstantPoolRecord *> _constantPoolIndex;

    static ConstantPoolRecord *add(const ConstantPoolRecord &data, const int size);

};


}
#endif //TARRACSH_CONSTANT_POOL_H
