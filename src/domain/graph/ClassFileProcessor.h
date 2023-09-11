

#ifndef TARRACSH_EXTRACTCLASSFILENODES_H
#define TARRACSH_EXTRACTCLASSFILENODES_H

#include "domain/db/CallGraphDb.h"
#include "domain/db/table/ClassFiles.h"
#include "domain/classfile/ClassFileParser.h"

namespace kapa::tarracsh::domain::graph {


class ClassFileProcessor {

public:
    explicit ClassFileProcessor(const db::table::ClassFileRow *row,
                                classfile::ClassFileParser &parser,
                                db::callgraph::CallGraphDb &db);

    void extractNodes();

private:
    const db::table::ClassFileRow *_row;
    db::callgraph::CallGraphDb &_db;
    classfile::ClassFileParser &_parser;
    classfile::constantpool::ConstantPool& _constantPool;

    void recordMethods();
    void recordFields();
    void recordMethodRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordClassRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordInterfaceMethodRef(const classfile::constantpool::ConstantPoolRecord& entry);
    void recordFieldRef(const classfile::constantpool::ConstantPoolRecord & entry);
    void recordRefs();

    void commonRecordMethodRef(classfile::constantpool::u2 nameAndTypeIndex);
};

}

#endif
