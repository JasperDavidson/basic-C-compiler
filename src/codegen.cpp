#include "codegen.h"
#include "ast.h"

#include <fstream>
#include <memory>
#include <stdexcept>
#include <string>

std::string AstAssembly::label_gen() {
  std::string base_label = "_label_";

  return base_label.append(std::to_string(label_num++));
}

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

// Optimization idea: Somehow find out how many binary operations there are and
// allocate the required memory ahead of time. This would help save space on
// these simple operations since arm assembly ABI has the stack as 16-byte
// aligned. So, if there were two operations and the program allocated 16 bytes,
// there wouldn't be wasted space. Currently for every push onto the stack the
// program wastes 8 bytes since it must stay 16-byte aligned
void AstAssembly::visit(const BinaryOpExpr *expr) {
  // Compute the first expression
  expr->expr_one->accept(this);

  // Determine the operation and combine the two expressions

  if (expr->op == OperationType::ADD || expr->op == OperationType::NEGATE ||
      expr->op == OperationType::MULT || expr->op == OperationType::DIVIDE) {
    // Compute the second expression and save it to x0
    asm_file << "\n\tstr\tx0, [sp, #-16]!";
    asm_file << "\n\tmov\tx0, "; // Is this line needed?
    expr->expr_two->accept(this);

    asm_file << "\n\tldr\tx1, [sp], #16";

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
      asm_file << "sdiv\t";
      break;
    default:
      __builtin_unreachable();
    }

    asm_file << "x0, x1, x0";
  } else if (expr->op == OperationType::EQUAL ||
             expr->op == OperationType::NOT_EQUAL ||
             expr->op == OperationType::LESS_THAN ||
             expr->op == OperationType::LESS_THAN_EQUAL ||
             expr->op == OperationType::GREATER_THAN ||
             expr->op == OperationType::GREATER_THAN_EQUAL) {
    // Compute the second expression and save it to x0
    asm_file << "\n\tstr\tx0, [sp, #-16]!";
    asm_file << "\n\tmov\tx0, "; // Is this line needed?
    expr->expr_two->accept(this);

    asm_file << "\n\tldr\tx1, [sp], #16";

    asm_file << "cmp\tx1, x0\n\t";

    switch (expr->op) {
    case OperationType::EQUAL:
      asm_file << "cset\tx0, eq";
      break;
    case OperationType::NOT_EQUAL:
      asm_file << "cset\tx0, ne";
      break;
    case OperationType::LESS_THAN:
      asm_file << "cset\tx0, lt";
      break;
    case OperationType::GREATER_THAN:
      asm_file << "cset\tx0, gt";
      break;
    case OperationType::LESS_THAN_EQUAL:
      asm_file << "cset\tx0, le";
      break;
    case OperationType::GREATER_THAN_EQUAL:
      asm_file << "cset\tx0, ge";
      break;
    default:
      __builtin_unreachable();
    }
  } else if (expr->op == OperationType::OR || expr->op == OperationType::AND) {
    // OR and AND are special operations. They follow "short circuiting" rules,
    // meaning that for OR: if the first statement is true, ignore the second
    // one, for AND: if the first statement is false, ignore the second one.
    // That's why the second expression calculation logic had to be moved inside
    // each block. Some programs *expect* the second expression to not be
    // executed in certain scenarios (e.g. a function that modifies state)
    std::string circuit_fail_label = label_gen();
    std::string end_label = label_gen();

    switch (expr->op) {
    case OperationType::OR:
      asm_file << "\n\tcmp\tx0, #0";
      asm_file << "\n\tb.eq\t" +
                      circuit_fail_label; // MAKE A LABEL GENERATING FUNCTION -
                                          // Duplicate labels are not allowed
      asm_file << "\n\tmov\tx0, #1";
      asm_file << "\n\tb\t" + end_label;
      asm_file << "\n" + circuit_fail_label + ":";

      // Only compute the second expression here - this is critical for short
      // circuiting
      asm_file << "\n\tmov\tx0, ";
      expr->expr_two->accept(this);

      asm_file << "\n\tcmp\tx0, #0";
      asm_file << "\n\tcset\tx0, ne";

      asm_file << "\n" + end_label + ":";

      break;
    case OperationType::AND:
      asm_file << "\n\tcmp\tx0, #0";
      asm_file << "\n\tb.ne\t" + circuit_fail_label;
      asm_file << "\n\tmov\tx0, #0";
      asm_file << "\n\tb\t" + end_label;
      asm_file << "\n" + circuit_fail_label + ":";

      // Compute the second expression (short circuit failure)
      asm_file << "\n\tmov\tx0, ";
      expr->expr_two->accept(this);

      asm_file << "\n\tcmp\tx0, #0";
      asm_file << "\n\tcset\tx0, ne";

      asm_file << "\n" + end_label + ":";

      break;
    default:
      __builtin_unreachable();
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
