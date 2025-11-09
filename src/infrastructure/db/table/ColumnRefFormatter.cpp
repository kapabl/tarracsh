#include "infrastructure/db/columns/Columns.h"
#include "infrastructure/db/table/Table.h"
#include "infrastructure/db/Database.h"

#include <fmt/format.h>

namespace kapa::infrastructure::db::table {

namespace {

struct RefColumnRegistrar {
    RefColumnRegistrar() {
        column::Properties::registerColumn(
            column::DisplayAs::AsRef,
            [](char *pValue,
               const column::Properties &properties,
               Database &db,
               bool displayRaw) -> std::string {
                const auto ref = reinterpret_cast<uint64_t &>(*pValue);
                if (displayRaw || ref == column::RefCol::InvalidRef) {
                    return fmt::format("{}", ref);
                }

                auto table = db.getTable(properties.refColProperties.table);
                if (table == nullptr) {
                    return fmt::format("{}", ref);
                }
                return fmt::format("{}", table->getColumnValue(ref, properties.refColProperties.displayColumn));
            });
    }
};

const RefColumnRegistrar kRegisterRefColumn{};

}  // namespace

}
