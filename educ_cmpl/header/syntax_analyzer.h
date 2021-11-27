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
	syntax_analyzer::Result S();
	syntax_analyzer::Result PROGRAM_BODY();
	syntax_analyzer::Result LANG_CONSTRUCT();
	syntax_analyzer::Result VAR_DECL();
	syntax_analyzer::Result TYPE();
	syntax_analyzer::Result IDENT();
	syntax_analyzer::Result FUNC_DEF();
	syntax_analyzer::Result FUNC_DECL();
	syntax_analyzer::Result FUNC_PARAM();
	syntax_analyzer::Result FUNC_BODY();
	syntax_analyzer::Result FUNC_CONSTRUCT();
	syntax_analyzer::Result RETURN_OP();
	syntax_analyzer::Result RETURN_EXPR();
	syntax_analyzer::Result FUNC_CALL();
	syntax_analyzer::Result ARG();
	syntax_analyzer::Result ARYTHM_EXPR();
	syntax_analyzer::Result ARYTHM_OPERATION();
	syntax_analyzer::Result OP();
	syntax_analyzer::Result OPERAND();
	syntax_analyzer::Result CONST_EXPR();
	syntax_analyzer::Result LOGICAL_EXPR();
	syntax_analyzer::Result LOGICAL_OPERATION();
	syntax_analyzer::Result LOGICAL_OP();
	syntax_analyzer::Result LOGICAL_STATEMENT();
	syntax_analyzer::Result LANG_OPERATOR();
	syntax_analyzer::Result WRITE_OP();
	syntax_analyzer::Result WHILE_OP();
	syntax_analyzer::Result IF_OP();
	syntax_analyzer::Result OPERATION();
public:
	syntax_analyzer(LexicalAnalyzer* la);
	void synt_analyz();
	Token get_token();
	~syntax_analyzer();
};