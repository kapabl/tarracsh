#include "ClassfilesTable.h"

using namespace kapa::tarracsh::db::digest;
using namespace kapa::infrastructure::db::tables::columns;
using namespace columns;


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
