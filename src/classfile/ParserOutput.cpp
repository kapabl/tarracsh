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
    cout << _accessModifiers.toString(accessFlags) << " ";
}

void ParserOutput::outputMethod(MethodInfo &methodInfo) {
    const auto accessModifiers = _accessModifiers.toString(methodInfo.accessFlags);
    const auto name = _constantPool[methodInfo.nameIndex].utf8Info.getAsUtf8();
    const auto &utf8DDesc = _constantPool[methodInfo.descriptorIndex].utf8Info;

    MethodDescriptorParser methodDescriptorParser(utf8DDesc.getAsUtf8());
    auto &methodDescriptor = methodDescriptorParser.getDescriptor();
    const auto returnType = methodDescriptor.returnType.toString();
    const auto arguments = methodDescriptor.argumentsToString();

    const auto attributesString = attributesToString(methodInfo.attributes);

    const vector<string> parts{attributesString, accessModifiers, returnType, name + arguments};

    cout << stringUtils::join<string>(parts, " ") << ";";
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

std::string ParserOutput::getClassInfoName(const u2 index) const {
    auto result = _constantPool.getClassInfoName(index);
    return result;
}

void ParserOutput::outputClass(const string &type) {
    const auto& mainClassInfo = _classFileAnalyzer.getMainClassInfo();
    cout << endl;
    cout << "//Class " << endl;
    cout << "//------------------------------------" << endl;
    const auto accessModifiers = _accessModifiers.toString(mainClassInfo.accessFlags);

    const auto attributesString = attributesToString(_classFileAnalyzer.getAttributes());

    cout << attributesString << accessModifiers << " " << type << " ";

    const string classname = getClassInfoName(mainClassInfo.thisClass);
    cout << classname;

    if (mainClassInfo.superClass != 0) {
        cout << " extends " << getClassInfoName(mainClassInfo.superClass);
    }

    // TODO implemented interfaces
}

void ParserOutput::outputClass() {

    outputClass(_classFileAnalyzer.getMainClassInfo().isInterface() ? "interface" : "class");
    cout << endl;
}

string ParserOutput::attributesToString(vector<AttributeInfo> &attributes) {

    vector<string> parts;

    parts.reserve(attributes.size());
    auto &attributesManager = _classFileAnalyzer.getAttributesManager();
    for (auto &attribute : attributes) {
        parts.push_back(attributesManager.toString(attribute));
    }

    auto result = stringUtils::join<string>(parts, "\n\n");
    if (!result.empty()) {
        result = "/*\n" + result + "\n*/\n";
    }
    return result;
}

void ParserOutput::outputField(FieldInfo &fieldInfo) {
    const auto accessModifiers = _accessModifiers.toString(fieldInfo.accessFlags);
    const auto name = _constantPool[fieldInfo.nameIndex].utf8Info.getAsUtf8();
    const auto descriptorString = _constantPool[fieldInfo.descriptorIndex].utf8Info.getAsUtf8();
    const auto descriptor = DescriptorParser(descriptorString).getDescriptor();

    const auto attributeString = attributesToString(fieldInfo.attributes);

    const vector<string> parts{attributeString, accessModifiers, descriptor.toString(), name};

    cout << stringUtils::join<string>(parts, " ") << ";";
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
