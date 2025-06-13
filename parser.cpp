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

std::unique_ptr<ExprAST> Parser::parse_expression() {
  if (check(TokenType::INT)) {
    Token int_token = advance();
    return std::make_unique<IntLiteralExpr>(std::get<int>(int_token.literal));
  }

  throw std::runtime_error("Expected an expression");
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

  return std::make_unique<FunctionDecl>(func_name, return_type, std::move(func_parameters), std::move(body));
}
