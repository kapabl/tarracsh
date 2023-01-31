#ifndef KAPA_DB_TABLE_H
#define KAPA_DB_TABLE_H
#include <optional>
#include <shared_mutex>
#include <unordered_set>
#include <unordered_map>
#include <filesystem>
#include <string>
#include "../Database.h"
#include "../StringPool.h"

#include "../columns/Columns.h"

#pragma pack( push, 1 )

namespace kapa::infrastructure::db::tables {


constexpr const char *TableExtension = ".kapamd";


struct Row {
};

struct AutoIncrementedRow : Row {
    columns::UInt64Col id{-1ull};
};

#define KAPA_TABLE_SIGNATURE 0xca5ab1a9ca
#define MAX_TABLE_NAME 128

struct TableLayout {

    struct Header {
        uint64_t signature{KAPA_TABLE_SIGNATURE};
        char name[MAX_TABLE_NAME];
        int columnCount{0};

        Header() {
            memset(&name, 0, MAX_TABLE_NAME);
        }
    } header;

    columns::Properties columns[1];

    TableLayout()
        : columns{} {

    }

};


#define DECLARE_COLUMN_PROP( name, storage, displayAs ) \
    const Properties name##Prop( #name, storage, displayAs, offsetof( RowType, name )); \
    _columns.push_back( name##Prop ); \
    _layout.header.columnCount = _columns.size()


template <typename T>
class Table {

public:

    typedef T RowType;

    explicit Table(db::Database &db, const std::string &tablename);
    virtual ~Table() = default;

    void init();

    [[nodiscard]] std::optional<T> get(std::string key) const;
    void update(T &row);
    [[nodiscard]] uint64_t addOrUpdate(T &row);
    [[nodiscard]] const T *getRow(uint64_t id);
    [[nodiscard]] uint64_t add(T &row);
    [[nodiscard]] bool write();
    bool clean();
    virtual void backup();
    bool read();
    [[nodiscard]] std::shared_ptr<StringPool> getStringPool() const;
    [[nodiscard]] const T *findByKey(const std::string &key);
    void printSchema();
    void list();
    [[nodiscard]] uint64_t size() { return _rows.size(); }

    enum {
        RowSize = sizeof(T)
    };


protected:
    db::Database &_db;
    std::unordered_map<std::string, T> _rows;
    std::vector<T *> _autoIncrementIndex;
    std::unordered_map<const T *, uint64_t> _reverseAutoincrementIndex;
    std::string _filename;
    bool _isDirty{false};

    enum DirtyType {
        isUpdate,
        isNew
    };
    std::unordered_map<uint64_t, DirtyType> _dirtyRows;
   
    std::shared_mutex _mutex;
    const std::shared_ptr<StringPool> _stringPool;
    std::vector<columns::Properties> _columns;
    TableLayout _layout;

    virtual void defineColumns() {}
    virtual std::string getKey(const T& row) = 0;

    [[nodiscard]] uint64_t getFileSize() const;
    [[nodiscard]] uint64_t getHeaderSize() const;
    [[nodiscard]] uint64_t calculateRowCount() const;

    void printLayout();
    void writeRows(FILE* file);
    void writeHeader(FILE* file);
    void writeSchema(FILE* file) const;
    bool readRows(FILE* file);
    bool readHeader(FILE* file);
    bool readSchema(FILE* file);
    void internalAdd(T &row, const std::string &key);
    void internalUpdate(T &row, const std::string &key);

    std::string generateStdOutHeader() const;
    std::string generateStdOutRow(T* pRow );
};



}

#pragma pack(pop)

#endif
