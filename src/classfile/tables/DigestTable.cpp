#include "DigestTable.h"

using namespace org::kapa::tarracsh::tables;

void DigestTable::updateJarIndex(const DigestRow* pBeforeRow,
    const DigestRow * pAfterRow) {
    if ( pBeforeRow != nullptr ) {
        const auto it = _jarIndex.find(pBeforeRow->filename);
        if ( it != _jarIndex.end() ) {
            it->second.erase(pBeforeRow);
        }
    }
    _jarIndex[pAfterRow->filename].insert(pAfterRow);
}

void DigestTable::updateClassnameIndex(const DigestRow *pBeforeRow,
    const DigestRow *pAfterRow) {
    if (pBeforeRow != nullptr) {
        const auto it = _classnameIndex.find(pBeforeRow->classname);
        if (it != _classnameIndex.end()) {
            it->second.erase(pBeforeRow);
        }
    }
    _classnameIndex[pAfterRow->classname].insert(pAfterRow);
}

void DigestTable::updateDigestIndex(const DigestRow *pBeforeRow, 
    const DigestRow *pAfterRow) {
    if (pBeforeRow != nullptr) {
        const auto digestHexString = stringUtils::bytesToHexString(pBeforeRow->md5.buf, MD5_DIGEST_LENGTH);
        const auto it = _digestIndex.find(digestHexString);
        if (it != _digestIndex.end()) {
            it->second.erase(pBeforeRow);
        }
    }
    const auto digestHexString = stringUtils::bytesToHexString(pAfterRow->md5.buf, MD5_DIGEST_LENGTH);
    _digestIndex[digestHexString].insert(pAfterRow);
}
