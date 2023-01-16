#include "../Options.h"
#include <iostream>
#include "ClassFileParser.h"
#include "MethodDescriptorParser.h"

using namespace kapa::tarracsh::domain::classfile;
using namespace constantpool;
using namespace attribute;

using namespace std;

ClassFileParser::ClassFileParser(reader::ClassFileReader &reader, Options &options, Results &results)
    : _options(options),
      _results(results),
      _reader(reader),
      _attributesManager(_constantPool) {
}


bool ClassFileParser::parse() {
    _parseSucceed = internalParse();
    return _parseSucceed;
}

bool ClassFileParser::internalParse() {

    auto result = true;
    try {
        processFile();
    } catch (const runtime_error &runtimeException) {
        result = false;
        const auto errorMessage = format("Error parsing file: {}, msg:{}", _options.classFilePath,
                                         runtimeException.what());
        _results.log->writeln(errorMessage);
    }
    catch (...) {
        result = false;
        const auto errorMessage = format("Error parsing file: {}", _options.classFilePath);
        _results.log->writeln(errorMessage);
    }

    return result;
}

std::string ClassFileParser::getMainClassname() const {
    std::string result = _constantPool.getClassInfoName(_mainClassInfo.thisClass);
    return result;
}

std::string ClassFileParser::getContainingFile() const {
    std::string result(!_options.jarFile.empty()
                           ? _options.jarFile
                           : _options.classFilePath);

    return result;
}

void ClassFileParser::initialize() {
    _attributesManager.setBigEndian(_reader.isBigEndian());
}


void ClassFileParser::readConstPoolEntry(int &index) {
    const auto tag = static_cast<ConstantPoolTag>(_reader.readU1());

    switch (tag) {
        case JVM_CONSTANT_Utf8: {
            _constantPool.addUtf8Record(_reader);
            break;
        }

        case JVM_CONSTANT_Float: {
            FloatInfo floatInfo{{tag}, _reader.readU4()};
            _constantPool.addRecord(floatInfo);
            break;
        }

        case JVM_CONSTANT_Integer: {
            IntegerInfo integerInfo{{tag}, static_cast<int>(_reader.readU4())};
            _constantPool.addRecord(integerInfo);
            break;
        }

        case JVM_CONSTANT_Long: {
            LongInfo longInfo{{tag}, _reader.readU4(), _reader.readU4()};
            _constantPool.addRecord(longInfo);
            _constantPool.addEmptyIndex();
            index++;
            break;
        }

        case JVM_CONSTANT_Double: {
            DoubleInfo doubleInfo{{tag}, _reader.readU4(), _reader.readU4()};
            _constantPool.addRecord(doubleInfo);
            _constantPool.addEmptyIndex();
            index++;
            break;
        }

        case JVM_CONSTANT_MethodType: {
            MethodTypeInfo methodTypeInfo{{tag}, _reader.readU2()};
            _constantPool.addRecord(methodTypeInfo);
            break;
        }
        case JVM_CONSTANT_String: {
            StringInfo stringInfo{{tag}, _reader.readU2()};
            _constantPool.addRecord(stringInfo);
            break;
        }

        case JVM_CONSTANT_Class: {
            ClassInfo classInfo{{tag}, _reader.readU2()};
            _constantPool.addRecord(classInfo);
            break;
        }

        case JVM_CONSTANT_NameAndType: {
            NameAndTypeInfo nameAndTypeInfo{{tag}, _reader.readU2(), _reader.readU2()};
            _constantPool.addRecord(nameAndTypeInfo);
            break;
        }

        case JVM_CONSTANT_Methodref: {
            MethodrefInfo methodrefInfo{{{tag}, _reader.readU2(), _reader.readU2()}};
            _constantPool.addRecord(methodrefInfo);
            break;
        }

        case JVM_CONSTANT_Fieldref: {
            FieldrefInfo fieldrefInfo{{{tag}, _reader.readU2(), _reader.readU2()}};
            _constantPool.addRecord(fieldrefInfo);
            break;
        }

        case JVM_CONSTANT_InterfaceMethodref: {
            InterfaceMethodrefInfo memberInfo{{{tag}, _reader.readU2(), _reader.readU2()}};
            _constantPool.addRecord(memberInfo);
            break;
        }

        case JVM_CONSTANT_MethodHandle: {
            MethodHandleInfo methodHandleInfo{{tag}, static_cast<MethodHandleSubtypes>(_reader.readU1()),
                                              _reader.readU2()};
            _constantPool.addRecord(methodHandleInfo);
            break;
        }

        case JVM_CONSTANT_InvokeDynamic: {
            InvokeDynamicInfo invokeDynamicInfo{{tag}, _reader.readU2(), _reader.readU2()};
            _constantPool.addRecord(invokeDynamicInfo);
            break;
        }
        case JVM_CONSTANT_Module: {
            ModuleInfo moduleInfo{{tag}, _reader.readU2()};
            _constantPool.addRecord(moduleInfo);
            break;
        }

        // case JVM_CONSTANT_ExternalMax:
        case JVM_CONSTANT_Unicode:
            cout << "Unused Cool Entry type found: JVM_CONSTANT_Unicode";
            break;

        case JVM_CONSTANT_Package: {
            PackageInfo packageInfo{{tag}, _reader.readU2()};
            _constantPool.addRecord(packageInfo);
            break;
        }

        case JVM_CONSTANT_Dynamic: {
            DynamicInfo dynamicInfo{{tag}, _reader.readU2(), _reader.readU2()};
            _constantPool.addRecord(dynamicInfo);
            break;
        }

        case JVM_CONSTANT_Empty:
            cout << "empty entry";
            break;

        default: // NOLINT(clang-diagnostic-covered-switch-default)
            const auto errorMessage = format("Error - Invalid const-pool tag: {} {}", static_cast<int>(tag),
                                             _options.classFilePath);
            _results.log->writeln(errorMessage);
            throw runtime_error(errorMessage);
    }
}


void ClassFileParser::readConstantsPool() {
    const u2 count = _reader.readU2();
    // _constantPool.setCount(count);
    int index = 1;
    while (index < count) {
        readConstPoolEntry(index);
        index++;
    }
    // _constantPool.relocate();
}

void ClassFileParser::readMainClassInfo() {
    _mainClassInfo.accessFlags = _reader.readU2();
    _mainClassInfo.thisClass = _reader.readU2();
    _mainClassInfo.superClass = _reader.readU2();
}

void ClassFileParser::readInterfaces() {
    const auto count = _reader.readU2();
    _interfaces.resize(count);
    for (auto i = 0; i < count; ++i) {
        _reader.read(_interfaces[i]);
    }
}

void ClassFileParser::readFields() {
    const auto count = _reader.readU2();

    for (auto i = 0; i < count; ++i) {
        FieldInfo fieldInfo;
        _reader.read(fieldInfo.accessFlags);
        _reader.read(fieldInfo.nameIndex);
        _reader.read(fieldInfo.descriptorIndex);
        const auto attributeCount = _reader.readU2();
        readAttributesSection(fieldInfo.attributes, attributeCount, AttributeOwner::Field);
        _fields.push_back(fieldInfo);
    }
}

void ClassFileParser::readAttributesSection(vector<AttributeInfo> &attributes, const int count,
                                              const AttributeOwner owner) {
    for (auto i = 0; i < count; ++i) {
        AttributeInfo attributeInfo;
        attributeInfo.owner = owner;
        _reader.read(attributeInfo.nameIndex);
        _reader.read(attributeInfo.length);

        attributeInfo.info.resize(attributeInfo.length);
        // const auto dest = reinterpret_cast<char *>(&*attributeInfo.info.begin());
        // _reader.readRaw(dest, attributeInfo.length);

        for (auto index = 0u; index < attributeInfo.length; ++index) {
            attributeInfo.info[index] = _reader.readU1();
        }
        attributes.push_back(attributeInfo);
    }
}

void ClassFileParser::readMethods() {
    const auto count = _reader.readU2();
    for (auto i = 0; i < count; ++i) {
        MethodInfo methodInfo;
        methodInfo.accessFlags = _reader.readU2();
        methodInfo.nameIndex = _reader.readU2();
        methodInfo.descriptorIndex = _reader.readU2();

        const auto attributeCount = _reader.readU2();
        readAttributesSection(methodInfo.attributes, attributeCount, AttributeOwner::Method);
        _methods.push_back(methodInfo);
    }
}

void ClassFileParser::readAttributes() {
    const auto count = _reader.readU2();
    readAttributesSection(_attributes, count, AttributeOwner::ClassFile);
}


void ClassFileParser::processFile() {
    if (!isValid()) return;

    initialize();
    readConstantsPool();
    readMainClassInfo();
    readInterfaces();
    readFields();
    readMethods();
    readAttributes();

}
