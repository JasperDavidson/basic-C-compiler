#include "lex.h"

#include <vector>
#include <string>
#include <fstream>
#include <stdexcept>
#include <iostream>

bool is_numeric(char token) {	
	if (token >= 48 && token <= 57) {
		return true;
	}

	return false;
}

int lex_int(int* file_index, std::ifstream& file) {
	int cur_index = *file_index + 1;
	int init_file_index = *file_index;
	while (is_numeric(file.peek())) {
		cur_index++;
		(*file_index)++;
		file.get();
	}

	file.seekg(init_file_index);
	std::string num; num.resize(cur_index - init_file_index);
	file.read(&num[0], cur_index - init_file_index);

	return std::stoi(num);
}

bool is_alphabetic(char token) {
	if ((token >= 65 && token <=90) || (token >= 97 && token <= 122)) {
		return true;
	}

	return false;
}

std::string lex_word(int* file_index, std::ifstream& file) {
	int cur_index = *file_index + 1;
	int init_file_index = *file_index;
	while (is_alphabetic(file.peek())) {
		cur_index++;
		(*file_index)++;
		file.get();
	}

	file.seekg(init_file_index);
	std::string word; word.resize(cur_index - init_file_index);
	file.read(&word[0], cur_index - init_file_index);

	std::cout << "word returned: " << word << '\n';

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
		std::cout << "file index: " << file_index << " char: " << cur_char << '\n';

		if (cur_char == EOF) { // End of file, return collected tokens
			break;
		} else if (!is_alphabetic(cur_char) && ! is_numeric(cur_char)) { // Single character tokens
			switch (cur_char) {
				case '{':
					file_tokens.push_back(Token(TokenType::OPEN_BRACE, std::monostate()));
					break;
				case '}':
					file_tokens.push_back(Token(TokenType::CLOSE_BRACE, std::monostate()));
					break;
				case '(':
					file_tokens.push_back(Token(TokenType::OPEN_PAREN, std::monostate()));
					break;
				case ')':
					file_tokens.push_back(Token(TokenType::CLOSE_PAREN, std::monostate()));
					break;
				case ';':
					file_tokens.push_back(Token(TokenType::SEMICOLON, std::monostate()));
					break;
				case ',':
					file_tokens.push_back(Token(TokenType::COMMA, std::monostate()));
					break;
			}
		} else if (is_numeric(cur_char)) { // Integer literals
			std::cout << "numeric char: " << cur_char << '\n';
			int int_literal = lex_int(&file_index, c_file);
			file_tokens.push_back(Token(TokenType::INT, int_literal));
		} else if (is_alphabetic(cur_char) || is_alphabetic(cur_char)) {
			std::string word = lex_word(&file_index, c_file);

			if (word == "return") {
				file_tokens.push_back(Token(TokenType::RETURN, std::monostate()));
			} else if (word == "int") {
				file_tokens.push_back(Token(TokenType::INT_TYPE, std::monostate()));
			} else if (word == "void") {
				file_tokens.push_back(Token(TokenType::VOID_TYPE, std::monostate()));
			} else {
				file_tokens.push_back(Token(TokenType::IDENTIFIER, word));
			}
		}

		file_index++;
	}

	return file_tokens;
}
