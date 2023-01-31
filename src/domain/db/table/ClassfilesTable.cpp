#include "ClassfilesTable.h"
#include "../../../infrastructure/db/table/Table.inl"

using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::infrastructure::db::tables::columns;
using namespace columns;


ClassfilesTable::ClassfilesTable(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<FilesTable> filesTable): Table(db, tablename), _filesTable(std::move(filesTable)) {
}

std::string ClassfilesTable::getKey(const ClassfileRow &row) {
    return getStrongClassname(row);
}

std::string ClassfilesTable::getStrongClassname(const ClassfileRow &row) const {
    const auto pFileRow = _filesTable->getRow(row.file.id);

    auto result = getStrongClassname(*pFileRow, _stringPool->getCString(row.classname));
    return result;
}

std::string ClassfilesTable::getStrongClassname(const FileRow &fileRow, const char *classname) const {
    std::string result(digestUtils::getStrongClassname(_filesTable->getFilename(fileRow), classname));
    return result;
}

void ClassfilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(file, StorageType::Ref, DisplayAs::AsRef);
    DECLARE_COLUMN_PROP(classname, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(package, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(size, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(crc, StorageType::Int32, DisplayAs::AsInt32);
    DECLARE_COLUMN_PROP(digest, StorageType::Digest, DisplayAs::AsDigest);
}
