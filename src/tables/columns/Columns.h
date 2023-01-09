#ifndef TARRACSH_COLUMNS_H
#define TARRACSH_COLUMNS_H
#include "../../app/App.h"
#include "../../utils/DigestUtils.h"

#pragma pack( push, 1 )

namespace org::kapa::tarracsh::db::tables::columns {


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
    AsEntryType
};

inline std::string DisplayAsToString(const DisplayAs displayAs) {
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
  
    }

    return "unknown";
}

#define MAX_COLUMN_NAME 128


enum EntryType { Classfile, Jar, Directory };


struct Properties {
    char name[MAX_COLUMN_NAME]{};
    StorageType type{StorageType::UInt64};
    DisplayAs displayAs{DisplayAs::AsUInt64};

    Properties()
        : type(StorageType::UInt64),
          displayAs(DisplayAs::AsUInt64) {
        memset(name, 0, MAX_COLUMN_NAME);

    }

    Properties(const char *name, const StorageType type, const DisplayAs displayAs) {
        memset(this->name, 0, MAX_COLUMN_NAME);
        strcpy_s(this->name, name);
        this->type = type;
        this->displayAs = displayAs;
    }
};


struct RefCol {
    uint64_t id;
};

typedef uint64_t UInt64Col;
typedef int32_t Int32Col;
typedef uint32_t UInt32Col;


struct DigestCol {
    unsigned char buf[DIGEST_LENGTH]{};
    bool operator==(const DigestCol &right) const = default;

    bool operator==(const std::vector<unsigned char> &left) const {
        if (left.size() != DIGEST_LENGTH) return false;
        const auto result = memcmp(buf, &*left.begin(), DIGEST_LENGTH) == 0;
        return result;
    }

    DigestCol &operator=(const std::vector<unsigned char> &left) {
        assert(left.size() == DIGEST_LENGTH);
        memcpy(buf, &*left.begin(), DIGEST_LENGTH);
        return *this;
    }
};
}


#pragma pack(pop)

#endif
