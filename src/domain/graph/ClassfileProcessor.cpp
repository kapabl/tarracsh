
#include "ClassFileProcessor.h"

#include "domain/db/table/ClassRefs.h"
#include "domain/db/table/Methods.h"
#include "domain/db/table/MethodRefs.h"
#include "domain/db/table/Fields.h"
#include "domain/db/table/FieldRefs.h"
#include "infrastructure/filesystem/Utils.h"


using namespace kapa::tarracsh::domain::graph;

using kapa::tarracsh::domain::classfile::ClassFileParser;
using kapa::tarracsh::domain::classfile::constantpool::ConstantPoolRecord;
using kapa::tarracsh::domain::classfile::constantpool::NameAndTypeInfo;
using kapa::tarracsh::domain::classfile::constantpool::u2;
using kapa::tarracsh::domain::db::table::MethodRow;
using kapa::tarracsh::domain::db::table::MethodRefRow;
using kapa::tarracsh::domain::db::table::Files;
using kapa::tarracsh::domain::db::table::FieldRow;
using kapa::tarracsh::domain::db::table::FieldRefRow;
using kapa::tarracsh::domain::db::table::ClassRefRow;
using kapa::tarracsh::domain::db::table::ClassFileRow;
using kapa::tarracsh::domain::db::callgraph::CallGraphDb;

ClassFileProcessor::ClassFileProcessor(
        const ClassFileRow *row, ClassFileParser &parser, CallGraphDb &db) :
        _row(row),
        _parser(parser),
        _db(db),
        _constantPool(parser.getConstantPool()) {

}

void ClassFileProcessor::extractNodes() {
    recordMethods();
    recordFields();
    recordRefs();
}

void ClassFileProcessor::recordFields() {
    const auto fieldTable = _db.getFields();
    for (auto &fieldInfo: _parser.getFields()) {
        auto &fieldRow = *static_cast<db::table::FieldRow *>(fieldTable->allocateRow());
        new(&fieldRow) FieldRow(*_row);
        fieldRow.name = _db.getPoolString(_constantPool.getString(fieldInfo.nameIndex));
        fieldRow.descriptor = _db.getPoolString(_constantPool.getString(fieldInfo.descriptorIndex));
        fieldTable->addOrUpdate(&fieldRow);
    }
}

void ClassFileProcessor::recordMethods() {
    const auto methodTable = _db.getMethods();
    for (auto &methodInfo: _parser.getMethods()) {
        auto &methodRow = *static_cast<db::table::MethodRow *>(methodTable->allocateRow());
        new(&methodRow) MethodRow(*_row);
        methodRow.name = _db.getPoolString(_constantPool.getString(methodInfo.nameIndex));
        methodRow.descriptor = _db.getPoolString(_constantPool.getString(methodInfo.descriptorIndex));
        methodTable->addOrUpdate(&methodRow);
    }
}

void ClassFileProcessor::recordClassRef(const ConstantPoolRecord &entry) {
    const auto classRefs = _db.getClassRefs();
    auto &classRefRow = *static_cast<db::table::ClassRefRow *>(classRefs->allocateRow());
    new(&classRefRow) ClassRefRow(*_row);
    classRefRow.name = _db.getPoolString(_constantPool.getString(entry.classInfo.nameIndex));
    classRefs->addOrUpdate(&classRefRow);
}

void ClassFileProcessor::commonRecordMethodRef(const u2 nameAndTypeIndex) {
    auto nameAndTypeInfo = _constantPool.getEntry(nameAndTypeIndex).nameAndTypeInfo;
    const auto methodRefs = _db.getMethodRefs();
    auto &methodRefRow = *static_cast<db::table::MethodRefRow *>(methodRefs->allocateRow());
    new(&methodRefRow) MethodRefRow(*_row);

    methodRefRow.name = _db.getPoolString(_constantPool.getString(nameAndTypeInfo.nameIndex));
    methodRefRow.descriptor = _db.getPoolString(_constantPool.getString(nameAndTypeInfo.descriptorIndex));
    methodRefs->addOrUpdate(&methodRefRow);
}


void ClassFileProcessor::recordMethodRef(const ConstantPoolRecord &entry) {
    commonRecordMethodRef(entry.methodrefInfo.nameAndTypeIndex);
}

void ClassFileProcessor::recordInterfaceMethodRef(const ConstantPoolRecord &entry) {
    commonRecordMethodRef(entry.interfaceMethodrefInfo.nameAndTypeIndex);
}

void ClassFileProcessor::recordFieldRef(const ConstantPoolRecord &entry) {
    const auto fieldRefs = _db.getFieldRefs();
    auto &fieldRefRow = *static_cast<db::table::FieldRefRow *>(fieldRefs->allocateRow());
    new(&fieldRefRow) FieldRefRow(*_row);

    auto nameAndTypeInfo = _constantPool.getEntry(entry.fieldrefInfo.nameAndTypeIndex).nameAndTypeInfo;
    fieldRefRow.name = _db.getPoolString(_constantPool.getString(nameAndTypeInfo.nameIndex));
    fieldRefRow.descriptor = _db.getPoolString(_constantPool.getString(nameAndTypeInfo.descriptorIndex));

    fieldRefs->addOrUpdate(&fieldRefRow);
}

void ClassFileProcessor::recordRefs() {

    for (u2 index = 1u; index < _constantPool.getPoolSize(); index = _constantPool.getNextIndex(index)) {
        const auto &entry = _constantPool.getEntry(index);
        if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Methodref) {
            recordMethodRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Class) {
            recordClassRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_InterfaceMethodref) {
            recordInterfaceMethodRef(entry);
        } else if (entry.base.tag == ConstantPoolTag::JVM_CONSTANT_Fieldref) {
            recordFieldRef(entry);
        }
    }
}
