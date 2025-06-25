#ifndef LEX_H
#define LEX_H

#include <string>
#include <variant>
#include <vector>

using TokenValue = std::variant<std::monostate, int, std::string>;

enum class TokenType {
  // Single character tokens
  OPEN_BRACE,
  CLOSE_BRACE,
  OPEN_PAREN,
  CLOSE_PAREN,
  SEMICOLON,
  COMMA,
  NEGATE,
  BITWISE,
  LOGIC_NEGATE,
  ADD,
  MULT,
  DIVIDE,

  // Logic
  AND,
  OR,
  EQUAL,
  NOT_EQUAL,
  LESS_THAN,
  LESS_THAN_EQUAL,
  GREATER_THAN,
  GREATER_THAN_EQUAL,

  // Literals
  INT,
  IDENTIFIER,

  // Keywords
  RETURN,
  INT_TYPE,
  VOID_TYPE
};

struct Token {
  TokenType token_type;
  TokenValue literal;

  Token(TokenType token_type, TokenValue literal)
      : token_type(token_type), literal(literal) {};
};

std::vector<Token> lex(const std::string &file_path);

#endif
