#ifndef ATTRIBUTE_TAGS_H
#define ATTRIBUTE_TAGS_H

/**
 * copied from
 * https://sourcegraph.com/github.com/kapabl/jdk/-/blob/src/hotspot/share/classfile/vmSymbols.hpp?L176
 * ~kapa
 *
 */


namespace kapa::tarracsh::domain::classfile::attribute {

#define SOURCE_FILE "SourceFile"
#define INNER_CLASSES "InnerClasses"
#define NEST_MEMBERS "NestMembers"
#define NEST_HOST "NestHost"
#define CONSTANT_VALUE "ConstantValue"
#define CODE "Code"
#define EXCEPTIONS "Exceptions"
#define LINE_NUMBER_TABLE "LineNumberTable"
#define LOCAL_VARIABLE_TABLE "LocalVariableTable"
#define LOCAL_VARIABLE_TYPE_TABLE "LocalVariableTypeTable"
#define METHOD_PARAMETERS "MethodParameters"
#define STACK_MAP_TABLE "StackMapTable"
#define SYNTHETIC "Synthetic"
#define DEPRECATED "Deprecated"
#define SOURCE_DEBUG_EXTENSION "SourceDebugExtension"
#define SIGNATURE "Signature"
#define RECORD "Record"
#define RUNTIME_VISIBLE_ANNOTATIONS "RuntimeVisibleAnnotations"
#define RUNTIME_INVISIBLE_ANNOTATIONS "RuntimeInvisibleAnnotations"
#define RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS "RuntimeVisibleParameterAnnotations"
#define RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS "RuntimeInvisibleParameterAnnotations"
#define ANNOTATION_DEFAULT "AnnotationDefault"
#define RUNTIME_VISIBLE_TYPE_ANNOTATIONS "RuntimeVisibleTypeAnnotations"
#define RUNTIME_INVISIBLE_TYPE_ANNOTATIONS "RuntimeInvisibleTypeAnnotations"
#define ENCLOSING_METHOD "EnclosingMethod"
#define BOOTSTRAP_METHODS "BootstrapMethods"
#define PERMITTED_SUBCLASSES "PermittedSubclasses"

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


}

#endif
