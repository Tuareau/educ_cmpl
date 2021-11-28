#pragma once
#include "lexical_analyzer.h"

class syntax_analyzer
{
private:
	LexicalAnalyzer* LexA;
	enum class Result {
		SUCCESS, ERROR, NOT_FOUND
	};
	enum class Statement {
		RECOGNIZED, UNRECOGNIZED
	};
	Result S();
	Result PROGRAM_BODY();
	Result LANG_CONSTRUCT();
	Result VAR_DECL();
	Result TYPE();
	Result IDENT();
	Result ARG();
	Result ARYTHM_EXPR();
	Result ARYTHM_OPERATION();
	Result ARYTHM_OP();
	Result OPERAND();
	Result LOGICAL_EXPR();
	Result LOGICAL_OPERATION();
	Result LOGICAL_OP();
	Result LOGICAL_STATEMENT();
	Result LANG_OPERATOR();
	Result WRITE_OP();
	Result WHILE_OP();
	Result IF_OP();
	Result OPERATION();
	Result ENDL_OP();
public:
	syntax_analyzer(LexicalAnalyzer* la);
	bool synt_analyz();
	Token get_token();
	void unget_token();
	~syntax_analyzer();
};