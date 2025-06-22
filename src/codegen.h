#include "ast.h"

#include <fstream>
#include <memory>
#include <string>

class AstAssembly : public ExprVisitor, public StmtVisitor, public DeclVisitor {
public:
  void generate(DeclAST *root_node, std::string asm_file_name);

  // Fulfilling ExprVisitor contract
  void visit(const IntLiteralExpr *expr) override;
  void visit(const VariableExpr *expr) override;
  void visit(const UnaryOpExpr *expr) override;

  // Fulfilling the StmtVisitor contract
  void visit(const ReturnStmt *stmt) override;

  // Fulfilling the DeclVisitor contract
  void visit(const VariableDecl *decl) override;
  void visit(const FunctionDecl *decl) override;

private:
  std::ofstream asm_file;
};
