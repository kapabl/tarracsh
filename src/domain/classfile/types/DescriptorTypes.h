#ifndef TARRACSH_DESCRIPTOR_TYPES_H
#define TARRACSH_DESCRIPTOR_TYPES_H

#include <string>
#include <vector>

#include "StructsCommon.h"

namespace kapa::tarracsh::domain::classfile::constantpool {

struct Descriptor {
    bool isArray{false};
    bool isClass{false};
    int dimensions{0};
    std::string type;

    Descriptor() = default;

    [[nodiscard]] std::string toString() const {
        std::string result(type);

        for (int i = 0; i < dimensions; i++) {
            result += "[]";
        }

        return result;
    }
};

struct MethodDescriptor {
    std::vector<Descriptor> arguments;
    Descriptor returnType;

    [[nodiscard]] std::string argumentsToString() const {
        std::string result("(");

        for (size_t i = 0; i < arguments.size(); ++i) {
            if (i != 0) {
                result.append(", ");
            }
            result.append(arguments[i].toString());
        }

        result.push_back(')');
        return result;
    }
};

}

#endif  // TARRACSH_DESCRIPTOR_TYPES_H
