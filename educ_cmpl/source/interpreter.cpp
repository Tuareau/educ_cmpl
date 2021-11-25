#include "interpreter.h"
#include "token.h"
#include "lexical_analyzer.h"
#include "rpn_element.h"

std::string Interpreter::_condition_operator = { "!COND" };

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
			if (this->is_const_logic_keyword(token)) {
				auto element = RpnElement(RpnElement::Type::OPERAND, token.value());
				this->_reverse_notation.push_back(element);
			}
			else if (this->is_title_keyword(token)) {
				auto element = RpnElement(RpnElement::Type::LABEL, token.value());
				this->_reverse_notation.push_back(element);
			}
			else if (this->is_type_keyword(token)) {
				while (la->get_next_token().value() != ";") {
					continue;
				}
			}
			else if (this->is_func_keyword(token)) {
				// i don't know what to do
			}
			else if (this->is_operator_keyword(token)) {
				auto & name = token.value();
				if (name == "then" || "DO") {
					while (!stack.empty()) {
						this->_reverse_notation.push_back(stack.top());
						stack.pop();
					}
				}
				this->note_operator(token);
			}
			else if (token.value() == "write") {
				stack.push(RpnElement(RpnElement::Type::OPERATOR, token.value()));
			}
			else {
				throw std::logic_error("Interpreter::construct_notation_output(): unknown type of KEYWORD");
			}

			break;
		}
		case Token::Type::OPERATION_SIGN:
		{
			auto op1 = token.value();
			while (!stack.empty()) {
				if (stack.top().type() == RpnElement::Type::OPERATOR) {
					auto op2 = stack.top().name();
					auto op2_priority = this->_operations_priorities.at(op2);
					auto op1_priority = this->_operations_priorities.at(op1);
					if (op2_priority >= op1_priority) {
						auto element = RpnElement(RpnElement::Type::OPERATOR, op2);
						this->_reverse_notation.push_back(element);
						stack.pop();
					}
					else {
						break;
					}
				}
				else if (stack.top().type() == RpnElement::Type::FUNC) {
					auto element = RpnElement(RpnElement::Type::FUNC, stack.top().name());
					this->_reverse_notation.push_back(element);
					stack.pop();
				}
				else {
					break;
				}
			}
			auto element = RpnElement(RpnElement::Type::OPERATOR, op1);
			stack.push(element);
			break;
		}
		case Token::Type::DELIMITER:
		{
			if (token.value() == "(") {
				stack.push(RpnElement(RpnElement::Type::DELIMITER, token.value()));
			}
			else if (token.value() == ";") {
				while (!stack.empty()) {
					this->_reverse_notation.push_back(stack.top());
					stack.pop();
				}
			}
			else if (token.value() == ")") {
				auto parenthesis_found = false;
				while (!stack.empty()) {
					if (stack.top().name() != "(") {
						this->_reverse_notation.push_back(stack.top());						
					}
					else {
						parenthesis_found = true;
						stack.pop();
						break;
					}
					stack.pop();
				}
				if (!parenthesis_found) {
					throw std::logic_error("Interpreter::construct_notation_output(): closing parenthesis not found");
				}
			}

			break;
		}
		case Token::Type::NONE:
			throw std::logic_error("Interpreter::construct_notation_output(): token type was NONE");
			break;
		default:
			throw std::logic_error("Interpreter::construct_notation_output(): token type mismatch");
			break;		
		}

		token = la->get_next_token();
	}

	while (!stack.empty()) {
		this->_reverse_notation.push_back(stack.top());
		stack.pop();
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

void Interpreter::note_operator(const Token & token) {
	const auto & keyword = token.value();
	if (keyword == "WHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("WHILE"));
	}
	else if (keyword == "DO") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ENDWHILE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::OPERATOR, this->_condition_operator);
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("DO"));
	}
	else if (keyword == "if") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("IF"));
	}
	else if (keyword == "then") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ELSE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::OPERATOR, this->_condition_operator);
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("THEN"));
	}
	else if (keyword == "EXITWHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ENDWHILE"));
	}
	else if (keyword == "else") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ELSE"));
	}
	else if (keyword == "endif") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ENDIF"));
	}
	else if (keyword == "ENDWHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$WHILE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ENDWHILE"));
	}
	else {
		throw std::logic_error("Interpreter::note_operator(): operator's keyword mismatch");
	}
}