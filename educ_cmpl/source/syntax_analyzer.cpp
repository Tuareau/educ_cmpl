#include "syntax_analyzer.h"

using std::cout;
using std::cin;
using std::endl;

syntax_analyzer::~syntax_analyzer()
{

}

syntax_analyzer::syntax_analyzer(LexicalAnalyzer* la) { LexA = la; }

void syntax_analyzer::synt_analyz()
{
	int i = 0;
	
}

Token syntax_analyzer::get_token() { return (LexA->get_next_token()); }

syntax_analyzer::Result syntax_analyzer::S()
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

syntax_analyzer::Result syntax_analyzer::PROGRAM_BODY()
{
	auto found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	while (result != Result::NOT_FOUND) {
		result = this->LANG_CONSTRUCT();
		if (result == Result::ERROR)
			return result;
		else if (result == Result::SUCCESS)
			found = Statement::RECOGNIZED;
	}
	if (found == Statement::RECOGNIZED)
		result = Result::SUCCESS;
	return result;
}

syntax_analyzer::Result syntax_analyzer::LANG_CONSTRUCT()
{
	auto result = VAR_DECL();
	if (result != Result::NOT_FOUND)
		return result;

	result = FUNC_DEF();
	if (result != Result::NOT_FOUND)
		return result;

	result = ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = get_token();
			if (token.value() != ";") {
				cout << "Expected ';'." << endl;
				return Result::ERROR;
			}
			else
				return Result::SUCCESS;
		} else {
			return Result::ERROR;
		}
	}

	result = FUNC_CALL();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = get_token();
			if (token.value() != ";") {
				cout << "Expected ';'." << endl;
				return Result::ERROR;
			}
			else
				return Result::SUCCESS;
		}
		else {
			return Result::ERROR;
		}
	}

	result = LANG_OPERATOR();
	return result;
}

syntax_analyzer::Result syntax_analyzer::VAR_DECL()
{
	TYPE();
	IDENT();
	auto token = get_token();
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::TYPE()
{
	auto token = get_token();
	if (token.value() != "integer" || token.value() != "string" || token.value() != "syntax_analyzer::Result")
	{
		cout << "Expected type." << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::IDENT()
{
	auto token = get_token();
	if (token.type() != Token::Type::IDENT)
	{
		cout << "Expecter identifier." << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::FUNC_DEF()
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

syntax_analyzer::Result syntax_analyzer::FUNC_DECL()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::FUNC_PARAM()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::FUNC_BODY()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::FUNC_CONSTRUCT()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::RETURN_OP()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::RETURN_EXPR()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::FUNC_CALL()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::ARG()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::ARYTHM_EXPR()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::ARYTHM_OPERATION()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::OP()
{
	auto token = get_token();
	if (token.type() != Token::Type::OPERATION_SIGN)
	{
		cout << "Expected operation sign." << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::OPERAND()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::CONST_EXPR()
{
	auto token = get_token();
	if (token.type() != Token::Type::CONSTANT)
	{
		cout << "Expected constant!" << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_EXPR()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_OPERATION()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_OP()
{
	auto token = get_token();
	if (token.value() == ">" || token.value() == "<" || token.value() == "==")
	{
		cout << "Expected logical operator." << endl;
		return false;
	}
	return true;
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_STATEMENT()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::LANG_OPERATOR()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::WRITE_OP()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::WHILE_OP()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::IF_OP()
{
	return false;
}

syntax_analyzer::Result syntax_analyzer::OPERATION()
{
	return false;
}
