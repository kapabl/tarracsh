#include "ClassFileReader.h"

using namespace kapa::tarracsh::domain::classfile::reader;


u2 ClassFileReader::readU2() {
    u2 result;
    read(result);
    return result;
}

u2 ClassFileReader::readU2Reversed() {
    u2 result;
    readReversed(result);
    return result;
}

u4 ClassFileReader::readU4() {
    u4 result;
    read(result);
    return result;
}

u1 ClassFileReader::readU1() {
    u1 result;
    readRaw(result);
    return result;
}
