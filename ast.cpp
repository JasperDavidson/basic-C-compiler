#include "ast.h"

#include <stdexcept>
#include <string>

std::string type_to_string(VariableType variable_type) {
  switch (variable_type) {
  case VariableType::INT:
    return "int";
  case VariableType::VOID:
    return "void";
  }

  throw std::runtime_error("Invalid type to convert to string");
}
