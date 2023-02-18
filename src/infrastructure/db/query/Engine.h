#ifndef KAPA_DB_QUERY_ENGINE
#define KAPA_DB_QUERY_ENGINE
#include <vector>
#include <xstring>

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
    void executeListQuery(const std::vector<std::string> &parts, const bool displayRaw) const;
    bool execute(const std::string &query, const bool displayRaw);

private:
    Database& _db;
    void list(const std::string& tablename, bool displayRaw) const;

};

}

#endif
