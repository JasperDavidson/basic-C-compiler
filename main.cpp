#include "lex.h"
#include <cstdlib>
#include <iostream>
#include <stdexcept>

int main(int argc, char** argv) {
	if (argc < 1) {
		std::cerr << "Error: Must have one argument (the source file)" << std::endl;
		return EXIT_FAILURE;
	} else if (argc > 1) {
		std::cerr << "Error: Must have only one argument (the source file)" << std::endl;
		return EXIT_FAILURE;
	}

	std::vector<Token> source_tokens;

	try {
		source_tokens = lex(argv[0]);
	} catch (const std::runtime_error& e) {
		std::cerr << "Exception caught: " << e.what() << std::endl;
	}
}
