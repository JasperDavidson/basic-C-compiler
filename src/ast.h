#ifndef AST_H
#define AST_H

#include "lex.h"

#include <memory>
#include <stdexcept>
#include <string>
#include <vector>

// Forward declaration of neccessary types
class ExprVisitor;
struct IntLiteralExpr;
struct VariableExpr;
struct UnaryOpExpr;
struct BinaryOpExpr;

class StmtVisitor;
struct ReturnStmt;

class DeclVisitor;
struct VariableDecl;
struct FunctionDecl;

// I want to decouple the parser from the lexer, so I define types and
// operations here
enum class VariableType { INT, VOID };
enum class OperationType {
  NEGATE,
  BITWISE,
  LOGIC_NEGATE,
  ADD,
  MULT,
  DIVIDE,
  AND,
  OR,
  EQUAL,
  NOT_EQUAL,
  GREATER_THAN,
  LESS_THAN,
  GREATER_THAN_EQUAL,
  LESS_THAN_EQUAL,
  MODULO,
  BITWISE_AND,
  BITWISE_OR,
  BITWISE_XOR,
  BITWISE_SHIFT_LEFT,
  BITWISE_SHIFT_RIGHT
};

std::string type_to_string(VariableType variable_type);

// Visitor object for expressions ('a + b', 'x', etc.)
class ExprVisitor {
public:
  virtual ~ExprVisitor() = default;

  virtual void visit(const IntLiteralExpr *expr) = 0;
  virtual void visit(const VariableExpr *expr) = 0;
  virtual void visit(const UnaryOpExpr *expr) = 0;
  virtual void visit(const BinaryOpExpr *expr) = 0;
};

// Visitor for statements ('return', 'if', etc.)
class StmtVisitor {
public:
  virtual ~StmtVisitor() = default;

  virtual void visit(const ReturnStmt *stmt) = 0;
};

// Visitor for declarations ('int x', func(..., ..., ...), etc.)
class DeclVisitor {
public:
  virtual ~DeclVisitor() = default;

  virtual void visit(const VariableDecl *decl) = 0;
  virtual void visit(const FunctionDecl *decl) = 0;
};

// Base struct for expression nodes
struct ExprAST {
  virtual ~ExprAST() = default;
  virtual void accept(ExprVisitor *visitor) = 0;
};

// Int literal node
struct IntLiteralExpr : public ExprAST {
  int value;

  explicit IntLiteralExpr(int value) : value(value) {};

  void accept(ExprVisitor *visitor) { visitor->visit(this); }
};

// Variable node as an *expression* (like 'return x')
struct VariableExpr : public ExprAST {
  std::string name;

  explicit VariableExpr(std::string name) : name(std::move(name)) {};

  void accept(ExprVisitor *visitor) { visitor->visit(this); }
};

// Unary Operation node
struct UnaryOpExpr : public ExprAST {
  OperationType op;
  std::unique_ptr<ExprAST> expr;

  UnaryOpExpr(OperationType op, std::unique_ptr<ExprAST> expr)
      : op(op), expr(std::move(expr)) {};

  void accept(ExprVisitor *visitor) { visitor->visit(this); };
};

// Binary Operation node
struct BinaryOpExpr : public ExprAST {
  OperationType op;
  std::unique_ptr<ExprAST> expr_one;
  std::unique_ptr<ExprAST> expr_two;

  BinaryOpExpr(OperationType op, std::unique_ptr<ExprAST> expr_one,
               std::unique_ptr<ExprAST> expr_two)
      : op(op), expr_one(std::move(expr_one)), expr_two(std::move(expr_two)) {};

  void accept(ExprVisitor *visitor) { visitor->visit(this); };
};

// Base struct for statement nodes
struct StmtAST {
  virtual ~StmtAST() = default;
  virtual void accept(StmtVisitor *visitor) = 0;
};

// Return statement node
struct ReturnStmt : public StmtAST {
  std::unique_ptr<ExprAST> expr;

  explicit ReturnStmt(std::unique_ptr<ExprAST> expr) : expr(std::move(expr)) {};

  void accept(StmtVisitor *visitor) { visitor->visit(this); }
};

struct DeclAST {
  virtual ~DeclAST() = default;
  virtual void accept(DeclVisitor *visitor) = 0;
};

struct VariableDecl : public DeclAST {
  VariableType type;
  std::string name;

  VariableDecl(VariableType type, std::string name)
      : type(type), name(std::move(name)) {};

  void accept(DeclVisitor *visitor) { visitor->visit(this); }
};

// Node for a function declaration
struct FunctionDecl : public DeclAST {
  std::string name;
  VariableType return_type;
  std::vector<std::unique_ptr<VariableDecl>> parameters;
  std::vector<std::unique_ptr<StmtAST>> body;

  FunctionDecl(std::string name, VariableType return_type,
               std::vector<std::unique_ptr<VariableDecl>> params,
               std::vector<std::unique_ptr<StmtAST>> body)
      : name(std::move(name)), return_type(return_type),
        parameters(std::move(params)), body(std::move(body)) {};

  void accept(DeclVisitor *visitor) { visitor->visit(this); };
};

#endif
