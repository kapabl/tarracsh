#include <iostream>
#include "ParserOutput.h"

#include "MethodDescriptorParser.h"
#include "ClassFileAnalyzer.h"


using namespace org::kapa::tarracsh;
using namespace attributes;
using namespace std;

ParserOutput::ParserOutput(ClassFileAnalyzer &classFileAnalyzer)
    : _classFileAnalyzer(classFileAnalyzer),
      _accessModifiers(classFileAnalyzer.getAccessModifiers()),
      _constantPool(classFileAnalyzer.getConstantPool()) {

}

void ParserOutput::run() {
    outputClass();
    outputMethods();
    outputFields();
    outputInterfaces();
}

void ParserOutput::outputAccessModifiers(const u2 accessFlags) const {
    wcout << _accessModifiers.toString(accessFlags) << " ";
}

void ParserOutput::outputMethod(MethodInfo &methodInfo) {
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

void ParserOutput::outputMethods() {
    auto& methods = _classFileAnalyzer.getMethods();
    cout << endl;
    cout << "//Methods: " << methods.size() << endl;
    cout << "//-----------------------------------" << endl;
    for (auto &methodInfo : methods) {
        outputMethod(methodInfo);
        cout << endl;
    }
    cout << endl;
}

wstring ParserOutput::getClassInfoName(const u2 index) const {
    auto result = _constantPool.getClassInfoName(index);
    return result;
}

void ParserOutput::outputClass(const wstring &type) {
    const auto& mainClassInfo = _classFileAnalyzer.getMainClassInfo();
    cout << endl;
    cout << "//Class " << endl;
    cout << "//------------------------------------" << endl;
    const auto accessModifiers = _accessModifiers.toString(mainClassInfo.accessFlags);

    const auto attributesString = attributesToString(_classFileAnalyzer.getAttributes());

    wcout << attributesString << accessModifiers << L" " << type << L" "
        << getClassInfoName(mainClassInfo.thisClass);

    if (mainClassInfo.superClass != 0) {
        wcout << L" extends " << getClassInfoName(mainClassInfo.superClass);
    }

    // TODO implemented interfaces
}

void ParserOutput::outputClass() {

    outputClass(_classFileAnalyzer.getMainClassInfo().isInterface() ? L"interface" : L"class");
    cout << endl;
}

wstring ParserOutput::attributesToString(vector<AttributeInfo> &attributes) {

    vector<wstring> parts;

    parts.reserve(attributes.size());
    auto &attributesManager = _classFileAnalyzer.getAttributesManager();
    for (auto &attribute : attributes) {
        parts.push_back(attributesManager.toString(attribute));
    }

    auto result = stringUtils::join<wstring>(parts, L"\n\n");
    if (!result.empty()) {
        result = L"/*\n" + result + L"\n*/\n";
    }
    return result;
}

void ParserOutput::outputField(FieldInfo &fieldInfo) {
    const auto accessModifiers = _accessModifiers.toString(fieldInfo.accessFlags);
    const auto name = _constantPool[fieldInfo.nameIndex].utf8Info.getValue();
    const auto descriptorString = _constantPool[fieldInfo.descriptorIndex].utf8Info.getValue();
    const auto descriptor = DescriptorParser(descriptorString).getDescriptor();

    const auto attributeString = attributesToString(fieldInfo.attributes);

    const vector parts{attributeString, accessModifiers, descriptor.toString(), name};

    wcout << stringUtils::join<wstring>(parts, L" ") << ";";
}

void ParserOutput::outputFields() {
    auto& fields = _classFileAnalyzer.getFields();
    cout << endl;
    cout << "//Fields: " << fields.size() << endl;
    cout << "//------------------------------------" << endl;
    for (auto &fieldInfo : fields) {
        outputField(fieldInfo);
        cout << endl << endl;
    }
    cout << endl;
}

void ParserOutput::outputInterfaces() {
    // TODO
}
