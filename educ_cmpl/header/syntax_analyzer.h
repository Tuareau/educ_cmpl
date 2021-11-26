#pragma once
#include "lexical_analyzer.h"

class syntax_analyzer
{
private:
	LexicalAnalyzer* LexA;
	/*typedef enum class bool {
		OK, TYPE, OPERATOR, OPERATION, KEYWORDS, DELIMITERS, OPER_SIGN
	}ret;*/
	bool S();
	bool PROGRAM_BODY();
	bool LANG_CONSTRUCT();
	bool VAR_DECL();
	bool TYPE();
	bool IDENT();
	bool FUNC_DEF();
	bool FUNC_DECL();
	bool FUNC_PARAM();
	bool FUNC_BODY();
	bool FUNC_CONSTRUCT();
	bool RETURN_OP();
	bool RETURN_EXPR();
	bool FUNC_CALL();
	bool ARG();
	bool ARYTHM_EXPR();
	bool ARYTHM_OPERATION();
	bool OP();
	bool OPERAND();
	bool CONST_EXPR();
	bool LOGICAL_EXPR();
	bool LOGICAL_OPERATION();
	bool LOGICAL_OP();
	bool LOGICAL_STATEMENT();
	bool LANG_OPERATOR();
	bool WRITE_OP();
	bool WHILE_OP();
	bool IF_OP();
	bool OPERATION();
public:
	syntax_analyzer(LexicalAnalyzer* la);
	void synt_analyz();
	Token get_token();
	~syntax_analyzer();
};