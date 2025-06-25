#include "codegen.h"
#include "ast.h"
#include "lex.h"

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

// Optimization idea: Somehow find out how many binary operations there are and allocate the required memory ahead of time. This would help save space on these simple operations since arm assembly
// ABI has the stack as 16-byte aligned. So, if there were two operations and the program allocated 16 bytes, there wouldn't be wasted space. Currently for every push onto the stack the program
// wastes 8 bytes since it must stay 16-byte aligned
void AstAssembly::visit(const BinaryOpExpr *expr) {
  // Compute the first expression and push it to the stack
  expr->expr_one->accept(this);
  asm_file << "\n\tstr\tx0, [sp, #-16]!";

  // Compute the second expression and save it to x0
  asm_file << "\n\tmov\tx0, ";
  expr->expr_two->accept(this);

  // Determine the operation and combine the two expressions
  asm_file << "\n\tldr\tx1, [sp], #16";

  if (expr->op == OperationType::ADD || expr->op == OperationType::NEGATE || expr->op == OperationType::MULT || expr->op == OperationType::DIVIDE) {
    asm_file << "\n\t";
    switch (expr->op) {
      case OperationType::ADD:
        asm_file << "add\t";
        break;
      case OperationType::NEGATE:
        asm_file << "sub\t";
        break;
      case OperationType::MULT:
        asm_file << "mul\t";
        break;
      case OperationType::DIVIDE:
        asm_file << "udiv\t";
        break;
      default:
        throw std::runtime_error("Expected a binary operation");
    }

    asm_file << "x0, x1, x0";
  } else if (expr->op == OperationType::EQUAL || expr->op == OperationType::NOT_EQUAL || expr->op == OperationType::LESS_THAN || expr->op == OperationType::LESS_THAN_EQUAL ||
             expr->op == OperationType::GREATER_THAN || expr->op == OperationType::GREATER_THAN_EQUAL) {
    asm_file << "cmp\tx0, x1";

    switch (expr->op) {
      case OperationType::EQUAL:
        asm_file << "cset\tx0, EQ";
        break;
      case OperationType::NOT_EQUAL:
        asm_file << "cset\tx0, NE";
        break;
    }
  }
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
