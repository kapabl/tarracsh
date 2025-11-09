#ifndef TARRACSH_ATTRIBUTES_MANAGER_H
#define TARRACSH_ATTRIBUTES_MANAGER_H


#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <array>
#include <fmt/format.h>
#include "AttributeTags.h"
#include "AttributeStructures.h"
#include "AccessModifiers.h"
#include "reader/VectorReader.h"
#include "AnnotationsParser.h"
#include "signature/SignatureParser.h"
#include "MethodDescriptorParser.h"
#include "infrastructure/string/StringUtils.h"


namespace kapa::tarracsh::domain::classfile::attribute {


#define ATTR_TO_STRING_FUNC_NAME(AttributeName) toString##AttributeName

#define TAG_TO_FUNCS( AttributeName ) \
    _tags2ToStringFunc[AttributeTag::AttributeName##Tag] = &AttributesManager::ATTR_TO_STRING_FUNC_NAME( AttributeName );


#define START_ATTR_TO_STRING(AttributeName) \
    std::string ATTR_TO_STRING_FUNC_NAME(AttributeName)(AttributeInfo & attribute) { \
        std::string result = "Attr - "#AttributeName; \
        reader::VectorReader reader(attribute.info, _isBigEndian);

#define END_ATTR_TO_STRING() \
        return result;\
    }


using constantpool::u1;
using constantpool::u2;
using constantpool::u4;

class AttributesManager final {
public:
    AttributesManager(const constantpool::ConstantPool &constantPool)
        : _constantPool(constantPool), _isBigEndian(true) {
        init();
    }

    void setBigEndian(bool value) { _isBigEndian = value; }

    [[nodiscard]] auto getType(const AttributeInfo& attribute) const {

        const auto tagName = _constantPool.getEntry(attribute.nameIndex).utf8Info.getAsUtf8();
        const auto result = getTag(tagName);
      
        return result;
    }


    [[nodiscard]] std::string toString(const AttributeInfo &attribute) const {

        const auto type = getType(attribute);

        const auto it = _tags2ToStringFunc.find(type);

        auto _this = const_cast<AttributesManager *>(this);
        std::string result = (it != _tags2ToStringFunc.end())
                                  ? (_this->*(it->second))(const_cast<AttributeInfo &>(attribute))
                                  : (fmt::format("Invalid Attribute:{} ", _constantPool.getEntry(attribute.nameIndex).utf8Info.getAsUtf8()));

        return result;
    }

private:
    typedef std::string (AttributesManager::*AttrToStringFunc)(AttributeInfo &);

    [[nodiscard]] AttributeTag getTag(const std::string& stringTag) const {
        const auto it = _tagsMap.find(stringTag);
        if (it != _tagsMap.end()) {
            return it->second;
        }
        return AttributeTag::InvalidTag;
    }

    std::unordered_map<std::string, AttributeTag> _tagsMap;
    std::unordered_map<AttributeTag, AttrToStringFunc> _tags2ToStringFunc;
    const constantpool::ConstantPool &_constantPool;
    bool _isBigEndian;

    [[nodiscard]] std::string formatClassName(u2 classIndex) const {
        if (classIndex == 0) {
            return "<invalid>";
        }
        auto name = _constantPool.getClassInfoName(classIndex);
        if (name.empty()) {
            name = fmt::format("<class:{}>", classIndex);
        }
        return name;
    }

    [[nodiscard]] std::string methodDescriptorToString(u2 descriptorIndex) const {
        const auto descriptorString = _constantPool.getEntry(descriptorIndex).utf8Info.getAsUtf8();
        MethodDescriptorParser parser(descriptorString);
        const auto &descriptor = parser.getDescriptor();
        return fmt::format("{} -> {}", descriptor.argumentsToString(), descriptor.returnType.toString());
    }

    [[nodiscard]] std::string formatNameAndType(u2 nameAndTypeIndex) const {
        if (nameAndTypeIndex == 0) {
            return "<none>";
        }
        const auto &nameAndType = _constantPool.getEntry(nameAndTypeIndex).nameAndTypeInfo;
        const auto name = _constantPool.getString(nameAndType.nameIndex);
        const auto descriptorString = _constantPool.getString(nameAndType.descriptorIndex);
        if (!descriptorString.empty() && descriptorString.front() == JVM_SIGNATURE_FUNC) {
            MethodDescriptorParser parser(descriptorString);
            const auto &descriptor = parser.getDescriptor();
            return fmt::format("{}{} -> {}", name, descriptor.argumentsToString(), descriptor.returnType.toString());
        }
        return fmt::format("{}:{}", name, _constantPool.getTypeString(nameAndType.descriptorIndex));
    }

    [[nodiscard]] std::string formatMemberReference(u2 memberIndex) const {
        if (memberIndex == 0) {
            return "<none>";
        }
        const auto &record = _constantPool.getEntry(memberIndex);
        const auto tag = record.base.tag;
        if (tag == JVM_CONSTANT_Fieldref) {
            const auto owner = formatClassName(record.fieldrefInfo.classIndex);
            const auto member = formatNameAndType(record.fieldrefInfo.nameAndTypeIndex);
            return fmt::format("{}::{}", owner, member);
        }
        if (tag == JVM_CONSTANT_Methodref || tag == JVM_CONSTANT_InterfaceMethodref) {
            const auto owner = formatClassName(record.methodrefInfo.classIndex);
            const auto member = formatNameAndType(record.methodrefInfo.nameAndTypeIndex);
            return fmt::format("{}::{}", owner, member);
        }
        return fmt::format("#{}(tag={})", memberIndex, static_cast<int>(tag));
    }

    [[nodiscard]] static std::string methodHandleKindToString(u1 kind) {
        static const std::array<const char *, 10> kinds{
            "unknown",
            "getField",
            "getStatic",
            "putField",
            "putStatic",
            "invokeVirtual",
            "invokeStatic",
            "invokeSpecial",
            "newInvokeSpecial",
            "invokeInterface"
        };
        if (kind < kinds.size()) {
            return kinds[kind];
        }
        return "unknown";
    }

    [[nodiscard]] std::string methodHandleToString(u2 methodHandleIndex) const {
        if (methodHandleIndex == 0) {
            return "<none>";
        }
        const auto &record = _constantPool.getEntry(methodHandleIndex);
        if (record.base.tag != JVM_CONSTANT_MethodHandle) {
            return fmt::format("#{}(tag={})", methodHandleIndex, static_cast<int>(record.base.tag));
        }
        const auto kind = methodHandleKindToString(static_cast<u1>(record.methodHandleInfo.referenceKind));
        const auto target = formatMemberReference(record.methodHandleInfo.referenceIndex);
        return fmt::format("{} {}", kind, target);
    }

    [[nodiscard]] std::string constantPoolIndexToString(u2 index) const {
        if (index == 0) {
            return "<invalid>";
        }
        const auto &record = _constantPool.getEntry(index);
        switch (record.base.tag) {
            case JVM_CONSTANT_Integer:
            case JVM_CONSTANT_Float:
            case JVM_CONSTANT_Long:
            case JVM_CONSTANT_Double:
                return _constantPool.getConstantValueString(index);
            case JVM_CONSTANT_String:
                return fmt::format("\"{}\"", _constantPool.getString(record.stringInfo.stringIndex, true));
            case JVM_CONSTANT_Class:
                return formatClassName(record.classInfo.nameIndex);
            case JVM_CONSTANT_Utf8:
                return record.utf8Info.getAsUtf8(true);
            case JVM_CONSTANT_NameAndType:
                return formatNameAndType(index);
            case JVM_CONSTANT_Fieldref:
            case JVM_CONSTANT_Methodref:
            case JVM_CONSTANT_InterfaceMethodref:
                return formatMemberReference(index);
            case JVM_CONSTANT_MethodHandle:
                return methodHandleToString(index);
            case JVM_CONSTANT_MethodType:
                return methodDescriptorToString(record.methodTypeInfo.descriptorIndex);
            default:
                return fmt::format("#{}(tag={})", index, static_cast<int>(record.base.tag));
        }
    }

    START_ATTR_TO_STRING(ConstantValue)
        result += ": ";

        ConstantValue constantValue{{attribute.nameIndex, attribute.length}, reader.readU2(), {}};

        auto &constantValueEntry = _constantPool.getEntry(constantValue.constantValueIndex).base;

        switch (constantValueEntry.tag) {

            case JVM_CONSTANT_Integer: {
                constantValue.value.intValue = static_cast<constantpool::IntegerInfo &>(constantValueEntry).value;
                result += std::to_string(constantValue.value.intValue);
                break;
            }

            case JVM_CONSTANT_Float: {

                constantValue.value.floatValue = static_cast<constantpool::FloatInfo &>(constantValueEntry).getFloat();
                result += std::to_string(constantValue.value.floatValue);
                break;
            }

            case JVM_CONSTANT_Long: {
                auto longInfo = static_cast<constantpool::LongInfo &>(constantValueEntry);
                constantValue.value.longValue = longInfo.getLongLong();
                result += std::to_string(constantValue.value.longValue);
                break;
            }

            case JVM_CONSTANT_Double: {
                constantValue.value.doubleValue = static_cast<constantpool::DoubleInfo &>(constantValueEntry).getDouble();
                result += std::to_string(constantValue.value.doubleValue);
                break;
            }

            case JVM_CONSTANT_String: {
                auto &stringInfo = static_cast<constantpool::StringInfo &>(constantValueEntry);
                constantValue.value.string = _constantPool.getString(stringInfo.stringIndex);
                result += constantValue.value.string;
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
        result += ": " + _constantPool.getEntry(sourceFile.sourceFileIndex).utf8Info.getAsUtf8();
    END_ATTR_TO_STRING()

    std::string innerClassToString(InnerClasses &innerClasses, reader::VectorReader &reader,
                                    u4 index) const {

        const InnerClass innerClass{
            reader.readU2(),
            reader.readU2(),
            reader.readU2(),
            reader.readU2()};

        const auto innerClassname = _constantPool.getClassInfoName(innerClass.innerClassInfoIndex);
        const auto outerClassname = _constantPool.getClassInfoName(innerClass.outerClassInfoIndex);

        // const auto name = _constantPool.getClassname(innerClass.innerNameIndex);
        const auto name = _constantPool.getClassname(innerClass.innerNameIndex);

        const auto accessModifier = accessmodifier::toString(innerClass.innerClassAccessFlags);
        const std::string classDecl =
            " class: " + name +
            ", outer class: " + outerClassname +
            ", inner class: " + innerClassname;

        std::string result = accessModifier + classDecl;

        return result;
    }

    START_ATTR_TO_STRING(InnerClasses)
        InnerClasses innerClasses;
        innerClasses.nameIndex = attribute.nameIndex;
        innerClasses.length = attribute.length;
        innerClasses.numberOfClasses = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(innerClasses.numberOfClasses);
        for (auto i = 0u; i < innerClasses.numberOfClasses; ++i) {
            parts.push_back(innerClassToString(innerClasses, reader, i));
        }
        result += "\n" + kapa::infrastructure::string::stringUtils::join<std::string>(parts, "\n");
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(NestMembers)
        const auto numberOfClasses = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(numberOfClasses);

        for (auto i = 0u; i < numberOfClasses; ++i) {
            const auto classIndex = reader.readU2();
            parts.push_back(_constantPool.getClassInfoName(classIndex));
        }

        if (parts.empty()) {
            result += " (empty)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" count={}, {}", numberOfClasses, list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(NestHost)
        const auto hostClassIndex = reader.readU2();
        const auto hostName = _constantPool.getClassInfoName(hostClassIndex);
        result += fmt::format(" host={}", hostName);
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
        result += " " + fmt::format("maxStack={}, maxLocals={}, codeLength={}, exceptionTableLength={}",
                                     codeAttribute.maxStack, codeAttribute.maxLocals, codeAttribute.codeLength,
                                     codeAttribute.exceptionTableLength);

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Exceptions)
        Exceptions exceptions;
        exceptions.nameIndex = attribute.nameIndex;
        exceptions.length = attribute.length;
        auto count = reader.readU2();
        std::vector<std::string> names;
        names.reserve(count);
        while (count > 0) {
            const auto exceptionIndex = reader.readU2();
            exceptions.exceptionIndexTable.push_back(exceptionIndex);
            names.push_back(formatClassName(exceptionIndex));
            count--;
        }
        if (names.empty()) {
            result += " (none)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(names, std::string(", "));
            result += fmt::format(" count={}, {}", names.size(), list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LineNumberTable)
        LineNumberTable lineNumberTable{};
        lineNumberTable.attribute_name_index = attribute.nameIndex;
        lineNumberTable.attribute_length = attribute.length;
        lineNumberTable.line_number_table_length = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(lineNumberTable.line_number_table_length);

        for (auto i = 0u; i < lineNumberTable.line_number_table_length; ++i) {
            LineNumber lineNumber{};
            lineNumber.start_pc = reader.readU2();
            lineNumber.line_number = reader.readU2();
            lineNumberTable.lineNumbers.push_back(lineNumber);
            parts.push_back(fmt::format("{{pc:{}, line:{}}}", lineNumber.start_pc, lineNumber.line_number));
        }

        if (parts.empty()) {
            result += " (empty)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" count={}, {}", lineNumberTable.line_number_table_length, list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LocalVariableTable)
        LocalVariableTable table{};
        table.nameIndex = attribute.nameIndex;
        table.length = attribute.length;
        table.local_variable_table_length = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(table.local_variable_table_length);

        for (auto i = 0u; i < table.local_variable_table_length; ++i) {
            LocalVariable localVariable{};
            localVariable.start_pc = reader.readU2();
            localVariable.length = reader.readU2();
            localVariable.name_index = reader.readU2();
            localVariable.descriptor_index = reader.readU2();
            localVariable.index = reader.readU2();
            table.variables.push_back(localVariable);

            const auto name = _constantPool.getString(localVariable.name_index);
            const auto type = _constantPool.getTypeString(localVariable.descriptor_index);
            parts.push_back(
                fmt::format("{{slot:{}, range:{}-{}, name:{}, type:{}}}",
                            localVariable.index,
                            localVariable.start_pc,
                            localVariable.start_pc + localVariable.length,
                            name,
                            type));
        }

        if (parts.empty()) {
            result += " (empty)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" count={}, {}", table.local_variable_table_length, list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(LocalVariableTypeTable)
        LocalVariableTypeTable table{};
        const auto count = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(count);

        for (auto i = 0u; i < count; ++i) {
            LocalVariableType localVariableType{};
            localVariableType.start_pc = reader.readU2();
            localVariableType.length = reader.readU2();
            localVariableType.name_index = reader.readU2();
            localVariableType.signature_index = reader.readU2();
            localVariableType.index = reader.readU2();
            table.types.push_back(localVariableType);

            const auto name = _constantPool.getString(localVariableType.name_index);
            const auto signature = _constantPool.getString(localVariableType.signature_index);
            parts.push_back(fmt::format("{{slot:{}, name:{}, signature:{}, range:{}-{}}}",
                                        localVariableType.index,
                                        name,
                                        signature,
                                        localVariableType.start_pc,
                                        localVariableType.start_pc + localVariableType.length));
        }

        if (parts.empty()) {
            result += " (empty)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" count={}, {}", count, list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(MethodParameters)
        const auto parameterCount = reader.readU1();
        std::vector<std::string> parts;
        parts.reserve(parameterCount);

        const auto formatAccessFlags = [](const u2 accessFlags) {
            std::vector<std::string> flags;
            if ((accessFlags & JVM_ACC_FINAL) != 0) {
                flags.emplace_back("final");
            }
            if ((accessFlags & JVM_ACC_SYNTHETIC) != 0) {
                flags.emplace_back("synthetic");
            }
            if (flags.empty()) {
                flags.emplace_back("default");
            }
            return kapa::infrastructure::string::stringUtils::join(flags, std::string(" "));
        };

        for (auto parameterIndex = 0u; parameterIndex < parameterCount; ++parameterIndex) {
            const auto nameIndex = reader.readU2();
            const auto accessFlags = reader.readU2();
            const auto name = nameIndex == 0 ? std::string("<unnamed>") : _constantPool.getString(nameIndex);
            const auto access = formatAccessFlags(accessFlags);
            parts.push_back(fmt::format("{{index:{}, name:{}, access:{}}}", parameterIndex, name, access));
        }

        if (parts.empty()) {
            result += " (no parameters)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" count={}, {}", parameterCount, list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(StackMapTable)
        const auto entryCount = reader.readU2();

        struct StackMapCounters {
            uint32_t same{};
            uint32_t sameLocals{};
            uint32_t sameLocalsExtended{};
            uint32_t chop{};
            uint32_t sameExtended{};
            uint32_t append{};
            uint32_t full{};
            uint32_t unknown{};
        } counters;

        const auto skipVerificationType = [&reader]() {
            const auto tag = reader.readU1();
            if (tag == JVM_ITEM_Object || tag == JVM_ITEM_Uninitialized) {
                reader.readU2();
            }
        };

        const auto skipVerificationTypes = [&skipVerificationType](u2 count) {
            for (auto i = 0u; i < count; ++i) {
                skipVerificationType();
            }
        };

        for (auto i = 0u; i < entryCount; ++i) {
            const auto frameType = reader.readU1();
            if (frameType <= 63) {
                ++counters.same;
            } else if (frameType <= 127) {
                ++counters.sameLocals;
                skipVerificationType();
            } else if (frameType == 247) {
                ++counters.sameLocalsExtended;
                reader.readU2();
                skipVerificationType();
            } else if (frameType >= 248 && frameType <= 250) {
                ++counters.chop;
                reader.readU2();
            } else if (frameType == 251) {
                ++counters.sameExtended;
                reader.readU2();
            } else if (frameType >= 252 && frameType <= 254) {
                ++counters.append;
                reader.readU2();
                skipVerificationTypes(static_cast<u2>(frameType - 251));
            } else if (frameType == 255) {
                ++counters.full;
                reader.readU2();
                const auto locals = reader.readU2();
                skipVerificationTypes(locals);
                const auto stackItems = reader.readU2();
                skipVerificationTypes(stackItems);
            } else {
                ++counters.unknown;
            }
        }

        if (entryCount == 0) {
            result += " (no frames)";
        } else {
            std::vector<std::string> summaryParts;
            const auto summary = std::array<std::pair<const char *, uint32_t>, 8>{
                std::make_pair("same", counters.same),
                std::make_pair("same_locals", counters.sameLocals),
                std::make_pair("same_locals_ext", counters.sameLocalsExtended),
                std::make_pair("chop", counters.chop),
                std::make_pair("same_ext", counters.sameExtended),
                std::make_pair("append", counters.append),
                std::make_pair("full", counters.full),
                std::make_pair("unknown", counters.unknown)
            };

            for (const auto &[label, count] : summary) {
                if (count > 0) {
                    summaryParts.push_back(fmt::format("{}={}", label, count));
                }
            }

            if (summaryParts.empty()) {
                result += fmt::format(" frames={}", entryCount);
            } else {
                const auto list = kapa::infrastructure::string::stringUtils::join(summaryParts, std::string(", "));
                result += fmt::format(" frames={}, [{}]", entryCount, list);
            }
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Synthetic)
        result += " (synthetic)";
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Deprecated)
        result += " (deprecated)";
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(SourceDebugExtension)
        SourceDebugExtension sourceDebugExtension;
        sourceDebugExtension.nameIndex = attribute.nameIndex;
        sourceDebugExtension.length = attribute.length;
        std::string value;
        for (auto i = 0u; i < sourceDebugExtension.length; i++) {
            const auto character = static_cast<char>(reader.readU1());
            sourceDebugExtension.debugExtensions.push_back(character);
            value.push_back(character);
        }

        result += " " + value;

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Signature)
        const signature::SignatureParser signatureParser(_constantPool, attribute, reader);
        result += " " + signatureParser.toString();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(Record)
        const auto componentCount = reader.readU2();

        std::vector<std::string> parts;
        parts.reserve(componentCount);

        for (auto i = 0u; i < componentCount; ++i) {
            const auto nameIndex = reader.readU2();
            const auto descriptorIndex = reader.readU2();
            const auto attributesCount = reader.readU2();

            for (auto j = 0u; j < attributesCount; ++j) {
                reader.readU2(); // attribute_name_index (unused)
                const auto attributeLength = reader.readU4();
                reader.jump(static_cast<int>(attributeLength));
            }

            const auto name = _constantPool.getString(nameIndex);
            const auto descriptor = _constantPool.getTypeString(descriptorIndex);
            parts.push_back(fmt::format("{}:{}", name, descriptor));
        }

        if (parts.empty()) {
            result += " (no components)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(parts, std::string(", "));
            result += fmt::format(" components={}, {}", componentCount, list);
        }
    END_ATTR_TO_STRING()


    START_ATTR_TO_STRING(RuntimeVisibleAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleParameterAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeParameterAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleParameterAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeParameterAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(AnnotationDefault)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringAnnotationDefault();

    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeVisibleTypeAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeTypeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(RuntimeInvisibleTypeAnnotations)
        const annotations::AnnotationsParser annotationsParser(_constantPool, attribute, reader);
        result += " " + annotationsParser.toStringRuntimeTypeAnnotations();
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(EnclosingMethod)
        EnclosingMethod enclosingMethod;
        enclosingMethod.nameIndex = attribute.nameIndex;
        enclosingMethod.length = attribute.length;
        enclosingMethod.classIndex = reader.readU2();
        enclosingMethod.methodIndex = reader.readU2();
        const auto hostName = formatClassName(enclosingMethod.classIndex);
        result += fmt::format(" host={}", hostName);
        if (enclosingMethod.methodIndex != 0) {
            const auto methodSummary = formatNameAndType(enclosingMethod.methodIndex);
            result += fmt::format(", method={}", methodSummary);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(BootstrapMethods)
        BootstrapMethods bootstrapMethods;
        bootstrapMethods.nameIndex = attribute.nameIndex;
        bootstrapMethods.length = attribute.length;
        auto count = reader.readU2();
        std::vector<std::string> entries;
        entries.reserve(count);
        while (count > 0) {
            BootstrapMethod bootstrapMethod;
            bootstrapMethod.bootstrapMethodRef = reader.readU2();
            auto argumentCount = reader.readU2();
            std::vector<std::string> args;
            args.reserve(argumentCount);
            while (argumentCount > 0) {
                const auto argumentIndex = reader.readU2();
                bootstrapMethod.bootstrapArguments.push_back(argumentIndex);
                args.push_back(constantPoolIndexToString(argumentIndex));
                argumentCount--;
            }
            const auto handleSummary = methodHandleToString(bootstrapMethod.bootstrapMethodRef);
            const auto argsSummary = args.empty()
                                         ? std::string()
                                         : fmt::format(" args=[{}]", kapa::infrastructure::string::stringUtils::join(args, std::string(", ")));
            entries.push_back(fmt::format("{}{}", handleSummary, argsSummary));
            count--;

        }
        if (entries.empty()) {
            result += " (none)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(entries, std::string("; "));
            result += fmt::format(" count={}, {}", entries.size(), list);
        }
    END_ATTR_TO_STRING()

    START_ATTR_TO_STRING(PermittedSubclasses)
        PermittedSubclasses permittedSubclasses;
        permittedSubclasses.nameIndex = attribute.nameIndex;
        permittedSubclasses.length = attribute.length;
        permittedSubclasses.numberOfClasses = reader.readU2();
        auto count = permittedSubclasses.numberOfClasses;
        std::vector<std::string> names;
        names.reserve(count);
        while (count > 0) {
            const auto classIndex = reader.readU2();
            permittedSubclasses.classes.push_back(classIndex);
            names.push_back(formatClassName(classIndex));
            count--;
        }
        if (names.empty()) {
            result += " (none)";
        } else {
            const auto list = kapa::infrastructure::string::stringUtils::join(names, std::string(", "));
            result += fmt::format(" count={}, {}", names.size(), list);
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
