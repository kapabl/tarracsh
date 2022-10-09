#ifndef ANNOTATIONS_PARSER_H
#define ANNOTATIONS_PARSER_H

#include <string>
#include "VectorReader.h"
#include "ConstantPool.h"

namespace org::kapa::tarracsh::annotations {
class AnnotationsParser {
public:
    AnnotationsParser(const ConstantPool &constantPool, const attributes::AttributeInfo &attribute,
                      readers::VectorReader &reader)
        : _constantPool(constantPool), _attribute(attribute), _reader(reader) {

    }

    [[nodiscard]] std::wstring toStringRuntimeAnnotations() const;
    [[nodiscard]] std::wstring toStringRuntimeParameterAnnotations() const;
    [[nodiscard]] std::wstring toStringRuntimeTypeAnnotations() const;
    [[nodiscard]] std::wstring toStringAnnotationDefault() const;

private:
    const ConstantPool &_constantPool;
    const attributes::AttributeInfo &_attribute;
    readers::VectorReader &_reader;

    [[nodiscard]] std::wstring localVarAnnotationToString(const attributes::TypeAnnotation &typeAnnotation) const;
    [[nodiscard]] std::wstring annotationToString(const attributes::AnnotationValuePair &annotationValuePair) const;
    [[nodiscard]] std::wstring annotationToString(const attributes::Annotation &annotation) const;
    [[nodiscard]] std::wstring annotationToString(const attributes::ParameterAnnotation &parameterAnnotation) const;
    [[nodiscard]] std::wstring annotationsToString(const attributes::RuntimeAnnotations &annotations) const;
    [[nodiscard]] std::wstring annotationsToString(const attributes::RuntimeParameterAnnotations &annotations) const;
    [[nodiscard]] std::wstring elementValueToString(const attributes::ElementValue &elementValue) const;
    [[nodiscard]] std::wstring annotationToString(const attributes::TypeAnnotation &typeAnnotation) const;
    [[nodiscard]] std::wstring annotationsToString(const attributes::RuntimeTypeAnnotations &typeAnnotations) const;


    void readElementValue(attributes::ElementValue &elementValue) const;
    void readAnnotationValuePair(attributes::AnnotationValuePair &annotationValuePair) const;
    void readAnnotation(attributes::Annotation &annotation) const;
    void readAnnotations(attributes::RuntimeAnnotations &annotations) const;
    void readAnnotation(attributes::ParameterAnnotation &parameterAnnotation) const;
    void readAnnotations(attributes::RuntimeParameterAnnotations &annotations) const;
    void readAnnotation(attributes::TypeAnnotation &typeAnnotation) const;
    void readAnnotations(attributes::RuntimeTypeAnnotations &runtimeTypeAnnotations) const;

    template <typename T>
    void readAnnotationValues(T &annotation) const {
        for (auto i = 0u; i < annotation.count; ++i) {
            attributes::AnnotationValuePair annotationValuePair;
            readAnnotationValuePair(annotationValuePair);
            annotation.values.push_back(annotationValuePair);
        }
    }


};


}


#endif
