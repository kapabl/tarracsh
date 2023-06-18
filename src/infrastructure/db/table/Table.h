#ifndef KAPA_DB_TABLE_H
#define KAPA_DB_TABLE_H
#include <shared_mutex>
#include <unordered_map>
#include <filesystem>
#include <string>
#include <cstring>
#include "../Database.h"
#include "../StringPool.h"

#include "../DbConsts.h"
#include "../columns/Columns.h"

#pragma pack( push, 1 )

namespace kapa::infrastructure::db::table {


constexpr const char *TableExtension = ".kapamd";


struct Row {
};

struct AutoIncrementedRow : Row {
    column::UInt64Col id{ std::numeric_limits<uint64_t>::max() };
};


struct TableLayout {

    struct Header {
        uint64_t signature{KAPA_TABLE_SIGNATURE};
        char name[MAX_TABLE_NAME];
        uint64_t columnCount{0};

        Header() {
            memset(&name, 0, MAX_TABLE_NAME);
        }
    } header;

    column::Properties columns[1];

    TableLayout()
        : columns{} {

    }

};


#define DECLARE_COLUMN_PROP( rowType, name, storage, displayAs ) \
   Properties name##Prop( #name, storage, displayAs, offsetof( rowType, name )); \
    _columnByName[ #name ] =  _columns.size(); \
    _columns.push_back( name##Prop ); \
    _layout.header.columnCount = _columns.size()

#define DECLARE_FOREIGN_COLUMN_PROP( rowType, name, storage, displayAs, refTable, refColumn ) \
    Properties name##Prop( #name, storage, displayAs, offsetof( rowType, name ), refTable, refColumn); \
    _columnByName[ #name ] = _columns.size(); \
    _columns.push_back( name##Prop ); \
    _layout.header.columnCount = _columns.size()


class Table {

public:
    typedef unsigned char byte;

    explicit Table(db::Database &db, const std::string &name, uint64_t rowSize);
    virtual ~Table();

    [[nodiscard]] std::string getName() const;
    void init();

    [[nodiscard]] AutoIncrementedRow* get(std::string key) const;
    void update(AutoIncrementedRow *row);
    uint64_t addOrUpdate(AutoIncrementedRow *row);
    [[nodiscard]] const AutoIncrementedRow *getRow(uint64_t id);
    uint64_t add(AutoIncrementedRow *row);
    [[nodiscard]] bool write();
    bool clean();
    virtual void backup();
    bool read();
    [[nodiscard]] std::shared_ptr<StringPool> getStringPool() const;
    [[nodiscard]] AutoIncrementedRow *findByKey(const std::string &key);
    void printSchema();
    void list(const std::function<bool(AutoIncrementedRow&)>& filter, bool displayRaw );
    [[nodiscard]] uint64_t size() const;
    std::string getColumnValue(uint64_t id, const char* columnName);

    [[nodiscard]] AutoIncrementedRow* allocateRow() const;
    static void freeRow(AutoIncrementedRow* row);

    bool isValidColumn(const std::string& columnName) const;


protected:
    db::Database &_db;
    std::unordered_map<std::string, AutoIncrementedRow*> _rows;
    std::vector<AutoIncrementedRow *> _autoIncrementIndex;
    std::string _filename;
    bool _isDirty{false};
    uint64_t _rowSize;
    std::string _name;

    enum DirtyType {
        isUpdate,
        isNew
    };

    std::unordered_map<uint64_t, DirtyType> _dirtyRows;

    std::shared_mutex _mutex;
    const std::shared_ptr<StringPool> _stringPool;
    std::vector<column::Properties> _columns;
    std::unordered_map<std::string, int> _columnByName;
    TableLayout _layout;

    virtual void defineColumns() {
    }

    void forEachColumn(const std::function<void(const column::Properties&, const int index)> &func) const;

    virtual std::string getKey(const AutoIncrementedRow *row) = 0;

    [[nodiscard]] uint64_t getFileSize() const;
    [[nodiscard]] uint64_t getHeaderSize() const;
    [[nodiscard]] uint64_t calculateRowCount() const;

    void printLayout();
    void writeRows(FILE *file);
    void writeHeader(FILE *file);
    void writeSchema(FILE *file) const;
    bool readRows(FILE *file);
    bool readHeader(FILE *file);
    bool readSchema(FILE *file);
    void internalAdd(AutoIncrementedRow *row, const std::string& key);
    void internalUpdate(AutoIncrementedRow *row, const std::string &key);

    [[nodiscard]] std::string generateStdOutHeader() const;
    [[nodiscard]] std::string generateStdOutRow(AutoIncrementedRow *row, bool displayRaw) const;
};


}

#pragma pack(pop)

#endif
