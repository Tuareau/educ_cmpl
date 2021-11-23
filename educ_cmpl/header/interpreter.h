#pragma once

#ifndef _INTERPRETER_H_
#define _INTERPRETER_H_

#include <string>
#include <vector>
#include <stack>
#include <stdexcept>

#include "rpn_element.h"

class LexicalAnalyzer;

class Interpreter
{
private:
	std::vector<RpnElement> _reverse_notation;

public:
	Interpreter() = default;

	void construct_notation_output(LexicalAnalyzer * la);
	void execute_notation();

};

#endif