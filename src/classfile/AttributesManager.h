#ifndef TARRASH_ATTRIBUTES_MANAGER_H
#define TARRASH_ATTRIBUTES_MANAGER_H


#include <string>
#include <cassert>
#include <unordered_map>
#include "includes/AttributeTags.h"
#include "AttributeStructures.h"
#include "DescriptorParser.h"
#include "AccessModifiers.h"


namespace org::kapa::tarrash::attributes {


#define ATTR_TO_STRING_FUNC_NAME(AttributeName) toString##AttributeName

#define TAG_TO_STRING_FUNC_ENTRY( AttributeName ) \
    _tags2ToStringFunc[AttributeTag::AttributeName##Tag] = &AttributesManager::ATTR_TO_STRING_FUNC_NAME( AttributeName );

#define START_ATTR_TO_STRING(AttributeName) \
    wstring ATTR_TO_STRING_FUNC_NAME(AttributeName)(AttributeInfo & attribute) { \
        wstring result = L"Attr - "#AttributeName; \
        VectorReader reader(attribute.info, _isBigEndian);
#define END_ATTR_TO_STRING() \
        return result;\
    }

class AttributesManager {
public:
    AttributesManager(const ConstantPool &constantPool, const bool isBigEndian)
        : _constantPool(constantPool), _isBigEndian(isBigEndian) {
        init();
    }

    AttributeTag getTag(const std::wstring &stringTag) const {
        const auto it = _tagsMap.find(stringTag);
        if (it != _tagsMap.end()) {
            return it->second;
        }
        return AttributeTag::InvalidTag;
    }

    wstring toString(AttributeInfo &attribute) {

        const auto tagName = _constantPool[attribute.nameIndex].utf8Info.getValue();

        const auto tag = getTag(tagName);
        const auto it = _tags2ToStringFunc.find(tag);
        wstring result = (it != _tags2ToStringFunc.end())
                             ? (this->*(it->second))(attribute)
                             : (L"Invalid Attribute: " + tagName);

        return result;
    }

private:
    typedef wstring (AttributesManager::*AttrToStringFunc)(AttributeInfo &);

    accessModifiers::AccessModifiers _accessModifiers;

    std::unordered_map<std::wstring, AttributeTag> _tagsMap;
    std::unordered_map<AttributeTag, AttrToStringFunc> _tags2ToStringFunc;
    const ConstantPool &_constantPool;
    bool _isBigEndian;

    struct VectorReader {
        explicit VectorReader(const vector<u1> &buffer, const bool isBigEndianArg)
            : bytesVector(buffer), isBigEndian(isBigEndianArg) {
        }

        const vector<u1> &bytesVector;
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
                        buffer = swapShort(buffer);
                        break;
                    case 4:
                        buffer = swapLong(buffer);
                        break;
                    default:
                        break;
                }
            }
        }

        template <typename T> void read(T &buffer) { read(buffer, sizeof(T)); }

        u1 readU1() {
            u1 result;
            read(result);
            return result;
        }

        u2 readU2() {
            u2 result;
            read(result);
            return result;
        }

        u4 readU4() {
            u4 result;
            read(result);
            return result;
        }
    };

    /**
         * TODO consider saving the constant string inside the attribute in the future
         */
    START_ATTR_TO_STRING(ConstantValue)
        // auto &constantValue = reinterpret_cast<ConstantValue &>(attribute);
        result += L": ";

        ConstantValue constantValue{attribute.nameIndex, attribute.length};
        constantValue.constantValueIndex = reader.readU2();

        auto &constantValueEntry = _constantPool[constantValue.constantValueIndex].base;

        switch (constantValueEntry.tag) {

            case JVM_CONSTANT_Integer:
                constantValue.value.intValue = static_cast<Integer_info &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.intValue);
                break;

            case JVM_CONSTANT_Float:
                constantValue.value.floatValue = static_cast<Float_info &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.floatValue);
                break;

            case JVM_CONSTANT_Long:
                constantValue.value.longValue = static_cast<Long_info &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.longValue);
                break;

            case JVM_CONSTANT_Double:
                constantValue.value.doubleValue = static_cast<Double_info &>(constantValueEntry).value;
                result += to_wstring(constantValue.value.doubleValue);
                break;

            case JVM_CONSTANT_String:
                auto &stringInfo = static_cast<String_info &>(constantValueEntry);
                constantValue.value.string = _constantPool.getString(stringInfo.stringIndex);
                result += to_wstring(constantValue.value.doubleValue);
                break;

            default:
                assert(false);
                break;
        }

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(SourceFile)
        SourceFile sourceFile{attribute.nameIndex, attribute.length};
        sourceFile.sourceFileIndex = reader.readU2();
        result += L": " + _constantPool[sourceFile.sourceFileIndex].utf8Info.getValue();
    END_ATTR_TO_STRING()

    wstring innerClassToString(InnerClasses &innerClasses, VectorReader &reader,
                               u4 index) const {

        const InnerClass innerClass{
            reader.readU2(),
            reader.readU2(),
            reader.readU2(),
            reader.readU2()};
        const auto innerClassname = innerClass.innerClassInfoIndex != 0
                                        ? _constantPool.getClassInfoName(innerClass.innerClassInfoIndex)
                                        : L"";

        const auto outerClassname = innerClass.outerClassInfoIndex != 0
                                        ? _constantPool.getClassInfoName(innerClass.outerClassInfoIndex)
                                        : L"<none>";
        const auto name = _constantPool.getClassInfoName(innerClass.innerNameIndex);
        const auto accessModifier = _accessModifiers.toString(innerClass.innerClassAccessFlags);
        wstring result =
            accessModifier +
            L" class: " + name +
            L", outer class: " + outerClassname +
            L", inner class: " + innerClassname;

        return result;
    }

    START_ATTR_TO_STRING(InnerClasses)
        InnerClasses innerClasses{attribute.nameIndex, attribute.length};
        innerClasses.numberOfClasses = reader.readU2();
        vector<wstring> parts;
        parts.reserve(innerClasses.numberOfClasses);
        for (auto i = 0u; i < innerClasses.numberOfClasses; ++i) {
            parts.push_back(innerClassToString(innerClasses, reader, i));
        }
        result += L"\n" + join<wstring>(parts, L"\n");
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


    void readAnnotationValue(ElementValue &elementValue, VectorReader &reader) {
        elementValue.tag = static_cast<SignatureTypes>(reader.readU1());
        wstring result;
        switch (elementValue.tag) {

            case JVM_SIGNATURE_ARRAY:
                elementValue.value.arrayValues.count = reader.readU2();
                for (auto i = 0u; i < elementValue.value.arrayValues.count; i++) {
                    std::shared_ptr<ElementValue> childElementValue(new ElementValue());
                    readAnnotationValue(*childElementValue, reader);
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

            default:
                assert(false);
                cout << "Invalid Annotation";
                break;
        }

    }
    
    void readAnnotationValues(Annotation &annotations, VectorReader &reader) {
        for (auto i = 0u; i < annotations.count; ++i) {
            AnnotationValuePair annotationValuePair;
            annotationValuePair.nameIndex = reader.readU2();

            // TODO convert to string
            auto valueTypeString = _constantPool.getTypeString(annotationValuePair.nameIndex);


            readAnnotationValue(annotationValuePair.value, reader);
            annotations.values.push_back(annotationValuePair);

        }
    }

    void readAnnotation(Annotation &annotation, VectorReader &reader) {
        annotation.typeIndex = reader.readU2();
        annotation.count = reader.readU2();
        readAnnotationValues(annotation, reader);
    }

    void readRuntimeAnnotations(RuntimeAnnotations &annotations, VectorReader &reader) {
        for (auto i = 0u; i < annotations.count; ++i) {
            Annotation annotation;
            readAnnotation(annotation, reader );
            auto typeString = _constantPool.getTypeString(annotation.typeIndex);
            annotations.annotations.push_back(annotation);
        }
    }


    void readParameterAnnotation(ParameterAnnotation &parameterAnnotation, VectorReader &reader) {
        parameterAnnotation.count = reader.readU2();
        for (auto i = 0u; i < parameterAnnotation.count; i++) {
            Annotation annotation;
            readAnnotation(annotation, reader);
            parameterAnnotation.annotations.push_back(annotation);
        }
     }


    void readRuntimeParameterAnnotations(RuntimeParameterAnnotations &annotations, VectorReader &reader) {
        for (auto i = 0u; i < annotations.parameterCount; ++i) {
            ParameterAnnotation annotation;
            readParameterAnnotation(annotation, reader);
            // auto typeString = _constantPool.getTypeString(annotation.typeIndex);
            // annotations.annotations.push_back(annotation);
            annotations.parameterAnnotations.push_back(annotation);
        }
    }

    START_ATTR_TO_STRING(RuntimeVisibleAnnotations)
        RuntimeVisibleAnnotations runtimeVisibleAnnotations{reader.readU2(), reader.readU4(), reader.readU2()};
        readRuntimeAnnotations(runtimeVisibleAnnotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleAnnotations)
        RuntimeInvisibleAnnotations runtimeVisibleAnnotations{reader.readU2(), reader.readU4(), reader.readU2()};
        readRuntimeAnnotations(runtimeVisibleAnnotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleParameterAnnotations)
        RuntimeVisibleParameterAnnotations annotations{reader.readU2(), reader.readU4(), reader.readU1()};
        readRuntimeParameterAnnotations(annotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleParameterAnnotations)
        RuntimeInvisibleParameterAnnotations annotations{reader.readU2(), reader.readU4(), reader.readU1()};
        readRuntimeParameterAnnotations(annotations, reader);
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(AnnotationDefault)
        // TODO

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

        TAG_TO_STRING_FUNC_ENTRY(SourceFile)
        TAG_TO_STRING_FUNC_ENTRY(InnerClasses)
        TAG_TO_STRING_FUNC_ENTRY(NestMembers)
        TAG_TO_STRING_FUNC_ENTRY(NestHost)
        TAG_TO_STRING_FUNC_ENTRY(ConstantValue)
        TAG_TO_STRING_FUNC_ENTRY(Code)
        TAG_TO_STRING_FUNC_ENTRY(Exceptions)
        TAG_TO_STRING_FUNC_ENTRY(LineNumberTable)
        TAG_TO_STRING_FUNC_ENTRY(LocalVariableTable)
        TAG_TO_STRING_FUNC_ENTRY(LocalVariableTypeTable)
        TAG_TO_STRING_FUNC_ENTRY(MethodParameters)
        TAG_TO_STRING_FUNC_ENTRY(StackMapTable)
        TAG_TO_STRING_FUNC_ENTRY(Synthetic)
        TAG_TO_STRING_FUNC_ENTRY(Deprecated)
        TAG_TO_STRING_FUNC_ENTRY(SourceDebugExtension)
        TAG_TO_STRING_FUNC_ENTRY(Signature)
        TAG_TO_STRING_FUNC_ENTRY(Record)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeVisibleAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeInvisibleAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeVisibleParameterAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeInvisibleParameterAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(AnnotationDefault)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeInvisibleTypeAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(RuntimeInvisibleTypeAnnotations)
        TAG_TO_STRING_FUNC_ENTRY(EnclosingMethod)
        TAG_TO_STRING_FUNC_ENTRY(BootstrapMethods)
        TAG_TO_STRING_FUNC_ENTRY(PermittedSubclasses)

    }
};

} // namespace org::kapa::tarrash::attributes


#undef ATTRIBUTE_TO_STRING

#endif
