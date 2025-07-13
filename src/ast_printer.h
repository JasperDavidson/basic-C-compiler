#include "ast.h"

#include <cstdio>
#include <memory>
#include <string>
#include <variant>

// const auto var_visitor_printer = Overload{
//     [](std::monostate) -> std::string { return "init"; },
//     [](int i) -> std::string { return std::to_string(i); },
// };

class AstPrinter : public ExprVisitor, public StmtVisitor, public DeclVisitor {
public:
  // Method to start the printing process
  void print_from_root(DeclAST *root_node) { root_node->accept(this); }

  // Fulfilling ExprVisitor contract
  void visit(const IntLiteralExpr *expr) override;
  void visit(const VariableExpr *expr) override;
  void visit(const UnaryOpExpr *expr) override;
  void visit(const BinaryOpExpr *expr) override;
  void visit(const VariableAssignExpr *expr) override;

  // Fulfilling the StmtVisitor contract
  void visit(const VariableDeclStmt *stmt) override;
  void visit(const ReturnStmt *stmt) override;
  void visit(const ExprStmt *stmt) override;

  // Fulfilling the DeclVisitor contract
  void visit(const FunctionDecl *decl) override;

private:
  int indentation = 0;

  // Helper to print indents to make tree structure clearer
  void print_indent();
};
