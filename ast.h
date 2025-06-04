#ifndef AST_H
#define AST_H

#include <memory>
#include <string>

// Base struct for expression nodes
struct ExprAST {
    virtual ~ExprAST() = default;
};

// Int literal node
struct IntLiteralExpr : public ExprAST {
    int value;

    explicit IntLiteralExpr(int value) : value(value) {};
};

// Base struct for statement nodes
struct StmtAST {
    virtual ~StmtAST() = default;
};

// Return statement node
struct ReturnStmt : public StmtAST {
    std::unique_ptr<ExprAST> expression;

    explicit ReturnStmt(std::unique_ptr<ExprAST> expr) : expression(std::move(expr)) {};
};

// Node for a function declaration
struct FunctionDecl {
    std::string name;
    std::unique_ptr<StmtAST> body; // For now, the body of a function is just one statement (return)

    FunctionDecl(std::string name, std::unique_ptr<StmtAST> body) : name(std::move(name)), body(std::move(body)) {};
};

#endif
