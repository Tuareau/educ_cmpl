#pragma once

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <ostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <stdexcept>

#include "rpn_element.h"

class LexicalAnalyzer;
class Token;

enum class OperationsPriority {
	LOW,
	MIDDLE,
	HIGH,	
};

class Interpreter
{
private:
	std::vector<RpnElement> _reverse_notation;

	std::vector<std::string> _type_keywords = {
		"integer", "bool", "string",
	};

	std::vector<std::string> _operator_keywords = {
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"if", "then", "endif", "else",
	};

	std::vector<std::string> _const_logic_keywords = {
		"true", "false",
	};

	std::vector<std::string> _func_keywords = {
		"FUNC", "ENDF", "return",
	};

	std::vector<std::string> _title_keywords = {
		"PROGRAMM", "END",
	};

	std::vector<std::string> _labels = {
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"IF", "THEN", "ENDIF", "ELSE",
	};

	std::map<std::string, OperationsPriority> _operations_priorities = {
		{ "=", OperationsPriority::LOW },
		{ "==", OperationsPriority::MIDDLE },
		{ "<", OperationsPriority::MIDDLE },
		{ ">", OperationsPriority::MIDDLE },
		{ "+", OperationsPriority::MIDDLE },
		{ "-", OperationsPriority::MIDDLE },
		{ "*", OperationsPriority::HIGH },
		{ "/", OperationsPriority::HIGH },
		{ "write", OperationsPriority::HIGH },
	};

	static std::string _condition_operator;

	bool is_operator_keyword(const Token & token) const;
	bool is_title_keyword(const Token & token) const;
	bool is_func_keyword(const Token & token) const;
	bool is_const_logic_keyword(const Token & token) const;
	bool is_type_keyword(const Token & token) const;

	void note_operator(const Token & token);

public:
	Interpreter() = default;

	void construct_notation_output(LexicalAnalyzer * la);
	//void execute_notation();

	void print_notation(std::ostream & os) const;

};

#endif