#include "interpreter.h"
#include "token.h"
#include "lexical_analyzer.h"
#include "rpn_element.h"

void Interpreter::construct_notation_output(LexicalAnalyzer * la) {
	std::stack<RpnElement> stack;
	auto token = la->get_next_token();
	while (token.valid()) {
		switch (token.type()) {
		case Token::Type::IDENT: 
		{
			const auto & ident_name = token.value();
			if (la->contains_ident(ident_name)) {
				const auto & ident = la->ident_ref(ident_name);
				if (ident.type() == Ident::Type::FUNC) {
					const auto element = RpnElement(RpnElement::Type::FUNC, ident.name());
					stack.push(element);
				}
				else {
					const auto element = RpnElement(RpnElement::Type::OPERAND, ident.name());
					this->_reverse_notation.push_back(element);
				}
			}
			break;
		}
		case Token::Type::CONSTANT:
		{
			auto element = RpnElement(RpnElement::Type::OPERAND, token.value());
			this->_reverse_notation.push_back(element);
			break;
		}
		case Token::Type::KEYWORD:
		{
			if (token.value() == "WHILE") {
				auto element = RpnElement(RpnElement::Type::LABEL, token.value());
				this->_reverse_notation.push_back(element);

			}


			break;
		}
		case Token::Type::OPERATION_SIGN:
		{
			stack.push(RpnElement(RpnElement::Type::OPERATOR, token.value()));
			break;
		}
		case Token::Type::DELIMITER:
		{
			if (token.value() == "(") {
				stack.push(RpnElement(RpnElement::Type::TEMP, token.value()));
			}

			if (token.value() == ";") {
				this->_reverse_notation.push_back(stack.top());
				stack.pop();
			}

			break;
		}
		case Token::Type::NONE:
			throw std::logic_error("Interpreter::construct_notation_output(): token type mismatch");
			break;
		default:
			break;		
		}
	}


}

void Interpreter::print_notation(std::ostream & os) const {
	const auto title_indent = std::setw(20);
	const auto value_indent = std::setw(25);
	os << "\n" << title_indent << "RPN TABLE" << "\n\n";
	os << title_indent << "ELEMENT TYPE" << value_indent << "ELEMENT VALUE" << std::endl;
	for (auto & element : this->_reverse_notation) {
		os << title_indent << RpnElement::type_as_str(element.type());
		os << value_indent << element.name();
		os << std::endl;
	}
}

bool Interpreter::is_operator_keyword(const Token & token) const {
	for (const auto & keyword : this->_operator_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_title_keyword(const Token & token) const {
	for (const auto & keyword : this->_title_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_func_keyword(const Token & token) const {
	for (const auto & keyword : this->_func_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_const_logic_keyword(const Token & token) const {
	for (const auto & keyword : this->_const_logic_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_type_keyword(const Token & token) const {
	for (const auto & keyword : this->_type_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}