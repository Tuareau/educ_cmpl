#pragma once
#include "lexical_analyzer.h"

class syntax_analyzer
{
private:

public:
	bool left_op();
	bool middle_op();
	bool right_op();
};