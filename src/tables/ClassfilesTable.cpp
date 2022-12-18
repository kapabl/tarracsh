#include "ClassfilesTable.h"

using namespace org::kapa::tarracsh::db::tables;
using namespace columns;




/*
 *     DECLARE_ROW_VALUE(Ref, file)
    DECLARE_ROW_VALUE(String, classname)
    DECLARE_ROW_VALUE(String, package)
    DECLARE_ROW_VALUE(UInt64, lastWriteTime)
    DECLARE_ROW_VALUE(UInt64, size)
    DECLARE_ROW_VALUE(Int32, crc)
    DECLARE_ROW_VALUE(Digest, digest)
 */
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
