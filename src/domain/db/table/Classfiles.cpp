#include "Classfiles.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


Classfiles::Classfiles(infrastructure::db::Database &db, const std::string &tablename,
    std::shared_ptr<Files> filesTable): Table(db, tablename, sizeof(ClassfileRow)), _filesTable(std::move(filesTable)) {
}



std::string Classfiles::getKey(const AutoIncrementedRow* row) {
    return getStrongClassname(reinterpret_cast<const ClassfileRow&>(*row));
}

std::string Classfiles::getStrongClassname(const uint64_t id) {
    const auto row = reinterpret_cast<const ClassfileRow&>(*getRow(id));
    auto result = getStrongClassname(row);
    return result;
}

std::string Classfiles::getStrongClassname(const ClassfileRow &row) const {
    const auto fileRow = reinterpret_cast<const FileRow&>(*_filesTable->getRow(row.file.id));

    auto result = getStrongClassname(fileRow, _stringPool->getCString(row.classname));
    return result;
}

std::string Classfiles::getStrongClassname(const FileRow &fileRow, const char *classname) const {
    std::string result(digestUtils::getStrongClassname(_filesTable->getFilename(&fileRow), classname));
    return result;
}

void Classfiles::defineColumns() {
    DECLARE_COLUMN_PROP(ClassfileRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(ClassfileRow, file, StorageType::Ref, DisplayAs::AsRef, _filesTable->getName().c_str(), "filename");
    DECLARE_COLUMN_PROP(ClassfileRow, classname, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(ClassfileRow, package, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(ClassfileRow, lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(ClassfileRow, size, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(ClassfileRow, crc, StorageType::Int32, DisplayAs::AsInt32);
    DECLARE_COLUMN_PROP(ClassfileRow, digest, StorageType::Digest, DisplayAs::AsDigest);

}
