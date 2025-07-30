#include "ast_printer.h"
#include "ast.h"

#include <cstddef>
#include <iostream>
#include <stdexcept>
#include <string>

void AstPrinter::print_indent() {
  for (int i = 0; i < indentation; ++i) {
    std::cout << " ";
  }
}

void AstPrinter::visit(const IntLiteralExpr *expr) {
  print_indent();
  std::cout << " IntLiteralExpr " << std::to_string(expr->value);
}

void AstPrinter::visit(const VariableExpr *expr) {
  print_indent();
  std::cout << " VariableExpr " << expr->name;
}

void AstPrinter::visit(const UnaryOpExpr *expr) {
  switch (expr->op) {
  case OperationType::NEGATE:
    std::cout << "Negate";
    break;
  case OperationType::BITWISE:
    std::cout << "Bitwise";
    break;
  case OperationType::LOGIC_NEGATE:
    std::cout << "Logical Negation";
    break;
  default:
    throw std::runtime_error("Expected a unary operation");
  }

  expr->expr->accept(this);
}

void AstPrinter::visit(const BinaryOpExpr *expr) {
  expr->expr_one->accept(this);

  switch (expr->op) {
  case OperationType::ADD:
    std::cout << " Add ";
    break;
  case OperationType::NEGATE:
    std::cout << " Subtract ";
    break;
  case OperationType::MULT:
    std::cout << " Multiply ";
    break;
  case OperationType::DIVIDE:
    std::cout << " Divide ";
    break;
  case OperationType::AND:
    std::cout << " And ";
    break;
  case OperationType::OR:
    std::cout << " Or ";
    break;
  case OperationType::EQUAL:
    std::cout << " Equal ";
    break;
  case OperationType::NOT_EQUAL:
    std::cout << " Not Equal ";
    break;
  case OperationType::LESS_THAN:
    std::cout << " Less Than ";
    break;
  case OperationType::LESS_THAN_EQUAL:
    std::cout << " Less Than or Equa l";
    break;
  case OperationType::GREATER_THAN:
    std::cout << " Greater Than ";
    break;
  case OperationType::GREATER_THAN_EQUAL:
    std::cout << " Greater Than or Equal ";
    break;
  case OperationType::MODULO:
    std::cout << " Modulo ";
    break;
  case OperationType::BITWISE_AND:
    std::cout << " Bitwise And ";
    break;
  case OperationType::BITWISE_OR:
    std::cout << " Bitwise Or ";
    break;
  case OperationType::BITWISE_XOR:
    std::cout << " Bitwise Xor ";
    break;
  case OperationType::BITWISE_SHIFT_LEFT:
    std::cout << " Bitwise Shift Left ";
    break;
  case OperationType::BITWISE_SHIFT_RIGHT:
    std::cout << " Bitwise Shift Right ";
    break;
  default:
    throw std::runtime_error("Expected a binary operation");
  }

  expr->expr_two->accept(this);
}

void AstPrinter::visit(const VariableAssignExpr *expr) {
  print_indent();
  std::cout << "VariableAssignment " << expr->var_name << " = ";

  expr->assign_expr->accept(this);

  std::cout << '\n';
}

void AstPrinter::visit(const ReturnStmt *stmt) {
  print_indent();
  std::cout << "ReturnStmt ";

  ++indentation;
  stmt->expr->accept(this);
  --indentation;

  std::cout << '\n';
}

void AstPrinter::visit(const VariableDeclStmt *stmt) {
  print_indent();
  std::cout << "VariableDecl " << type_to_string(stmt->type) << " "
            << stmt->name << " = ";

  if (stmt->decl_expr != nullptr) {
    stmt->decl_expr->accept(this);
  } else {
    std::cout << "init";
  }

  std::cout << '\n';
}

void AstPrinter::visit(const ExprStmt *stmt) {
  print_indent();
  std::cout << "ExprStmt ";

  stmt->expr->accept(this);

  std::cout << '\n';
}

void AstPrinter::visit(const FunctionDecl *decl) {
  print_indent();
  std::cout << "FunctionDecl name=" << decl->name
            << ", return=" << type_to_string(decl->return_type)
            << ", parameters=";

  for (int i = 0; i < decl->parameters.size(); ++i) {
    std::cout << "(" << type_to_string(decl->parameters[i]->type) << " "
              << decl->parameters[i]->name << ")";
  }
  std::cout << ":\n";

  ++indentation;
  for (int i = 0; i < decl->body.size(); ++i) {
    decl->body[i]->accept(this);
  }
  --indentation;

  std::cout << '\n';
}
