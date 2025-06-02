#include "lex.h"

#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>

bool is_numeric(char token) {	
	if (token >= 48 && token <= 57) {
		return true;
	}

	return false;
}

int lex_int(int file_index, std::ifstream& file) {
	int cur_index = file_index + 1;
	while (is_numeric(file.peek())) {
		cur_index++;
	}

	file.seekg(file_index);
	std::string num; num.resize(cur_index - file_index);
	file.read(&num[0], cur_index - file_index);

	return std::stoi(num);
}

bool is_alphabetic(char token) {
	if ((token >= 65 && token <=90) || (token >= 97 && token <= 122)) {
		return true;
	}

	return false;
}

std::string lex_word(int file_index, std::ifstream& file) {
	int cur_index = file_index + 1;
	while (is_alphabetic(file.peek())) {
		cur_index++;
	}

	file.seekg(file_index);
	std::string word; word.resize(cur_index - file_index);
	file.read(&word[0], cur_index - file_index);

	return word;
}

std::vector<Token> lex(const std::string& file_path) {
	std::ifstream c_file (file_path);
	std::vector<Token> file_tokens;
	int file_index = 0;

	if (c_file.bad() || c_file.fail()) {
		throw std::runtime_error("Failed to open source file");
	}

	while (c_file) {
		char cur_char = c_file.get();

		if (cur_char == EOF) { // End of file, return collected tokens
			break;
		} else if (!is_alphabetic(cur_char) && ! is_numeric(cur_char)) { // Single character tokens
			switch (cur_char) {
				case '{':
					file_tokens.push_back(Token(OPEN_BRACE, std::monostate()));
					break;
				case '}':
					file_tokens.push_back(Token(CLOSE_BRACE, std::monostate()));
					break;
				case '(':
					file_tokens.push_back(Token(OPEN_PAREN, std::monostate()));
					break;
				case ')':
					file_tokens.push_back(Token(CLOSE_PAREN, std::monostate()));
					break;
				case ';':
					file_tokens.push_back(Token(SEMICOLON, std::monostate()));
					break;
			}
		} else if (is_numeric(cur_char)) { // Integer literals
			int int_literal = lex_int(file_index, c_file);
			file_tokens.push_back(Token(INT, int_literal));
		} else if (is_alphabetic(cur_char) || is_alphabetic(cur_char)) {
			std::string word = lex_word(file_index, c_file);

			if (word == "return") {
				file_tokens.push_back(Token(RETURN, std::monostate()));
			} else if (word == "int") {
				file_tokens.push_back(Token(INT_TYPE, std::monostate()));
			} else {
				file_tokens.push_back(Token(IDENTIFIER, word));
			}
		}

		file_index++;
	}

	return file_tokens;
}
