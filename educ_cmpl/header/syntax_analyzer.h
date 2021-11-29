#pragma once
#include "lexical_analyzer.h"

class SyntaxAnalyzer
{
private:
	LexicalAnalyzer* _la;
	enum class Result {
		SUCCESS, ERROR, NOT_FOUND
	};
	enum class Statement {
		RECOGNIZED, UNRECOGNIZED
	};
	Result START() const;
	Result PROGRAM_BODY() const;
	Result LANG_CONSTRUCT() const;
	Result VAR_DECL() const;
	Result TYPE() const;
	Result IDENT() const;
	Result ARG() const;
	Result ARYTHM_EXPR() const;
	Result ARYTHM_OPERATION() const;
	Result ARYTHM_OP() const;
	Result OPERAND() const;
	Result LOGICAL_EXPR() const;
	Result LOGICAL_OPERATION() const;
	Result LOGICAL_OP() const;
	Result LOGICAL_STATEMENT() const;
	Result LANG_OPERATOR() const;
	Result WRITE_OP() const;
	Result WHILE_OP() const;
	Result IF_OP() const;
	Result OPERATION() const;
	Result ENDL_OP() const;
public:
	SyntaxAnalyzer(LexicalAnalyzer* la);
	bool analyze_syntax();
};