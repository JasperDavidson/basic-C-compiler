#include "parser.h"
#include "ast.h"
#include "lex.h"
#include <memory>
#include <stdexcept>

std::unique_ptr<FunctionDecl> Parser::parse() {
    return parse_function();
}

bool Parser::check(const TokenType& type) {
    if (is_at_end()) return false;
    return tokens[current_token].token_type == type;
}

Token Parser::consume(const TokenType& type, const std::string& error_message) {
    if (check(type)) return advance();
    throw std::runtime_error(error_message);
}

Token Parser::advance() {
    if (!is_at_end()) current_token++;
    return tokens[current_token - 1];
}

bool Parser::is_at_end() {
    return current_token >= tokens.size();
}

std::unique_ptr<ExprAST> Parser::parse_expression() {
    if (check(INT)) {
        Token int_token = advance();
        return std::make_unique<IntLiteralExpr>(IntLiteralExpr(std::get<int>(int_token.literal)));
    }

    throw new std::runtime_error("Expected an expression");
}

std::unique_ptr<StmtAST> Parser::parse_statement() {
    if (check(RETURN)) {
        advance(); // Consume the return token
        auto expr = parse_expression();
        consume(SEMICOLON, "Expected 'j' after return value");
        return std::make_unique<StmtAST>(ReturnStmt(std::move(expr)));
    }

    throw new std::runtime_error("Expected a statement");
}

std::unique_ptr<FunctionDecl> Parser::parse_function() {
    // Assumes only the standard main function for now
    consume(INT_TYPE, "Expected return value of type 'int'");
    std::string func_name = std::get<std::string>(consume(IDENTIFIER, "Incorrect function definition: Check function identifier").literal);
    consume(OPEN_PAREN, "Incorrect function definition: Check parentheses");
    consume(CLOSE_PAREN, "Incorrect function definition: Check parentheses");
    consume(OPEN_BRACE, "Incorrect function definition: Check braces");

    auto statement = parse_statement(); // Assume the function has only one statement for now (the return statement)

    return std::make_unique<FunctionDecl>(FunctionDecl(func_name, std::move(statement)));
}
