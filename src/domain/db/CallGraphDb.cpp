#include "CallGraphDb.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::callgraph;


void CallGraphDb::init() {
    Database::init();
    //TODO
}

void CallGraphDb::clean() {
    Database::clean();
    //TODO
}

bool CallGraphDb::read() {
    return Database::read();
    //TODO
}

bool CallGraphDb::write() {
    return Database::write();
    //TODO
}

void CallGraphDb::printSchema() {
    //TODO
}

std::shared_ptr<CallGraphDb> CallGraphDb::create(
    const std::string& dataDir,
    infrastructure::log::Log& log, const bool doClean) {

    std::shared_ptr<CallGraphDb> result;

    const auto db = std::make_shared<CallGraphDb>(dataDir, log);

    if (Database::init(*result, doClean)) {
        result = db;
    }
    return result;

}
