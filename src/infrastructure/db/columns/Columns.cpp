#include "Columns.h"
#include <fmt/format.h>
#include <fmt/chrono.h>
#include <chrono>
#include "../Database.h"
#include "../table/Table.h"


using namespace kapa::infrastructure::db::table::column;

std::string kapa::infrastructure::db::table::column::displayAsToString(const DisplayAs displayAs) {
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
                       const DisplayAs displayAs, uint64_t offsetInRow) {
    memset(this->name, 0, MAX_COLUMN_NAME);
    strcpy_s(this->name, name);
    this->type = type;
    this->displayAs = displayAs;
    this->offsetInRow = offsetInRow;
}

Properties::Properties(const char *name, const StorageType type,
                       const DisplayAs displayAs, uint64_t offsetInRow,
                       const char *refTable, const char *displayColumn) {
    memset(this->name, 0, MAX_COLUMN_NAME);
    strcpy_s(this->name, name);

    memset(this->refColProperties.table, 0, MAX_COLUMN_NAME);
    strcpy_s(this->refColProperties.table, refTable);

    memset(this->refColProperties.displayColumn, 0, MAX_COLUMN_NAME);
    strcpy_s(this->refColProperties.displayColumn, displayColumn);
    this->type = type;
    this->displayAs = displayAs;
    this->offsetInRow = offsetInRow;
}

std::string Properties::valueToString(char *pValue, Database &db, const bool displayRaw) const {
    std::string result = toStringMap[displayAs](pValue, *this, db, displayRaw);
    return result;
}

std::unordered_map<int, Properties::ToStringFunc> Properties::toStringMap;

void Properties::registerColumn(const int displayAsValue, const ToStringFunc &func) {
    toStringMap[displayAsValue] = func;
}

DigestCol::DigestCol() {
    std::memset(buf, 0, DIGEST_LENGTH);
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
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            const auto microseconds = std::chrono::microseconds(*reinterpret_cast<uint64_t *>(pValue));
            const std::chrono::file_clock::time_point timePoint(microseconds);
            const auto timeTType = std::chrono::duration_cast<std::chrono::seconds>(timePoint.time_since_epoch()).count();
            auto result = fmt::format("{:%F %T}", fmt::localtime(timeTType));
            return result;
        });
    Properties::registerColumn(
        DisplayAs::AsSize,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            const auto value = *reinterpret_cast<uint64_t *>(pValue);
            auto result = kapa::infrastructure::string::stringUtils::sizeToHumanReadable(value);
            return result;
        });
    Properties::registerColumn(
        DisplayAs::AsInt32,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            auto result = fmt::format("{}", reinterpret_cast<int32_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsUInt32,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            auto result = fmt::format("{}", reinterpret_cast<uint32_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsUInt64,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            auto result = fmt::format("{}", reinterpret_cast<uint64_t &>(*pValue));
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsString,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            std::string result;
            auto &stringCol = reinterpret_cast<StringCol &>(*pValue);
            if (displayRaw) {
                result = fmt::format("{}", stringCol);
            } else {
                result = db.getStringPool()->getCString(stringCol);
            }
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsRef,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            std::string result;
            const auto ref = reinterpret_cast<uint64_t &>(*pValue);
            if (displayRaw) {
                result = fmt::format("{}", ref);
            } else {
                auto table = db.getTable(properties.refColProperties.table);
                result = fmt::format("{}", table->getColumnValue(ref, properties.refColProperties.displayColumn));
            }
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsDigest,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            std::string result;
            for (auto i = 0u; i < DIGEST_LENGTH; i++) {
                result += fmt::format(" {:02x}", static_cast<unsigned char>(pValue[i]));
            }
            return result;
        });

    Properties::registerColumn(
        DisplayAs::AsBool,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            auto result = (*pValue & 1) == 1 ? "true" : "false";
            return result;
        });

    return true;
}

static bool registerColumnResult = registerColumns();
