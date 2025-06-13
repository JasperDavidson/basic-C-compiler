#include "ast.h"
#include "ast_printer.h"
#include "lex.h"
#include "parser.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <stdexcept>

int main(int argc, char **argv) {
  if (argc != 2) {
    std::cerr << "Error: Must have one argument (the source file)" << std::endl;
    return EXIT_FAILURE;
  }

  std::vector<Token> source_tokens;
  const char *source_filename = argv[1];

  try {
    source_tokens = lex(source_filename);
  } catch (const std::runtime_error &e) {
    std::cerr << "Exception caught: '" << e.what() << "'" << std::endl;
  }

  std::unique_ptr<FunctionDecl> main_func;
  Parser parser(source_tokens);

  try {
    main_func = parser.parse();
  } catch (const std::runtime_error &e) {
    std::cerr << "Exception caught: '" << e.what() << "'" << std::endl;
  }

  AstPrinter printer;
  if (main_func) {
    main_func->accept(&printer);
  }
}
