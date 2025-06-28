#include "parser.h"
#include "ast.h"
#include "lex.h"

#include <memory>
#include <stdexcept>

std::unique_ptr<FunctionDecl> Parser::parse() { return parse_function(); }

bool Parser::check(const TokenType &type) {
  if (is_at_end())
    return false;
  return tokens[current_token].token_type == type;
}

Token Parser::consume(const TokenType &type, const std::string &error_message) {
  if (check(type))
    return advance();
  throw std::runtime_error(error_message);
}

Token Parser::advance() {
  if (!is_at_end())
    current_token++;
  return tokens[current_token - 1];
}

bool Parser::check_advance(const TokenType &token_type) {
  if (token_type == tokens[current_token].token_type) {
    advance();

    return true;
  }

  return false;
}

bool Parser::is_at_end() { return current_token >= tokens.size(); }

VariableType Parser::parse_type() {
  Token type_token = advance();

  switch (type_token.token_type) {
  case TokenType::INT_TYPE:
    return VariableType::INT;
  case TokenType::VOID_TYPE:
    return VariableType::VOID;
  default:
    throw std::runtime_error("Syntax Error: Expected a type name");
  }
}

OperationType Parser::parse_operator() {
  Token op_token = advance();

  switch (op_token.token_type) {
  case TokenType::NEGATE:
    return OperationType::NEGATE;
  case TokenType::BITWISE:
    return OperationType::BITWISE;
  case TokenType::LOGIC_NEGATE:
    return OperationType::LOGIC_NEGATE;
  case TokenType::ADD:
    return OperationType::ADD;
  case TokenType::MULT:
    return OperationType::MULT;
  case TokenType::DIVIDE:
    return OperationType::DIVIDE;
  case TokenType::AND:
    return OperationType::AND;
  case TokenType::OR:
    return OperationType::OR;
  case TokenType::EQUAL:
    return OperationType::EQUAL;
  case TokenType::NOT_EQUAL:
    return OperationType::NOT_EQUAL;
  case TokenType::LESS_THAN:
    return OperationType::LESS_THAN;
  case TokenType::LESS_THAN_EQUAL:
    return OperationType::LESS_THAN_EQUAL;
  case TokenType::GREATER_THAN:
    return OperationType::GREATER_THAN;
  case TokenType::GREATER_THAN_EQUAL:
    return OperationType::GREATER_THAN_EQUAL;
  case TokenType::MODULO:
    return OperationType::MODULO;
  case TokenType::BITWISE_AND:
    return OperationType::BITWISE_AND;
  case TokenType::BITWISE_OR:
    return OperationType::BITWISE_OR;
  case TokenType::BITWISE_XOR:
    return OperationType::BITWISE_XOR;
  case TokenType::BITWISE_LEFT_SHIFT:
    return OperationType::BITWISE_SHIFT_LEFT;
  case TokenType::BITWISE_RIGHT_SHIFT:
    return OperationType::BITWISE_SHIFT_RIGHT;
  default:
    throw std::runtime_error("Syntax Error: Expected an operator");
  }
}

std::vector<std::unique_ptr<VariableDecl>> Parser::parse_func_parameters() {
  consume(TokenType::OPEN_PAREN,
          "Incorrect function definition, check parentheses");

  std::vector<std::unique_ptr<VariableDecl>> parameters;
  if (!check(TokenType::CLOSE_PAREN)) {
    do {
      VariableType param_type = parse_type();
      Token param_token =
          consume(TokenType::IDENTIFIER, "Expected parameter name");
      std::string param_name = std::get<std::string>(param_token.literal);

      parameters.push_back(
          std::make_unique<VariableDecl>(param_type, param_name));
    } while (check_advance(TokenType::COMMA));
  }

  consume(TokenType::CLOSE_PAREN,
          "Incorrect function definition, check parentheses");

  return parameters;
}

std::unique_ptr<ExprAST> Parser::parse_factor() {
  if (check(TokenType::OPEN_PAREN)) {
    consume(TokenType::OPEN_PAREN, "Expected an open parenthesis");

    auto expr = parse_expression();
    consume(TokenType::CLOSE_PAREN,
            "Parentheses mismatch on bounded expression");

    return expr;
  } else if (check(TokenType::NEGATE) || check(TokenType::BITWISE) ||
             check(TokenType::LOGIC_NEGATE)) {
    OperationType op = parse_operator();
    auto factor = parse_factor();
    auto un_op = std::make_unique<UnaryOpExpr>(op, std::move(factor));

    return un_op;
  } else if (check(TokenType::INT)) {
    Token num = advance();
    auto num_expr =
        std::make_unique<IntLiteralExpr>(std::get<int>(num.literal));

    return num_expr;
  }

  throw std::runtime_error("Expected a 'factor'");
}

std::unique_ptr<ExprAST> Parser::parse_term() {
  std::unique_ptr<ExprAST> factor = parse_factor();

  while (check(TokenType::MULT) || check(TokenType::DIVIDE) ||
         check(TokenType::MODULO)) {
    OperationType op = parse_operator();
    auto next_factor = parse_factor();

    factor = std::make_unique<BinaryOpExpr>(op, std::move(factor),
                                            std::move(next_factor));
  }

  return factor;
}

std::unique_ptr<ExprAST> Parser::parse_additive() {
  std::unique_ptr<ExprAST> term = parse_term();

  while (check(TokenType::ADD) || check(TokenType::NEGATE)) {
    OperationType op = parse_operator();
    auto next_term = parse_term();

    term = std::make_unique<BinaryOpExpr>(op, std::move(term),
                                          std::move(next_term));
  }

  return term;
}

std::unique_ptr<ExprAST> Parser::parse_bitshift() {
  std::unique_ptr<ExprAST> additive_expr = parse_additive();

  while (check(TokenType::BITWISE_LEFT_SHIFT) ||
         check(TokenType::BITWISE_RIGHT_SHIFT)) {
    OperationType op = parse_operator();
    auto next_additive = parse_additive();

    additive_expr = std::make_unique<BinaryOpExpr>(op, std::move(additive_expr),
                                                   std::move(next_additive));
  }

  return additive_expr;
}

std::unique_ptr<ExprAST> Parser::parse_relational() {
  std::unique_ptr<ExprAST> bitshift_expr = parse_bitshift();

  while (check(TokenType::LESS_THAN) || check(TokenType::GREATER_THAN) ||
         check(TokenType::LESS_THAN_EQUAL) ||
         check(TokenType::GREATER_THAN_EQUAL)) {
    OperationType op = parse_operator();
    auto next_bitshift = parse_bitshift();

    bitshift_expr = std::make_unique<BinaryOpExpr>(op, std::move(bitshift_expr),
                                                   std::move(next_bitshift));
  }

  return bitshift_expr;
}

std::unique_ptr<ExprAST> Parser::parse_equality() {
  std::unique_ptr<ExprAST> relational_expr = parse_relational();

  while (check(TokenType::EQUAL) || check(TokenType::NOT_EQUAL)) {
    OperationType op = parse_operator();
    auto next_relational = parse_relational();

    relational_expr = std::make_unique<BinaryOpExpr>(
        op, std::move(relational_expr), std::move(next_relational));
  }

  return relational_expr;
}

std::unique_ptr<ExprAST> Parser::parse_bitwise_and() {
  std::unique_ptr<ExprAST> equality_expr = parse_equality();

  while (check(TokenType::BITWISE_AND)) {
    OperationType op = parse_operator();
    auto next_equality = parse_equality();

    equality_expr = std::make_unique<BinaryOpExpr>(op, std::move(equality_expr),
                                                   std::move(next_equality));
  }

  return equality_expr;
}

std::unique_ptr<ExprAST> Parser::parse_bitwise_xor() {
  std::unique_ptr<ExprAST> bitwise_and_expr = parse_bitwise_and();

  while (check(TokenType::BITWISE_XOR)) {
    OperationType op = parse_operator();
    auto next_bitand_expr = parse_bitwise_and();

    bitwise_and_expr = std::make_unique<BinaryOpExpr>(
        op, std::move(bitwise_and_expr), std::move(next_bitand_expr));
  }

  return bitwise_and_expr;
}

std::unique_ptr<ExprAST> Parser::parse_bitwise_or() {
  std::unique_ptr<ExprAST> bitwise_xor_expr = parse_bitwise_xor();

  while (check(TokenType::BITWISE_OR)) {
    OperationType op = parse_operator();
    auto next_bitxor_expr = parse_bitwise_xor();

    bitwise_xor_expr = std::make_unique<BinaryOpExpr>(
        op, std::move(bitwise_xor_expr), std::move(next_bitxor_expr));
  }

  return bitwise_xor_expr;
}

std::unique_ptr<ExprAST> Parser::parse_logical_and() {
  std::unique_ptr<ExprAST> bitwise_or_expr = parse_bitwise_or();

  while (check(TokenType::AND)) {
    OperationType op = parse_operator();
    auto next_bitor_expr = parse_bitwise_or();

    bitwise_or_expr = std::make_unique<BinaryOpExpr>(
        op, std::move(bitwise_or_expr), std::move(next_bitor_expr));
  }

  return bitwise_or_expr;
}

std::unique_ptr<ExprAST> Parser::parse_expression() {
  std::unique_ptr<ExprAST> and_expr = parse_logical_and();

  while (check(TokenType::OR)) {
    OperationType op = parse_operator();
    auto next_and = parse_logical_and();

    and_expr = std::make_unique<BinaryOpExpr>(op, std::move(and_expr),
                                              std::move(next_and));
  }

  return and_expr;
}

std::unique_ptr<StmtAST> Parser::parse_statement() {
  if (check(TokenType::RETURN)) {
    advance(); // Consume the return token
    auto expr = parse_expression();
    consume(TokenType::SEMICOLON, "Expected ';' after return value");
    return std::make_unique<ReturnStmt>(std::move(expr));
  }

  throw std::runtime_error("Expected a statement");
}

std::unique_ptr<FunctionDecl> Parser::parse_function() {
  VariableType return_type = parse_type();
  std::string func_name = std::get<std::string>(
      consume(TokenType::IDENTIFIER,
              "Incorrect function definition: Check function identifier")
          .literal);
  std::vector<std::unique_ptr<VariableDecl>> func_parameters =
      parse_func_parameters();
  consume(TokenType::OPEN_BRACE, "Incorrect function definition: Check braces");

  auto statement =
      parse_statement(); // Assume the function has only one statement for now
                         // (the return statement)

  std::vector<std::unique_ptr<StmtAST>> body;
  body.push_back(std::move(statement));

  consume(TokenType::CLOSE_BRACE,
          "Incoreect function definition: Check braces");

  return std::make_unique<FunctionDecl>(
      func_name, return_type, std::move(func_parameters), std::move(body));
}
