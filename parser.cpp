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

std::vector<std::unique_ptr<VariableDeclAST>> Parser::parse_func_parameters() {
    consume(TokenType::OPEN_PAREN, "Incorrect function definition: Check parentheses");

    std::vector<std::unique_ptr<VariableDeclAST>> parameters;
    Token next_token = advance();
    while(next_token.token_type != TokenType::CLOSE_PAREN) {
        std::string type_name = std::get<std::string>(next_token.literal);
        next_token = advance();
        std::string variable_name = std::get<std::string>(next_token.literal);
        VariableType variable_type = parse_type(type_name);

        parameters.push_back(std::make_unique<VariableDeclAST>(variable_type, variable_name));

        next_token = advance();

        if (next_token.token_type == TokenType::COMMA) {
            next_token = advance();
        }
    }

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
    // Assumes only the standard main function for now
    consume(TokenType::INT_TYPE, "Expected return value of type 'int'");
    std::string func_name = std::get<std::string>(consume(TokenType::IDENTIFIER, "Incorrect function definition: Check function identifier").literal);
    std::vector<std::unique_ptr<VariableDeclAST>> func_parameters = parse_func_parameters();
    consume(TokenType::OPEN_BRACE, "Incorrect function definition: Check braces");

    auto statement = parse_statement(); // Assume the function has only one statement for now (the return statement)

    consume(TokenType::CLOSE_BRACE, "Incoreect function definition: Check braces");

    return std::make_unique<FunctionDecl>(FunctionDecl(func_name, std::move(func_parameters), std::move(statement)));
}
