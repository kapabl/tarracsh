#ifndef ATTRIBUTE_TAGS_H
#define ATTRIBUTE_TAGS_H

/**
 * copied from
 * https://sourcegraph.com/github.com/kapabl/jdk/-/blob/src/hotspot/share/classfile/vmSymbols.hpp?L176
 * ~kapa
 *
 */


namespace org::kapa::tarrash::attributes {

#define SOURCE_FILE L"SourceFile"
#define INNER_CLASSES L"InnerClasses"
#define NEST_MEMBERS L"NestMembers"
#define NEST_HOST L"NestHost"
#define CONSTANT_VALUE L"ConstantValue"
#define CODE L"Code"
#define EXCEPTIONS L"Exceptions"
#define LINE_NUMBER_TABLE L"LineNumberTable"
#define LOCAL_VARIABLE_TABLE L"LocalVariableTable"
#define LOCAL_VARIABLE_TYPE_TABLE L"LocalVariableTypeTable"
#define METHOD_PARAMETERS L"MethodParameters"
#define STACK_MAP_TABLE L"StackMapTable"
#define SYNTHETIC L"Synthetic"
#define DEPRECATED L"Deprecated"
#define SOURCE_DEBUG_EXTENSION L"SourceDebugExtension"
#define SIGNATURE L"Signature"
#define RECORD L"Record"
#define RUNTIME_VISIBLE_ANNOTATIONS L"RuntimeVisibleAnnotations"
#define RUNTIME_INVISIBLE_ANNOTATIONS L"RuntimeInvisibleAnnotations"
#define RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS L"RuntimeVisibleParameterAnnotations"
#define RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS L"RuntimeInvisibleParameterAnnotations"
#define ANNOTATION_DEFAULT L"AnnotationDefault"
#define RUNTIME_VISIBLE_TYPE_ANNOTATIONS L"RuntimeVisibleTypeAnnotations"
#define RUNTIME_INVISIBLE_TYPE_ANNOTATIONS L"RuntimeInvisibleTypeAnnotations"
#define ENCLOSING_METHOD L"EnclosingMethod"
#define BOOTSTRAP_METHODS L"BootstrapMethods"
#define PERMITTED_SUBCLASSES L"PermittedSubclasses"

enum AttributeTag {
    InvalidTag,
    SourceFileTag,
    InnerClassesTag,
    NestMembersTag,
    NestHostTag,
    ConstantValueTag,
    CodeTag,
    ExceptionsTag,
    LineNumberTableTag,
    LocalVariableTableTag,
    LocalVariableTypeTableTag,
    MethodParametersTag,
    StackMapTableTag,
    SyntheticTag,
    DeprecatedTag,
    SourceDebugExtensionTag,
    SignatureTag,
    RecordTag,
    RuntimeVisibleAnnotationsTag,
    RuntimeInvisibleAnnotationsTag,
    RuntimeVisibleParameterAnnotationsTag,
    RuntimeInvisibleParameterAnnotationsTag,
    AnnotationDefaultTag,
    RuntimeVisibleTypeAnnotationsTag,
    RuntimeInvisibleTypeAnnotationsTag,
    EnclosingMethodTag,
    BootstrapMethodsTag,
    PermittedSubclassesTag
};


} // namespace org::kapa::tarrash::attributes

#endif
