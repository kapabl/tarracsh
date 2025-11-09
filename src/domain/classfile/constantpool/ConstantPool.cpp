#include <string>
#include <vector>
#include <cstring>


#include "ConstantPool.h"

#include <cassert>
#include "../DescriptorParser.h"


using namespace kapa::tarracsh::domain::classfile::constantpool;

void ConstantPool::addEmptyIndex() {
     _constantPoolIndex.push_back(nullptr);
}

void ConstantPool::addUtf8Record(const std::string &value) {
    const auto length = static_cast<u2>(value.size());
    const auto size = sizeof(ConstantPoolRecord::utf8Info.tag) + sizeof(ConstantPoolRecord::utf8Info.length) + length + 1;
    const auto utf8Record = static_cast<ConstantPoolRecord *>(malloc(size));
    utf8Record->utf8Info.tag = JVM_CONSTANT_Utf8;
    utf8Record->utf8Info.length = length;
    memcpy(utf8Record->utf8Info.bytes, value.data(), length);
    utf8Record->utf8Info.bytes[length] = 0;
    _constantPoolIndex.push_back(utf8Record);
}

u2 ConstantPool::getNextIndex(u2 index) const {
    while (index < _constantPoolIndex.size()) {
        ++index;
        if (index >= _constantPoolIndex.size() || _constantPoolIndex[index] != nullptr) {
            break;
        }
    }
    return index;
}

ConstantPool::ConstantPool() {
    _constantPoolIndex.reserve(64*1024+1);
    addEmptyIndex();
}

ConstantPool::~ConstantPool() {
    for(const auto constPoolRecordPtr: _constantPoolIndex ) {
        free(constPoolRecordPtr);
    }
    _constantPoolIndex.clear();

}


ConstantPoolRecord* ConstantPool::add(const ConstantPoolRecord& data, const int size) {
    auto * result = static_cast<ConstantPoolRecord*>(malloc(size));
    memcpy(result, &data, size);
    return result;
}

void ConstantPool::addRecord(ConstantPoolRecord &data, int size) {
    const auto newRecord = add(data, size);
    _constantPoolIndex.push_back(newRecord);

}

std::string ConstantPool::getClassInfoName(const u2 classInfoIndex) const {
    if (classInfoIndex == 0) return "";
    const auto &classInfo = getEntry(classInfoIndex).classInfo;
    auto result = getClassname(classInfo.nameIndex);
    return result;
}

std::string ConstantPool::getClassname(const u2 nameIndex) const {
    if (nameIndex == 0) return "<anonymous>";
    auto result = getEntry(nameIndex).utf8Info.getAsUtf8();
    return result;
}

std::string ConstantPool::getString(const u2 stringIndex, const bool withEscaped) const {
    auto result = getEntry(stringIndex).utf8Info.getAsUtf8(withEscaped);
    return result;
}

std::string ConstantPool::getTypeString(const u2 stringIndex) const {

    const auto type = getString(stringIndex);

    DescriptorParser descriptorParser(type);
    auto &descriptor = descriptorParser.getDescriptor();
    auto result = descriptor.toString();
    return result;
}

std::string ConstantPool::getConstantValueString(const u2 constantIndex) const {
    const auto &constantPoolRecord = getEntry(constantIndex);
    std::string result;

    switch (constantPoolRecord.base.tag) {

        case JVM_CONSTANT_Integer:
            result = std::to_string(constantPoolRecord.integerInfo.value);
            break;

        case JVM_CONSTANT_Float:
            result = std::to_string(constantPoolRecord.floatInfo.getFloat());
            break;

        case JVM_CONSTANT_Long:
            result = std::to_string(constantPoolRecord.longInfo.getLongLong());
            break;

        case JVM_CONSTANT_Double:
            result = std::to_string(constantPoolRecord.doubleInfo.getDouble());
            break;

        case JVM_CONSTANT_Utf8:
            result = "\"" + constantPoolRecord.utf8Info.getAsUtf8(true) + "\"";
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

ConstantPoolRecord &ConstantPool::getEntry(const u2 index) const {
    assert(index < _constantPoolIndex.size());
    auto &result = *_constantPoolIndex[index];
    return result;
}
