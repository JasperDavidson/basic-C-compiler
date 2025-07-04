#include "ast.h"

#include <memory>

class AstPrinter : public ExprVisitor, public StmtVisitor, public DeclVisitor {
public:
  // Method to start the printing process
  void print_from_root(DeclAST *root_node) { root_node->accept(this); }

  // Fulfilling ExprVisitor contract
  void visit(const IntLiteralExpr *expr) override;
  void visit(const VariableExpr *expr) override;
  void visit(const UnaryOpExpr *expr) override;
  void visit(const BinaryOpExpr *expr) override;

  // Fulfilling the StmtVisitor contract
  void visit(const ReturnStmt *stmt) override;

  // Fulfilling the DeclVisitor contract
  void visit(const VariableDecl *decl) override;
  void visit(const FunctionDecl *decl) override;

private:
  int indentation = 0;

  // Helper to print indents to make tree structure clearer
  void print_indent();
};
