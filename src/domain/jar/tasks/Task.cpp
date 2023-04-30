#include "Task.h"


using namespace  kapa::tarracsh::domain::jar;
using kapa::tarracsh::domain::db::table::FileRow;
using kapa::tarracsh::domain::db::digest::column::EntryType;

Task::Task(Options options, stats::Results &results):
    _results(results),
    _options(std::move(options)) {
}

Task::~Task() {
}




