#include "ast.h"
#include "lex.h"

#include <memory>
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tokens) : tokens(std::move(tokens)) {};

    std::unique_ptr<FunctionDecl> parse();
      
private:
    std::vector<Token> tokens;
    int current_token;

    /* Helper functions */

    // Helper to check the type of the current token without consuming it
    bool check(const TokenType& type);

    // Helper to consume the current token, throws an error if it's not of the passed (assumed to be correct) type
    Token consume(const TokenType& type, const std::string& error_message);

    // Helper to advance through the list of tokens
    Token advance();        

    // Check the current token and advance if it is valid, return boolean based on result
    bool check_advance(const TokenType& token_type);

    // Helper to check if the list of tokens has been exhausted
    bool is_at_end();

    // Helper to parse VariableTypes
    VariableType parse_type();

    // Helper to parse parameters from a function
    std::vector<std::unique_ptr<VariableDeclAST>> parse_func_parameters();

    /* Grammar Matching Mehods */

    // Corresponds to the 'expression rule' (only ints for now)
    std::unique_ptr<ExprAST> parse_expression();

    // Corresponds to the 'statement' rule (only return statements for now)
    std::unique_ptr<StmtAST> parse_statement();
    
    // Corresponds to the 'function declaration' rule
    std::unique_ptr<FunctionDecl> parse_function();
};
