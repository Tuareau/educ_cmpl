#pragma once

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string>
#include <vector>
#include <stdexcept>

class LexicalAnalyzer;

class Interpreter
{
private:
	std::vector<std::string> _reverse_notation;

public:
	Interpreter();

	void construct_notation_output(LexicalAnalyzer * la);
	void execute_notation();

};

#endif