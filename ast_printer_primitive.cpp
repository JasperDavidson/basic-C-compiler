#include "ast.h"

#include <iostream>
#include <memory>

void print_func(std::unique_ptr<FunctionDecl> func) {
  // Print the header of the function
  std::cout << "FUNCTION " << type_to_string(func->return_type) << " "
            << func->name << ":\n";

  // Print the parameters of the function
  std::cout << "\tparams: (";

  for (int i = 0; i < func->parameters.size(); ++i) {
    std::string param_type = type_to_string(func->parameters[i]->type);
    std::string param_name = func->parameters[i]->name;

    std::cout << param_type << " " << param_name << ", ";
  }

  std::cout << ")\n";

  // Print the body of the function
  std::cout << "\tbody:\n";

  for (int i = 0; i < func->body.size(); ++i) {
  }
}
