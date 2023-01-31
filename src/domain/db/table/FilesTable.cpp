#include "FilesTable.h"
#include "../../../infrastructure/db/table/Table.inl"

using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::infrastructure::db::tables::columns;
using namespace columns;

namespace kapa::tarracsh::domain::db::digest {

FilesTable::FilesTable(infrastructure::db::Database &db, const std::string &tablename)
    : Table(db, tablename) {
}

std::string FilesTable::createKey(const char *filename) {
    std::string result(filename);
    return result;
}

std::string FilesTable::createKey(const infrastructure::db::tables::columns::StringCol &filename) const {
    std::string result(createKey(_stringPool->getCString(filename)));
    return result;
}

std::string FilesTable::getKey(const FileRow &row) {
    return createKey(row);
}

const char *FilesTable::getFilename(const FileRow &row) const {
    const auto result = _stringPool->getCString(row.filename);
    return result;
}

std::string FilesTable::createKey(const FileRow &row) const {
    auto result = createKey(row.filename);
    return result;
}

void FilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(type, StorageType::Int32, DisplayAs::AsEntryType);
    DECLARE_COLUMN_PROP(filename, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(fileSize, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(classfileCount, StorageType::UInt32, DisplayAs::AsUInt32);
    DECLARE_COLUMN_PROP(digest, StorageType::Digest, DisplayAs::AsDigest);

    Properties::registerColumn(
        DisplayAs::AsEntryType,
        [](char *pValue, Properties &properties, kapa::infrastructure::db::Database &db) -> std::string {
            const auto entryType = static_cast<EntryType>(*reinterpret_cast<int *>(pValue));
            switch (entryType) {
                case Classfile:
                    return "classfile";
                case Jar:
                    return "jar";
                case Directory:
                    return "directory";
            }
            return "unknown";

        });
}
}
