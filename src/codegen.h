#include "ast.h"

#include <fstream>
#include <memory>
#include <string>

class AstAssembly : public ExprVisitor, public StmtVisitor, public DeclVisitor {
public:
  void generate(DeclAST *root_node, std::string asm_file_name);

  // Fulfilling ExprVisitor contract
  void visit(const IntLiteralExpr *expr) override;
  void visit(const UnaryOpExpr *expr) override;
  void visit(const BinaryOpExpr *expr) override;
  void visit(const VariableExpr *expr) override;
  void visit(const VariableAssignExpr *expr) override;

  // Fulfilling the StmtVisitor contract
  void visit(const VariableDeclStmt *stmt) override;
  void visit(const ReturnStmt *stmt) override;
  void visit(const ExprStmt *stmt) override;

  // Fulfilling the DeclVisitor contract
  void visit(const FunctionDecl *decl) override;

private:
  std::ofstream asm_file;
  int label_num = 0;


  // Keep track of variables in current stack frame
  std::unordered_map<std::string, int> stack_variables;
  int stack_index = 0;
  int STACK_DIFFERENCE = 16;

  // Helper function to generate unique labels
  std::string label_gen();
};
