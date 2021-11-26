#include "syntax_analyzer.h"

using std::cout;
using std::cin;
using std::endl;
//typedef enum class RET_ERR {
//	OK, TYPE, OPERATOR, OPERATION, KEYWORDS, DELIMITERS, OPER_SIGN
//}ret;

syntax_analyzer::~syntax_analyzer()
{

}

syntax_analyzer::syntax_analyzer(LexicalAnalyzer* la) { LexA = la; }

void syntax_analyzer::synt_analyz()
{
	int i = 0;
	
}

Token syntax_analyzer::get_token() { return (LexA->get_next_token()); }

bool syntax_analyzer::S()
{
	auto token = get_token();
	if (token.value() != "PROGRAMM") {
		cout << "ERROR! Expected 'PROGRAMM'." << endl;
		return false;
	}
	token = get_token();
	if (token.value() != ";") {
		cout << "ERROR! Expected ';'." << endl;
		return false;
	}
	PROGRAM_BODY();
	token = get_token();
	if (token.value() != "END") {
		cout << "ERROR! Expected 'END'." << endl;
		return false;
	}
	token = get_token();
	if (token.value() != ".") {
		cout << "ERROR! Expected '.'." << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::PROGRAM_BODY()
{
	if (!LANG_CONSTRUCT())
		return false;
	return true;
}

bool syntax_analyzer::LANG_CONSTRUCT()
{
	VAR_DECL();
	FUNC_DEF();
	ARYTHM_EXPR(); get_token() != ";";
	FUNC_CALL(); get_token() != ";";
	LANG_OPERATOR();
}

bool syntax_analyzer::VAR_DECL()
{
	auto token = get_token();
	TYPE();
	IDENT();
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::TYPE()
{
	auto token = get_token();
	if (token.value() != "integer" || token.value() != "string" || token.value() != "bool")
	{
		cout << "Expected type." << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::IDENT()
{
	auto token = get_token();
	if (token.type() != Token::Type::IDENT)
	{
		cout << "Expecter identifier." << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::FUNC_DEF()
{
	auto token = get_token();
	FUNC_DECL();
	FUNC_BODY();
	if (token.value() != "ENDF")
	{
		cout << "Expected 'ENDF'" << endl;
		return false;
	}
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::FUNC_DECL()
{
	return false;
}

bool syntax_analyzer::FUNC_PARAM()
{
	return false;
}

bool syntax_analyzer::FUNC_BODY()
{
	return false;
}

bool syntax_analyzer::FUNC_CONSTRUCT()
{
	return false;
}

bool syntax_analyzer::RETURN_OP()
{
	return false;
}

bool syntax_analyzer::RETURN_EXPR()
{
	return false;
}

bool syntax_analyzer::FUNC_CALL()
{
	return false;
}

bool syntax_analyzer::ARG()
{
	return false;
}

bool syntax_analyzer::ARYTHM_EXPR()
{
	return false;
}

bool syntax_analyzer::ARYTHM_OPERATION()
{
	return false;
}

bool syntax_analyzer::OP()
{
	auto token = get_token();
	if (token.type() != Token::Type::OPERATION_SIGN)
	{
		cout << "Expected operation sign." << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::OPERAND()
{
	return false;
}

bool syntax_analyzer::CONST_EXPR()
{
	auto token = get_token();
	if (token.type() != Token::Type::CONSTANT)
	{
		cout << "Expected constant!" << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::LOGICAL_EXPR()
{
	return false;
}

bool syntax_analyzer::LOGICAL_OPERATION()
{
	return false;
}

bool syntax_analyzer::LOGICAL_OP()
{
	auto token = get_token();
	if (token.value() == ">" || token.value() == "<" || token.value() == "==")
	{
		cout << "Expected logical operator." << endl;
		return false;
	}
	return true;
}

bool syntax_analyzer::LOGICAL_STATEMENT()
{
	return false;
}

bool syntax_analyzer::LANG_OPERATOR()
{
	return false;
}

bool syntax_analyzer::WRITE_OP()
{
	return false;
}

bool syntax_analyzer::WHILE_OP()
{
	return false;
}

bool syntax_analyzer::IF_OP()
{
	return false;
}

bool syntax_analyzer::OPERATION()
{
	return false;
}
