#include "FilesTable.h"

using namespace kapa::tarracsh::domain::db::digest;
using namespace kapa::infrastructure::db::tables::columns;
using namespace columns;

namespace kapa::tarracsh::domain::db::digest {

std::string FilesTable::createKey(const FileRow &row) const {
    auto result = createKey(row.filename);
    return result;
}

void FilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(id, infrastructure::db::tables::columns::StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(type, StorageType::Int32, DisplayAs::AsEntryType);
    DECLARE_COLUMN_PROP(filename, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(fileSize, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(classfileCount, StorageType::UInt32, DisplayAs::AsUInt32);
    DECLARE_COLUMN_PROP(digest, StorageType::Digest, DisplayAs::AsDigest);
}
}
