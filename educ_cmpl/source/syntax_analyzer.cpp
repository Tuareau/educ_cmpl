#include "syntax_analyzer.h"

SyntaxAnalyzer::SyntaxAnalyzer(LexicalAnalyzer* la) 
	: _la(la) {}

void SyntaxAnalyzer::print_error(const std::string & expected, const Token & token) const {
	auto title_indent = std::setw(12);
	auto message_indent = std::setw(25);
	std::cout << title_indent << "ERROR!";
	std::cout << message_indent << "Expected: " << expected << std::endl;
	std::cout << title_indent << "line " << token.line() << ": ";
	std::cout << this->_la->get_file_line(token.line()) << std::endl;
}

void SyntaxAnalyzer::print_warning(const std::string & warning) const {
	auto title_indent = std::setw(12);
	std::cout << title_indent << "WARNING!";
	std::cout << warning << std::endl;
}

bool SyntaxAnalyzer::analyze_syntax() const {
	auto result = this->START();
	if (result == Result::SUCCESS) {
		return true;
	}
	else {
		return false;
	}	
}

SyntaxAnalyzer::Result SyntaxAnalyzer::START() const {
	auto token = this->_la->get_next_token();
	if (token.value() != "PROGRAMM") {
		this->print_error("PROGRAMM", token);
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != ";") {
		this->print_error(";", token);
		return Result::ERROR;
	}

	auto result = PROGRAM_BODY();
	if (result == Result::ERROR) {
		return Result::ERROR;
	}
	else if (result == Result::NOT_FOUND) {
		this->print_warning("Program body is empty");
	}

	token = this->_la->get_next_token();
	if (token.value() != "END") {
		this->print_error("END", token);
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != ".") {
		this->print_error(".", token);
		return Result::ERROR;
	}

	return Result::SUCCESS;
}

/*
	if (result == Result::ERROR) {
		
	}
	else if (result == Result::NOT_FOUND) {
		
	}
	else if (result == Result::SUCCESS) {

	}

		token = this->_la->get_next_token();

*/

SyntaxAnalyzer::Result SyntaxAnalyzer::PROGRAM_BODY() const {
	auto found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	while (result != Result::NOT_FOUND) {
		result = this->LANG_CONSTRUCT();
		if (result == Result::ERROR) {
			return result;
		}
		else if (result == Result::SUCCESS) {
			found = Statement::RECOGNIZED;
		}
	}
	if (found == Statement::RECOGNIZED) {
		result = Result::SUCCESS;
	}
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_CONSTRUCT() const {
	auto result = this->VAR_DECL();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->FUNC_DEF();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->ARYTHM_EXPR(); 
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			if (token.value() != ";") {
				this->print_error(";", token);
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

	result = this->FUNC_CALL();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			if (token.value() != ";") {
				this->print_error(";", token);
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
	return result;
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

		token = this->_la->get_next_token();
		if (token.value() != ";") {
			this->print_error(";", token);
			result = Result::ERROR;
		}

		result = Result::SUCCESS;
	}
	else {
		result = Result::NOT_FOUND;
	}
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_DEF() const {

	auto result = this->FUNC_DECL();
	if (result != Result::SUCCESS) {
		return result;
	}

	result = this->FUNC_BODY();
	if (result != Result::SUCCESS) {
		return Result::ERROR;
	}

	auto token = this->_la->get_next_token();
	if (token.value() != "ENDF") {
		this->print_error("ENDF", token);
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != ";") {
		this->print_error(";", token);
		return Result::ERROR;
	}

	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_DECL() const {

	auto token = this->_la->get_next_token();
	if (token.value() != "FUNC") {
		return Result::NOT_FOUND;
	}

	token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT) {
		this->print_error("identificator", token);
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != "(") {
		this->print_error("(", token);
		return Result::ERROR;
	}

	auto parameter_found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	result = this->FUNC_PARAM();
	while (result != Result::NOT_FOUND) {
		if (parameter_found == Statement::RECOGNIZED) {
			token = this->_la->get_next_token();
			if (token.value() != ",") {
				this->print_error(",", token);
				return Result::ERROR;
			}
		}

		parameter_found = Statement::UNRECOGNIZED;
		if (result == Result::ERROR) {
			return Result::ERROR;
		}
		if (result == Result::SUCCESS) {
			parameter_found = Statement::RECOGNIZED;
		}
		result = this->FUNC_PARAM();
	}

	token = this->_la->get_next_token();
	if (token.value() != ")") {
		this->print_error(")", token);
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != ";") {
		this->print_error(";", token);
		return Result::ERROR;
	}

	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_PARAM() const {
	auto result = Result::NOT_FOUND;
	auto token = this->_la->get_next_token();
	if (token.value() == "integer" || token.value() == "string" || token.value() == "bool") {

		token = this->_la->get_next_token();
		if (token.type() != Token::Type::IDENT) {
			this->print_error("identificator", token);
			result = Result::ERROR;
		}
		result = Result::SUCCESS;
	}
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_BODY() const {
	auto found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	while (result != Result::NOT_FOUND) {
		result = this->FUNC_CONSTRUCT();
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
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_CONSTRUCT() const {
	auto result = this->VAR_DECL();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->RETURN_OP();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			if (token.value() != ";") {
				this->print_error(";", token);
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

	result = this->FUNC_CALL();
	if (result != Result::NOT_FOUND) {
		if (result == Result::SUCCESS) {
			auto token = this->_la->get_next_token();
			if (token.value() != ";") {
				this->print_error(";", token);
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
	return result;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::RETURN_OP() const {
	auto token = this->_la->get_next_token();
	if (token.value() != "return") {
		return Result::NOT_FOUND;
	}

	auto result = this->RETURN_EXPR();
	if (result != Result::SUCCESS) {
		return Result::ERROR;
	}

	token = this->_la->get_next_token();
	if (token.value() != ";") {
		this->print_error(";", token);
		return Result::ERROR;
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::RETURN_EXPR() const {
	auto result = this->FUNC_CALL();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->ARYTHM_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->LOGICAL_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}

	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::FUNC_CALL() const {
	auto token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT) {
		return Result::NOT_FOUND;
	}

	token = this->_la->get_next_token();
	if (token.value() != "(") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}

	auto arg_found = Statement::UNRECOGNIZED;
	auto result = Result::SUCCESS;
	result = this->ARG();
	while (result != Result::NOT_FOUND) {
		if (arg_found == Statement::RECOGNIZED) {
			token = this->_la->get_next_token();
			if (token.value() != ",") {
				this->print_error(",", token);
				return Result::ERROR;
			}
		}

		arg_found = Statement::UNRECOGNIZED;
		if (result == Result::ERROR) {
			return Result::ERROR;
		}
		if (result == Result::SUCCESS) {
			arg_found = Statement::RECOGNIZED;
		}
		result = this->ARG();
	}

	token = this->_la->get_next_token();
	if (token.value() != ")") {
		this->print_error(")", token);
		return Result::ERROR;
	}

	return Result::SUCCESS;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARG() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}

	token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}

	return Result::NOT_FOUND;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_EXPR() const {
	auto token = this->_la->get_next_token();
	if (token.type() != Token::Type::IDENT) {
		return Result::NOT_FOUND;
	}

	token = this->_la->get_next_token();
	if (token.value() != "=") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}

	auto result = this->ARYTHM_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}

	result = this->LOGICAL_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->CONST_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::ARYTHM_OPERATION() const {
	auto result = this->OPERAND();
	if (result != Result::SUCCESS) {
		return Result::NOT_FOUND;
	}

	auto token = this->_la->get_next_token();
	if (token.value() != "+" && token.value() != "-" 
		&& token.value() != "/" && token.value() != "*") {
		return Result::NOT_FOUND;
	}

	result = this->ARYTHM_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->OPERAND();
	if (result != Result::SUCCESS) {
		this->print_error("operand", token);
		return Result::ERROR;
	}

	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERAND() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}

	token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}

	auto result = this->FUNC_CALL();
	if (result != Result::SUCCESS) {
		return Result::NOT_FOUND;
	}
	else {
		return Result::SUCCESS;
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::CONST_EXPR() const {
	auto token = this->_la->get_next_token();
	if (token.type() == Token::Type::CONSTANT) {
		return Result::SUCCESS;
	}
	else {
		return Result::NOT_FOUND;
	}
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_EXPR() const {
	auto result = this->LOGICAL_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->LOGICAL_STATEMENT();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	auto token = this->_la->get_next_token();
	if (token.value() != "(") {
		this->_la->unget_token();
		return Result::NOT_FOUND;
	}

	result = this->ARYTHM_OPERATION();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	token = this->_la->get_next_token();
	if (token.type() == Token::Type::IDENT) {
		return Result::SUCCESS;
	}

	result = this->LOGICAL_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	result = this->CONST_EXPR();
	if (result != Result::NOT_FOUND) {
		return result;
	}

	return Result::ERROR;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OPERATION() const {
	return false;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_OP() const {
	auto token = get_token();
	if (token.value() == ">" || token.value() == "<" || token.value() == "==")
	{
		cout << "Expected logical operator." << endl;
		return false;
	}
	return true;
}

SyntaxAnalyzer::Result SyntaxAnalyzer::LOGICAL_STATEMENT() const {

}

SyntaxAnalyzer::Result SyntaxAnalyzer::LANG_OPERATOR() const {

}

SyntaxAnalyzer::Result SyntaxAnalyzer::WRITE_OP() const {

}

SyntaxAnalyzer::Result SyntaxAnalyzer::WHILE_OP() const {

}

SyntaxAnalyzer::Result SyntaxAnalyzer::IF_OP() const {

}

SyntaxAnalyzer::Result SyntaxAnalyzer::OPERATION() const {

}
