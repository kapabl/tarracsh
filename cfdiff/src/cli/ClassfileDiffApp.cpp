#include "CfDiff.h"
#include "ClassfileDiffApp.h"

#include <ranges>
#include <iostream>

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


using namespace kapa::cfdiff::app;

using namespace std;

unique_ptr<ClassfileDiffApp> ClassfileDiffApp::_app;


void ClassfileDiffApp::setupCliOptions() {
    set_version_flag("--version", "version " CF_DIFF_VERSION);
    set_help_all_flag("--help-all");

    add_flag("--ignore-classname", _ignoreClassname, "Ignore classname during comparison");
    add_option("--ignore-attributes", _ignoredAttributes,
               "Coma separated list fo attributes to ignore eg: --ignore-attributes=Code SourceFile");
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


void ClassfileDiffApp::validateFile(const string &filename) const {
    if (!filesystem::exists(filename)) {
        throw CLI::ValidationError(fmt::format("File does not exist: {}", filename), CLI::ExitCodes::FileError);
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
        _log->writeln(fmt::format("{} differences found", _differenceCount));
    }
}

ExitCode ClassfileDiffApp::start(int argc, char *argv[]) {
    setupCliOptions();
    auto exitCode = parseCli(argc, argv);
    if (exitCode != 0 || count("--help") > 0 ) {
        return exitCode;
    }

    FileReader leftReader(_leftFile);
    _leftParser = make_unique<ClassFileParser>(leftReader, _leftFile, _log);

    FileReader rightReader(_rightFile);
    _rightParser = make_unique<ClassFileParser>(rightReader, _rightFile, _log);
    if (_leftParser->parse() && _rightParser->parse()) {
        this->compare();
        outputResult();
    } else {
        if (!_leftParser->succeeded()) {
            _log->writeln(fmt::format("Failed to parse {}", _leftFile));
        }

        if (!_rightParser->succeeded()) {
            _log->writeln(fmt::format("Failed to parse {}", _rightFile));
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
    //         _log->writeln(fmt::format("Failed to parse {}", _rightFile));
    //         exitCode = 1;
    //     }
    //
    // } else {
    //     _log->writeln(fmt::format("Failed to parse {}", _leftFile));
    //     exitCode = 1;
    // }

    return exitCode;
}

ExitCode ClassfileDiffApp::run(int argc, char **argv) {

    const auto log = make_shared<Log>();
    log->forceStdout(true);
    log->setFile("cfdiff.log");

    _app.reset(new ClassfileDiffApp("", "cfdiff", log));

    const auto result = _app->start(argc, argv);

    if (_app->_options.pause) {
        cin.get();
    }

    return result;

}

ClassfileDiffApp::ClassfileDiffApp(const string &description, const string &name,
                                   const shared_ptr<Log> log)
    : CliApp(description, name), _log(log), _results(_options) {

    _results.log = log;
}

string ClassfileDiffApp::getClassInfoName(const unique_ptr<ClassFileParser> &parser,
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
        [this](const bool result, const string &left, const string &right) {
            if (result) {
                _log->writelnGreen(fmt::format("Superclass is equal: {}", left));
            } else {
                _differenceCount++;
                _log->writelnRed(fmt::format("Superclass differs: {}, {}", left, right));
            }
        });

}

auto ClassfileDiffApp::compareAccessFlags(const u2 accessFlagsLeft, const u2 accessFlagsRight,
                                          string &message) -> bool {
    const auto result = accessFlagsLeft == accessFlagsRight;
    if (!result) {
        _differenceCount++;
        message = fmt::format("\033[31mAccess flags differ:\nleft({:x}):{}\nright({:x}):{}\033[0m",
                         accessFlagsLeft,
                         accessmodifier::toString(accessFlagsLeft),
                         accessFlagsRight,
                         accessmodifier::toString(accessFlagsRight));
    } else {
        message = fmt::format("\033[32mAccess flags are equal: {}\033[0m", accessmodifier::toString(accessFlagsRight));
    }
    return result;
}

void ClassfileDiffApp::compareMainClassAccessFlags() {
    string message;
    compareAccessFlags(_leftParser->getMainClassInfo().accessFlags, _rightParser->getMainClassInfo().accessFlags,
                       message);
    _log->writeln(message);
}

void ClassfileDiffApp::compareMainClassname() {
    const auto leftClassname = _leftParser->getMainClassname();
    const auto rightClassname = _rightParser->getMainClassname();
    if (_ignoreClassname) {
        _log->writeln(fmt::format("Main class name ignored: {}, {}",
                             leftClassname, rightClassname));
    } else {
        if (leftClassname != rightClassname) {
            _differenceCount++;
            _log->writelnRed(fmt::format("Main class name differs: {}, {}",
                                    leftClassname, rightClassname));
        } else {
            _log->writelnGreen("Main class name is equal");
        }
    }
}

void ClassfileDiffApp::compareMainClassInterfaces() {

    const auto leftInterfaces = _leftParser->getInterfaces();
    unordered_map<string, u2> leftMap;
    for (const auto &leftInterface : leftInterfaces) {
        leftMap[getClassInfoName(_leftParser, leftInterface)] = leftInterface;
    }

    const auto rightInterfaces = _rightParser->getInterfaces();
    unordered_map<string, u2> rightMap;
    for (const auto &rightInterface : rightInterfaces) {
        rightMap[getClassInfoName(_rightParser, rightInterface)] = rightInterface;
    }

    for (const auto &leftName : leftMap | views::keys) {
        if (rightMap.contains(leftName)) {
            rightMap.erase(leftName);
            _log->writelnGreen(fmt::format("Interface {} is equal", leftName));
        } else {
            _differenceCount++;
            _log->writelnRed(fmt::format("Interface {} is only in left", leftName));
        }
    }

    for (const auto &rightName : rightMap | views::keys) {
        _differenceCount++;
        _log->writelnRed(fmt::format("Interface {} is only in right", rightName));
    }

}

auto ClassfileDiffApp::getLeftString(const u2 constantPoolIndex) const -> string {
    return getString(_leftParser, constantPoolIndex);
}

auto ClassfileDiffApp::getRightString(const u2 constantPoolIndex) const -> string {
    return getString(_rightParser, constantPoolIndex);
}

auto ClassfileDiffApp::getString(const unique_ptr<ClassFileParser> &parser,
                                 const u2 constantPoolIndex) -> string {
    auto result = parser->getConstantPool().getString(constantPoolIndex);
    return result;
}

auto ClassfileDiffApp::compareAttributes(const vector<attribute::AttributeInfo> &left,
                                         const vector<attribute::AttributeInfo> &right,
                                         string &message) -> bool {

    unordered_map<string, attribute::AttributeInfo> leftMap;
    for (const auto &leftAttribute : left) {
        leftMap[getLeftString(leftAttribute.nameIndex)] = leftAttribute;
    }

    unordered_map<string, attribute::AttributeInfo> rightMap;
    for (const auto &rightAttribute : right) {
        rightMap[getRightString(rightAttribute.nameIndex)] = rightAttribute;
    }

    auto attributeDifferences = 0u;

    vector<string> messages;
    for (auto &[attributeName, leftAttributeInfo] : leftMap) {
        if (_ignoredAttributes.contains(attributeName)) {
            continue;
        }
        if (rightMap.contains(attributeName)) {
            const auto &rightAttributeInfo = rightMap[attributeName];

            const auto areEqual = leftAttributeInfo.length == rightAttributeInfo.length &&
                                  leftAttributeInfo.info == rightAttributeInfo.info;

            if (areEqual) {
                messages.push_back(fmt::format("\033[32mAttribute {} is equal\033[0m", attributeName));
            } else {
                attributeDifferences++;
                messages.push_back(fmt::format("\033[31mAttribute {} differs\033[0m", attributeName));
            }

            rightMap.erase(attributeName);
        } else {
            messages.push_back(fmt::format("\033[31mAttribute {} is missing in right\033[0m", attributeName));
        }
    }

    for (const auto &attributeName : rightMap | views::keys) {
        if (_ignoredAttributes.contains(attributeName)) {
            continue;
        }
        attributeDifferences++;
        messages.push_back(fmt::format("\033[31mAttribute {} is missing in left\033[0m", attributeName));
    }

    const auto result = attributeDifferences == 0;
    if (result) {
        messages.emplace_back("\033[32mAttributes are equal\033[0m");
    }
    message = infrastructure::string::stringUtils::join(messages, string("\n"));

    _differenceCount += attributeDifferences;

    return result;
}

void ClassfileDiffApp::compareMainClassAttributes() {
    string message;
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
                                       const constantpool::MethodInfo &rightMethod, string &message) -> bool {

    string accessFlagsMessage;
    auto result = compareAccessFlags(leftMethod.accessFlags, rightMethod.accessFlags, accessFlagsMessage);

    string descriptorMessage;
    result = compareDescriptors(leftMethod.descriptorIndex, rightMethod.descriptorIndex, descriptorMessage) && result;

    string attributeMessage;
    result = compareAttributes(leftMethod.attributes, rightMethod.attributes, attributeMessage) && result;

    message = fmt::format("{}\n{}\n{}", accessFlagsMessage, descriptorMessage, attributeMessage);

    return result;
}

void ClassfileDiffApp::compareToRightMethods(const constantpool::MethodInfo &leftMethod,
                                             unordered_map<string, vector<constantpool::MethodInfo>> &rightMap,
                                             vector<constantpool::MethodInfo> &rightMethods) {
    const auto methodName = getLeftString(leftMethod.nameIndex);
    const auto leftDescriptor = getLeftString(leftMethod.descriptorIndex);
    _log->writeln(fmt::format("Comparing {} {}", methodName, leftDescriptor));
    auto matchFound = false;
    string allMessages;
    auto rightIndex = 0u;
    const auto prevDifferenceCount = _differenceCount;
    for (auto &rightMethod : rightMethods) {
        string message;
        if (areMethodsEqual(leftMethod, rightMethod, message)) {
            _log->writelnGreen(message);
            _log->writelnGreen(fmt::format("Method is equal {} {}", methodName, leftDescriptor));
            matchFound = true;
            break;
        } else {
            message += fmt::format("\n\033[31mMethod {} differs\n\033[0m", methodName);
        }
        allMessages += message;
        rightIndex++;
    }
    if (matchFound) {
        _differenceCount = prevDifferenceCount;

        rightMethods.erase(rightMethods.begin() + rightIndex);
        if (rightMethods.empty()) {
            rightMap.erase(methodName);
        }
    } else {
        _log->writeln(allMessages);
    }
}

void ClassfileDiffApp::extractMethods(const unique_ptr<ClassFileParser> &parser,
                                      unordered_map<string, vector<constantpool::MethodInfo>> &methodMap) const {
    for (auto &methodInfo : parser->getMethods()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(methodInfo.accessFlags)) continue;
        }
        methodMap[getString(parser, methodInfo.nameIndex)].push_back(methodInfo);
    }
}

void ClassfileDiffApp::checkOverload(const std::string &position, const std::string &methodName,
                                     const vector<constantpool::MethodInfo> &methodInfos) const {
    if (methodInfos.size() > 1) {
        _log->writeln(fmt::format("{} method {} has {} overloads", position, methodName, methodInfos.size()));
    }
}

void ClassfileDiffApp::compareMethods() {
    _log->writeln("");
    _log->writeln("Comparing methods:");

    unordered_map<string, vector<constantpool::MethodInfo>> leftMap;
    extractMethods(_leftParser, leftMap);

    unordered_map<string, vector<constantpool::MethodInfo>> rightMap;
    extractMethods(_rightParser, rightMap);

    for (auto &[methodName, leftMethods] : leftMap) {
        checkOverload("Left", methodName, leftMethods);
        if (rightMap.contains(methodName)) {
            auto &rightMethods = rightMap[methodName];
            _log->writeln("");
            _log->writeln(fmt::format("Method: {}", methodName));

            checkOverload("Right", methodName, rightMethods);
            for (auto &leftMethod : leftMethods) {
                compareToRightMethods(leftMethod, rightMap, rightMethods);
                if (rightMethods.empty()) break;
            }

            _log->writeln("");
        } else {

            for (const auto &methodInfo : leftMethods) {
                _differenceCount++;
                _log->writelnRed(fmt::format("Method {} {} is only in left", methodName,
                                        getLeftString(methodInfo.descriptorIndex)));
            }

        }
    }

    for (auto &[methodName, rightMethods] : rightMap) {
        for (const auto &rightMethod : rightMethods) {
            _differenceCount++;
            _log->writelnRed(fmt::format("Method {} {} is only in right", methodName,
                                    getRightString(rightMethod.descriptorIndex)));
        }
    }
}

auto ClassfileDiffApp::compareDescriptors(const u2 leftIndex,
                                          const u2 rightIndex, string &message) -> bool {
    const auto leftDescriptor = getLeftString(leftIndex);
    const auto rightDescriptor = getRightString(rightIndex);
    const auto result = leftDescriptor == rightDescriptor;
    if (result) {
        message = fmt::format("\033[32mDescriptors are equal: {}\033[0m", leftDescriptor);
    } else {
        _differenceCount++;
        message = fmt::format("\033[31mDescriptors differ: {}, {}\033[0m", leftDescriptor, rightDescriptor);
    }

    return result;
}

void ClassfileDiffApp::compareFields() {

    _log->writeln("");
    _log->writeln("Comparing fields:");

    unordered_map<string, constantpool::FieldInfo> leftMap;
    for (const auto &leftField : _leftParser->getFields()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(leftField.accessFlags)) continue;
        }
        leftMap[getLeftString(leftField.nameIndex)] = leftField;
    }

    unordered_map<string, constantpool::FieldInfo> rightMap;
    for (const auto &rightField : _rightParser->getFields()) {
        if (_publicOnly) {
            if (!accessmodifier::isPublic(rightField.accessFlags)) continue;
        }
        rightMap[getRightString(rightField.nameIndex)] = rightField;
    }

    for (auto &[leftName, leftFieldInfo] : leftMap) {
        if (rightMap.contains(leftName)) {
            const auto &rightFieldInfo = rightMap[leftName];

            const auto prevDifferenceCount = _differenceCount;

            _log->writeln("");
            _log->writeln(fmt::format("Field: {}", leftName));

            string accessFlagsMessage;
            compareAccessFlags(leftFieldInfo.accessFlags, rightFieldInfo.accessFlags, accessFlagsMessage);
            _log->writeln(accessFlagsMessage);

            string descriptorMessage;
            compareDescriptors(leftFieldInfo.descriptorIndex, rightFieldInfo.descriptorIndex, descriptorMessage);
            _log->writeln(descriptorMessage);

            string attributeMessage;
            compareAttributes(leftFieldInfo.attributes, rightFieldInfo.attributes, attributeMessage);
            _log->writeln(attributeMessage);

            rightMap.erase(leftName);

            if (_differenceCount == prevDifferenceCount) {
                _log->writeln(fmt::format("\033[32mField {} is equal\033[0m", leftName));
            }

        } else {
            _differenceCount++;
            _log->writeln(fmt::format("\033[31mField {} is missing in right\033[0m", leftName));
        }
    }
    _log->writeln("");
    for (const auto &rightName : rightMap | views::keys) {
        _differenceCount++;
        _log->writeln(fmt::format("\033[31mField {} is missing in left\033[0m", rightName));
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
