#include "ClassfilesTable.h"

using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::infrastructure::db::tables::columns;
using kapa::infrastructure::db::tables::AutoIncrementedRow;
using namespace columns;


ClassfilesTable::ClassfilesTable(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<FilesTable> filesTable): Table(db, tablename, sizeof(ClassfileRow)), _filesTable(std::move(filesTable)) {
}

std::string ClassfilesTable::getKey(const AutoIncrementedRow* row) {
    return getStrongClassname(reinterpret_cast<const ClassfileRow&>(*row));
}

std::string ClassfilesTable::getStrongClassname(const ClassfileRow &row) const {
    const auto fileRow = reinterpret_cast<const FileRow&>(*_filesTable->getRow(row.file.id));

    auto result = getStrongClassname(fileRow, _stringPool->getCString(row.classname));
    return result;
}

std::string ClassfilesTable::getStrongClassname(const FileRow &fileRow, const char *classname) const {
    std::string result(digestUtils::getStrongClassname(_filesTable->getFilename(&fileRow), classname));
    return result;
}

void ClassfilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(ClassfileRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(ClassfileRow, file, StorageType::Ref, DisplayAs::AsRef, _filesTable->getName().c_str(), "filename");
    DECLARE_COLUMN_PROP(ClassfileRow, classname, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(ClassfileRow, package, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(ClassfileRow, lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(ClassfileRow, size, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(ClassfileRow, crc, StorageType::Int32, DisplayAs::AsInt32);
    DECLARE_COLUMN_PROP(ClassfileRow, digest, StorageType::Digest, DisplayAs::AsDigest);

}
