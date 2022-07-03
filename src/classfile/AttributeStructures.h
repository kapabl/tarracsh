//
// Created by xman on 6/29/2022.
//

#ifndef ATTRIBUTE_STRUCTURES_H
#define ATTRIBUTE_STRUCTURES_H

#include <memory>
#include <string>
#include <vector>
#include "StructsCommon.h"
#include "StackFrame.h"

#pragma pack(push,1)

namespace org::kapa::tarrash::attributes {

struct AnnotationValuePair;

using common::u1;
using common::u2;
using common::u4;

enum AttributeOwner { ClassFile, Method, Field };

struct AttributeBase {
    u2 nameIndex{};
    u4 length{};
};

struct AttributeInfo : AttributeBase {
    std::vector<u1> info;
    AttributeOwner owner;
};

struct ConstantValue : AttributeBase {

    u2 constantValueIndex{};

    struct Value {
        union {
            long long longValue{};
            float floatValue;
            double doubleValue;
            int intValue;
            bool boolValue;
            char charValue;
            short shortValue;
            std::byte byteValue;
        };

        std::wstring string;
    } value;
};

struct ExceptionTable {
    u2 startPC;
    u2 endPC;
    u2 handlerPC;
    u2 catchType;
};

struct Attribute {
    // TODO
};

struct StackMapTable : AttributeBase {
    u2 entryCount;
    std::vector<stack::StackMapFrame> entries;
};

struct Code : AttributeBase {
    u2 maxStack;
    u2 maxLocals;
    u4 codeLength;
    std::vector<u1> code;
    u2 exceptionTableLength;
    std::vector<ExceptionTable> exceptionTable;
    u2 attributesCount;
    std::vector<Attribute> attributes;
};

struct Exceptions : AttributeBase {
    u2 number_of_exceptions;
    std::vector<u2> exceptionIndexTable;
};

struct InnerClass {
    u2 innerClassInfoIndex;
    u2 outerClassInfoIndex;
    u2 innerNameIndex;
    u2 innerClassAccessFlags;
};

struct InnerClasses : AttributeBase {
    u2 numberOfClasses;
    std::vector<InnerClass> classes;
};

struct EnclosingMethod : AttributeBase {

    u2 class_index{};
    u2 method_index{};
};

struct Synthetic : AttributeBase {
};

struct Signature : AttributeBase {
    u2 signatureIndex{};
};

struct SourceFile : AttributeBase {
    u2 sourceFileIndex{};
};

struct SourceDebugExtension : AttributeBase {
    std::vector<u1> debugExtensions;
};

struct LineNumber {
    u2 start_pc;
    u2 line_number;
};

struct LineNumberTable {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    std::vector<LineNumber> lineNumbers;
};

struct LocalVariable {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 descriptor_index;
    u2 index;
};

struct LocalVariableTable : AttributeBase {
    u2 local_variable_table_length;
    std::vector<LocalVariable> variables;
};

struct LocalVariableType {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;
};

struct LocalVariableTypeTable : AttributeBase {
    std::vector<LocalVariableType> types;
};

struct Deprecated {
    u2 attribute_name_index;
    u4 attribute_length;
};


/**
 *
 * Note: here you can see the values of tag hard-coded in openjdk
 * https://sourcegraph.com/github.com/openjdk/jdk/-/blob/src/hotspot/share/classfile/classFileParser.cpp?L1077
 */

struct Annotation;

struct ElementValue {

    ElementValue() = default;
    ElementValue(const ElementValue &value) = default;
    ElementValue(ElementValue &&value) = default;

    ~ElementValue() = default;

    ElementValue &operator=(const ElementValue &right) = default;
    ElementValue &operator=(ElementValue &&right) = default;

    SignatureTypes tag = SignatureTypes::JVM_SIGNATURE_VOID;


    struct Value {

        u2 constValueIndex = 0;

        struct {
            u2 typeNameIndex = 0;
            u2 constNameIndex = 0;
        } enumConstValue;

        u2 classInfoIndex = 0;

        std::shared_ptr<Annotation> annotationValue;

        struct ArrayValues {
            ArrayValues() = default;
            ArrayValues(const ArrayValues &value) = default;
            ArrayValues(ArrayValues &&value) = default;

            ~ArrayValues() = default;

            ArrayValues &operator=(const ArrayValues &right) = default;
            ArrayValues &operator=(ArrayValues &&right) = default;

            u2 count = 0;
            std::vector<std::shared_ptr<ElementValue>> values;
        } arrayValues;

        Value() = default;
        Value(const Value &value) = default;
        Value(Value &&value) = default;

        ~Value() = default;

        Value &operator=(const Value &right) = default;
        Value &operator=(Value &&right) = default;

    } value;

};

struct AnnotationValuePair {
    u2 nameIndex = 0;
    ElementValue value;

    AnnotationValuePair() = default;

    AnnotationValuePair(const AnnotationValuePair &right) {
        nameIndex = right.nameIndex;
        value = right.value;
    }

    AnnotationValuePair(AnnotationValuePair &&right) noexcept {
        nameIndex = right.nameIndex;
        value = right.value;
    }

    AnnotationValuePair &operator=(const AnnotationValuePair &right) = default;
    AnnotationValuePair &operator=(AnnotationValuePair &&right) = default;


    ~AnnotationValuePair() = default;

};

struct Annotation {
    u2 typeIndex = 0;
    u2 count = 0;
    std::vector<AnnotationValuePair> values;
};

struct RuntimeAnnotations : AttributeBase {
    u2 count{};
    std::vector<Annotation> items;
};

struct RuntimeVisibleAnnotations : RuntimeAnnotations {
};

struct RuntimeInvisibleAnnotations : RuntimeAnnotations {
};


struct ParameterAnnotation {
    u2 count = 0;
    std::vector<Annotation> items;
};

struct RuntimeParameterAnnotations : AttributeBase {
    u1 parameterCount = 0;
    std::vector<ParameterAnnotation> items;

};

struct RuntimeVisibleParameterAnnotations : RuntimeParameterAnnotations {
};

struct RuntimeInvisibleParameterAnnotations : RuntimeParameterAnnotations {
};

struct AnnotationDefault : AttributeBase {
    ElementValue defaultValue;
};

#pragma pack(pop)

}
#endif // ATTRIBUTE_STRUCTURES_H
