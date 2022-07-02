//
// Created by xman on 6/29/2022.
//

#ifndef ATTRIBUTE_STRUCTURES_H
#define ATTRIBUTE_STRUCTURES_H

#include "StructsCommon.h"
#include "StackFrame.h"
#include <vector>

using namespace org::kapa::tarrash::common;
using namespace org::kapa::tarrash::stack;

namespace org {
namespace kapa {
namespace tarrash {
namespace attributes {



struct AttributeBase {
    u2 nameIndex;
    u4 length;
};


struct ConstantValue: AttributeBase {
    u2 constantValueIndex;

    struct {
        union {
            long long longValue;
            float floatValue;
            double doubleValue;
            int intValue;
            bool boolValue;
            char charValue;
            short shortValue;
            byte byteValue;
            wstring string;
        };
    } value;

    ~ConstantValue() {

    }
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
    vector<StackMapFrame> entries;
};

struct Code : AttributeBase {
    u2 maxStack;
    u2 maxLocals;
    u4 codeLength;
    vector<u1> code;
    u2 exceptionTableLength;
    vector<ExceptionTable> exceptionTable;
    u2 attributesCount;
    vector<Attribute> attributes;
};

struct Exceptions : AttributeBase {
    u2 number_of_exceptions;
    vector<u2> exceptionIndexTable;
};

struct InnerClass {
    u2 innerClassInfoIndex;
    u2 outerClassInfoIndex;
    u2 innerNameIndex;
    u2 innerClassAccessFlags;
};

struct InnerClasses : AttributeBase {
    u2 numberOfClasses;
    vector<InnerClass> classes;
};

struct EnclosingMethod : AttributeBase {

    u2 class_index;
    u2 method_index;
};

struct Synthetic : AttributeBase {
};

struct Signature : AttributeBase {
    u2 signature_index;
};

struct SourceFile : AttributeBase {;
    u2 sourceFileIndex;
};

struct SourceDebugExtension : AttributeBase {
    vector<u1> debugExtensions;
};

struct LineNumber {
    u2 start_pc;
    u2 line_number;
};

struct LineNumberTable {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 line_number_table_length;
    vector<LineNumber> lineNumbers;
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
    vector<LocalVariable> variables;
};

struct LocalVariableType {
    u2 start_pc;
    u2 length;
    u2 name_index;
    u2 signature_index;
    u2 index;
};

struct LocalVariableTypeTable : AttributeBase {
    vector<LocalVariableType> types;
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

    ElementValue()
        : value() {
    }

    ~ElementValue() {
    }

    SignatureTypes tag;

    union Value {

        //TODO move and copy contructors
        Value() {
        }

        ~Value() {
        }

        u2 constValueIndex;

        struct {
            u2 typeNameIndex;
            u2 constNameIndex;
        } enumConstValue;

        u2 classInfoIndex;

        std::shared_ptr<Annotation> annotationValue;

        struct {
            u2 count;
            vector<std::shared_ptr<ElementValue>> values;
        } arrayValues;
    } value;
};

struct AnnotationValuePair {
    u2 nameIndex;
    ElementValue value;

    AnnotationValuePair() {
        
    }

    ~AnnotationValuePair() {

    }

};

struct Annotation {
    u2 typeIndex;
    u2 count;
    vector<AnnotationValuePair> values;
};

struct RuntimeAnnotations : AttributeBase {
    u2 count;
    vector<Annotation> annotations;
};

struct RuntimeVisibleAnnotations : RuntimeAnnotations {};

struct RuntimeInvisibleAnnotations : RuntimeAnnotations {};


struct ParameterAnnotation {
    u2 count;
    vector<Annotation> annotations;
};

struct RuntimeParameterAnnotations : AttributeBase {
    u1 parameterCount;
    vector<ParameterAnnotation> parameterAnnotations;

};

struct RuntimeVisibleParameterAnnotations : RuntimeParameterAnnotations {};

struct RuntimeInvisibleParameterAnnotations : RuntimeParameterAnnotations {};

struct AnnotationDefault : AttributeBase {
    ElementValue defaultValue;
};

} // namespace attributes
} // namespace tarrash
} // namespace kapa
} // namespace org
#endif // ATTRIBUTE_STRUCTURES_H
