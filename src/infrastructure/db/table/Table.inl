#include "Table.h"

namespace kapa::infrastructure::db::tables {


template <typename T>
void Table<T>::printSchema() {
    printLayout();
}

template <typename T>
void Table<T>::printLayout() {
    std::cout << std::endl;
    std::cout << std::format("table name: {}", _layout.header.name) << std::endl;
    std::cout << std::format("column count: {}", _layout.header.columnCount) << std::endl;
    std::cout << std::format("row count: {}", calculateRowCount()) << std::endl;
    std::cout << std::endl;
    std::cout << "columns:" << std::endl;
    auto index = 1u;
    for (auto &column : _columns) {
        std::cout << std::right
            << std::setw(7) << std::format("no: {}, ", index)
            << std::setw(25) << std::format("name: {}, ", column.name)
            << std::setw(25) << std::format("type: {}, ", StorageTypeToString(column.type))
            << std::setw(25) << std::format("display as: {}", DisplayAsToString(column.displayAs))
            << std::endl;
        index++;
    }
}
}
