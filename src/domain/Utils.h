#ifndef TARRACSH_DOMAIN_UTILS_H
#define TARRACSH_DOMAIN_UTILS_H

#include <string>
#include <locale>



#include "infrastructure/string/StringUtils.h"
#include "infrastructure/db/columns/Columns.h"

namespace kapa::tarracsh::domain::utils {


[[nodiscard]] inline std::string getStrongClassname(const char *filename, const char *jvmClassname) {
    std::string result(filename);

    if (jvmClassname != nullptr && jvmClassname[0] != 0) {
        result += "@";
        result += jvmClassname;
    }

    return result;
}

[[nodiscard]] inline std::vector<std::string> splitStrongClassname(const std::string &strongClassname) {
    auto result = kapa::infrastructure::string::stringUtils::split(strongClassname, "@");
    return result;
}

}

#endif
