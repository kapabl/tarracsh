#include "CfDiff.h"
#include "ClassfileDiffApp.h"

#include <ranges>

#include "infrastructure/filesystem/Utils.h"
#include "domain/classfile/ClassFileParser.h"
#include "domain/classfile/reader/FileReader.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

using namespace kapa::tarracsh::domain::classfile;

using kapa::infrastructure::app::cli::ExitCode;
using kapa::tarracsh::domain::classfile::ClassFileParser;
using reader::FileReader;
using kapa::infrastructure::log::Log;

using constantpool::u4;
using constantpool::u2;
using constantpool::u1;

using kapa::infrastructure::filesystem::utils::ensureDir;


using namespace kapa::cldiff::app;

std::unique_ptr<ClassfileDiffApp> ClassfileDiffApp::_app;


void ClassfileDiffApp::setupCliOptions() {
    set_version_flag("--version", "version " CF_DIFF_VERSION);
    set_help_all_flag("--help-all");

    add_flag("--ignore-classname", _ignoreClassname, "Ignore classname during comparison");
    add_flag("--ignore-code", _ignoreCode, "Ignore code attributes of methods");
    add_flag("-p,--public-only", _publicOnly, "yes/no, default yes");

    allow_extras();

    callback([this]() {
        const auto &args = remaining();
        if (args.size() != 2) {
            throw CLI::ValidationError("Exactly two files are required.");
        }
        _leftFile = args[0];
        _rightFile = args[1];
    });
}


void ClassfileDiffApp::validateFile(const std::string &filename) const {
    if (!std::filesystem::exists(filename)) {
        throw CLI::ValidationError(std::format("File does not exist: {}", filename), CLI::ExitCodes::FileError);
    }
}

ExitCode ClassfileDiffApp::parseCli(int argc, char **argv) {
    ExitCode result = 0;
    try {
        parse(argc, argv);

        validateFile(_leftFile);
        validateFile(_rightFile);

    } catch (const CLI::ParseError &e) {
        result = exit(e);
    }
    return result;
}

void ClassfileDiffApp::outputResult() const {

    _log->writeln("");
    _log->writeln("");

    if (_differenceCount == 0) {
        _log->writeln("No differences found");
    } else {
        _log->writeln(std::format("{} differences found", _differenceCount));
    }
}

ExitCode ClassfileDiffApp::start(int argc, char *argv[]) {
    setupCliOptions();
    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0) {
        return exitCode;
    }

    FileReader leftReader(_leftFile);
    _leftParser = std::make_unique<ClassFileParser>(leftReader, _leftFile, _log);

    FileReader rightReader(_rightFile);
    _rightParser = std::make_unique<ClassFileParser>(rightReader, _rightFile, _log);
    if (_leftParser->parse() && _rightParser->parse()) {
        this->compare();
        outputResult();
    } else {
        if (!_leftParser->succeeded()) {
            _log->writeln(std::format("Failed to parse {}", _leftFile));
        }

        if (!_rightParser->succeeded()) {
            _log->writeln(std::format("Failed to parse {}", _rightFile));
        }

        exitCode = 1;

    }

    // if (_leftParser->parse()) {
    //     FileReader rightReader(_rightFile);
    //     ClassFileParser _rightParser(rightReader, _rightFile, _log);
    //     if (_rightParser->parse()) {
    //         this->compare();
    //         outputResult();
    //     } else {
    //         _log->writeln(std::format("Failed to parse {}", _rightFile));
    //         exitCode = 1;
    //     }
    //
    // } else {
    //     _log->writeln(std::format("Failed to parse {}", _leftFile));
    //     exitCode = 1;
    // }

    return exitCode;
}

ExitCode ClassfileDiffApp::run(int argc, char **argv) {

    const auto log = std::make_shared<Log>();
    log->forceStdout(true);
    log->setFile("cfdiff.log");

    _app.reset(new ClassfileDiffApp("", "cfdiff", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        std::cin.get();
    }

    return result;

}

ClassfileDiffApp::ClassfileDiffApp(const std::string &description, const std::string &name,
                                   const std::shared_ptr<Log> log)
    : CliApp(description, name), _log(log), _results(_options) {

    _results.log = log;
}

std::string ClassfileDiffApp::getClassInfoName(const std::unique_ptr<ClassFileParser> &parser,
                                               const u2 classInfoIndex) {
    return parser->getConstantPool().getClassInfoName(classInfoIndex);
}

auto ClassfileDiffApp::areClassnamesEqual(const u2 leftClassInfo, const u2 rightClassInfo,
                                          const ClassnameCompareCallback &callback) const
    -> bool {

    const auto leftClassname = getClassInfoName(_leftParser, leftClassInfo);
    const auto rightClassname = getClassInfoName(_rightParser, rightClassInfo);
    const auto result = leftClassname == rightClassname;
    callback(result, leftClassname, rightClassname);

    return result;

}

auto ClassfileDiffApp::areClassnamesEqual(const u2 leftClassInfo, const u2 rightClassInfo) const
    -> bool {

    const auto leftClassname = _leftParser->getConstantPool().getClassInfoName(leftClassInfo);
    const auto rightClassname = _rightParser->getConstantPool().getClassInfoName(rightClassInfo);
    const auto result = leftClassname == rightClassname;

    return result;

}


void ClassfileDiffApp::compareSuperClass() {

    areClassnamesEqual(
        _leftParser->getMainClassInfo().superClass,
        _rightParser->getMainClassInfo().superClass,
        [this](const bool result, const std::string &left, const std::string &right) {
            if (result) {
                _log->writeln("Superclass is equal");
            } else {
                _differenceCount++;
                _log->writeln(std::format("Superclass differs: {}, {}", left, right));
            }
        });

}

auto ClassfileDiffApp::compareAccessFlags(const u2 accessFlagsLeft, const u2 accessFlagsRight,
                                          std::string &message) -> bool {
    const auto result = accessFlagsLeft == accessFlagsRight;
    if (!result) {
        _differenceCount++;
        message = std::format("Access flags differ:\nleft({:x}):{}\nright({:x}):{}\n",
                              accessFlagsLeft,
                              accessmodifier::toString(accessFlagsLeft),
                              accessFlagsRight,
                              accessmodifier::toString(accessFlagsRight));
    } else {
        message = "Access flags are equal";
    }
    return result;
}

void ClassfileDiffApp::compareMainClassAccessFlags() {
    std::string message;
    compareAccessFlags(_leftParser->getMainClassInfo().accessFlags, _rightParser->getMainClassInfo().accessFlags,
                       message);
    _log->writeln(message);
}

void ClassfileDiffApp::compareMainClassname() {
    if (_ignoreClassname) {
        _log->writeln("Main class name ignored");
    } else {
        const auto leftClassname = _leftParser->getMainClassname();
        const auto rightClassname = _rightParser->getMainClassname();
        if (leftClassname != rightClassname) {
            _differenceCount++;
            _log->writeln(std::format("Main class name differs: {}, {}",
                                      leftClassname, rightClassname));
        } else {
            _log->writeln("Main class name is equal");
        }
    }
}

void ClassfileDiffApp::compareMainClassInterfaces() {

    const auto leftInterfaces = _leftParser->getInterfaces();
    std::unordered_map<std::string, u2> leftMap;
    for (const auto &leftInterface : leftInterfaces) {
        leftMap[getClassInfoName(_leftParser, leftInterface)] = leftInterface;
    }

    const auto rightInterfaces = _rightParser->getInterfaces();
    std::unordered_map<std::string, u2> rightMap;
    for (const auto &rightInterface : rightInterfaces) {
        rightMap[getClassInfoName(_rightParser, rightInterface)] = rightInterface;
    }

    for (const auto &leftName : leftMap | std::views::keys) {
        if (rightMap.contains(leftName)) {
            rightMap.erase(leftName);
            _log->writeln(std::format("Interface {} is equal", leftName));
        } else {
            _differenceCount++;
            _log->writeln(std::format("Interface {} is only in left", leftName));
        }
    }

    for (const auto &rightName : rightMap | std::views::keys) {
        _differenceCount++;
        _log->writeln(std::format("Interface {} is only in right", rightName));
    }

}

auto ClassfileDiffApp::getLeftString(const u2 constantPoolIndex) const -> std::string {
    return getString(_leftParser, constantPoolIndex);
}

auto ClassfileDiffApp::getRightString(const u2 constantPoolIndex) const -> std::string {
    return getString(_rightParser, constantPoolIndex);
}

auto ClassfileDiffApp::getString(const std::unique_ptr<ClassFileParser> &parser,
                                 const u2 constantPoolIndex) -> std::string {
    auto result = parser->getConstantPool().getString(constantPoolIndex);
    return result;
}

auto ClassfileDiffApp::compareAttributes(const std::vector<attribute::AttributeInfo> &left,
                                         const std::vector<attribute::AttributeInfo> &right,
                                         std::string &message) -> bool {

    std::unordered_map<std::string, attribute::AttributeInfo> leftMap;
    for (const auto &leftAttribute : left) {
        leftMap[getLeftString(leftAttribute.nameIndex)] = leftAttribute;
    }

    std::unordered_map<std::string, attribute::AttributeInfo> rightMap;
    for (const auto &rightAttribute : right) {
        rightMap[getRightString(rightAttribute.nameIndex)] = rightAttribute;
    }

    auto attributeDifferences = 0u;

    std::vector<std::string> messages;
    for (auto &[attributeName, leftAttributeInfo] : leftMap) {
        if (attributeName == CODE && _ignoreCode) {
            continue;
        }
        if (rightMap.contains(attributeName)) {
            const auto &rightAttributeInfo = rightMap[attributeName];

            const auto areEqual = leftAttributeInfo.length == rightAttributeInfo.length &&
                                  leftAttributeInfo.info == rightAttributeInfo.info;

            if (areEqual) {
                messages.push_back(std::format("Attribute {} is equal", attributeName));
            } else {
                attributeDifferences++;
                messages.push_back(std::format("Attribute {} differs", attributeName));
            }

            rightMap.erase(attributeName);
        } else {
            messages.push_back(std::format("Attribute {} is missing in right", attributeName));
        }
    }

    for (const auto &attributeName : rightMap | std::views::keys) {
        if (attributeName == CODE && _ignoreCode) {
            continue;
        }
        attributeDifferences++;
        messages.push_back(std::format("Attribute {} is missing in left", attributeName));
    }

    message = infrastructure::string::stringUtils::join(messages, std::string("\n"));

    _differenceCount += attributeDifferences;
    const auto result = attributeDifferences == 0;
    return result;
}

void ClassfileDiffApp::compareMainClassAttributes() {
    std::string message;
    compareAttributes(_leftParser->getAttributes(), _rightParser->getAttributes(), message);
    _log->writeln(message);
}

void ClassfileDiffApp::compareMainClass() {
    _log->writeln("Comparing main class:");
    compareMainClassname();
    compareSuperClass();
    compareMainClassAccessFlags();
    compareMainClassInterfaces();
    compareMainClassAttributes();
}

auto ClassfileDiffApp::areMethodsEqual(const constantpool::MethodInfo &leftMethod,
                                       const constantpool::MethodInfo &rightMethod, std::string &message) -> bool {

    std::string accessFlagsMessage;
    auto result = compareAccessFlags(leftMethod.accessFlags, rightMethod.accessFlags, accessFlagsMessage);

    std::string descriptorMessage;
    result = compareDescriptors(leftMethod.descriptorIndex, rightMethod.descriptorIndex, descriptorMessage) && result;

    std::string attributeMessage;
    result = compareAttributes(leftMethod.attributes, rightMethod.attributes, attributeMessage) && result;

    message = std::format("{}\n{}\n{}", accessFlagsMessage, descriptorMessage, attributeMessage);

    return result;
}

void ClassfileDiffApp::compareMethods() {
    _log->writeln("");
    _log->writeln("Comparing methods:");

    std::unordered_map<std::string, std::vector<constantpool::MethodInfo>> leftMap;

    for (auto &leftMethod : _leftParser->getMethods()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(leftMethod.accessFlags)) continue;
        }
        leftMap[getLeftString(leftMethod.nameIndex)].push_back(leftMethod);
    }

    std::unordered_map<std::string, std::vector<constantpool::MethodInfo>> rightMap;
    for (auto &rightMethod : _rightParser->getMethods()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(rightMethod.accessFlags)) continue;
        }
        rightMap[getRightString(rightMethod.nameIndex)].push_back(rightMethod);
    }

    for (auto &[methodName, leftMethods] : leftMap) {
        if (rightMap.contains(methodName)) {
            auto &rightMethods = rightMap[methodName];
            _log->writeln(std::format("Method: {}", methodName));

            if (leftMethods.size() > 1) {
                _log->writeln(std::format("Left method {} has {} overloads", methodName, leftMethods.size()));
            }

            if (rightMethods.size() > 1) {
                _log->writeln(std::format("Right method {} has {} overloads", methodName, rightMethods.size()));
            }

            const auto prevDifferenceCount = _differenceCount;
            auto matchFound = false;
            std::string allMessages;
            for (auto &leftMethod : leftMethods) {
                for (auto &rightMethod : rightMethods) {
                    std::string message;
                    _log->writeln("Comparing...");
                    if (areMethodsEqual(leftMethod, rightMethod, message)) {
                        rightMap.erase(methodName);
                        _log->writeln(std::format("Method {} is equal", methodName));
                        matchFound = true;
                        break;
                    } else {
                        message += std::format("\nMethod {} differs\n", methodName);
                    }
                    allMessages += message;
                }
                if (matchFound) {
                    break;
                }
            }
            if (matchFound) {
                _differenceCount = prevDifferenceCount;
            } else {
                _log->writeln(allMessages);
            }
            _log->writeln("");
        } else {
            _differenceCount++;
            _log->writeln(std::format("Method {} is only in left", methodName));
        }
    }
}

auto ClassfileDiffApp::compareDescriptors(const u2 leftIndex,
                                          const u2 rightIndex, std::string &message) -> bool {
    const auto leftDescriptor = getLeftString(leftIndex);
    const auto rightDescriptor = getRightString(rightIndex);
    const auto result = leftDescriptor == rightDescriptor;
    if (!result) {
        _differenceCount++;
        message = std::format("Descriptors differ: {}, {}", leftDescriptor, rightDescriptor);
    }

    return result;
}

void ClassfileDiffApp::compareFields() {

    _log->writeln("");
    _log->writeln("Comparing fields:");

    std::unordered_map<std::string, constantpool::FieldInfo> leftMap;
    for (const auto &leftField : _leftParser->getFields()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(leftField.accessFlags)) continue;
        }
        leftMap[getLeftString(leftField.nameIndex)] = leftField;
    }

    std::unordered_map<std::string, constantpool::FieldInfo> rightMap;
    for (const auto &rightField : _rightParser->getFields()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(rightField.accessFlags)) continue;
        }
        rightMap[getLeftString(rightField.nameIndex)] = rightField;
    }

    for (auto &[leftName, leftFieldInfo] : leftMap) {
        if (rightMap.contains(leftName)) {
            const auto &rightFieldInfo = rightMap[leftName];

            _log->writeln(std::format("Field: {}", leftName));

            std::string accessFlagsMessage;
            compareAccessFlags(leftFieldInfo.accessFlags, rightFieldInfo.accessFlags, accessFlagsMessage);
            _log->writeln(accessFlagsMessage);

            std::string descriptorMessage;
            compareDescriptors(leftFieldInfo.descriptorIndex, rightFieldInfo.descriptorIndex, descriptorMessage);
            _log->writeln(descriptorMessage);

            std::string attributeMessage;
            compareAttributes(leftFieldInfo.attributes, rightFieldInfo.attributes, attributeMessage);
            _log->writeln(attributeMessage);

            rightMap.erase(leftName);

        } else {
            _differenceCount++;
            _log->writeln(std::format("Field {} is missing in right", leftName));
        }
    }

    for (const auto &rightName : rightMap | std::views::keys) {
        _differenceCount++;
        _log->writeln(std::format("Field {} is missing in left", rightName));
    }
}

void ClassfileDiffApp::compareMembers() {
    compareMethods();
    compareFields();
}

void ClassfileDiffApp::compare() {
    compareMainClass();
    compareMembers();
}

void ClassfileDiffApp::init() const {

    auto result = true;

#ifdef _WIN32
    prepareConsoleForVT100();
    prepareConsoleForUTF8();
#endif

    ensureDir(_options.outputDir);
    _log->init(_options.logFile);

}
