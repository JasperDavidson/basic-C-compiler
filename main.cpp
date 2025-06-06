#include "lex.h"
#include "ast.h"
#include "parser.h"

#include <memory>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
	if (argc != 2) {
		std::cerr << "Error: Must have one argument (the source file)" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Token> source_tokens;
	const char* source_filename = argv[1];

	try {
		source_tokens = lex(source_filename);

		for (Token token : source_tokens) {
			std::cout << token.token_type << '\n';
		}
	} catch (const std::runtime_error& e) {
		std::cerr << "Exception caught: '" << e.what() << "'" << std::endl;
	}

	Parser parser(source_tokens);
	
	try {
		std::unique_ptr<FunctionDecl> main_func = parser.parse();
	} catch (const std::runtime_error& e) {
		std::cerr << "Exception caught: '" << e.what() << "'" << std::endl;
	}
}
