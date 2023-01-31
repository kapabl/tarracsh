#include "Columns.h"
#include <format>
#include <chrono>
#include "../Database.h"


using namespace kapa::infrastructure::db::tables::columns;

std::string kapa::infrastructure::db::tables::columns::displayAsToString(const DisplayAs displayAs) {
    switch (displayAs) {

        case AsInt32:
            return "signed 32 bits";
        case AsUInt32:
            return "unsigned 32 bits";
        case AsUInt64:
            return "unsigned 64 bits";
        case AsRef:
            return "reference into another table";
        case AsString:
            return "String";
        case AsDigest:
            return "Digest";
        case AsDatetime:
            return "Datetime";
        case AsSize:
            return "Size";
        case AsEntryType:
            return "EntryType(Classfile, Jar, Directory)";
        case AsBool:
            return "bool true/false";

    }

    return "unknown";
}

Properties::Properties()
    : type(StorageType::UInt64),
      displayAs(DisplayAs::AsUInt64) {
    memset(name, 0, MAX_COLUMN_NAME);

}

Properties::Properties(const char *name, const StorageType type,
                       const DisplayAs displayAs, int offsetInRow) {
    memset(this->name, 0, MAX_COLUMN_NAME);
    strcpy_s(this->name, name);
    this->type = type;
    this->displayAs = displayAs;
    this->offsetInRow = offsetInRow;
}

std::string Properties::valueToString(char *pValue, Database &db) {
    std::string result = toStringMap[displayAs](pValue, *this, db);
    return result;
}

std::unordered_map<int, Properties::ToStringFunc> Properties::toStringMap;

void Properties::registerColumn(const int displayAsValue, const ToStringFunc &func) {
    toStringMap[displayAsValue] = func;
}

bool DigestCol::operator==(const DigestCol &right) const = default;

bool DigestCol::operator==(const std::vector<unsigned char> &left) const {
    if (left.size() != DIGEST_LENGTH) return false;
    const auto result = memcmp(buf, &*left.begin(), DIGEST_LENGTH) == 0;
    return result;
}

DigestCol &DigestCol::operator=(const std::vector<unsigned char> &left) {
    assert(left.size() == DIGEST_LENGTH);
    memcpy(buf, &*left.begin(), DIGEST_LENGTH);
    return *this;
}


static bool registerColumns() {
    Properties::registerColumn(
        DisplayAs::AsDatetime,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            const auto microseconds = std::chrono::microseconds(*reinterpret_cast<uint64_t *>(pValue));
            std::chrono::file_clock::time_point timePoint(microseconds);
            auto result = std::format("{:%F %T}", timePoint);
            return result;
        });
    Properties::registerColumn(
        DisplayAs::AsSize,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            const auto value = *reinterpret_cast<uint64_t *>(pValue);
            auto result = kapa::infrastructure::string::stringUtils::sizeToHumanReadable(value);
            return result;
        });
    Properties::registerColumn(
        DisplayAs::AsInt32,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            auto result = std::format("{}", reinterpret_cast<int32_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsUInt32,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            auto result = std::format("{}", reinterpret_cast<uint32_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsUInt64,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            auto result = std::format("{}", reinterpret_cast<uint64_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsString,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            std::string result = db.getStringPool()->getCString(reinterpret_cast<StringCol &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsRef,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            //TODO lookup on the referenced table
            auto result = std::format("{}", reinterpret_cast<uint64_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsDigest,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            std::string result;
            for (auto i = 0u; i < DIGEST_LENGTH; i++) {
                result += std::format(" {:02x}", static_cast<unsigned char>(pValue[i]));
            }
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsBool,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            auto result = (*pValue & 1) == 1 ? "true" : "false";
            return result;
        });

    return true;
}

static bool registerColumnResult = registerColumns();
