#include "ClassFiles.h"
#include "domain/Utils.h"

using namespace kapa::tarracsh::domain::db::table;
using namespace kapa::infrastructure::db::table::column;
using kapa::infrastructure::db::table::AutoIncrementedRow;


ClassFiles::ClassFiles(infrastructure::db::Database &db, const std::string &tablename,
                       std::shared_ptr<Files> filesTable): Table(db, tablename, sizeof(ClassFileRow)), _filesTable(std::move(filesTable)) {
}

auto ClassFiles::getClassFileRow( uint64_t rowId ) const -> ClassFileRow* {
    return reinterpret_cast<ClassFileRow*>(getRow(rowId));
}

std::string ClassFiles::getKey(const AutoIncrementedRow* row) {
    return getStrongClassname(reinterpret_cast<const ClassFileRow&>(*row));
}

std::string ClassFiles::getStrongClassname(const uint64_t id) {
    const auto row = reinterpret_cast<const ClassFileRow&>(*getRow(id));
    auto result = getStrongClassname(row);
    return result;
}

std::string ClassFiles::getStrongClassname(const ClassFileRow &row) const {
    const auto fileRow = reinterpret_cast<const FileRow&>(*_filesTable->getRow(row.file.id));

    auto result = getStrongClassname(fileRow, _stringPool->getCString(row.classname));
    return result;
}

std::string ClassFiles::getClassname(const ClassFileRow &row) const {
    //TODO maybe we should use a string_view for performance
    auto result = _stringPool->getCString(row.classname);
    return result;
}

std::string ClassFiles::getStrongClassname(const FileRow &fileRow, const char *classname) const {
    std::string result(utils::getStrongClassname(_filesTable->getFilename(&fileRow), classname));
    return result;
}

void ClassFiles::defineColumns() {
    DECLARE_COLUMN_PROP(ClassFileRow, id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_FOREIGN_COLUMN_PROP(ClassFileRow, file, StorageType::Ref, DisplayAs::AsRef, _filesTable->getName().c_str(), "filename");
    DECLARE_COLUMN_PROP(ClassFileRow, classname, StorageType::String, DisplayAs::AsString);
//    DECLARE_COLUMN_PROP(ClassFileRow, package, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(ClassFileRow, lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(ClassFileRow, size, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(ClassFileRow, crc, StorageType::Int32, DisplayAs::AsInt32);
    DECLARE_COLUMN_PROP(ClassFileRow, digest, StorageType::Digest, DisplayAs::AsDigest);
    DECLARE_COLUMN_PROP(ClassFileRow, refCount, StorageType::UInt64, DisplayAs::AsUInt64);

}
