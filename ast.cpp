#include "ast.h"

VariableType parse_type(const std::string& type) {
    if (type == "int") {
        return VariableType::INT;
    }

    throw std::runtime_error("Invalid type while parsing the program: " + type);
};
