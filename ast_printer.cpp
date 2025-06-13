#include "ast_printer.h"
#include "ast.h"

#include <iostream>
#include <string>

void AstPrinter::print_indent() {
    for (int i = 0; i < indentation; ++i) {
        std::cout << " ";
    }
}

void AstPrinter::visit(IntLiteralExpr *expr) {
    print_indent();
    std::cout << "(IntLiteralExpr " << std::to_string(expr->value) << ")";
}


void AstPrinter::visit(VariableExpr *expr) {
    print_indent();
    std::cout << "(VariableExpr " << expr->name << ")";
}

void AstPrinter::visit(ReturnStmt *stmt) {
    print_indent();
    std::cout << "(ReturnStmt\n";

    ++indentation;
    stmt->expression->accept(this);
    --indentation;

    std::cout << ")";
}

void AstPrinter::visit(VariableDecl *decl) {
    print_indent();
    std::cout << "(VariableDecl " << type_to_string(decl->type) << " " << decl->name << ")\n";
}

void AstPrinter::visit(FunctionDecl *decl) {
    print_indent();
    std::cout << "(FunctionDecl name=" << decl->name << ", return=" << type_to_string(decl->return_type) << ", parameters=";

    for (int i = 0; i < decl->parameters.size(); ++i) {
        std::cout << "(" << type_to_string(decl->parameters[i]->type) << " " << decl->parameters[i]->name << ")";
    }
    std::cout << '\n';

    ++indentation;
    for (int i = 0; i < decl->body.size(); ++i) {
        decl->body[i]->accept(this);
    }
    --indentation;

    std::cout << ")\n";
}
