#include "codegen.h"
#include "ast.h"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

void AstAssembly::generate(DeclAST *root_node, std::string asm_file_name) {
  // std::string asm_file_path = std::string("../tests/").append(asm_file_name);
  asm_file.open(asm_file_name);

  root_node->accept(this);

  asm_file.close();
}

void AstAssembly::visit(const IntLiteralExpr *expr) {
  asm_file << "#" << std::to_string(expr->value);
}

// Implement later
void AstAssembly::visit(const VariableExpr *expr) {}

void AstAssembly::visit(const UnaryOpExpr *expr) {
  expr->expr->accept(this);
  asm_file << "\n\t";
  
  switch (expr->op) {
    case OperationType::NEGATE:
      asm_file << "neg\tx0, x0";
      break;
    case OperationType::BITWISE:
      asm_file << "mvn\tx0, x0";
      break;
    case OperationType::LOGIC_NEGATE:
      asm_file << "cmp\tx0, #0";
      asm_file << "\n\tcset\tx0, EQ";
      break;
    default:
      throw std::runtime_error("Expected a unary operation");
  }
}

void AstAssembly::visit(const BinaryOpExpr *expr) {
  // switch(expr->op) {
    
  // }
}

void AstAssembly::visit(const ReturnStmt *stmt) {
  asm_file << "\tmov\tx0, ";

  stmt->expr->accept(this);

  asm_file << "\n\tret";
}

// Implement later
void AstAssembly::visit(const VariableDecl *decl) {}

void AstAssembly::visit(const FunctionDecl *decl) {
  asm_file << "\t.globl _" << decl->name << "\n_" << decl->name << ":\n";

  for (int i = 0; i < decl->body.size(); ++i) {
    decl->body[i]->accept(this);
  }
}
