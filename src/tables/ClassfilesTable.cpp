#include "ClassfilesTable.h"

using namespace org::kapa::tarracsh::tables;


void ClassfilesTable::updateClassnameIndex(const ClassfileRow *pBeforeRow,
    const ClassfileRow *pAfterRow) {
    if (pBeforeRow != nullptr) {
        const auto it = _classnameIndex.find(pBeforeRow->classname);
        if (it != _classnameIndex.end()) {
            it->second.erase(pBeforeRow);
        }
    }
    _classnameIndex[pAfterRow->classname].insert(pAfterRow);
}

void ClassfilesTable::updateDigestIndex(const ClassfileRow *pBeforeRow, 
    const ClassfileRow *pAfterRow) {
    if (pBeforeRow != nullptr) {
        const auto digestHexString = stringUtils::bytesToHexString(pBeforeRow->digest.buf, MD5_DIGEST_LENGTH);
        const auto it = _digestIndex.find(digestHexString);
        if (it != _digestIndex.end()) {
            it->second.erase(pBeforeRow);
        }
    }
    const auto digestHexString = stringUtils::bytesToHexString(pAfterRow->digest.buf, MD5_DIGEST_LENGTH);
    _digestIndex[digestHexString].insert(pAfterRow);
}
