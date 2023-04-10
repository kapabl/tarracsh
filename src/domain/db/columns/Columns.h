#ifndef TARRACSH_COLUMNS_H
#define TARRACSH_COLUMNS_H
#include <cassert>
#include "infrastructure/db/columns/Columns.h"
#include "../../digest/DigestUtils.h"

#pragma pack( push, 1 )

namespace kapa::tarracsh::domain::db::digest::column {


enum EntryType { Classfile, Jar, Directory };

}

//TODO register column


#pragma pack(pop)

#endif

