#include "ast.h"

class AstPrinter : public ExprVisitor, public StmtVisitor, public DeclVisitor {
public:
    // Method to start the printing process
    void print_from_root(DeclAST *root_node) { root_node->accept(this); }

    // Fulfilling ExprVisitor contract
    void visit(IntLiteralExpr *expr) override;
    void visit(VariableExpr *expr) override;

    // Fulfilling the StmtVisitor contract
    void visit(ReturnStmt *stmt) override;

    // Fulfilling the DeclVisitor contract
    void visit(VariableDecl *decl) override;
    void visit(FunctionDecl *decl) override;

private:
    int indentation = 0;

    // Helper to print indents to make tree structure clearer
    void print_indent();
};
