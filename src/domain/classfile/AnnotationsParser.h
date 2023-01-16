#ifndef ANNOTATIONS_PARSER_H
#define ANNOTATIONS_PARSER_H

#include <string>
#include "reader/VectorReader.h"
#include "constantpool/ConstantPool.h"

namespace kapa::tarracsh::domain::classfile::annotations {
class AnnotationsParser {
public:
    AnnotationsParser(const constantpool::ConstantPool &constantPool, const attribute::AttributeInfo &attribute,
                      reader::VectorReader &reader)
        : _constantPool(constantPool), _attribute(attribute), _reader(reader) {

    }

    [[nodiscard]] std::string toStringRuntimeAnnotations() const;
    [[nodiscard]] std::string toStringRuntimeParameterAnnotations() const;
    [[nodiscard]] std::string toStringRuntimeTypeAnnotations() const;
    [[nodiscard]] std::string toStringAnnotationDefault() const;

private:
    const constantpool::ConstantPool &_constantPool;
    const attribute::AttributeInfo &_attribute;
    reader::VectorReader &_reader;

    [[nodiscard]] std::string localVarAnnotationToString(const attribute::TypeAnnotation &typeAnnotation) const;
    [[nodiscard]] std::string annotationToString(const attribute::AnnotationValuePair &annotationValuePair) const;
    [[nodiscard]] std::string annotationToString(const attribute::Annotation &annotation) const;
    [[nodiscard]] std::string annotationToString(const attribute::ParameterAnnotation &parameterAnnotation) const;
    [[nodiscard]] std::string annotationsToString(const attribute::RuntimeAnnotations &annotations) const;
    [[nodiscard]] std::string annotationsToString(const attribute::RuntimeParameterAnnotations &annotations) const;
    [[nodiscard]] std::string annotationToString(const attribute::TypeAnnotation &typeAnnotation) const;
    [[nodiscard]] std::string annotationsToString(const attribute::RuntimeTypeAnnotations &typeAnnotations) const;

    [[nodiscard]] std::string elementValueToString(const attribute::ElementValue& elementValue) const;


    void readElementValue(attribute::ElementValue &elementValue) const;
    void readAnnotationValuePair(attribute::AnnotationValuePair &annotationValuePair) const;
    void readAnnotation(attribute::Annotation &annotation) const;
    void readAnnotations(attribute::RuntimeAnnotations &annotations) const;
    void readAnnotation(attribute::ParameterAnnotation &parameterAnnotation) const;
    void readAnnotations(attribute::RuntimeParameterAnnotations &annotations) const;
    void readAnnotation(attribute::TypeAnnotation &typeAnnotation) const;
    void readAnnotations(attribute::RuntimeTypeAnnotations &runtimeTypeAnnotations) const;

    template <typename T>
    void readAnnotationValues(T &annotation) const {
        for (auto i = 0u; i < annotation.count; ++i) {
            attribute::AnnotationValuePair annotationValuePair;
            readAnnotationValuePair(annotationValuePair);
            annotation.values.push_back(annotationValuePair);
        }
    }


};


}


#endif
