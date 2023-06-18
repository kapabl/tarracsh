#ifndef KAPA_DB_QUERY_ENGINE
#define KAPA_DB_QUERY_ENGINE
#include <vector>
//#include <xstring>
#include <string>

#define TQ_SCHEMA "schema"
#define TQ_LIST "list"

namespace kapa::infrastructure::db {
class Database;
}

namespace kapa::infrastructure::db::query {

class Engine {
public:
    explicit Engine(Database& db);
    void log(const std::string &msg, bool doStdout = false) const;
    [[nodiscard]] auto execute(const std::string &query, bool displayRaw) const -> bool;

private:
    Database& _db;

};

}

#endif
