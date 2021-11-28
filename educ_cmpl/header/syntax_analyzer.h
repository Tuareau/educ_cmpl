#pragma once

#include "lexical_analyzer.h"
#include "token.h"

class SyntaxAnalyzer
{
private:
	LexicalAnalyzer * _la;

	enum class Result {
		ERROR,
		SUCCESS,
		NOT_FOUND,
	};

	enum class Statement {
		RECOGNIZED,
		UNRECOGNIZED,
	};

	Result START() const;
	Result PROGRAM_BODY() const;
	Result LANG_CONSTRUCT() const;

	Result VAR_DECL() const;
	Result ARYTHM_EXPR() const;
	Result LANG_OPERATOR() const;

	Result FUNC_DEF() const;
	Result FUNC_DECL() const;
	Result FUNC_PARAM() const;
	Result FUNC_BODY() const;
	Result FUNC_CONSTRUCT() const;
	Result RETURN_OP() const;
	Result RETURN_EXPR() const;
	Result FUNC_CALL() const;
	Result ARG() const;


	Result ARYTHM_OPERATION() const;
	Result OPERAND() const;
	Result CONST_EXPR() const;

	Result LOGICAL_EXPR() const;
	//Result LOGICAL_OPERATION() const;
	//Result LOGICAL_OP() const;
	//Result LOGICAL_STATEMENT() const;

	//Result LANG_OPERATOR() const;
	//Result WRITE_OP() const;
	//Result WHILE_OP() const;
	//Result IF_OP() const;
	//Result OPERATION() const;

	void print_error(const std::string & expected, const Token & token) const;
	void print_warning(const std::string & warning) const;

public:
	SyntaxAnalyzer() = delete;
	SyntaxAnalyzer(LexicalAnalyzer * la);

	bool analyze_syntax() const;

};