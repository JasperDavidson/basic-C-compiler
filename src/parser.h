#include "ast.h"
#include "lex.h"

#include <memory>
#include <vector>

/*
Grammer for the parser:

<program> ::= <function>
<function> ::= "int" <id> "(" ")" "{" { <statement> } "}"
<statement> ::= "return" <expr> ";"
<expr> ::= <logical_and_expr> { "||" <logical_and_expr> }
<logical_and_expr> ::= <bitwise_or_expr> { "&&" <bitwise_or_expr> }
<bitwise_or_expr> ::= <bitwise_xor__expr> { "|" <bitwise_xor_expr> }
<bitwise_xor_expr> ::= <bitwise_and_expr> { "^" <bitwise_and_expr> }
<bitwise_and_expr> ::= <equality_expr> { "&" <equality_expr> }
<equality_expr> ::= <relational_expr> { ("==" | "!=") <relational_expr> }
<relational_expr> ::= <bitshift_expr> { ("<" | ">" | "<=" | ">=")
<bitshift_expr> } <bitshift_expr> ::= <additive_expr> { ("<<" | ">>")
<additive_expr> } <additive_expr> ::= <term> { ("+" | "-") <term> } <term> ::=
<factor> { ("*" | "/" | "%") <factor> } <factor> ::= "(" <expr> ")" | <unary_op>
<factor> | int <unary_op> ::= "!" | "~" | "-"

The expression grammar is designed this way for two key reasons. First, it
avoids infinite left recursion that something like <expr> ::= <expr> (operation)
<expr> ... might encounter (the example could recursively parse expressions
forever). It also serves to protect associativity and operator precedence. It
achieves this through uses *repitition* craft expressions (the "{}") that are
still *recursively* interpreted in the AST. In the example definition of <expr>,
not only might it be left recursive infinitely, but it also doesn't distinguish
between 1 - 2 - 3 being interpreted as (1 - 2) - 3 (correct) and 1 - (2 - 3)
(incorrect). It also ensures that unary operations take place before binary
expressions, as expected.
*/
class Parser {
public:
  explicit Parser(const std::vector<Token> &tokens)
      : tokens(std::move(tokens)) {};

  std::unique_ptr<FunctionDecl> parse();

private:
  std::vector<Token> tokens;
  int current_token = 0;

  /* Helper functions */

  // Helper to check the type of the current token without consuming it
  bool check(const TokenType &type);

  // Helper to consume the current token, throws an error if it's not of the
  // passed (assumed to be correct) type
  Token consume(const TokenType &type, const std::string &error_message);

  // Helper to advance through the list of tokens
  Token advance();

  // Check the current token and advance if it is valid, return boolean based on
  // result
  bool check_advance(const TokenType &token_type);

  // Helper to check if the list of tokens has been exhausted
  bool is_at_end();

  // Helper to parse VariableTypes from tokens
  VariableType parse_type();

  // Helper to parse OperationTypes from tokens
  OperationType parse_operator();

  // Helper to parse parameters from a function
  std::vector<std::unique_ptr<VariableDecl>> parse_func_parameters();

  /* Grammar Matching Methods */

  // Corresponds to the 'expr' rule
  std::unique_ptr<ExprAST> parse_expression();

  // Corresponds to the 'logical_and_expr' rule
  std::unique_ptr<ExprAST> parse_logical_and();

  // Corresponds to the 'bitwise_or_expr' rule
  std::unique_ptr<ExprAST> parse_bitwise_or();

  // Corresponds to the 'bitwise_xor_expr' rule
  std::unique_ptr<ExprAST> parse_bitwise_xor();

  // Corresponds to the 'bitwise_and_expr' rule
  std::unique_ptr<ExprAST> parse_bitwise_and();

  // Corresponds to the 'equality_expr' rule
  std::unique_ptr<ExprAST> parse_equality();

  // Corresponds to the 'relational_expr' rule
  std::unique_ptr<ExprAST> parse_relational();

  // Corresponds to the 'bitshift_expr' rule
  std::unique_ptr<ExprAST> parse_bitshift();

  // Corresponds to the 'additive_expr' rule
  std::unique_ptr<ExprAST> parse_additive();

  // Corresponds to the 'term' rule
  std::unique_ptr<ExprAST> parse_term();

  // Corresponds to the 'factor' rule
  std::unique_ptr<ExprAST> parse_factor();

  // Corresponds to the 'statement' rule (only return statements for now)
  std::unique_ptr<StmtAST> parse_statement();

  // Corresponds to the 'function declaration' rule
  std::unique_ptr<FunctionDecl> parse_function();
};
