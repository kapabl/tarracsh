#ifndef TARRACSH_DB_QUERY_API_H
#define TARRACSH_DB_QUERY_API_H

#include <string>

namespace kapa::infrastructure::db {
class Database;
}

namespace kapa::infrastructure::db::query {

bool ExecuteQuery(Database &db, const std::string &query, bool displayRaw);

}

#endif
