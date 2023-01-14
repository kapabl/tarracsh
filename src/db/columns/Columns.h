#ifndef TARRACSH_COLUMNS_H
#define TARRACSH_COLUMNS_H
#include <cassert>
#include "../../infrastructure/db/columns/Columns.h"
#include "../../domain/utils/DigestUtils.h"

#pragma pack( push, 1 )

namespace kapa::tarracsh::db::digest::columns {


enum EntryType { Classfile, Jar, Directory };

struct DigestCol {
    unsigned char buf[DIGEST_LENGTH]{};
    bool operator==(const DigestCol &right) const = default;

    bool operator==(const std::vector<unsigned char> &left) const {
        if (left.size() != DIGEST_LENGTH) return false;
        const auto result = memcmp(buf, &*left.begin(), DIGEST_LENGTH) == 0;
        return result;
    }

    DigestCol &operator=(const std::vector<unsigned char> &left) {
        assert(left.size() == DIGEST_LENGTH);
        memcpy(buf, &*left.begin(), DIGEST_LENGTH);
        return *this;
    }
};
}


#pragma pack(pop)

#endif
