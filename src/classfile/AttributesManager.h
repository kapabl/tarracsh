#ifndef TARRASH_ATTRIBUTES_MANAGER_H
#define TARRASH_ATTRIBUTES_MANAGER_H


#include <string>
#include <cassert>
#include <unordered_map>
#include "AttributeTags.h"
#include "AttributeStructures.h"
#include "AccessModifiers.h"
#include "VectorReader.h"
#include "AnnotationsParser.h"
#include "signatures/SignatureParser.h"


namespace org::kapa::tarracsh::attributes {


#define ATTR_TO_STRING_FUNC_NAME(AttributeName) toString##AttributeName

#define TAG_TO_FUNCS( AttributeName ) \
    _tags2ToStringFunc[AttributeTag::AttributeName##Tag] = &AttributesManager::ATTR_TO_STRING_FUNC_NAME( AttributeName );
    

#define START_ATTR_TO_STRING(AttributeName) \
    std::wstring ATTR_TO_STRING_FUNC_NAME(AttributeName)(AttributeInfo & attribute) { \
        std::wstring result = L"Attr - "#AttributeName; \
        readers::VectorReader reader(attribute.info, _isBigEndian);

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

    START_ATTR_TO_STRING(ConstantValue)
        result += L": ";

        ConstantValue constantValue{{attribute.nameIndex, attribute.length}, reader.readU2(), {}};

        auto &constantValueEntry = _constantPool[constantValue.constantValueIndex].base;

        switch (constantValueEntry.tag) {

            case JVM_CONSTANT_Integer: {
                constantValue.value.intValue = static_cast<IntegerInfo &>(constantValueEntry).value;
                result += std::to_wstring(constantValue.value.intValue);
                break;
            }

            case JVM_CONSTANT_Float: {

                constantValue.value.floatValue = static_cast<FloatInfo &>(constantValueEntry).getFloat();
                result += std::to_wstring(constantValue.value.floatValue);
                break;
            }

            case JVM_CONSTANT_Long: {
                auto longInfo = static_cast<LongInfo&>(constantValueEntry);
                constantValue.value.longValue = longInfo.getLongLong();
                result += std::to_wstring(constantValue.value.longValue);
                break;
            }

            case JVM_CONSTANT_Double: {
                constantValue.value.doubleValue = static_cast<DoubleInfo &>(constantValueEntry).getDouble();
                result += std::to_wstring(constantValue.value.doubleValue);
                break;
            }

            case JVM_CONSTANT_String: {
                auto &stringInfo = static_cast<StringInfo &>(constantValueEntry);
                constantValue.value.string = _constantPool.getString(stringInfo.stringIndex);
                result += std::to_wstring(constantValue.value.doubleValue);
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

    std::wstring innerClassToString(InnerClasses &innerClasses, readers::VectorReader &reader,
                                    u4 index) const {

        const InnerClass innerClass{
            reader.readU2(),
            reader.readU2(),
            reader.readU2(),
            reader.readU2()};

        const auto innerClassname = _constantPool.getClassInfoName(innerClass.innerClassInfoIndex);
        const auto outerClassname = _constantPool.getClassInfoName(innerClass.outerClassInfoIndex);
        const auto name = _constantPool.getClassname(innerClass.innerNameIndex);

        const auto accessModifier = _accessModifiers.toString(innerClass.innerClassAccessFlags);
        std::wstring result =
            accessModifier +
            L" class: " + name +
            L", outer class: " + outerClassname +
            L", inner class: " + innerClassname;

        return result;
    }

    START_ATTR_TO_STRING(InnerClasses)
        InnerClasses innerClasses;
        innerClasses.nameIndex = attribute.nameIndex;
        innerClasses.length = attribute.length;
        innerClasses.numberOfClasses = reader.readU2();

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
        Code codeAttribute;
        codeAttribute.nameIndex = attribute.nameIndex;
        codeAttribute.length = attribute.length;
        codeAttribute.maxStack = reader.readU2();
        codeAttribute.maxLocals = reader.readU2();
        codeAttribute.codeLength = reader.readU4();

        reader.jump(codeAttribute.codeLength);

        // for (auto i = 0u; i < codeAttribute.codeLength; i++) {
        //     codeAttribute.code.push_back(reader.readU1());
        // }

        codeAttribute.exceptionTableLength = reader.readU2();

        for (auto i = 0u; i < codeAttribute.exceptionTableLength; i++) {
            ExceptionTable exceptionTable{};
            exceptionTable.startPC = reader.readU2();
            exceptionTable.endPC = reader.readU2();
            exceptionTable.handlerPC = reader.readU2();
            exceptionTable.catchType = reader.readU2();
            codeAttribute.exceptionTable.push_back(exceptionTable);
        }
        result += L" " + std::format(L"maxStack={}, maxLocals={}, codeLength={}, exceptionTableLength={}",
                                     codeAttribute.maxStack, codeAttribute.maxLocals, codeAttribute.codeLength,
                                     codeAttribute.exceptionTableLength);

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Exceptions)
        Exceptions exceptions;
        exceptions.nameIndex = attribute.nameIndex;
        exceptions.length = attribute.length;
        auto count = reader.readU2();
        while (count > 0) {
            exceptions.exceptionIndexTable.push_back(reader.readU2());
            count--;
        }
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
        SourceDebugExtension sourceDebugExtension;
        sourceDebugExtension.nameIndex = attribute.nameIndex;
        sourceDebugExtension.length = attribute.length;
        std::wstring value;
        for (auto i = 0u; i < sourceDebugExtension.length; i++) {
            const auto character = static_cast<char>(reader.readU1());
            sourceDebugExtension.debugExtensions.push_back(character);
            value.push_back(character);
        }

        result += L" " + value;

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Signature)
        const signatures::SignatureParser signatureParser(_constantPool, attribute, reader);
        result += L" " + signatureParser.toString();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Record)
        // TODO
    END_ATTR_TO_STRING()


    START_ATTR_TO_STRING(RuntimeVisibleAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += L" " + annotationsParser.toStringRuntimeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += L" " + annotationsParser.toStringRuntimeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleParameterAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += L" " + annotationsParser.toStringRuntimeParameterAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleParameterAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += L" " + annotationsParser.toStringRuntimeParameterAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(AnnotationDefault)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += L" " + annotationsParser.toStringAnnotationDefault();

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleTypeAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleTypeAnnotations)
        // TODO
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(EnclosingMethod)
        EnclosingMethod enclosingMethod;
        enclosingMethod.nameIndex = attribute.nameIndex;
        enclosingMethod.length = attribute.length;
        enclosingMethod.classIndex = reader.readU2();
        enclosingMethod.methodIndex = reader.readU2();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(BootstrapMethods)
        BootstrapMethods bootstrapMethods;
        bootstrapMethods.nameIndex = attribute.nameIndex;
        bootstrapMethods.length = attribute.length;
        auto count = reader.readU2();
        while (count > 0) {
            BootstrapMethod bootstrapMethod;
            bootstrapMethod.bootstrapMethodRef = reader.readU2();
            auto argumentCount = reader.readU2();
            while (argumentCount > 0) {
                bootstrapMethod.bootstrapArguments.push_back(reader.readU2());
                argumentCount--;
            }
            count--;

        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(PermittedSubclasses)
        PermittedSubclasses permittedSubclasses;
        permittedSubclasses.nameIndex = attribute.nameIndex;
        permittedSubclasses.length = attribute.length;
        permittedSubclasses.numberOfClasses = reader.readU2();
        auto count = permittedSubclasses.numberOfClasses;
        while (count > 0) {
            permittedSubclasses.classes.push_back(reader.readU2());
            count--;
        }
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
        _tagsMap[RUNTIME_VISIBLE_TYPE_ANNOTATIONS] = AttributeTag::RuntimeVisibleTypeAnnotationsTag;
        _tagsMap[RUNTIME_INVISIBLE_TYPE_ANNOTATIONS] = AttributeTag::RuntimeInvisibleTypeAnnotationsTag;
        _tagsMap[ENCLOSING_METHOD] = AttributeTag::EnclosingMethodTag;
        _tagsMap[BOOTSTRAP_METHODS] = AttributeTag::BootstrapMethodsTag;
        _tagsMap[PERMITTED_SUBCLASSES] = AttributeTag::PermittedSubclassesTag;

        TAG_TO_FUNCS(SourceFile)
        TAG_TO_FUNCS(InnerClasses)
        TAG_TO_FUNCS(NestMembers)
        TAG_TO_FUNCS(NestHost)
        TAG_TO_FUNCS(ConstantValue)
        TAG_TO_FUNCS(Code)
        TAG_TO_FUNCS(Exceptions)
        TAG_TO_FUNCS(LineNumberTable)
        TAG_TO_FUNCS(LocalVariableTable)
        TAG_TO_FUNCS(LocalVariableTypeTable)
        TAG_TO_FUNCS(MethodParameters)
        TAG_TO_FUNCS(StackMapTable)
        TAG_TO_FUNCS(Synthetic)
        TAG_TO_FUNCS(Deprecated)
        TAG_TO_FUNCS(SourceDebugExtension)
        TAG_TO_FUNCS(Signature)
        TAG_TO_FUNCS(Record)
        TAG_TO_FUNCS(RuntimeVisibleAnnotations)
        TAG_TO_FUNCS(RuntimeInvisibleAnnotations)
        TAG_TO_FUNCS(RuntimeVisibleParameterAnnotations)
        TAG_TO_FUNCS(RuntimeInvisibleParameterAnnotations)
        TAG_TO_FUNCS(AnnotationDefault)
        TAG_TO_FUNCS(RuntimeVisibleTypeAnnotations)
        TAG_TO_FUNCS(RuntimeInvisibleTypeAnnotations)
        TAG_TO_FUNCS(EnclosingMethod)
        TAG_TO_FUNCS(BootstrapMethods)
        TAG_TO_FUNCS(PermittedSubclasses)

    }
};

}

#undef ATTR_TO_STRING_FUNC_NAME
#undef TAG_TO_FUNCS
#undef START_ATTR_TO_STRING
#undef END_ATTR_TO_STRING

#endif
