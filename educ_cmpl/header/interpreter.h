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

	std::vector<std::string> _constant_keywords = {
		"true", "false",
	};

	std::vector<std::string> _func_keywords = {
		"FUNC", "ENDF", "return",
	};

	std::vector<std::string> _title_keywords = {
		"PROGRAMM", "END",
	};

	std::vector<std::string> _labels = {
		"PROGRAMM", "END",
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"IF", "THEN", "ENDIF", "ELSE",
	};

public:
	Interpreter() = default;

	void construct_notation_output(LexicalAnalyzer * la);
	void execute_notation();

	void print_notation(std::ostream & os) const;

};

#endif