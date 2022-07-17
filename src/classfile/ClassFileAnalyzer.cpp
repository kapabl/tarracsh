#include <iostream>
#include "ClassFileAnalyzer.h"

#include "MethodDescriptorParser.h"


using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace std;

ClassFileAnalyzer::ClassFileAnalyzer(string fileName, string classPath)
    : _fileName(move(fileName)),
      _classPath(move(classPath)),
      _attributesManager(_constantPool) {

    processFile();
}

void ClassFileAnalyzer::output() {
    outputClass();
    outputMethods();
    outputFields();
    outputInterfaces();
}

void ClassFileAnalyzer::run() {
    //TODO
}

void ClassFileAnalyzer::outputAccessModifiers(const u2 accessFlags) const {
    wcout << _accessModifiers.toString(accessFlags) << " ";
}

void ClassFileAnalyzer::outputMethod(MethodInfo &methodInfo) {
    const auto accessModifiers = _accessModifiers.toString(methodInfo.accessFlags);
    const auto name = _constantPool[methodInfo.nameIndex].utf8Info.getValue();
    const auto &utf8DDesc = _constantPool[methodInfo.descriptorIndex].utf8Info;

    MethodDescriptorParser methodDescriptorParser(utf8DDesc.getValue());
    auto &methodDescriptor = methodDescriptorParser.getDescriptor();
    const auto returnType = methodDescriptor.returnType.toString();
    const auto arguments = methodDescriptor.argumentsToString();

    const auto attributesString = attributesToString(methodInfo.attributes);

    const vector parts{attributesString, accessModifiers, returnType, name + arguments};

    wcout << stringUtils::join<wstring>(parts, L" ") << L";";
    cout << endl;
}

void ClassFileAnalyzer::outputMethods() {
    cout << endl;
    cout << "//Methods: " << _methods.size() << endl;
    cout << "//-----------------------------------" << endl;
    for (auto &methodInfo : _methods) {
        outputMethod(methodInfo);
        cout << endl;
    }
    cout << endl;
}

wstring ClassFileAnalyzer::getClassInfoName(const u2 index) const {
    const ClassInfo &classInfo = _constantPool[index].classInfo;
    const auto &classname = _constantPool[classInfo.nameIndex].utf8Info;
    auto result = classname.getValueAsClassname();
    return result;
}

void ClassFileAnalyzer::outputClass(const wstring &type) {
    cout << endl;
    cout << "//Class " << endl;
    cout << "//------------------------------------" << endl;
    const auto accessModifiers = _accessModifiers.toString(_mainClassInfo.accessFlags);

    const auto attributesString = attributesToString(_attributes);

    wcout << attributesString << accessModifiers << L" " << type << L" "
        << getClassInfoName(_mainClassInfo.thisClass);

    if (_mainClassInfo.superClass != 0) {
        wcout << L" extends " << getClassInfoName(_mainClassInfo.superClass);
    }

    // TODO implemented interfaces
}

void ClassFileAnalyzer::outputClass() {

    outputClass(_mainClassInfo.isInterface() ? L"interface" : L"class");
    cout << endl;
}

wstring ClassFileAnalyzer::attributesToString(vector<AttributeInfo> &attributes) {

    vector<wstring> parts;

    parts.reserve(attributes.size());
    for (auto &attribute : attributes) {
        parts.push_back(_attributesManager.toString(attribute));
    }

    auto result = stringUtils::join<wstring>(parts, L"\n\n");
    if (!result.empty()) {
        result = L"/*\n" + result + L"\n*/\n";
    }
    return result;
}

void ClassFileAnalyzer::outputField(FieldInfo &fieldInfo) {
    const auto accessModifiers = _accessModifiers.toString(fieldInfo.accessFlags);
    const auto name = _constantPool[fieldInfo.nameIndex].utf8Info.getValue();
    const auto descriptorString = _constantPool[fieldInfo.descriptorIndex].utf8Info.getValue();
    const auto descriptor = DescriptorParser(descriptorString).getDescriptor();

    const auto attributeString = attributesToString(fieldInfo.attributes);

    const vector parts{attributeString, accessModifiers, descriptor.toString(), name};

    wcout << stringUtils::join<wstring>(parts, L" ") << ";";
}

void ClassFileAnalyzer::outputFields() {
    cout << endl;
    cout << "//Fields: " << _fields.size() << endl;
    cout << "//------------------------------------" << endl;
    for (auto &fieldInfo : _fields) {
        outputField(fieldInfo);
        cout << endl << endl;
    }
    cout << endl;
}

void ClassFileAnalyzer::outputInterfaces() {
    // TODO
}

u2 ClassFileAnalyzer::readU2() {
    u2 result;
    read(result);
    return result;
}

u4 ClassFileAnalyzer::readU4() {
    u4 result;
    read(result);
    return result;
}

u1 ClassFileAnalyzer::readU1() {
    u1 result;
    readRaw(result);
    return result;
}

bool ClassFileAnalyzer::readHeader() {
    readRaw(_header, sizeof(ClassFileHeader));
    _isBigEndian = _header.magic == 0x0cafebabe;
    if (!_isBigEndian && _header.magic != stringUtils::swapLong(0x0cafebabe)) {
        cout << "Invalid class file " << _fileName << endl;
        _isValid = false;
    }
    _attributesManager.setBigEndian(_isBigEndian);

    return _isValid;
}

void ClassFileAnalyzer::readConstPoolRecord() {
    ConstantPoolTag tag;
    readRaw(tag);

    switch (tag) {
        case JVM_CONSTANT_Utf8: {
            u2 length;
            read(length);
            const auto recordSize = 1 + length + sizeof(Utf8Info);
            Utf8Info &utf8Info = *static_cast<Utf8Info *>(malloc(recordSize));
            utf8Info.tag = tag;
            utf8Info.length = length;
            const auto data = reinterpret_cast<u1 *>(utf8Info.bytes);
            readRaw(*data, utf8Info.length);
            utf8Info.bytes[length] = 0;
            _constantPool.addRecord(utf8Info, static_cast<int>(recordSize));

            free(&utf8Info);

            break;
        }

        case JVM_CONSTANT_Float:
        case JVM_CONSTANT_Integer: {
            IntegerInfo integerInfo{{tag}, static_cast<int>(readU4())};
            _constantPool.addRecord(integerInfo);
            break;
        }

        case JVM_CONSTANT_Long:
        case JVM_CONSTANT_Double: {
            LongInfo longInfo{{tag}, {{readU4(), readU4()}}};
            _constantPool.addRecord(longInfo);
            break;
        }

        case JVM_CONSTANT_String:
        case JVM_CONSTANT_MethodType:
        case JVM_CONSTANT_Class: {
            ClassInfo classInfo{{tag}, 0};
            read(classInfo.nameIndex);
            _constantPool.addRecord(classInfo);
            break;
        }

        case JVM_CONSTANT_NameAndType:
        case JVM_CONSTANT_Methodref:
        case JVM_CONSTANT_Fieldref:
        case JVM_CONSTANT_InterfaceMethodref: {
            MemberInfo memberInfo{{tag}, readU2(), readU2()};
            _constantPool.addRecord(memberInfo);
            break;
        }

        case JVM_CONSTANT_MethodHandle: {
            MethodHandleInfo methodHandleInfo{{tag}, readU1(), readU2()};
            _constantPool.addRecord(methodHandleInfo);
            break;
        }

        case JVM_CONSTANT_InvokeDynamic: {
            InvokeDynamicInfo invokeDynamicInfo{{tag}, readU2(), readU2()};
            _constantPool.addRecord(invokeDynamicInfo);
            break;
        }

        // case JVM_CONSTANT_ExternalMax:
        case JVM_CONSTANT_Package:
        case JVM_CONSTANT_Dynamic:
        case JVM_CONSTANT_Module:
        case JVM_CONSTANT_Unicode:
            // TODO newer java version
            break;

        default: // NOLINT(clang-diagnostic-covered-switch-default)
            assert(false);
            break;
    }
}

void ClassFileAnalyzer::readConstantsPool() {
    u2 count;
    read(count);
    _constantPool.setCount(count);
    int index = 0;
    while (index + 1 < count) {
        readConstPoolRecord();
        index++;
    }
    _constantPool.relocate();
}

void ClassFileAnalyzer::readMainClassInfo() {
    read(_mainClassInfo.accessFlags);
    read(_mainClassInfo.thisClass);
    read(_mainClassInfo.superClass);
}

void ClassFileAnalyzer::readInterfaces() {
    const auto count = readU2();
    _interfaces.resize(count);
    for (auto i = 0; i < count; ++i) {
        read(_interfaces[i]);
    }
}

void ClassFileAnalyzer::readFields() {
    const auto count = readU2();

    for (auto i = 0; i < count; ++i) {
        FieldInfo fieldInfo;
        read(fieldInfo.accessFlags);
        read(fieldInfo.nameIndex);
        read(fieldInfo.descriptorIndex);
        const auto attributeCount = readU2();
        readAttributesSection(fieldInfo.attributes, attributeCount, AttributeOwner::Field);
        _fields.push_back(fieldInfo);
    }
}

void ClassFileAnalyzer::readAttributesSection(vector<AttributeInfo> &attributes, const int count,
                                            const AttributeOwner owner) {
    for (auto i = 0; i < count; ++i) {
        AttributeInfo attributeInfo;
        attributeInfo.owner = owner;
        read(attributeInfo.nameIndex);
        read(attributeInfo.length);
        for (auto length = 0u; length < attributeInfo.length; ++length) {
            const auto byte = readU1();
            attributeInfo.info.push_back(byte);
        }
        attributes.push_back(attributeInfo);
    }
}

void ClassFileAnalyzer::readMethods() {
    const auto count = readU2();
    for (auto i = 0; i < count; ++i) {
        MethodInfo methodInfo;
        methodInfo.accessFlags = readU2();
        methodInfo.nameIndex = readU2();
        methodInfo.descriptorIndex = readU2();

        const auto attributeCount = readU2();
        readAttributesSection(methodInfo.attributes, attributeCount, AttributeOwner::Method);
        _methods.push_back(methodInfo);
    }
}

void ClassFileAnalyzer::readAttributes() {
    const auto count = readU2();
    readAttributesSection(_attributes, count, AttributeOwner::ClassFile);
}

void ClassFileAnalyzer::processFile() {

    _fileSize = filesystem::file_size(_fileName);

    _file.open(_fileName, ifstream::binary);

    if (readHeader()) {
        readConstantsPool();
        readMainClassInfo();
        readInterfaces();
        readFields();
        readMethods();
        readAttributes();
    }

    _file.close();
}
