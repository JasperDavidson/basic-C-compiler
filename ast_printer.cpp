#include "ast.h"

#include <iostream>
#include <memory>

void print_ast(std::unique_ptr<FunctionDecl> main_func) {
    // Always int since we don't gather return type yet
    std::cout << "FUNCTION INT: " << main_func->name;
}
