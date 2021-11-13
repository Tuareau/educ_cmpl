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

	std::vector<std::string> _keywords = {
	"PROGRAMM", "END",
	"integer", "bool", "string",
	"WHILE", "DO", "EXITWHILE", "ENDWHILE",
	"FUNC", "ENDF", "return", "write",
	"if", "then", "endif",
	"true", "false",
	};

	enum class State {
		START, IDENT, CONSTANT, OPERATION_SIGN, DELIMITER, COMMENT, ERROR
	};
	State _state = State::START;

public:
	LexicalAnalyzer() = default;

	void construct_token_table(const std::string & filename);
	//IdentTable * construct_ident_table(const std::string & filename);

	void print_token_table() const;

};

#endif
