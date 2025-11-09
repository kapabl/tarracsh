#include "QueryApi.h"

#include "infrastructure/db/Database.h"
#include "infrastructure/db/query/Engine.h"

namespace kapa::infrastructure::db::query {

bool ExecuteQuery(Database &db, const std::string &query, const bool displayRaw) {
    Engine engine(db);
    return engine.execute(query, displayRaw);
}

}
