#include "AttributeStructures.h"
#include "VectorReader.h"
#include "AnnotationsParser.h"

#include <iostream>
#include <format>

using namespace org::kapa::tarrash::annotations;
using namespace org::kapa::tarrash::readers;
using namespace org::kapa::tarrash::attributes;
using namespace org::kapa::tarrash::stringUtils;
using namespace std;

wstring AnnotationsParser::toStringRuntimeAnnotations() const {
    RuntimeVisibleAnnotations annotations;
    annotations.nameIndex = _attribute.nameIndex;
    annotations.length = _attribute.length;
    annotations.count = _reader.readU2();
    readRuntimeAnnotations(annotations);
    wstring result = runtimeAnnotationsToString(annotations);
    return result;
}

wstring AnnotationsParser::toStringRuntimeParameterAnnotations() const {
    RuntimeVisibleParameterAnnotations annotations;
    annotations.nameIndex = _attribute.nameIndex;
    annotations.length = _attribute.length;
    annotations.parameterCount = _reader.readU1();
    readRuntimeParameterAnnotations(annotations);
    wstring result = runtimeParameterAnnotationsToString(annotations);
    return result;

}

std::wstring AnnotationsParser::toStringAnnotationDefault() const {
    AnnotationDefault annotation;
    annotation.nameIndex = _attribute.nameIndex;
    annotation.length = _attribute.length;
    readElementValue(annotation.defaultValue);
    const auto name = _constantPool.getString(annotation.nameIndex);
    const auto value = elementValueToString(annotation.defaultValue);
    wstring result = format( L"{}={}",name, value );
    return result;
}

wstring AnnotationsParser::annotationValuePairToString(
    const AnnotationValuePair &annotationValuePair) const {
    const auto name = _constantPool.getString(annotationValuePair.nameIndex);
    const auto value = elementValueToString(annotationValuePair.value);
    wstring result = format(L"{}=({})", name, value);
    return result;
}

wstring AnnotationsParser::annotationToString(const Annotation &annotation) const {
    const auto typeString = _constantPool.getTypeString(annotation.typeIndex);
    vector<wstring> valuePairs;

    for (auto &annotationValuePair : annotation.values) {
        valuePairs.push_back(annotationValuePairToString(annotationValuePair));
    }

    auto result = format(L"{}: ({})", typeString, join<wstring>(valuePairs, L", " ));

    return result;
}

wstring AnnotationsParser::parameterAnnotationToString(const ParameterAnnotation &parameterAnnotation) const {
    vector<wstring> valuePairs;

    for (auto &annotation : parameterAnnotation.items) {
        valuePairs.push_back(annotationToString(annotation));
    }
    auto result = join<wstring>(valuePairs, L", ");

    return result;
}

wstring AnnotationsParser::runtimeAnnotationsToString(const RuntimeAnnotations &annotations) const {

    vector<wstring> parts;
    for (auto &annotation : annotations.items) {
        parts.push_back(annotationToString(annotation));
    }
    auto result = join<wstring>(parts, L", ");
    return result;
}

wstring AnnotationsParser::runtimeParameterAnnotationsToString(const RuntimeParameterAnnotations &annotations) const {

    vector<wstring> parts;
    for (auto &parameterAnnotation : annotations.items) {
        parts.push_back(parameterAnnotationToString(parameterAnnotation));
    }
    auto result = format(L"({})", join<wstring>(parts, L", "));
    return result;
}

void AnnotationsParser::readElementValue(ElementValue &elementValue) const {
    elementValue.tag = static_cast<SignatureTypes>(_reader.readU1());
    wstring result;
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

void AnnotationsParser::readAnnotationValues(Annotation &annotations) const {
    for (auto i = 0u; i < annotations.count; ++i) {
        AnnotationValuePair annotationValuePair;
        annotationValuePair.nameIndex = _reader.readU2();

        // auto valueTypeString = _constantPool.getTypeString(annotationValuePair.nameIndex);

        readElementValue(annotationValuePair.value);
        annotations.values.push_back(annotationValuePair);
    }
}

void AnnotationsParser::readAnnotation(Annotation &annotation) const {
    annotation.typeIndex = _reader.readU2();
    annotation.count = _reader.readU2();
    readAnnotationValues(annotation);
}

void AnnotationsParser::readRuntimeAnnotations(RuntimeAnnotations &annotations) const {
    for (auto i = 0u; i < annotations.count; ++i) {
        Annotation annotation;
        readAnnotation(annotation);
        annotations.items.push_back(annotation);
    }
}

void AnnotationsParser::readParameterAnnotation(ParameterAnnotation &parameterAnnotation) const {
    parameterAnnotation.count = _reader.readU2();
    for (auto i = 0u; i < parameterAnnotation.count; i++) {
        Annotation annotation;
        readAnnotation(annotation);
        parameterAnnotation.items.push_back(annotation);
    }
}

void AnnotationsParser::readRuntimeParameterAnnotations(RuntimeParameterAnnotations &annotations) const {
    for (auto i = 0u; i < annotations.parameterCount; ++i) {
        ParameterAnnotation annotation;
        readParameterAnnotation(annotation);
        // auto typeString = _constantPool.getTypeString(annotation.typeIndex);
        // items.items.push_back(annotation);
        annotations.items.push_back(annotation);
    }
}

wstring AnnotationsParser::elementValueToString(const ElementValue &elementValue) const {
    wstring result;
    const auto &[constValueIndex, enumConstValue, classInfoIndex, annotationValue, arrayValues] =
        elementValue.value;

    switch (elementValue.tag) {
        case JVM_SIGNATURE_ARRAY: {
            vector<wstring> parts;
            for (auto &child : arrayValues.values) {
                parts.push_back(elementValueToString(*child));
            }
            result = join<wstring>(parts, L",");
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

        case JVM_SIGNATURE_CLASS2:
            result = _constantPool.getClassname(classInfoIndex);
            break;

        case JVM_SIGNATURE_ANNOTATION: {
            result = annotationToString(*annotationValue);
            break;
        }

        case JVM_SIGNATURE_ENUM2: {
            result = format(L"{}={}", 
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
