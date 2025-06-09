#ifndef AST_H
#define AST_H

#include "lex.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// I want to decouple the parser from the lexer, so I define types here
enum class VariableType {
    INT,
    VOID
};

// Base struct for expression nodes
struct ExprAST {
    virtual ~ExprAST() = default;
};

// Int literal node
struct IntLiteralExpr : public ExprAST {
    int value;

    explicit IntLiteralExpr(int value) : value(value) {};
};

// Variable node as an *expression* (like 'return x')
struct VariableExpr : public ExprAST {
    std::string name;

    explicit VariableExpr(std::string name) : name(std::move(name)) {};
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

struct DeclAST {
    virtual ~DeclAST() = default;
};

struct VariableDeclAST : public DeclAST {
    VariableType type;
    std::string name;

    VariableDeclAST(VariableType type, std::string name) : type(type), name(std::move(name)) {};
};

// Node for a function declaration
struct FunctionDecl : public DeclAST {
    std::string name;
    VariableType return_type;
    std::vector<std::unique_ptr<VariableDeclAST>> parameters;
    std::unique_ptr<StmtAST> body; // For now, the body of a function is just one statement (return)

    FunctionDecl(std::string name, VariableType return_type, std::vector<std::unique_ptr<VariableDeclAST>> params, std::unique_ptr<StmtAST> body) :
                 name(std::move(name)), return_type(return_type), parameters(std::move(params)), body(std::move(body)) {};
};

#endif
