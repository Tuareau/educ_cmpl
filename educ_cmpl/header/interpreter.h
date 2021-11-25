#pragma once

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <ostream>
#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

class RpnElement;
class LexicalAnalyzer;
class Token;

class Interpreter
{
private:
	std::vector<RpnElement> _reverse_notation;

	std::vector<std::string> _type_keywords = {
		"integer", "bool", "string",
	};

	std::vector<std::string> _operator_keywords = {
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"write",
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

	static std::string _condition_operator;

	bool is_operator_keyword(const Token & token) const;
	bool is_title_keyword(const Token & token) const;
	bool is_func_keyword(const Token & token) const;
	bool is_const_logic_keyword(const Token & token) const;
	bool is_type_keyword(const Token & token) const;


public:
	Interpreter() = default;

	void construct_notation_output(LexicalAnalyzer * la);
	void execute_notation();

	void print_notation(std::ostream & os) const;

};

std::string Interpreter::_condition_operator = { "!F" };

#endif