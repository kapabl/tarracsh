#include "CallGraphDb.h"


using namespace std;
using namespace kapa::tarracsh::domain::db::callgraph;


void CallGraphDb::init() {
    Database::init();
    //TODO
}

std::shared_ptr<CallGraphDb> CallGraphDb::create(
    const Config& config, const bool doClean) {

    std::shared_ptr<CallGraphDb> result;

    const auto db = std::make_shared<CallGraphDb>(config);

    if (Database::init(*result, doClean)) {
        result = db;
    }
    return result;

}
