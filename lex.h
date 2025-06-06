#ifndef LEX_H
#define LEX_H

#include <string>
#include <vector>
#include <variant>

using TokenValue = std::variant<std::monostate, int, std::string>;

enum class TokenType {
	// Single character tokens
	OPEN_BRACE, CLOSE_BRACE, OPEN_PAREN, CLOSE_PAREN, SEMICOLON, COMMA,

	// Literals
	INT, IDENTIFIER,

	// Keywords
	RETURN, INT_TYPE,
};

struct Token {
	TokenType token_type;
	TokenValue literal;

	Token(TokenType token_type, TokenValue literal) : token_type(token_type), literal(literal) {};
};

std::vector<Token> lex(const std::string& file_path);

#endif
