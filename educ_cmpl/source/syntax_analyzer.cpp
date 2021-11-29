#include "syntax_analyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer* la) 
	: _la(la) {}

void SyntaxAnalyzer::print_error(const std::string & expected, const Token & token) const {
	std::cout << "\n>>> ERROR:\n";
	//std::cout << ">>> found '" << token.value() << "'" << std::endl;
	std::cout << ">>> expected '" << expected << "'" << std::endl;
	std::cout << ">>> line " << token.line() << ": ";
	std::cout << this->_la->get_file_line(token.line()) << std::endl;
}

void SyntaxAnalyzer::print_warning(const std::string & warning) const {
	std::cout  << "\n>>> WARNING: ";
	std::cout << warning << std::endl;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::check_token(const std::string & lexeme, const Token & token) const {
	if (token.value() != lexeme) {
		this->print_error(lexeme, token);
		return Result::ERROR;
	}
	else {
		return Result::SUCCESS;
	}
}

bool SyntaxAnalyzer::analyze_syntax() const {
	auto result = this->START();
	this->_la->reset_tokens_stream();
	if (result == Result::SUCCESS) {
		return true;
	}
	else {
		return false;
	}	
}

SyntaxAnalyzer::Result SyntaxAnalyzer::START() const {
	auto token = this->_la->get_next_token();
	auto checked = this->check_token("PROGRAMM", token);
	if (checked == Result::ERROR) {
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	checked = this->check_token(";", token);
	if (checked == Result::ERROR) {
		return Result::ERROR;
	}

	auto result = this->PROGRAM_BODY();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::NOT_FOUND) {
		this->print_warning("Program body is empty");
	}

	token = this->_la->get_next_token();
	checked = this->check_token("END", token);
	if (checked == Result::ERROR) {
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	checked = this->check_token(".", token);
	if (checked == Result::ERROR) {
		return Result::ERROR;
	}

	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::PROGRAM_BODY() const {
	auto found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	while (result != Result::NOT_FOUND) {
		result = this->LANG_CONSTRUCT();
		if (result == Result::ERROR) {
			return Result::ERROR;
		}
		else if (result == Result::SUCCESS) {
			found = Statement::RECOGNIZED;
		}
	}
	if (found == Statement::RECOGNIZED) {
		result = Result::SUCCESS;
	}
	return result;
	// while LANG_CONSTRUCT found
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_CONSTRUCT() const {
	auto result = this->VAR_DECL();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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

	result = this->ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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

	result = this->LANG_OPERATOR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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
	return result;
	// VAR_DECL ; | ARYTHM_EXPR ; | LANG_OPERATOR ;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::VAR_DECL() const {
	auto result = Result::SUCCESS;
	auto token = this->_la->get_next_token();
	if (token.value() == "integer" || token.value() == "string" || token.value() == "bool") {
		token = this->_la->get_next_token();
		if (token.type() != Token::Type::IDENT) {
			this->print_error("identificator", token);
			result = Result::ERROR;
		}
	}
	else {
		this->_la->unget_token();
		result = Result::NOT_FOUND;
	}
	return result;
	// type ident
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARG() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}
	else if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	// ident | constant
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_EXPR() const {
	auto token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT) {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}

	token = this->_la->get_next_token();
	if (token.value() != "=") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	// ident =

	auto result = this->ARYTHM_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->LOGICAL_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->OPERAND();
	return result;
	// ident = OPERAND | LOGICAL_EXPR | ARYTHM_OPERATION
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_OPERATION() const {
	// collect expression
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
	if (this->balanced_mathematic_expression(expression)) {
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

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERAND() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}
	else if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	// ident | constant
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_EXPR() const {

	auto result = this->LOGICAL_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->LOGICAL_STATEMENT();
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OPERATION() const {

	// collect expression
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
		//else if (token.value() == "true" || token.value() == "false") {
		//	expression.push_back(token);
		//}
		else if (token.value() == ">" || token.value() == "==" || token.value() == "<") {
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
		++tokens_given;
		token = this->_la->get_next_token();
	}

	// check expression
	if (this->balanced_mathematic_expression(expression)) {
		this->_la->unget_token();
		return Result::SUCCESS;
	}
	else {
		// unget
		while (tokens_given--) {
			this->_la->unget_token();
		}
		this->print_error("balanced logical expression", token);
		return Result::ERROR;
	}
}

bool SyntaxAnalyzer::balanced_mathematic_expression(const std::vector<Token> & expression) const {

	// to reverse notation
	std::stack<Token> stack;
	std::vector<Token> output;
	for (auto & token : expression) {
		if (token.type() == Token::Type::IDENT) {
			output.push_back(token);
		}
		else if (token.type() == Token::Type::CONSTANT) {
			output.push_back(token);
		}
		//else if (token.value() == "false" || token.value() == "true") {
		//	output.push_back(token);
		//}
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
	for (auto & token : output) {

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

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_STATEMENT() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}
	else if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else if (token.value() == "true" || token.value() == "false") {
		return Result::SUCCESS;
	}
	else {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	// ident | constant
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_OPERATOR() const {
	auto result = this->WRITE_OP();
	if (result != Result::NOT_FOUND) {
		return result;
	}
	// write

	auto token = this->_la->get_next_token();
	if (token.value() != "endl") {
		this->_la->unget_token();
		result = Result::NOT_FOUND;
	}
	else {
		return Result::SUCCESS;
	}
	// endl

	result = this->WHILE_OP();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->IF_OP();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	return result;
	// WRITE_OP ; | ENDL ; | WHILE_OP ; | IF_OP ;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::WRITE_OP() const {
	auto token = this->_la->get_next_token();
	if (token.value() != "write") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	else {
		auto token = this->_la->get_next_token();
		auto checked = this->check_token("(", token);
		if (checked == Result::ERROR) {
			return Result::ERROR;
		}

		token = this->_la->get_next_token();
		if (token.type() != Token::Type::IDENT 
				&& token.type() != Token::Type::CONSTANT) {
			return Result::ERROR;
		}
		// write ( ident | const

		token = this->_la->get_next_token();
		checked = this->check_token(")", token);
		if (checked == Result::ERROR) {
			return Result::ERROR;
		}
		else {
			return Result::SUCCESS;
		} // write ( ident | const )
	}	
}

SyntaxAnalyzer::Result SyntaxAnalyzer::WHILE_OP() const {
	auto token = this->_la->get_next_token();
	if (token.value() != "WHILE") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	else {
		auto result = this->LOGICAL_EXPR();
		if (result != Result::SUCCESS) {
			return result;
		} // WHILE CONDITION

		token = this->_la->get_next_token();
		result = this->check_token("DO", token);
		if (result == Result::ERROR) {
			return Result::ERROR;
		} // WHILE CONDITION DO

		auto operation_found = Statement::UNRECOGNIZED;
		while (result != Result::NOT_FOUND) {
			result = this->OPERATION();
			if (result == Result::ERROR) {
				return Result::ERROR;
			}
			else if (result == Result::SUCCESS) {
				operation_found = Statement::RECOGNIZED;
			}
		}
		if (operation_found == Statement::RECOGNIZED) {
			result = Result::SUCCESS;
		} // WHILE CONDITION DO OPERATIONS

		token = this->_la->get_next_token();
		result = this->check_token("ENDWHILE", token);
		if (result == Result::ERROR) {
			return Result::ERROR;
		} // WHILE CONDITION DO OPERATIONS ENDWHILE

		return result;		
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::IF_OP() const {
	auto token = this->_la->get_next_token();
	if (token.value() != "if") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}
	else {
		auto result = this->LOGICAL_EXPR();
		if (result != Result::SUCCESS) {
			return result;
		} // if CONDITION

		token = this->_la->get_next_token();
		result = this->check_token("then", token);
		if (result == Result::ERROR) {
			return Result::ERROR;
		} // if WHILE CONDITION then

		auto operation_found = Statement::UNRECOGNIZED;
		while (result != Result::NOT_FOUND) {
			result = this->OPERATION();
			if (result == Result::ERROR) {
				return Result::ERROR;
			}
			else if (result == Result::SUCCESS) {
				operation_found = Statement::RECOGNIZED;
			}
		}
		if (operation_found == Statement::RECOGNIZED) {
			result = Result::SUCCESS;
		} // if CONDITION then OPERATIONS

		// check if there's an else branch
		result = Result::SUCCESS;
		token = this->_la->get_next_token();
		if (token.value() == "else") {
			operation_found = Statement::UNRECOGNIZED;
			while (result != Result::NOT_FOUND) {
				result = this->OPERATION();
				if (result == Result::ERROR) {
					return Result::ERROR;
				}
				else if (result == Result::SUCCESS) {
					operation_found = Statement::RECOGNIZED;
				}
			}
			if (operation_found == Statement::RECOGNIZED) {
				result = Result::SUCCESS;
			}
		} // if CONDITION then OPERATIONS else OPERATIONS
		else {
			this->_la->unget_token();
		}

		token = this->_la->get_next_token();
		result = this->check_token("endif", token);
		return result;
		// if CONDITION then OPERATIONS endif
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERATION() const {
	auto result = this->VAR_DECL();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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

	result = this->ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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

	result = this->LANG_OPERATOR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			auto checked = this->check_token(";", token);
			if (checked == Result::ERROR) {
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

	auto token = this->_la->get_next_token();
	if (token.value() != "EXITWHILE") {
		this->_la->unget_token();
		result = Result::NOT_FOUND;
	}
	else {
		auto token = this->_la->get_next_token();
		auto checked = this->check_token(";", token);
		if (checked == Result::ERROR) {
			return Result::ERROR;
		}
		else {
			return Result::SUCCESS;
		}
	}

	return result;
	// VAR_DECL ; | ARYTHM_EXPR ; | LANG_OPERATOR ; | EXITWHILE ;
}