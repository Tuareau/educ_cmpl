#include "interpreter.h"
#include "token.h"
#include "lexical_analyzer.h"

void Interpreter::construct_notation_output(LexicalAnalyzer * la) {
	std::stack<RpnElement> stack;
	auto token = la->get_next_token();
	while (token.valid()) {
		switch (token.type()) {
		case Token::Type::IDENT:
			this->_reverse_notation.push_back(RpnElement(RpnElement::Type::OPERAND, token.value()));
			break;
		case Token::Type::CONSTANT:
			this->_reverse_notation.push_back(RpnElement(RpnElement::Type::OPERAND, token.value()));
			break;
		case Token::Type::KEYWORD:



			break;
		case Token::Type::OPERATION_SIGN:



			break;

		case Token::Type::DELIMITER:


			break;

		case Token::Type::NONE:
			throw std::logic_error("Interpreter::construct_notation_output(): token type mismatch");
			break;
		default:
			break;		
		}
	}


}