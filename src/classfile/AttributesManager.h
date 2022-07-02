#ifndef TARRASH_ATTRIBUTES_MANAGER_H
#define TARRASH_ATTRIBUTES_MANAGER_H


#include <string>
#include <cassert>
#include <unordered_map>
#include "includes/AttributeTags.h"
#include "AttributeStructures.h"
#include "AccessModifiers.h"


namespace org::kapa::tarrash::attributes {


#define ATTR_TO_STRING_FUNC_NAME(AttributeName) toString##AttributeName

#define TAG_TO_STRING_FUNC( AttributeName ) \
    _tags2ToStringFunc[AttributeTag::AttributeName##Tag] = &AttributesManager::ATTR_TO_STRING_FUNC_NAME( AttributeName );

#define START_ATTR_TO_STRING(AttributeName) \
    std::wstring ATTR_TO_STRING_FUNC_NAME(AttributeName)(AttributeInfo & attribute) { \
        std::wstring result = L"Attr - "#AttributeName; \
        VectorReader reader(attribute.info, _isBigEndian);
#define END_ATTR_TO_STRING() \
        return result;\
    }


using common::u1;
using common::u2;
using common::u4;

class AttributesManager final {
public:
    AttributesManager(const ConstantPool &constantPool)
        : _constantPool(constantPool), _isBigEndian(true) {
        init();
    }

    void setBigEndian(bool value) { _isBigEndian = value; }

    AttributeTag getTag(const std::wstring &stringTag) const {
        const auto it = _tagsMap.find(stringTag);
        if (it != _tagsMap.end()) {
            return it->second;
        }
        return AttributeTag::InvalidTag;
    }

    std::wstring toString(AttributeInfo &attribute) {

        const auto tagName = _constantPool[attribute.nameIndex].utf8Info.getValue();

        const auto tag = getTag(tagName);
        const auto it = _tags2ToStringFunc.find(tag);
        std::wstring result = (it != _tags2ToStringFunc.end())
                                  ? (this->*(it->second))(attribute)
                                  : (L"Invalid Attribute: " + tagName);

        return result;
    }

private:
    typedef std::wstring (AttributesManager::*AttrToStringFunc)(AttributeInfo &);

    accessModifiers::AccessModifiers _accessModifiers;

    std::unordered_map<std::wstring, AttributeTag> _tagsMap;
    std::unordered_map<AttributeTag, AttrToStringFunc> _tags2ToStringFunc;
    const ConstantPool &_constantPool;
    bool _isBigEndian;

    struct VectorReader {
        explicit VectorReader(const std::vector<u1> &buffer, const bool isBigEndianArg)
            : bytesVector(buffer), isBigEndian(isBigEndianArg) {
        }

        const std::vector<u1> &bytesVector;
        int position{0};
        bool isBigEndian;

        template <typename T> void readRaw(T &buffer, unsigned int byteCount) {

            assert(position + byteCount <= bytesVector.size());

            const auto charBuffer = reinterpret_cast<u1 *>(&buffer);

            for (auto i = 0u; i < byteCount; ++i) {
                charBuffer[i] = bytesVector[position];
                position++;
            }

        }

        template <typename T> void readRaw(T &buffer) { readRaw(buffer, sizeof(buffer)); }

        template <typename T> void read(T &buffer, unsigned int byteCount) {

            readRaw(buffer, byteCount);
            if (isBigEndian) {
                switch (byteCount) {
                    case 2:
                        buffer = stringUtils::swapShort(buffer);
                        break;
                    case 4:
                        buffer = stringUtils::swapLong(buffer);
                        break;
                    default:
                        break;
                }
            }
        }


        template <typename T> void read(T &buffer) { read(buffer, sizeof(T)); }
        template <typename T> void readReversed(T &buffer) { readReversed(buffer, sizeof(T)); }

        u1 readU1() {
            u1 result;
            readRaw(result, 1);
            return result;
        }

        u2 readU2() {
            u2 result;
            readRaw(result, 2);
            if (isBigEndian) {
                result = stringUtils::swapShort(result);
            }
            return result;
        }


        u4 readU4() {
            u4 result;
            readRaw(result, 4);
            if (isBigEndian) {
                result = stringUtils::swapLong(result);
            }
            return result;
        }


    };

    /**
         * TODO consider saving the constant string inside the attribute in the future
         */
    START_ATTR_TO_STRING(ConstantValue)
        result += L": ";

        ConstantValue constantValue{{attribute.nameIndex, attribute.length}, reader.readU2(), {}};

        auto &constantValueEntry = _constantPool[constantValue.constantValueIndex].base;

        switch (constantValueEntry.tag) {

            case JVM_CONSTANT_Integer: {
                constantValue.value.intValue = static_cast<IntegerInfo &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.intValue);
                break;
            }

            case JVM_CONSTANT_Float: {

                constantValue.value.floatValue = static_cast<FloatInfo &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.floatValue);
                break;
            }

            case JVM_CONSTANT_Long: {
                constantValue.value.longValue = static_cast<LongInfo &>(constantValueEntry).valueUnion.value;
                result += to_wstring(constantValue.value.longValue);
                break;
            }

            case JVM_CONSTANT_Double: {
                constantValue.value.doubleValue = static_cast<DoubleInfo &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.doubleValue);
                break;
            }

            case JVM_CONSTANT_String: {
                auto &stringInfo = static_cast<StringInfo &>(constantValueEntry);
                constantValue.value.string = _constantPool.getString(stringInfo.stringIndex);
                result += to_wstring(constantValue.value.doubleValue);
                break;
            }

            case JVM_CONSTANT_Utf8:
            case JVM_CONSTANT_Unicode:
            case JVM_CONSTANT_Class:
            case JVM_CONSTANT_Fieldref:
            case JVM_CONSTANT_Methodref:
            case JVM_CONSTANT_InterfaceMethodref:
            case JVM_CONSTANT_NameAndType:
            case JVM_CONSTANT_MethodHandle:
            case JVM_CONSTANT_MethodType:
            case JVM_CONSTANT_Dynamic:
            case JVM_CONSTANT_InvokeDynamic:
            case JVM_CONSTANT_Module:
            case JVM_CONSTANT_Package:
                assert(false);
                break;
        }

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(SourceFile)
        const SourceFile sourceFile{{attribute.nameIndex, attribute.length}, reader.readU2()};
        result += L": " + _constantPool[sourceFile.sourceFileIndex].utf8Info.getValue();
    END_ATTR_TO_STRING()

    std::wstring innerClassToString(InnerClasses &innerClasses, VectorReader &reader,
                                    u4 index) const {

        const InnerClass innerClass{
            reader.readU2(),
            reader.readU2(),
            reader.readU2(),
            reader.readU2()};

        const auto innerClassname = _constantPool.getClassInfoName(innerClass.innerClassInfoIndex);
        const auto outerClassname = _constantPool.getClassInfoName(innerClass.outerClassInfoIndex);
        const auto name = _constantPool.getClassInfoName(innerClass.innerNameIndex);

        const auto accessModifier = _accessModifiers.toString(innerClass.innerClassAccessFlags);
        std::wstring result =
            accessModifier +
            L" class: " + name +
            L", outer class: " + outerClassname +
            L", inner class: " + innerClassname;

        return result;
    }

    START_ATTR_TO_STRING(InnerClasses)
        InnerClasses innerClasses{{attribute.nameIndex, attribute.length}, reader.readU2(), {}};
        std::vector<std::wstring> parts;
        parts.reserve(innerClasses.numberOfClasses);
        for (auto i = 0u; i < innerClasses.numberOfClasses; ++i) {
            parts.push_back(innerClassToString(innerClasses, reader, i));
        }
        result += L"\n" + stringUtils::join<std::wstring>(parts, L"\n");
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(NestMembers)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(NestHost)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Code)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Exceptions)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LineNumberTable)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LocalVariableTable)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LocalVariableTypeTable)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(MethodParameters)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(StackMapTable)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Synthetic)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Deprecated)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(SourceDebugExtension)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Signature)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Record)
        // TODO
    END_ATTR_TO_STRING()

    std::wstring runtimeAnnotationsToString(const RuntimeAnnotations &annotations) const {

        std::vector<std::wstring> parts;
        for (auto &annotation : annotations.items) {
            parts.push_back(annotationToString(annotation));
        }
        auto result = stringUtils::join<std::wstring>(parts, L", ");
        return result;
    }


    void readElementValue(ElementValue &elementValue, VectorReader &reader) const {
        elementValue.tag = static_cast<SignatureTypes>(reader.readU1());
        std::wstring result;
        switch (elementValue.tag) {

            case JVM_SIGNATURE_ARRAY:
                elementValue.value.arrayValues.count = reader.readU2();
                for (auto i = 0u; i < elementValue.value.arrayValues.count; i++) {
                    std::shared_ptr<ElementValue> childElementValue(new ElementValue());
                    readElementValue(*childElementValue, reader);
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
                elementValue.value.constValueIndex = reader.readU2();
                break;

            case JVM_SIGNATURE_CLASS2:
                elementValue.value.classInfoIndex = reader.readU2();

                break;

            case JVM_SIGNATURE_ANNOTATION: {
                const std::shared_ptr<Annotation> childAnnotation(new Annotation());
                readAnnotation(*childAnnotation, reader);
                elementValue.value.annotationValue = childAnnotation;
                break;
            }

            case JVM_SIGNATURE_ENUM2:
                elementValue.value.enumConstValue.typeNameIndex = reader.readU2();
                elementValue.value.enumConstValue.constNameIndex = reader.readU2();
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

    void readAnnotationValues(Annotation &annotations, VectorReader &reader) const {
        for (auto i = 0u; i < annotations.count; ++i) {
            AnnotationValuePair annotationValuePair;
            annotationValuePair.nameIndex = reader.readU2();

            //auto valueTypeString = _constantPool.getTypeString(annotationValuePair.nameIndex);

            readElementValue(annotationValuePair.value, reader);
            annotations.values.push_back(annotationValuePair);

        }
    }

    void readAnnotation(Annotation &annotation, VectorReader &reader) const {
        annotation.typeIndex = reader.readU2();
        annotation.count = reader.readU2();
        readAnnotationValues(annotation, reader);
    }

    void readRuntimeAnnotations(RuntimeAnnotations &annotations, VectorReader &reader) const {
        for (auto i = 0u; i < annotations.count; ++i) {
            Annotation annotation;
            readAnnotation(annotation, reader);
            annotations.items.push_back(annotation);
        }
    }


    void readParameterAnnotation(ParameterAnnotation &parameterAnnotation, VectorReader &reader) const {
        parameterAnnotation.count = reader.readU2();
        for (auto i = 0u; i < parameterAnnotation.count; i++) {
            Annotation annotation;
            readAnnotation(annotation, reader);
            parameterAnnotation.items.push_back(annotation);
        }
    }


    void readRuntimeParameterAnnotations(RuntimeParameterAnnotations &annotations, VectorReader &reader) {
        for (auto i = 0u; i < annotations.parameterCount; ++i) {
            ParameterAnnotation annotation;
            readParameterAnnotation(annotation, reader);
            // auto typeString = _constantPool.getTypeString(annotation.typeIndex);
            // items.items.push_back(annotation);
            annotations.items.push_back(annotation);
        }
    }

    std::wstring elementValueToString(const ElementValue &elementValue) const {
        std::wstring result;
        const auto &[
            constValueIndex,
            enumConstValue,
            classInfoIndex,
            annotationValue,
            arrayValues] = elementValue.value;

        switch (elementValue.tag) {
            case JVM_SIGNATURE_ARRAY: {
                std::vector<std::wstring> parts;
                for (auto &child : arrayValues.values) {
                    parts.push_back(elementValueToString(*child));
                }
                result = stringUtils::join<std::wstring>(parts, L",");
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
                const auto name = _constantPool.getString(enumConstValue.typeNameIndex);
                const auto valueString = _constantPool.getConstantValueString(enumConstValue.constNameIndex);
                result = name + L"=" + valueString;
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

    std::wstring annotationValuePairToString(const AnnotationValuePair &annotationValuePair) const {
        const auto name = _constantPool.getString(annotationValuePair.nameIndex);
        const auto value = elementValueToString(annotationValuePair.value);
        std::wstring result = name + L"=(" + value + L")";
        return result;
    }

    std::wstring annotationToString(const Annotation &annotation) const {
        auto typeString = _constantPool.getTypeString(annotation.typeIndex);
        std::vector<std::wstring> valuePairs;

        for (auto &annotationValuePair : annotation.values) {
            valuePairs.push_back(annotationValuePairToString(annotationValuePair));
        }
        std::wstring result = typeString + L": " + stringUtils::join<std::wstring>(valuePairs, L", ");

        return result;
    }


    START_ATTR_TO_STRING(RuntimeVisibleAnnotations)
        RuntimeVisibleAnnotations annotations;
        annotations.nameIndex = attribute.nameIndex;
        annotations.length = attribute.length;
        annotations.count = reader.readU2();
        readRuntimeAnnotations(annotations, reader);
        result += L" " + runtimeAnnotationsToString(annotations);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleAnnotations)
        RuntimeInvisibleAnnotations annotations;
        annotations.nameIndex = attribute.nameIndex;
        annotations.length = attribute.length;
        annotations.count = reader.readU2();
        readRuntimeAnnotations(annotations, reader);
        result += L" " + runtimeAnnotationsToString(annotations);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleParameterAnnotations)
        RuntimeVisibleParameterAnnotations annotations;
        annotations.nameIndex = attribute.nameIndex;
        annotations.length = attribute.length;
        annotations.parameterCount = reader.readU1();
        readRuntimeParameterAnnotations(annotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleParameterAnnotations)
        RuntimeInvisibleParameterAnnotations annotations;
        annotations.nameIndex = attribute.nameIndex;
        annotations.length = attribute.length;
        annotations.parameterCount = reader.readU1();
        readRuntimeParameterAnnotations(annotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(AnnotationDefault)
        AnnotationDefault annotation;
        annotation.nameIndex = reader.readU2();
        annotation.length = reader.readU2();
        readElementValue(annotation.defaultValue, reader);
        //TODO convert to string

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleTypeAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleTypeAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(EnclosingMethod)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(BootstrapMethods)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(PermittedSubclasses)
        // TODO
    END_ATTR_TO_STRING()


    void init() {
        _tagsMap[SOURCE_FILE] = AttributeTag::SourceFileTag;
        _tagsMap[INNER_CLASSES] = AttributeTag::InnerClassesTag;
        _tagsMap[NEST_MEMBERS] = AttributeTag::NestMembersTag;
        _tagsMap[NEST_HOST] = AttributeTag::NestHostTag;
        _tagsMap[CONSTANT_VALUE] = AttributeTag::ConstantValueTag;
        _tagsMap[CODE] = AttributeTag::CodeTag;
        _tagsMap[EXCEPTIONS] = AttributeTag::ExceptionsTag;
        _tagsMap[LINE_NUMBER_TABLE] = AttributeTag::LineNumberTableTag;
        _tagsMap[LOCAL_VARIABLE_TABLE] = AttributeTag::LocalVariableTableTag;
        _tagsMap[LOCAL_VARIABLE_TYPE_TABLE] = AttributeTag::LocalVariableTypeTableTag;
        _tagsMap[METHOD_PARAMETERS] = AttributeTag::MethodParametersTag;
        _tagsMap[STACK_MAP_TABLE] = AttributeTag::StackMapTableTag;
        _tagsMap[SYNTHETIC] = AttributeTag::SyntheticTag;
        _tagsMap[DEPRECATED] = AttributeTag::DeprecatedTag;
        _tagsMap[SOURCE_DEBUG_EXTENSION] = AttributeTag::SourceDebugExtensionTag;
        _tagsMap[SIGNATURE] = AttributeTag::SignatureTag;
        _tagsMap[RECORD] = AttributeTag::RecordTag;
        _tagsMap[RUNTIME_VISIBLE_ANNOTATIONS] = AttributeTag::RuntimeVisibleAnnotationsTag;
        _tagsMap[RUNTIME_INVISIBLE_ANNOTATIONS] = AttributeTag::RuntimeInvisibleAnnotationsTag;
        _tagsMap[RUNTIME_VISIBLE_PARAMETER_ANNOTATIONS] = AttributeTag::RuntimeVisibleParameterAnnotationsTag;
        _tagsMap[RUNTIME_INVISIBLE_PARAMETER_ANNOTATIONS] = AttributeTag::RuntimeInvisibleParameterAnnotationsTag;
        _tagsMap[ANNOTATION_DEFAULT] = AttributeTag::AnnotationDefaultTag;
        _tagsMap[RUNTIME_VISIBLE_TYPE_ANNOTATIONS] = AttributeTag::RuntimeInvisibleTypeAnnotationsTag;
        _tagsMap[RUNTIME_INVISIBLE_TYPE_ANNOTATIONS] = AttributeTag::RuntimeInvisibleTypeAnnotationsTag;
        _tagsMap[ENCLOSING_METHOD] = AttributeTag::EnclosingMethodTag;
        _tagsMap[BOOTSTRAP_METHODS] = AttributeTag::BootstrapMethodsTag;
        _tagsMap[PERMITTED_SUBCLASSES] = AttributeTag::PermittedSubclassesTag;

        TAG_TO_STRING_FUNC(SourceFile)
        TAG_TO_STRING_FUNC(InnerClasses)
        TAG_TO_STRING_FUNC(NestMembers)
        TAG_TO_STRING_FUNC(NestHost)
        TAG_TO_STRING_FUNC(ConstantValue)
        TAG_TO_STRING_FUNC(Code)
        TAG_TO_STRING_FUNC(Exceptions)
        TAG_TO_STRING_FUNC(LineNumberTable)
        TAG_TO_STRING_FUNC(LocalVariableTable)
        TAG_TO_STRING_FUNC(LocalVariableTypeTable)
        TAG_TO_STRING_FUNC(MethodParameters)
        TAG_TO_STRING_FUNC(StackMapTable)
        TAG_TO_STRING_FUNC(Synthetic)
        TAG_TO_STRING_FUNC(Deprecated)
        TAG_TO_STRING_FUNC(SourceDebugExtension)
        TAG_TO_STRING_FUNC(Signature)
        TAG_TO_STRING_FUNC(Record)
        TAG_TO_STRING_FUNC(RuntimeVisibleAnnotations)
        TAG_TO_STRING_FUNC(RuntimeInvisibleAnnotations)
        TAG_TO_STRING_FUNC(RuntimeVisibleParameterAnnotations)
        TAG_TO_STRING_FUNC(RuntimeInvisibleParameterAnnotations)
        TAG_TO_STRING_FUNC(AnnotationDefault)
        TAG_TO_STRING_FUNC(RuntimeInvisibleTypeAnnotations)
        TAG_TO_STRING_FUNC(RuntimeInvisibleTypeAnnotations)
        TAG_TO_STRING_FUNC(EnclosingMethod)
        TAG_TO_STRING_FUNC(BootstrapMethods)
        TAG_TO_STRING_FUNC(PermittedSubclasses)

    }
};

}

#undef ATTR_TO_STRING_FUNC_NAME
#undef TAG_TO_STRING_FUNC
#undef START_ATTR_TO_STRING
#undef END_ATTR_TO_STRING

#endif
