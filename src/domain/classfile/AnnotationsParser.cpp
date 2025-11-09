#include "AttributeStructures.h"
#include "reader/VectorReader.h"
#include "AnnotationsParser.h"

#include <iostream>
#include <fmt/format.h>
using namespace std;
using namespace kapa::tarracsh::domain::classfile;
using namespace annotations;
using namespace kapa::tarracsh::domain::classfile::reader;
using namespace attribute;
using namespace kapa::infrastructure::string::stringUtils;


string AnnotationsParser::toStringRuntimeAnnotations() const {
    RuntimeVisibleAnnotations annotations;
    annotations.nameIndex = _attribute.nameIndex;
    annotations.length = _attribute.length;
    annotations.count = _reader.readU2();
    readAnnotations(annotations);
    string result = annotationsToString(annotations);
    return result;
}

string AnnotationsParser::toStringRuntimeParameterAnnotations() const {
    RuntimeVisibleParameterAnnotations annotations;
    annotations.nameIndex = _attribute.nameIndex;
    annotations.length = _attribute.length;
    annotations.parameterCount = _reader.readU1();
    readAnnotations(annotations);
    string result = annotationsToString(annotations);
    return result;

}


string AnnotationsParser::localVarAnnotationToString(const TypeAnnotation &typeAnnotation) const {
    std::vector<string> locations;
    for (const auto &[startPC, length, index] : typeAnnotation.local_var_target.items) {
        locations.push_back(
            fmt::format("(start_pc:{},length:{},index:{})", startPC, length, index));
    }
    auto locationTables = join<string>(locations, ",");
    const auto result = fmt::format("local var locations: {}", locationTables);
    return result;
}

string AnnotationsParser::annotationToString(
    const attribute::TypeAnnotation &typeAnnotation) const {

    string target_description;
    switch (typeAnnotation.targetType) {
        case 0x00:
            target_description = fmt::format("index: {} class/interface",
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;

        case 0x01:
            target_description = fmt::format("index: {} member/constructor",
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;

        case 0x10:
            target_description = fmt::format("index: {} super-type", typeAnnotation.targetInfo.supertype_target);
            break;

        case 0x11:
            target_description = fmt::format("index, bound: {},{} class/interface",
                            typeAnnotation.targetInfo.type_parameter_bound_target.type_parameter_index,
                            typeAnnotation.targetInfo.type_parameter_bound_target.bound_index);
            break;

        case 0x12:
            target_description = fmt::format("index, bound: {},{} member/constructor",
                            typeAnnotation.targetInfo.type_parameter_bound_target.type_parameter_index,
                            typeAnnotation.targetInfo.type_parameter_bound_target.bound_index);
            break;

        case 0x13:
        case 0x14:
        case 0x15:
            target_description = "field";
            break;

        case 0x16:
            target_description = fmt::format("param index: {}",
                            typeAnnotation.targetInfo.formal_parameter_target.formal_parameter_index);
            break;

        case 0x17:
            target_description = fmt::format("exception index: {}",
                            typeAnnotation.targetInfo.throws_target.throws_type_index);
            break;

        case 0x40:
            target_description = fmt::format("local var: {}", localVarAnnotationToString(typeAnnotation));
            break;

        case 0x41:
            target_description = fmt::format("local res var: {}", localVarAnnotationToString(typeAnnotation));
            break;

        case 0x42:
            target_description = fmt::format("type exception param index: {}",
                            typeAnnotation.targetInfo.catch_target.exception_table_index);
            break;

        case 0x43:
            target_description = fmt::format("instanceof bytecode offset: {}",
                            typeAnnotation.targetInfo.offset_target.offset);
            break;
        case 0x44:
            target_description = fmt::format("new bytecode offset: {}",
                            typeAnnotation.targetInfo.offset_target.offset);
            break;
        case 0x45:
            target_description = fmt::format("methodref ::new bytecode offset: {}",
                            typeAnnotation.targetInfo.offset_target.offset);
            break;
        case 0x46:
            target_description = fmt::format("methodref ::Identifier bytecode offset: {}",
                            typeAnnotation.targetInfo.offset_target.offset);
            break;

        case 0x47:
            target_description = fmt::format("type-cast bytecode offset: {}, index: {}",
                            typeAnnotation.targetInfo.type_argument_target.offset,
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;

        case 0x48:
            target_description = fmt::format("arg. gen constructor bytecode offset: {}, index: {}",
                            typeAnnotation.targetInfo.type_argument_target.offset,
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;

        case 0x49:
            target_description = fmt::format("arg. gen member call bytecode offset: {}, index: {}",
                            typeAnnotation.targetInfo.type_argument_target.offset,
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;
        case 0x4A:
            target_description = fmt::format("arg. gen constructor methodref ::new bytecode offset: {}, index: {}",
                            typeAnnotation.targetInfo.type_argument_target.offset,
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;
        case 0x4B:
            target_description = fmt::format("arg. gen constructor methodref ::Identifier bytecode offset: {}, index: {}",
                            typeAnnotation.targetInfo.type_argument_target.offset,
                            typeAnnotation.targetInfo.type_argument_target.type_argument_index);
            break;

        default:
            target_description = "Invalid Runtime type annotation type";
    }

    vector<string> valuePairs;
    for (auto &annotationValuePair : typeAnnotation.values) {
        valuePairs.push_back(annotationToString(annotationValuePair));
    }
    const auto valueSuffix =
        valuePairs.empty() ? string() : fmt::format(" [{}]", join<string>(valuePairs, ", "));
    const auto typeString = _constantPool.getTypeString(typeAnnotation.typeIndex);
    return fmt::format("{}: {}{}", typeString, target_description, valueSuffix);
}

std::string AnnotationsParser::annotationsToString(
    const attribute::RuntimeTypeAnnotations &typeAnnotations) const {
    vector<string> parts;
    for (auto &typeAnnotation : typeAnnotations.items) {
        parts.push_back(annotationToString(typeAnnotation));
    }

    auto result = join<string>(parts, "|");

    return result;
}

string AnnotationsParser::toStringRuntimeTypeAnnotations() const {
    RuntimeTypeAnnotations typeAnnotations;
    typeAnnotations.nameIndex = _attribute.nameIndex;
    typeAnnotations.length = _attribute.length;
    typeAnnotations.count = _reader.readU2();
    readAnnotations(typeAnnotations);
    string result = annotationsToString(typeAnnotations);
    return result;

}

std::string AnnotationsParser::toStringAnnotationDefault() const {
    AnnotationDefault annotation;
    annotation.nameIndex = _attribute.nameIndex;
    annotation.length = _attribute.length;
    readElementValue(annotation.defaultValue);
    const auto name = _constantPool.getString(annotation.nameIndex);
    const auto value = elementValueToString(annotation.defaultValue);
    string result = fmt::format("{}={}", name, value);
    return result;
}

string AnnotationsParser::annotationToString(
    const AnnotationValuePair &annotationValuePair) const {
    const auto name = _constantPool.getString(annotationValuePair.nameIndex);
    const auto value = elementValueToString(annotationValuePair.value);
    string result = fmt::format("{}=({})", name, value);
    return result;
}

string AnnotationsParser::annotationToString(const Annotation &annotation) const {
    const auto typeString = _constantPool.getTypeString(annotation.typeIndex);
    vector<string> valuePairs;

    for (auto &annotationValuePair : annotation.values) {
        valuePairs.push_back(annotationToString(annotationValuePair));
    }

    auto result = fmt::format("{}: ({})", typeString, join<string>(valuePairs, ", "));

    return result;
}

string AnnotationsParser::annotationToString(const ParameterAnnotation &parameterAnnotation) const {
    vector<string> valuePairs;

    for (auto &annotation : parameterAnnotation.items) {
        valuePairs.push_back(annotationToString(annotation));
    }
    auto result = join<string>(valuePairs, ", ");

    return result;
}

string AnnotationsParser::annotationsToString(const RuntimeAnnotations &annotations) const {

    vector<string> parts;
    for (auto &annotation : annotations.items) {
        parts.push_back(annotationToString(annotation));
    }
    auto result = join<string>(parts, ", ");
    return result;
}

string AnnotationsParser::annotationsToString(const RuntimeParameterAnnotations &annotations) const {

    vector<string> parts;
    for (auto &parameterAnnotation : annotations.items) {
        parts.push_back(annotationToString(parameterAnnotation));
    }
    auto result = fmt::format("({})", join<string>(parts, ", "));
    return result;
}

void AnnotationsParser::readElementValue(ElementValue &elementValue) const {
    elementValue.tag = static_cast<SignatureTypes>(_reader.readU1());
    string result;
    switch (elementValue.tag) {

        case JVM_SIGNATURE_ARRAY:
            elementValue.value.arrayValues.count = _reader.readU2();
            for (auto i = 0u; i < elementValue.value.arrayValues.count; i++) {
                shared_ptr<ElementValue> childElementValue(new ElementValue());
                readElementValue(*childElementValue);
                elementValue.value.arrayValues.values.push_back(childElementValue);
            }

            break;

        case JVM_SIGNATURE_INT:
        case JVM_SIGNATURE_LONG:
        case JVM_SIGNATURE_SHORT:
        case JVM_SIGNATURE_BOOLEAN:
        case JVM_SIGNATURE_CHAR:
        case JVM_SIGNATURE_BYTE:
        case JVM_SIGNATURE_FLOAT:
        case JVM_SIGNATURE_DOUBLE:
        case JVM_SIGNATURE_STRING:
            elementValue.value.constValueIndex = _reader.readU2();
            break;

        case JVM_SIGNATURE_CLASS2:
            elementValue.value.classInfoIndex = _reader.readU2();

            break;

        case JVM_SIGNATURE_ANNOTATION: {
            const shared_ptr<Annotation> childAnnotation(new Annotation());
            readAnnotation(*childAnnotation);
            elementValue.value.annotationValue = childAnnotation;
            break;
        }

        case JVM_SIGNATURE_ENUM2:
            elementValue.value.enumConstValue.typeNameIndex = _reader.readU2();
            elementValue.value.enumConstValue.constNameIndex = _reader.readU2();
            break;

        case JVM_SIGNATURE_SLASH:
        case JVM_SIGNATURE_DOT:
        case JVM_SIGNATURE_SPECIAL:
        case JVM_SIGNATURE_ENDSPECIAL:
        case JVM_SIGNATURE_CLASS:
        case JVM_SIGNATURE_ENDCLASS:
        case JVM_SIGNATURE_ENUM:
        case JVM_SIGNATURE_FUNC:
        case JVM_SIGNATURE_ENDFUNC:
        case JVM_SIGNATURE_VOID:
            assert(false);
            cout << "Invalid Element Value Annotation";
            break;
    }
}


void AnnotationsParser::readAnnotationValuePair(AnnotationValuePair &annotationValuePair) const {
    annotationValuePair.nameIndex = _reader.readU2();
    readElementValue(annotationValuePair.value);
}

void AnnotationsParser::readAnnotation(Annotation &annotation) const {
    annotation.typeIndex = _reader.readU2();
    annotation.count = _reader.readU2();
    readAnnotationValues(annotation);
}

void AnnotationsParser::readAnnotations(RuntimeAnnotations &annotations) const {
    for (auto i = 0u; i < annotations.count; ++i) {
        Annotation annotation;
        readAnnotation(annotation);
        annotations.items.push_back(annotation);
    }
}

void AnnotationsParser::readAnnotation(ParameterAnnotation &parameterAnnotation) const {
    parameterAnnotation.count = _reader.readU2();
    for (auto i = 0u; i < parameterAnnotation.count; i++) {
        Annotation annotation;
        readAnnotation(annotation);
        parameterAnnotation.items.push_back(annotation);
    }
}

void AnnotationsParser::readAnnotations(RuntimeParameterAnnotations &annotations) const {
    for (auto i = 0u; i < annotations.parameterCount; ++i) {
        ParameterAnnotation annotation;
        readAnnotation(annotation);
        annotations.items.push_back(annotation);
    }
}

void AnnotationsParser::readAnnotation(TypeAnnotation &typeAnnotation) const {
    typeAnnotation.targetType = _reader.readU1();
    switch (typeAnnotation.targetType) {
        case 0x00:
        case 0x01:
            typeAnnotation.targetInfo.type_parameter_target = _reader.readU1();
            break;

        case 0x10:
            typeAnnotation.targetInfo.supertype_target = _reader.readU2();
            break;

        case 0x11:
        case 0x12:
            typeAnnotation.targetInfo.type_parameter_bound_target.type_parameter_index = _reader.readU1();
            typeAnnotation.targetInfo.type_parameter_bound_target.bound_index = _reader.readU1();
            break;

        case 0x13:
        case 0x14:
        case 0x15:
            break;

        case 0x16:
            typeAnnotation.targetInfo.formal_parameter_target.formal_parameter_index = _reader.readU1();
            break;

        case 0x17:
            typeAnnotation.targetInfo.throws_target.throws_type_index = _reader.readU2();
            break;

        case 0x40:
        case 0x41: {
            const auto count = _reader.readU2();
            typeAnnotation.local_var_target.table_length = count;
            for (auto i = 0u; i < count; i++) {
                LocalVarTargetEntry localVarTargetEntry{};
                localVarTargetEntry.startPC = _reader.readU2();
                localVarTargetEntry.length = _reader.readU2();
                localVarTargetEntry.index = _reader.readU2();
                typeAnnotation.local_var_target.items.push_back(localVarTargetEntry);
            }

            break;
        }

        case 0x42:
            typeAnnotation.targetInfo.catch_target.exception_table_index = _reader.readU2();
            break;

        case 0x43:
        case 0x44:
        case 0x45:
        case 0x46:
            typeAnnotation.targetInfo.offset_target.offset = _reader.readU2();
            break;

        case 0x47:
        case 0x48:
        case 0x49:
        case 0x4A:
        case 0x4B:
            typeAnnotation.targetInfo.type_argument_target.offset = _reader.readU2();
            typeAnnotation.targetInfo.type_argument_target.type_argument_index = _reader.readU2();
            break;

        default:
            assert(false);
            cout << "Invalid Runtime type annotation type\n";
    }

    typeAnnotation.targetPath.length = _reader.readU1();
    for (u1 index = 0; index < typeAnnotation.targetPath.length; index++) {
        TargetPathItem item{_reader.readU1(), _reader.readU1()};
        typeAnnotation.targetPath.items.push_back(item);
    }

    typeAnnotation.typeIndex = _reader.readU2();
    typeAnnotation.count = _reader.readU2();

    readAnnotationValues(typeAnnotation);

}

void AnnotationsParser::readAnnotations(RuntimeTypeAnnotations &runtimeTypeAnnotations) const {
    for (auto i = 0u; i < runtimeTypeAnnotations.count; ++i) {
        TypeAnnotation typeAnnotation;
        readAnnotation(typeAnnotation);
        runtimeTypeAnnotations.items.push_back(typeAnnotation);
    }
}

string AnnotationsParser::elementValueToString(const ElementValue &elementValue) const {
    string result;
    const auto &[constValueIndex, enumConstValue, classInfoIndex, annotationValue, arrayValues] =
        elementValue.value;

    switch (elementValue.tag) {
        case JVM_SIGNATURE_ARRAY: {
            vector<string> parts;
            for (auto &child : arrayValues.values) {
                parts.push_back(elementValueToString(*child));
            }
            result = join<string>(parts, ",");
            break;
        }

        case JVM_SIGNATURE_INT:
        case JVM_SIGNATURE_LONG:
        case JVM_SIGNATURE_SHORT:
        case JVM_SIGNATURE_BOOLEAN:
        case JVM_SIGNATURE_CHAR:
        case JVM_SIGNATURE_BYTE:
        case JVM_SIGNATURE_FLOAT:
        case JVM_SIGNATURE_DOUBLE:
        case JVM_SIGNATURE_STRING:
            result = _constantPool.getConstantValueString(constValueIndex);
            break;

        case JVM_SIGNATURE_CLASS2: {
            result = _constantPool.getClassname(classInfoIndex);
        }
        break;

        case JVM_SIGNATURE_ANNOTATION: {
            result = annotationToString(*annotationValue);
            break;
        }

        case JVM_SIGNATURE_ENUM2: {
            result = fmt::format("{}={}",
                            _constantPool.getString(enumConstValue.typeNameIndex),
                            _constantPool.getConstantValueString(enumConstValue.constNameIndex));
            break;
        }

        case JVM_SIGNATURE_SLASH:
        case JVM_SIGNATURE_DOT:
        case JVM_SIGNATURE_SPECIAL:
        case JVM_SIGNATURE_ENDSPECIAL:
        case JVM_SIGNATURE_CLASS:
        case JVM_SIGNATURE_ENDCLASS:
        case JVM_SIGNATURE_ENUM:
        case JVM_SIGNATURE_FUNC:
        case JVM_SIGNATURE_ENDFUNC:
        case JVM_SIGNATURE_VOID:
            assert(false);
            cout << "Invalid Element Value Annotation";
            break;
    }
    return result;
}
