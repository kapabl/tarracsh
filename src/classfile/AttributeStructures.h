//
// Created by xman on 6/29/2022.
//

#ifndef ATTRIBUTE_STRUCTURES_H
#define ATTRIBUTE_STRUCTURES_H

#include <memory>
#include <string>
#include <vector>
#include "constpool/StructsCommon.h"
#include "constpool/StackFrame.h"

#pragma pack(push,1)

namespace org::kapa::tarracsh::attributes {

struct AnnotationValuePair;

using common::u1;
using common::u2;
using common::u4;

enum AttributeOwner { ClassFile, Method, Field, CodeAttribute, RecordComponentInfoAttribute };

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

        std::string string;
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


struct BootstrapMethod {
    u2 bootstrapMethodRef;
    // u2 numBootstrapArguments;
    std::vector<u2> bootstrapArguments;
};


struct BootstrapMethods : AttributeBase {
    // u2 numBootstrapMethods;
    std::vector<BootstrapMethods> bootstrapMethods;
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
    // u2 numberOfExceptions;
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

    u2 classIndex{};
    u2 methodIndex{};
};

struct Synthetic : AttributeBase {
};

struct Signature : AttributeBase {
    u2 signatureIndex{};
};

struct SourceFile : AttributeBase {
    u2 sourceFileIndex{};
};

/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se15/preview/specs/records-jvms.html
 */
struct RecordComponentInfo {
    u2 nameIndex;
    u2 descriptorIndex;
    u2 count;
    std::vector<AttributeInfo> items;
};

/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se15/preview/specs/records-jvms.html
 */
struct Record: AttributeBase {
    u2 count{};
    std::vector<RecordComponentInfo> items;
};


/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se15/preview/specs/sealed-classes-jvms.html#jvms-4.7.30
 */
struct PermittedSubclasses : AttributeBase {
    u2 numberOfClasses;
    std::vector<u2> classes;
};


struct SourceDebugExtension : AttributeBase {
    // std::vector<u1> debugExtensions;
    std::string debugExtensions;
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


/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.21
 */
struct LocalVarTargetEntry {
    u2 startPC;
    u2 length;
    u2 index;
};


/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.20.2
 */
struct TargetPathItem {
    u1 type_path_kind;
    u1 type_argument_index;
};

/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.20.2
 */
struct TargetPath {
    u1 length;
    std::vector<TargetPathItem> items;
};


/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.21
 */
struct TypeAnnotation {
    u1 targetType;

    union TargetInfo {
        u1 type_parameter_target;
        u2 supertype_target;

        struct TypeParameterBoundTarget {
            u1 type_parameter_index;
            u1 bound_index;
        } type_parameter_bound_target;

        struct FormalParameterTarget {
            u1 formal_parameter_index;
        } formal_parameter_target;

        // empty_target;
        //u2 throws_target;
        struct ThrowsTarget {
            u2 throws_type_index;
        } throws_target;

        struct CatchTarget {
            u2 exception_table_index;
        } catch_target;

        struct OffsetTarget {
            u2 offset;
        } offset_target;

        struct TypeArgumentTarget {
            u2 offset;
            u2 type_argument_index;
        } type_argument_target;
    } targetInfo;

    struct LocalVarTarget {
        u2 table_length;
        std::vector<LocalVarTargetEntry> items;
    } local_var_target;

    TargetPath targetPath;
    u2 typeIndex;
    u2 count;
    std::vector<AnnotationValuePair> values;

};


/**
 * ref: https://docs.oracle.com/javase/specs/jvms/se8/html/jvms-4.html#jvms-4.7.20
 *      https://docs.oracle.com/javase/specs/jvms/se15/preview/specs/records-jvms.html
 */
struct RuntimeTypeAnnotations : AttributeBase {
    u2 count;
    std::vector<TypeAnnotation> items;
};

struct RuntimeVisibleTypeAnnotations : RuntimeTypeAnnotations {
};

struct RuntimeInvisibleTypeAnnotations : RuntimeTypeAnnotations {
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
