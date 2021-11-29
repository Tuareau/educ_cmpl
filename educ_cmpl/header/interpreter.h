#pragma once

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <ostream>
#include <string>
#include <vector>
#include <stack>
#include <map>
#include <stdexcept>
#include <functional>
#include <algorithm>

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
	LexicalAnalyzer * _la;

	std::vector<RpnElement> _reverse_notation;

	std::map<std::string, int> _int_values;
	std::map<std::string, std::string> _str_values;
	std::map<std::string, bool> _bool_values = {
		{ "false", false },
		{ "true", true },
	};

	std::vector<std::string> _type_keywords = {
		"integer", "bool", "string",
	};

	std::vector<std::string> _operator_keywords = {
		"WHILE", "DO", "EXITWHILE", "ENDWHILE",
		"if", "then", "endif", "else", "endl",
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
	static std::string _endline_operator;

	bool is_operator_keyword(const Token & token) const;
	bool is_title_keyword(const Token & token) const;
	bool is_func_keyword(const Token & token) const;
	bool is_const_logic_keyword(const Token & token) const;
	bool is_type_keyword(const Token & token) const;

	void note_operator(const Token & token);

	std::vector<RpnElement>::iterator shift_to_label(std::vector<RpnElement>::iterator pos, const RpnElement & label);

	void execute_integer_operation(const std::string & op, std::stack<RpnElement> & stack);
	void execute_string_operation(const std::string & op, std::stack<RpnElement> & stack);
	void execute_logic_operation(const std::string & op, std::stack<RpnElement> & stack);

public:
	Interpreter(LexicalAnalyzer * la) : _la(la) {}

	void initialize_memory();
	void construct_notation_output();
	void execute_notation();
	static bool is_operation_higher_priority(const std::string& oper1, const std::string& oper2);

	void print_notation(std::ostream & os) const;
	void print_memory(std::ostream & os) const;

};

#endif