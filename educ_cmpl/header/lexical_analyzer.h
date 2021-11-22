#pragma once

#ifndef _LEXICAL_ANALYZER_H_
#define _LEXICAL_ANALYZER_H_

#include <fstream>
#include <iostream>
#include <iomanip>
#include <string>
#include <map>
#include <vector>
#include <stdexcept>
#include <cctype>
#include <sstream>

#include "token.h"

//class Ident;
//class IdentTable;

class LexicalAnalyzer
{
private:
	std::map<size_t, Token> _token_table;

	size_t _tokens_stream_pos;

	std::vector<std::string> _keywords = {
		"PROGRAMM", "END",
		"integer", "bool", "string",
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"FUNC", "ENDF", "return", "write",
		"if", "then", "endif", "else",
		"true", "false",
	};

	std::vector<std::string> _operation_signs = {
		"=", "==", "<", ">",
		"+", "-", "*", "/",
	};

	std::vector<std::string> _delimiters = {
		"(", ")", ",", ";", ".",
	};

	enum class State {
		START, IDENT, CONSTANT, OPERATION_SIGN, DELIMITER, COMMENT, ERROR,
	};
	State _state = State::START;

	bool is_operation_sign(const char c) const;
	bool is_delimiter(const char c) const;
	bool is_keyword(const std::string & str) const;

public:
	LexicalAnalyzer();

	void construct_token_table(const std::string & filename);
	//IdentTable * construct_ident_table(const std::string & filename);

	void print_token_table(std::ostream & os) const;

	Token get_next_token();
	void reset_tokens_stream();
	bool token_stream_ended() const;

	size_t tokens_count() const;
	Token get_token(size_t key) const;

};

#endif
