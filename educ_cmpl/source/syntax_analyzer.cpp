#include "syntax_analyzer.h"

using std::cout;
using std::cin;
using std::endl;

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer* la) { _la = la; }

bool SyntaxAnalyzer::analyze_syntax()
{
	int i = 0;
	
}

SyntaxAnalyzer::Result SyntaxAnalyzer::START() const
{
	auto result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() != "PROGRAMM") {
		cout << "ERROR! Expected 'PROGRAMM'." << endl;
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";") {
		cout << "ERROR! Expected ';'." << endl;
		return Result::ERROR;
	}
	if ((this->PROGRAM_BODY()) == Result::ERROR) {
		cout << "ERROR in PROGRAM BODY! " << endl;
	}
	if ((this->PROGRAM_BODY()) == Result::NOT_FOUND) {
		cout << "The corresponding token was not found." << endl;	  // не найдена соответствующая лексема
	}
	token = this->_la->get_next_token();
	if (token.value() != "END") {
		cout << "ERROR! Expected 'END'." << endl;
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ".") {
		cout << "ERROR! Expected '.'." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::PROGRAM_BODY() const
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
	if (found == Statement::RECOGNIZED) {
		result = Result::SUCCESS;
	}
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_CONSTRUCT() const
{
	auto result = VAR_DECL();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			if (token.value() != ";") {
				cout << "Expected ';'." << endl;
				return Result::ERROR;
			}
			else {
				return Result::SUCCESS;
			}
		} 
		else {
			return Result::ERROR;
		}
	}

	result = LANG_OPERATOR();
	if (result != Result::NOT_FOUND) {
		return result;
	}
	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::VAR_DECL() const
{
	if (TYPE() == Result::ERROR) {
		return Result::ERROR;
	}
	else if (TYPE() == Result::NOT_FOUND) {
		return Result::NOT_FOUND;
	}
	if (IDENT() == Result::ERROR) {
		return Result::ERROR;
	}
	else if (IDENT() == Result::NOT_FOUND) {
		return Result::NOT_FOUND;
	}
	auto token = this->_la->get_next_token();
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::TYPE() const
{
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT)
		return Result::NOT_FOUND;
	if (token.value() != "integer" || token.value() != "string" || token.value() != "bool")
	{
		cout << "Expected type." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::IDENT() const
{
	auto token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT)
	{
		cout << "Expecter identifier." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARG() const
{
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}
	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_EXPR() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_OP() const
{
	auto token = this->_la->get_next_token();
	if (token.value() != "+" || token.value() != "-" || token.value() != "*" || token.value() != "/") {
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_OPERATION() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERAND() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_EXPR() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OPERATION() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OP() const
{
	auto token = this->_la->get_next_token();
	if (token.value() == ">" || token.value() == "<" || token.value() == "==")
	{
		cout << "Expected logical operator." << endl;
		return Result();
	}
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_STATEMENT() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_OPERATOR() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::WRITE_OP() const
{
	Result result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() != "write") {
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != "(") {
		return Result::ERROR;
	}
	while (1)
	{
		token = this->_la->get_next_token();
		if (token.type() == Token::Type::IDENT || token.type() == Token::Type::CONSTANT)
		{
			this->_la->unget_token();
			result = ARG();
			if (result == Result::ERROR) {
				return Result::ERROR;
			}
		}
		else if (token.value() == ",") {
			continue;
		}
		else if (token.value() == ")")
		{
			this->_la->unget_token();
			break;
		}
	}
	token = this->_la->get_next_token();
	if (token.value() != ")") {
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";") {
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::WHILE_OP() const
{
	Result result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() != "WHILE") {
		return Result::ERROR;
	}
	result = LOGICAL_EXPR();
	if (result == Result::NOT_FOUND || result == Result::ERROR) {
		return Result::ERROR;
	}
	if (token.value() != "DO") {
		return Result::ERROR;
	}
	while (1)
	{
		if ((token = this->_la->get_next_token()).value() == "ENDWHILE")
		{
			this->_la->unget_token();
			break;
		}
		if ((result = OPERATION()) == Result::ERROR) {
			return Result::ERROR;
		}
	}
	token = this->_la->get_next_token();
	if (token.value() != "ENDWHILE") {
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";") {
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::IF_OP() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERATION() const
{
	return Result();
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ENDL_OP() const
{
	auto token = this->_la->get_next_token();
	if (token.value() != "endl") {
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}