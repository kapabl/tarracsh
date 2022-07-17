#ifndef ANNOTATIONS_PARSER_H
#define ANNOTATIONS_PARSER_H

#include <string>
#include "VectorReader.h"
#include "ConstantPool.h"

namespace org::kapa::tarracsh::annotations {
class AnnotationsParser {
public:
    AnnotationsParser(const ConstantPool &constantPool, const attributes::AttributeInfo &attribute, readers::VectorReader &reader)
        : _constantPool(constantPool), _attribute(attribute), _reader(reader) {

    }

    void readElementValue(attributes::ElementValue &elementValue) const;
    void readAnnotationValues(attributes::Annotation &annotations) const;
    void readAnnotation(attributes::Annotation &annotation) const;
    void readRuntimeAnnotations(attributes::RuntimeAnnotations &annotations) const;
    void readParameterAnnotation(attributes::ParameterAnnotation &parameterAnnotation) const;
    void readRuntimeParameterAnnotations(attributes::RuntimeParameterAnnotations &annotations) const;

    std::wstring toStringRuntimeAnnotations() const;
    std::wstring toStringRuntimeParameterAnnotations() const;
    std::wstring toStringAnnotationDefault() const;

private:
    const ConstantPool &_constantPool;
    const attributes::AttributeInfo &_attribute;
    readers::VectorReader &_reader;

    std::wstring annotationValuePairToString(const attributes::AnnotationValuePair &annotationValuePair) const;
    std::wstring annotationToString(const attributes::Annotation &annotation) const;
    std::wstring parameterAnnotationToString(const attributes::ParameterAnnotation &parameterAnnotation) const;
    std::wstring runtimeAnnotationsToString(const attributes::RuntimeAnnotations &annotations) const;
    std::wstring runtimeParameterAnnotationsToString(const attributes::RuntimeParameterAnnotations &annotations) const;
    std::wstring elementValueToString(const attributes::ElementValue &elementValue) const;


};


}


#endif
