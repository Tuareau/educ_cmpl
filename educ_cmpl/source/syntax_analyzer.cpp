#include "syntax_analyzer.h"

using std::cout;
using std::cin;
using std::endl;

syntax_analyzer::~syntax_analyzer() {}

syntax_analyzer::syntax_analyzer(LexicalAnalyzer* la) { LexA = la; }

bool syntax_analyzer::synt_analyz()
{
	int i = 0;
	
}

Token syntax_analyzer::get_token() { return (LexA->get_next_token()); }

void syntax_analyzer::unget_token() { LexA->unget_token(); }

syntax_analyzer::Result syntax_analyzer::S()
{
	auto result = Result::SUCCESS;
	auto token = get_token();
	if (token.valid() == false) {
		return Result::NOT_FOUND;
	}
	if (token.value() != "PROGRAMM") {
		cout << "ERROR! Expected 'PROGRAMM'." << endl;
		return Result::ERROR;
	}
	token = get_token();
	if (token.value() != ";") {
		cout << "ERROR! Expected ';'." << endl;
		return Result::ERROR;
	}
	PROGRAM_BODY();
	token = get_token();
	if (token.value() != "END") {
		cout << "ERROR! Expected 'END'." << endl;
		return Result::ERROR;
	}
	token = get_token();
	if (token.value() != ".") {
		cout << "ERROR! Expected '.'." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
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

	result = LANG_OPERATOR();
	if (result != Result::NOT_FOUND)
		return result;
	return Result::ERROR;
}

syntax_analyzer::Result syntax_analyzer::VAR_DECL()
{
	if (TYPE() == Result::ERROR)
		return Result::ERROR;
	else if (TYPE() == Result::NOT_FOUND)
		return Result::NOT_FOUND;
	if (IDENT() == Result::ERROR)
		return Result::ERROR;
	else if (IDENT() == Result::NOT_FOUND)
		return Result::NOT_FOUND;
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::TYPE()
{
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "integer" || token.value() != "string" || token.value() != "bool")
	{
		cout << "Expected type." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::IDENT()
{
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.type() != Token::Type::IDENT)
	{
		cout << "Expecter identifier." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::ARG()
{
	auto token = get_token();

	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.type() == Token::Type::CONSTANT)
		return Result::SUCCESS;
	if (token.type() == Token::Type::IDENT)
		return Result::SUCCESS;
	return Result::ERROR;
}

syntax_analyzer::Result syntax_analyzer::ARYTHM_EXPR()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::ARYTHM_OP()
{
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "+" || token.value() != "-" || token.value() != "*" || token.value() != "/")
		return Result::ERROR;
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::ARYTHM_OPERATION()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::OPERAND()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_EXPR()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_OPERATION()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_OP()
{
	auto token = get_token();
	if (token.value() == ">" || token.value() == "<" || token.value() == "==")
	{
		cout << "Expected logical operator." << endl;
		return Result();
	}
	return Result();
}

syntax_analyzer::Result syntax_analyzer::LOGICAL_STATEMENT()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::LANG_OPERATOR()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::WRITE_OP()
{
	Result result = Result::SUCCESS;
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "write")
		return Result::ERROR;
	token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "(")
		return Result::ERROR;
	while (1)
	{
		token = get_token();
		if (token.type() == Token::Type::IDENT || token.type() == Token::Type::CONSTANT)
		{
			unget_token();
			result = ARG();
			if (result == Result::ERROR)
				return Result::ERROR;
		}
		else if (token.value() == ",")
			continue;
		else if (token.value() == ")")
		{
			unget_token();
			break;
		}
	}
	token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != ")")
		return Result::ERROR;
	token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != ";")
		return Result::ERROR;
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::WHILE_OP()
{
	Result result = Result::SUCCESS;
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "WHILE")
		return Result::ERROR;
	result = LOGICAL_EXPR();
	if (result == Result::NOT_FOUND || result == Result::ERROR)
		return Result::ERROR;
	if (token.value() != "DO")
		return Result::ERROR;
	while (1)
	{
		if ((token = get_token()).value() == "ENDWHILE")
		{
			unget_token();
			break;
		}
		if ((result = OPERATION()) == Result::ERROR)
			return Result::ERROR;
	}
	token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "ENDWHILE")
		return Result::ERROR;
	token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != ";")
		return Result::ERROR;
	return Result::SUCCESS;
}

syntax_analyzer::Result syntax_analyzer::IF_OP()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::OPERATION()
{
	return Result();
}

syntax_analyzer::Result syntax_analyzer::ENDL_OP()
{
	auto token = get_token();
	if (token.valid() == false)
		return Result::NOT_FOUND;
	if (token.value() != "endl")
		return Result::ERROR;
	token = get_token();
	if (token.valid() == false)
	{
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}
