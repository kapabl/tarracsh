#ifndef KAPA_DB_COLUMNS_H
#define KAPA_DB_COLUMNS_H
#include <functional>
#include <string>
#include <limits>
#include <cstdint>
#include <unordered_map>
#include "src/infrastructure/db/DbConsts.h"

#pragma pack( push, 1 )

namespace kapa::infrastructure::db {
class Database;
}

namespace kapa::infrastructure::db::table::column {


enum StorageType {
    Int32,
    UInt32,
    UInt64,
    Ref,
    String,
    Digest
};

inline std::string StorageTypeToString(const StorageType storageType) {
    switch (storageType) {

        case Int32:
            return "signed 32 bits";
        case UInt32:
            return "unsigned 32 bits";
        case UInt64:
            return "unsigned 64 bits";
        case Ref:
            return "Ref";
        case String:
            return "String";
        case Digest:
            return "Digest";
    }

    return "unknown";
}

enum DisplayAs {
    AsInt32,
    AsUInt32,
    AsUInt64,
    AsRef,
    AsString,
    AsDigest,
    AsDatetime,
    AsSize,
    AsEntryType,
    AsBool,
    AsAccessFlags
};

std::string displayAsToString(const DisplayAs displayAs);



struct RefCol {
    const static uint64_t InvalidRef = std::numeric_limits<uint64_t>::max();
    uint64_t id{};

    [[nodiscard]] auto isInvalid() const {
        const bool result = id == InvalidRef;
        return result;
    }
};

struct RefColProperties {
    char table[MAX_TABLE_NAME];
    char displayColumn[MAX_COLUMN_NAME];
};

struct Properties {
    char name[MAX_COLUMN_NAME]{};
    StorageType type{StorageType::UInt64};
    DisplayAs displayAs{DisplayAs::AsUInt64};
    uint64_t offsetInRow{ 0 };
    RefColProperties refColProperties{};

    Properties();

    explicit Properties(const char *name, StorageType type, const DisplayAs displayAs, uint64_t offsetInRow);
    explicit Properties(const char *name, StorageType type, const DisplayAs displayAs, uint64_t offsetInRow,
        const char* refTable, const char* displayColumn );

    std::string valueToString(char *pValue, Database& db, bool displayRaw) const;

    typedef std::function < std::string(char* pValue, const Properties& properties, Database& db, bool displayRaw )> ToStringFunc;

    static void registerColumn(int displayAsValue, const ToStringFunc& func);
    static std::unordered_map<int, ToStringFunc > toStringMap;
};




typedef uint64_t UInt64Col;
typedef int64_t Int64Col;
typedef int32_t Int32Col;
typedef uint32_t UInt32Col;

#define SHA_256_DIGEST_LENGTH 40
#define DIGEST_LENGTH SHA_256_DIGEST_LENGTH

struct DigestCol {
    DigestCol();

    unsigned char buf[DIGEST_LENGTH]{};
    bool operator==(const DigestCol& right) const = default;

    bool operator==(const std::vector<unsigned char>& left) const;

    DigestCol& operator=(const std::vector<unsigned char>& left);
};

}


#pragma pack(pop)

#endif
