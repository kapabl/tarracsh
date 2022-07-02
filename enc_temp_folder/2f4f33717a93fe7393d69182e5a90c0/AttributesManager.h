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

    struct VectorReader;

    wstring innerClassToString(InnerClasses &innerClasses, VectorReader &reader,
                               u4 index) const {

        const InnerClass innerClass{
            reader.readU2(),
            reader.readU2(),
            reader.readU2(),
            reader.readU2()};
        auto innerClassname = innerClass.innerClassInfoIndex != 0
                                  ? _constantPool.getClassInfoName(innerClass.innerClassInfoIndex)
                                  : L"";

        const auto outerClassname = innerClass.outerClassInfoIndex != 0
                                        ? _constantPool.getClassInfoName(innerClass.outerClassInfoIndex)
                                        : L"<none>";
        const auto name = _constantPool.getClassInfoName( innerClass.innerNameIndex );
        const auto accessModifier = _accessModifiers.toString( innerClass.innerClassAccessFlags );
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

    START_ATTR_TO_STRING(RuntimeVisibleAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleParameterAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleParameterAnnotations)
        // TODO
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
