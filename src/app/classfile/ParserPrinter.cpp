#include <iostream>
#include "ParserPrinter.h"

#include "../../domain/classfile/MethodDescriptorParser.h"
#include "../../domain/classfile/ClassFileParser.h"


using namespace kapa::tarracsh::app::classfile;
using namespace kapa::tarracsh::domain::classfile;
using namespace attribute;
using namespace constantpool;
using namespace std;

ParserPrinter::ParserPrinter(ClassFileParser &classFileParser)
    : _classFileParser(classFileParser),
      _accessModifiers(classFileParser.getAccessModifiers()),
      _constantPool(classFileParser.getConstantPool()) {

}

void ParserPrinter::print() {
    outputClass();
    outputMethods();
    outputFields();
    outputInterfaces();
}

void ParserPrinter::outputAccessModifiers(const u2 accessFlags) const {
    cout << _accessModifiers.toString(accessFlags) << " ";
}

void ParserPrinter::outputMethod(MethodInfo &methodInfo) {
    const auto accessModifiers = _accessModifiers.toString(methodInfo.accessFlags);
    const auto name = _constantPool.getEntry(methodInfo.nameIndex).utf8Info.getAsUtf8();
    const auto &utf8DDesc = _constantPool.getEntry(methodInfo.descriptorIndex).utf8Info;

    MethodDescriptorParser methodDescriptorParser(utf8DDesc.getAsUtf8());
    auto &methodDescriptor = methodDescriptorParser.getDescriptor();
    const auto returnType = methodDescriptor.returnType.toString();
    const auto arguments = methodDescriptor.argumentsToString();

    const auto attributesString = attributesToString(methodInfo.attributes);

    const vector<string> parts{attributesString, accessModifiers, returnType, name + arguments};

    cout << infrastructure::string::stringUtils::join<string>(parts, " ") << ";";
    cout << endl;
}

void ParserPrinter::outputMethods() {
    auto& methods = _classFileParser.getMethods();
    cout << endl;
    cout << "//Methods: " << methods.size() << endl;
    cout << "//-----------------------------------" << endl;
    for (auto &methodInfo : methods) {
        outputMethod(methodInfo);
        cout << endl;
    }
    cout << endl;
}

std::string ParserPrinter::getClassInfoName(const u2 index) const {
    auto result = _constantPool.getClassInfoName(index);
    return result;
}

void ParserPrinter::outputClass(const string &type) {
    const auto& mainClassInfo = _classFileParser.getMainClassInfo();
    cout << endl;
    cout << "//Class " << endl;
    cout << "//------------------------------------" << endl;
    const auto accessModifiers = _accessModifiers.toString(mainClassInfo.accessFlags);

    const auto attributesString = attributesToString(_classFileParser.getAttributes());

    cout << attributesString << accessModifiers << " " << type << " ";

    const string classname = getClassInfoName(mainClassInfo.thisClass);
    cout << classname;

    if (mainClassInfo.superClass != 0) {
        cout << " extends " << getClassInfoName(mainClassInfo.superClass);
    }

    // TODO implemented interfaces
}

void ParserPrinter::outputClass() {

    outputClass(_classFileParser.getMainClassInfo().isInterface() ? "interface" : "class");
    cout << endl;
}

string ParserPrinter::attributesToString(vector<AttributeInfo> &attributes) {

    vector<string> parts;

    parts.reserve(attributes.size());
    const auto &attributesManager = _classFileParser.getAttributesManager();
    for (auto &attribute : attributes) {
        parts.push_back(attributesManager.toString(attribute));
    }

    auto result = infrastructure::string::stringUtils::join<string>(parts, "\n\n");
    if (!result.empty()) {
        result = "/*\n" + result + "\n*/\n";
    }
    return result;
}

void ParserPrinter::outputField(FieldInfo &fieldInfo) {
    const auto accessModifiers = _accessModifiers.toString(fieldInfo.accessFlags);
    const auto name = _constantPool.getEntry(fieldInfo.nameIndex).utf8Info.getAsUtf8();
    const auto descriptorString = _constantPool.getEntry(fieldInfo.descriptorIndex).utf8Info.getAsUtf8();
    const auto descriptor = DescriptorParser(descriptorString).getDescriptor();

    const auto attributeString = attributesToString(fieldInfo.attributes);

    const vector parts{attributeString, accessModifiers, descriptor.toString(), name};

    cout << infrastructure::string::stringUtils::join<string>(parts, " ") << ";";
}

void ParserPrinter::outputFields() {
    auto& fields = _classFileParser.getFields();
    cout << endl;
    cout << "//Fields: " << fields.size() << endl;
    cout << "//------------------------------------" << endl;
    for (auto &fieldInfo : fields) {
        outputField(fieldInfo);
        cout << endl << endl;
    }
    cout << endl;
}

void ParserPrinter::outputInterfaces() {
    // TODO
}
