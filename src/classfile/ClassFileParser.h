//
// Created by xman on 6/24/2022.
//

#ifndef TARRASH_CLASSFILEPARSER_H
#define TARRASH_CLASSFILEPARSER_H

#include <bit>
#include "ClassFileStructure.h"
#include "ConstantPool.h"


class ClassFileParser {

public:
    ClassFileParser(string fileName) : _fileName(fileName), _bytesRead(0) {
        processFile();
    }

    void output() {
        //TODO
    }

private:

    std::string _fileName;
    bool _isBigEndian;

    ClassFileHeader _header;
    ConstantPool _constantPool;
    ClassFileMid _mid;

    unsigned int _fileSize;
    unsigned int _bytesRead;

    ifstream _file;

    u2 swapShort(u2 value) {
        const u2 result = value >> 8 | value << 8;
        return result;
    }

    u4 swapLong(u4 value) {
        const u4 result =
                value >> 24 |
                (value << 8 & 0x00FF0000) |
                (value >> 8 & 0x0000FF00) |
                value << 24;
        return result;

    }

    template<typename T>
    void readRaw(T &buffer, unsigned int byteCount) {

        assert(_bytesRead + byteCount <= _fileSize);
        char *charBuffer = reinterpret_cast<char *>(&buffer);

        _file.read(charBuffer, byteCount);

        _bytesRead += byteCount;
    }

    template<typename T>
    void readRaw(T &buffer) {
        readRaw(buffer, sizeof(buffer));
    }

    template<typename T>
    void read(T &buffer, unsigned int byteCount) {

        readRaw(buffer, byteCount);
        if (!_isBigEndian) {
            switch (byteCount) {
                case 2:
                    buffer = swapShort(buffer);
                    break;
                case 4:
                    buffer = swapLong(buffer);
                    break;
                default:
                    break;
            }
        }
    }

    template<typename T>
    void read(T &buffer) { read(buffer, sizeof(T)); }

    void readHeader() {
        readRaw(_header, sizeof(ClassFileHeader));
        _isBigEndian = _header.magic == 0x0cafebabe;
        if (!_isBigEndian && _header.magic != swapLong(0x0cafebabe)) {
            cout << "Invalid class file " << _fileName << endl;
            exit(1);
        }
    }


    void readConstPoolRecord() {
        ConstantPoolTag tag;
        readRaw(tag, 1);
        //_constantPool.startRecord(tag);

        u1 byte;
        u2 twoBytes;
        u4 fourBytes;
        switch (tag) {
            case JVM_CONSTANT_Utf8: {
                u2 length;
                read( length );
                auto recordSize = sizeof(CONSTANT_Utf8_info) + length;
                CONSTANT_Utf8_info &utf8Info = *reinterpret_cast<CONSTANT_Utf8_info*>(malloc( recordSize ));
                utf8Info.tag = tag;
                utf8Info.length = length;
                u1 *data = reinterpret_cast<u1 *>(utf8Info.bytes);
                readRaw(*data, utf8Info.length);
                utf8Info.bytes[ length ] = 0;
                _constantPool.addRecord( utf8Info, recordSize );

                free( &utf8Info );

                break;
            }

            case JVM_CONSTANT_Float:
            case JVM_CONSTANT_Integer: {
                CONSTANT_Integer_info integerInfo {tag};
                read(integerInfo.value);
                _constantPool.addRecord(integerInfo);
                break;
            }

            case JVM_CONSTANT_Long:
            case JVM_CONSTANT_Double: {
                CONSTANT_Long_info longInfo {tag};
                read(longInfo.value);
                _constantPool.addRecord(longInfo);
                break;
            }

            case JVM_CONSTANT_String:
            case JVM_CONSTANT_MethodType:
            case JVM_CONSTANT_Class: {
                CONSTANT_Class_info classInfo {tag};
                read(classInfo.nameIndex);
                _constantPool.addRecord(classInfo);
                break;
            }

            case JVM_CONSTANT_NameAndType:
            case JVM_CONSTANT_Methodref:
            case JVM_CONSTANT_Fieldref:
            case JVM_CONSTANT_InterfaceMethodref: {
                MemberInfo memberInfo {tag};
                read(memberInfo.classIndex);
                read(memberInfo.nameAndTypeIndex);
                _constantPool.addRecord(memberInfo);
                break;
            }

            case JVM_CONSTANT_MethodHandle: {
                CONSTANT_MethodHandle_info methodHandleInfo {tag};
                read(methodHandleInfo.referenceKind);
                read(methodHandleInfo.referenceIndex);
                _constantPool.addRecord(methodHandleInfo);
                break;
            }

            case JVM_CONSTANT_InvokeDynamic: {
                CONSTANT_InvokeDynamic_info invokeDynamicInfo {tag};
                read(invokeDynamicInfo.bootstrapMethodAttrIndex);
                read(invokeDynamicInfo.nameAndTypeIndex);
                _constantPool.addRecord(invokeDynamicInfo);
                break;
            }

                //case JVM_CONSTANT_ExternalMax:
            case JVM_CONSTANT_Package:
            case JVM_CONSTANT_Dynamic:
            case JVM_CONSTANT_Module:
            case JVM_CONSTANT_Unicode:
                //TODO newer java version
                break;

            default:
                assert(false);
                break;
        }
    }

    void readConstantsPool() {
        u2 count;
        read(count);
        _constantPool.setCount(count);
        int index = 0;
        while (index + 1 < count) {
            readConstPoolRecord();
            index++;
        }
        _constantPool.relocate();
    }


    void processFile() {

        _fileSize = file_size(_fileName);

        _file.open(_fileName, ifstream::binary);
        readHeader();
        readConstantsPool();
        //TODO

        _file.close();


    }

};


#endif //TARRASH_CLASSFILEPARSER_H
