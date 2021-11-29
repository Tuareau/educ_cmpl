#include "syntax_analyzer.h"
#include "interpreter.h"

using std::cout;
using std::cin;
using std::endl;

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer* la) { _la = la; }

bool SyntaxAnalyzer::analyze_syntax()
{
	if (START() == Result::ERROR) {
		return false;
	}
	else {
		return true;
	}
}

void SyntaxAnalyzer::print_error(const std::string& expected, const Token& token) const {
	std::cout << "\n>>> ERROR:\n";
	//std::cout << ">>> found '" << token.value() << "'" << std::endl;
	std::cout << ">>> expected '" << expected << "'" << std::endl;
	std::cout << ">>> line " << token.line() << ": ";
	std::cout << this->_la->get_file_line(token.line()) << std::endl;
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
	this->PROGRAM_BODY();
	if (result == Result::ERROR) {
		cout << "ERROR in PROGRAM BODY! " << endl;
		return Result::ERROR;
	}
	if (result == Result::NOT_FOUND) {
		cout << "The corresponding token was not found." << endl;	  // не найдена соответствующая лексема
		return Result::NOT_FOUND;
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
	if (result != Result::ERROR) {
		return result;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
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
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::VAR_DECL() const
{
	auto result = TYPE();
	if (result == Result::NOT_FOUND) {
		return Result::NOT_FOUND;
	}
	else if (result == Result::ERROR) {
		return Result::ERROR;
	}
	
	result = IDENT();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	auto token = this->_la->get_next_token();
	if (token.value() != ";") {
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::TYPE() const
{
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::NOT_FOUND;
	}
	if (token.value() != "integer" || token.value() != "string" || token.value() != "bool") {
		cout << "Expected type." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::IDENT() const
{
	auto token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT) {
		cout << "Expected identifier." << endl;
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
	auto token = this->_la->get_next_token();
	auto result = OPERAND();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	
	token = this->_la->get_next_token();
	result = ARYTHM_OPERATION();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}

	token = this->_la->get_next_token();
	result = LOGICAL_EXPR();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	return Result::ERROR;
}

bool SyntaxAnalyzer::balaced_mathematic_expression(const std::vector<Token>& expression) const {

	// to reverse notation
	std::stack<Token> stack;
	std::vector<Token> output;
	for (auto& token : expression) {

		if (token.type() == Token::Type::IDENT) {
			output.push_back(token);
		}
		else if (token.type() == Token::Type::CONSTANT) {
			output.push_back(token);
		}
		else if (token.value() == "false" || token.value() == "true") {
			output.push_back(token);
		}
		else if (token.value() == "(") {
			stack.push(token);
		}
		else if (token.value() == ")") {
			auto popped = false;
			while (!stack.empty()) {
				if (stack.top().value() != "(") {
					output.push_back(stack.top());
					stack.pop();
				}
				else {
					popped = true;
					stack.pop();
					break;
				}
			}
			if (!popped) {
				return false;
			}
		}
		else if (token.type() == Token::Type::OPERATION_SIGN) {
			while (!stack.empty() && stack.top().type() != Token::Type::DELIMITER
				&& !(Interpreter::is_operation_higher_priority(token.value(), stack.top().value()))) {
				output.push_back(stack.top());
				stack.pop();
			}
			stack.push(token);
		}
	}
	while (!stack.empty()) {
		if (stack.top().value() != "(") {
			output.push_back(stack.top());
			stack.pop();
		}
		else {
			return false;
		}
	}

	// preudo execute
	for (auto& token : output) {

		if (token.type() == Token::Type::IDENT) {
			stack.push(token);
		}
		else if (token.type() == Token::Type::CONSTANT) {
			stack.push(token);
		}
		else if (token.value() == "false" || token.value() == "true") {
			stack.push(token);
		}
		else if (token.type() == Token::Type::OPERATION_SIGN) {
			if (stack.size() < 2) { // all operations are binary
				return false;
			}
			else {
				stack.pop(); // 2 opds -> 1 opd
			}
		}
	}

	// error
	if (stack.empty() || stack.size() > 1) {
		return false;
	}

	return true;
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
	std::vector<Token> expression;
	auto token = this->_la->get_next_token();
	size_t tokens_given = 1;

	while (token.value() != ";" && token.value() != "then" && token.value() != "DO") {
		if (token.type() == Token::Type::IDENT) {
			expression.push_back(token);
		}
		else if (token.type() == Token::Type::CONSTANT) {
			expression.push_back(token);
		}
		else if (token.value() == "*" || token.value() == "/"
			|| token.value() == "+" || token.value() == "-") {
			expression.push_back(token);
		}
		else if (token.value() == ")" || token.value() == "(") {
			expression.push_back(token);
		}
		else {
			// unget
			while (tokens_given--) {
				this->_la->unget_token();
			}
			return Result::NOT_FOUND;
		}
		token = this->_la->get_next_token();
		++tokens_given;
	}

	// check expression
	if (this->balaced_mathematic_expression(expression)) {
		this->_la->unget_token();
		return Result::SUCCESS;
	}
	else {
		// unget
		while (tokens_given--) {
			this->_la->unget_token();
		}
		this->print_error("balanced arythmetic expression", token);
		return Result::ERROR;
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERAND() const
{
	auto result = IDENT();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	_la->unget_token();
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else {
		_la->unget_token();
		return Result::NOT_FOUND;
	}
	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_EXPR() const
{
	auto result = LOGICAL_OPERATION();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	result = LOGICAL_STATEMENT();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OPERATION() const
{
	auto result = OPERAND();
	if (result == Result::ERROR) {
		cout << "An error occurred with the operand." << endl;
		return Result::ERROR;
	} 
	else if (result == Result::NOT_FOUND) {
		cout << "Expected any operand." << endl;
		return Result::NOT_FOUND;
	}
	result = LOGICAL_OP();
	if (result == Result::ERROR) {
		cout << "An error occurred with the logical operator." << endl;
		return Result::ERROR;
	}
	else if (result == Result::NOT_FOUND) {
		cout << "Expected any logical operator." << endl;
		return Result::NOT_FOUND;
	}
	result = OPERAND();
	if (result == Result::ERROR) {
		cout << "An error occurred with the operand." << endl;
		return Result::ERROR;
	}
	else if (result == Result::NOT_FOUND) {
		cout << "Expected any operand." << endl;
		return Result::NOT_FOUND;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OP() const
{
	auto token = this->_la->get_next_token();
	if (token.value() != ">" || token.value() != "<" || token.value() != "==") {
		cout << "Expected logical operator." << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_STATEMENT() const
{
	auto result = IDENT();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	_la->unget_token();
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else {
		return Result::NOT_FOUND;
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_OPERATOR() const
{
	auto result = WHILE_OP();
	//auto token = this->_la->get_next_token();
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	result = IF_OP();
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	result = WRITE_OP();
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	result = ENDL_OP();
	if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	return Result::NOT_FOUND;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::WRITE_OP() const
{
	Result result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() != "write") {
		return Result::NOT_FOUND;
	}
	token = this->_la->get_next_token();
	if (token.value() != "(") {
		return Result::ERROR;
	}
	while (1) {
		token = this->_la->get_next_token();
		if (token.type() == Token::Type::IDENT || token.type() == Token::Type::CONSTANT) {
			this->_la->unget_token();
			result = ARG();
			if (result == Result::ERROR) {
				return Result::ERROR;
			}
		}
		else if (token.value() == ",") {
			continue;
		}
		else if (token.value() == ")") {
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
		_la->unget_token();
		return Result::NOT_FOUND;
	}
	result = LOGICAL_EXPR();
	if (result == Result::NOT_FOUND || result == Result::ERROR) {
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != "DO") {
		cout << "Expected 'DO'" << endl;
		return Result::ERROR;
	}
	while (1) {
		if ((token = this->_la->get_next_token()).value() == "ENDWHILE") {
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
	auto result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() != "if") {
		_la->unget_token();
		return Result::NOT_FOUND;
	}
	result = LOGICAL_EXPR();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::NOT_FOUND) {
		return Result::NOT_FOUND;
	}
	token = this->_la->get_next_token();
	if (token.value() != "then") {
		return Result::ERROR;
	}
	while (1) {
		result = OPERATION();
		if (result == Result::NOT_FOUND) {
			break;
		}
		else if (result == Result::ERROR) {
			return Result::ERROR;
		}
	}
	token = this->_la->get_next_token();
	if (token.value() == "endif") {
		token = this->_la->get_next_token();
		if (token.value() == ";") {
			return Result::SUCCESS;
		}
		else {
			return Result::ERROR;
		}
	}
	if (token.value() != "else") {
		cout << "Expected 'else'" << endl;
		return Result::ERROR;
	}
	while (1) {
		result = OPERATION();
		if (result == Result::NOT_FOUND) {
			break;
		} 
		else if (result == Result::ERROR) {
			return Result::ERROR;
		}
	}
	token = this->_la->get_next_token();
	if (token.value() != "endif") {
		cout << "Expected 'endif'" << endl;
		return Result::ERROR;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";") {
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERATION() const
{
	auto result = LANG_OPERATOR();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}

	result = ARYTHM_EXPR();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}

	result = VAR_DECL();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::SUCCESS) {
		return Result::SUCCESS;
	}

	return Result::NOT_FOUND;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ENDL_OP() const
{
	auto token = this->_la->get_next_token();
	if (token.value() != "endl") {
		return Result::NOT_FOUND;
	}
	token = this->_la->get_next_token();
	if (token.value() != ";")
	{
		cout << "Expected ';'" << endl;
		return Result::ERROR;
	}
	return Result::SUCCESS;
}