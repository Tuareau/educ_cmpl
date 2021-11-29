#pragma once

#include <stack>
#include <vector>

#include "interpreter.h"
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

	Result ARG() const;
	Result OPERAND() const;

	Result ARYTHM_OPERATION() const;

	Result LOGICAL_EXPR() const;
	Result LOGICAL_OPERATION() const;
	Result LOGICAL_STATEMENT() const;

	Result WRITE_OP() const;
	Result WHILE_OP() const;
	Result IF_OP() const;
	Result OPERATION() const;

	void print_error(const std::string & expected, const Token & token) const;
	void print_warning(const std::string & warning) const;

	Result check_token(const std::string & lexeme, const Token & token) const;

	bool balanced_mathematic_expression(const std::vector<Token> & expression) const;

public:
	SyntaxAnalyzer() = delete;
	SyntaxAnalyzer(LexicalAnalyzer * la);

	bool analyze_syntax() const;

};