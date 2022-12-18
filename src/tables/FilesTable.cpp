#include "FilesTable.h"

using namespace org::kapa::tarracsh::db::tables;
using namespace columns;

void FilesTable::defineColumns() {
    DECLARE_COLUMN_PROP(id, StorageType::UInt64, DisplayAs::AsUInt64);
    DECLARE_COLUMN_PROP(type, StorageType::Int32, DisplayAs::AsEntryType);
    DECLARE_COLUMN_PROP(filename, StorageType::String, DisplayAs::AsString);
    DECLARE_COLUMN_PROP(lastWriteTime, StorageType::UInt64, DisplayAs::AsDatetime);
    DECLARE_COLUMN_PROP(fileSize, StorageType::UInt64, DisplayAs::AsSize);
    DECLARE_COLUMN_PROP(classfileCount, StorageType::UInt32, DisplayAs::AsUInt32);
    DECLARE_COLUMN_PROP(digest, StorageType::Digest, DisplayAs::AsDigest);
}
