#include <string>
#include <vector>

#include "ConstantPool.h"

#include <cassert>
#include <iostream>
#include <ostream>

#include "DescriptorParser.h"


using namespace org::kapa::tarracsh;
using namespace std;

void ConstantPool::addEmptyIndex() {
    _constantPoolIndex.push_back(
        reinterpret_cast<ConstantPoolRecord *>(_position));
}

ConstantPool::ConstantPool() {
    // _buffer.reserve(1024 * 10240);
    _buffer = static_cast<u1*>(malloc(_size));
    _constantPoolIndex.reserve(20 * 1024);
    addEmptyIndex();
}

ConstantPool::~ConstantPool() {
    free(_buffer);
    _buffer = nullptr;
}

void ConstantPool::relocate() {
    //const auto baseAddress = reinterpret_cast<u1 *>(_buffer.data());
    const auto baseAddress = reinterpret_cast<u1 *>(_buffer);
    for (auto &pConstantPoolRecord : _constantPoolIndex) {
        const auto offset = reinterpret_cast<std::intptr_t>(pConstantPoolRecord);
        pConstantPoolRecord = reinterpret_cast<ConstantPoolRecord *>(baseAddress + offset);
    }
}

ConstantPoolRecord &ConstantPool::operator[](const u2 index) const {
    return getEntry(index);
}


std::wstring ConstantPool::getClassInfoName(const u2 classInfoIndex) const {
    if (classInfoIndex == 0) return L"";
    const auto &classInfo = getEntry(classInfoIndex).classInfo;
    auto name = getClassname(classInfo.nameIndex);
    return name;
}

std::wstring ConstantPool::getClassname(const u2 nameIndex) const {
    if (nameIndex == 0) return L"<anonymous>";
    auto name = getEntry(nameIndex).utf8Info.getValueAsClassname();
    return name;
}

std::wstring ConstantPool::getString(const u2 stringIndex, const bool withEscaped) const {
    auto name = getEntry(stringIndex).utf8Info.getValue(withEscaped);
    return name;
}

std::wstring ConstantPool::getTypeString(const u2 stringIndex) const {

    const auto type = getString(stringIndex);

    DescriptorParser descriptorParser(type);
    auto &descriptor = descriptorParser.getDescriptor();
    auto result = descriptor.toString();
    return result;
}

std::wstring ConstantPool::getConstantValueString(const u2 constantIndex) const {
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

ConstantPoolRecord &ConstantPool::getEntry(const u2 index) const {
    assert(index < _constantPoolIndex.size());
    auto &result = *_constantPoolIndex[index];
    return result;
}
