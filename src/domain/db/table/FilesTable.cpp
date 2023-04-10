#include "FilesTable.h"

using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::infrastructure::db::table::column;
using namespace column;

using namespace kapa::tarracsh::domain::db::table;

FilesTable::FilesTable(infrastructure::db::Database &db, const std::string &name)
    : Table(db, name, sizeof(FileRow)) {
}

std::string FilesTable::createKey(const char *filename) {
    std::string result(filename);
    return result;
}

std::string FilesTable::createKey(const infrastructure::db::table::column::StringCol &filename) const {
    std::string result(createKey(_stringPool->getCString(filename)));
    return result;
}

std::string FilesTable::getKey(const infrastructure::db::table::AutoIncrementedRow *row) {
    std::string result = createKey(row);
    return result;
}

const char *FilesTable::getFilename(const FileRow *row) const {
    const auto result = _stringPool->getCString(row->filename);
    return result;
}

std::string FilesTable::createKey(const infrastructure::db::table::AutoIncrementedRow *row) const {
    const auto fileRow = static_cast<const FileRow *>(row);
    auto result = createKey(fileRow->filename);
    return result;
}

void FilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(FileRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(FileRow, type, StorageType::Int32, DisplayAs::AsEntryType);
    DECLARE_COLUMN_PROP(FileRow, filename, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(FileRow, lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(FileRow, fileSize, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(FileRow, classfileCount, StorageType::UInt32, DisplayAs::AsUInt32);
    DECLARE_COLUMN_PROP(FileRow, digest, StorageType::Digest, DisplayAs::AsDigest);

    Properties::registerColumn(
        DisplayAs::AsEntryType,
        [](char *pValue, const Properties &properties, kapa::infrastructure::db::Database &db,
           bool displayRaw) -> std::string {
            const auto entryType = static_cast<EntryType>(*reinterpret_cast<int *>(pValue));
            if (displayRaw) return std::format("{}", static_cast<int>(entryType));
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
