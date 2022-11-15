#include <iostream>
#include "ClassFileAnalyzer.h"

#include "ClassFileDigest.h"
#include "ConstantPoolPrinter.h"
#include "../tables/ClassfilesTable.h"

#include "MethodDescriptorParser.h"
#include "ParserOutput.h"

using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace stats;
using namespace std;

ClassFileAnalyzer::ClassFileAnalyzer(readers::ClassFileReader &reader, Options &options, Results &results)
    : _options(options),
      _results(results),
      _reader(reader),
      _attributesManager(_constantPool) {
}


bool ClassFileAnalyzer::run() {

    auto result = true;
    try {
        processFile();
        if (_options.printClassParse) {
            ParserOutput parserOutput(*this);
            parserOutput.run();
        }
    } catch (const runtime_error &runtimeException) {
        result = false;
        const auto errorMessage = format("Error parsing file: {}, msg:{}", _options.classFilePath,
                                         runtimeException.what());
        _results.resultLog.writeln(errorMessage);
    }
    catch (...) {
        result = false;
        const auto errorMessage = format("Error parsing file: {}", _options.classFilePath);
        _results.resultLog.writeln(errorMessage);
    }

    return result;
}

optional<tables::DigestColumn> ClassFileAnalyzer::getPublicDigest() {
    optional<tables::DigestColumn> result;
    if (run()) {
        const ClassFileDigest classFileDigest(*this);
        result = classFileDigest.digest();
    }
    return result;
}

wstring ClassFileAnalyzer::getClassInfoName(const u2 index) const {
    const ClassInfo &classInfo = _constantPool[index].classInfo;
    const auto &classname = _constantPool[classInfo.nameIndex].utf8Info;
    auto result = classname.getValueAsClassname();
    return result;
}

void ClassFileAnalyzer::initialize() {
    _attributesManager.setBigEndian(_reader.isBigEndian());
}


void ClassFileAnalyzer::readConstPoolEntry(int &index) {
    const auto tag = static_cast<ConstantPoolTag>(_reader.readU1());

    switch (tag) {
        case JVM_CONSTANT_Utf8: {
            const u2 length = _reader.readU2();
            const auto recordSize = length + sizeof(Utf8Info);
            Utf8Info &utf8Info = *static_cast<Utf8Info *>(malloc(recordSize));
            utf8Info.tag = tag;
            utf8Info.length = length;
            const auto data = reinterpret_cast<u1 *>(utf8Info.bytes);
            _reader.readRaw(*data, utf8Info.length);
            utf8Info.bytes[length] = 0;
            _constantPool.addRecord(utf8Info, static_cast<int>(recordSize));

            free(&utf8Info);

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
            _results.resultLog.writeln(errorMessage);
            throw runtime_error(errorMessage);
    }
}


bool ClassFileAnalyzer::canPrintConstantPool() const {
    const auto result = !_options.classfileOption->empty() && _options.printConstantPool;
    return result;
}

void ClassFileAnalyzer::readConstantsPool() {
    const u2 count = _reader.readU2();
    _constantPool.setCount(count);
    int index = 1;
    while (index < count) {
        readConstPoolEntry(index);
        index++;
    }
    _constantPool.relocate();

    if (canPrintConstantPool()) {
        const ConstantPoolPrinter printer(_constantPool);
        printer.print();
    }
}

void ClassFileAnalyzer::readMainClassInfo() {
    _mainClassInfo.accessFlags = _reader.readU2();
    _mainClassInfo.thisClass = _reader.readU2();
    _mainClassInfo.superClass = _reader.readU2();
}

void ClassFileAnalyzer::readInterfaces() {
    const auto count = _reader.readU2();
    _interfaces.resize(count);
    for (auto i = 0; i < count; ++i) {
        _reader.read(_interfaces[i]);
    }
}

void ClassFileAnalyzer::readFields() {
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

void ClassFileAnalyzer::readAttributesSection(vector<AttributeInfo> &attributes, const int count,
                                              const AttributeOwner owner) {
    for (auto i = 0; i < count; ++i) {
        AttributeInfo attributeInfo;
        attributeInfo.owner = owner;
        _reader.read(attributeInfo.nameIndex);
        _reader.read(attributeInfo.length);
        for (auto length = 0u; length < attributeInfo.length; ++length) {
            const auto byte = _reader.readU1();
            attributeInfo.info.push_back(byte);
        }
        attributes.push_back(attributeInfo);
    }
}

void ClassFileAnalyzer::readMethods() {
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

void ClassFileAnalyzer::readAttributes() {
    const auto count = _reader.readU2();
    readAttributesSection(_attributes, count, AttributeOwner::ClassFile);
}


void ClassFileAnalyzer::processFile() {

    if (!isValid()) return;

    initialize();
    readConstantsPool();
    readMainClassInfo();
    readInterfaces();
    readFields();
    readMethods();
    readAttributes();

}
