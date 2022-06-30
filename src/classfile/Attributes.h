//
// Created by xman on 6/29/2022.
//

#ifndef TARRASH_STACKFRAME_H
#define TARRASH_STACKFRAME_H

#include "StructsCommon.h"
#include "StackFrame.h"
#include <vector>

using namespace org::kapa::tarrash::common;
using namespace org::kapa::tarrash::stack;

namespace org {
namespace kapa {
namespace tarrash {
namespace attributes {


/*

struct ConstantValue {
    u2 nameIndex;
    u4 length;
    u2 constantValueIndex;
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

struct StackMapTable {
    u2 nameIndex;
    u4 length;
    u2 entryCount;
    vector<StackMapFrame> entries;
}

struct Code {
    u2 nameIndex;
    u4 length;
    u2 maxStack;
    u2 maxLocals;
    u4 codeLength;
    vector<u1> code;
    u2 exceptionTableLength;
    vector<ExceptionTable> exceptionTable;
    u2 attributesCount;
    vector<Attribute> attributes;
};

struct Exceptions {
    u2 nameIndex;
    u4 length;
    u2 number_of_exceptions;
    vector<u2> exceptionIndexTable;
};

struct InnerClass {
    u2 innerClassInfoIndex;
    u2 outerClassInfoIndex;
    u2 innerNameIndex;
    u2 innerClassAccessFlags;
};

struct InnerClasses {
    u2 nameIndex;
    u4 length;
    u2 number_of_classes;
    vector<InnerClass> classes;
};

struct EnclosingMethod {
    u2 nameIndex;
    u4 length;
    u2 class_index;
    u2 method_index;
};

struct Synthetic {
    u2 nameIndex;
    u4 length;
};

struct Signature {
    u2 nameIndex;
    u4 length;
    u2 signature_index;
};

struct SourceFile {
    u2 nameIndex;
    u4 length;
    u2 sourcefile_index;
};

struct SourceDebugExtension {
    u2 nameIndex;
    u4 length;
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

struct LocalVariableTable {
    u2 attribute_name_index;
    u4 attribute_length;
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

struct LocalVariableTypeTable {
    u2 attribute_name_index;
    u4 attribute_length;
    vector<LocalVariableType> types;
};

struct Deprecated {
    u2 attribute_name_index;
    u4 attribute_length;
};

*/
/**
 *
 * Note: here you can see the values of tag hard-coded in openjdk
 * https://sourcegraph.com/github.com/openjdk/jdk/-/blob/src/hotspot/share/classfile/classFileParser.cpp?L1077
 *//*

struct ElementValue {

    u1 tag;
    union {
        u2 const_value_index;

        struct {
            u2 type_name_index;
            u2 const_name_index;
        } enum_const_value;

        u2 class_info_index;

        ElementValue *annotation_value;
        struct {
            u2 num_values;
            vector<ElementValue *> values;
        } array_value;
    } value;
};

struct AnnotationValuePair {
    u2 element_name_index;
    ElementValue value;
};

struct Annotation {
    u2 type_index;
    u2 num_element_value_pairs;
    vector<AnnotationValuePair> values;
};

struct RuntimeVisibleAnnotations {
    u2 attribute_name_index;
    u4 attribute_length;
    u2 num_annotations;
    vector<Annotation> annotations;
};
*/

} // namespace attributes
} // namespace tarrash
} // namespace kapa
} // namespace org
#endif // TARRASH_STACKFRAME_H
