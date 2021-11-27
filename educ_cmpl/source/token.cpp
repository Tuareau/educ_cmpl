#include "token.h"

Token::Token()
	: _type(Type::NONE), _value(""), _line(0) {}

Token::Token(Type type, size_t line, std::string value) 
	: _type(type), _value(value), _line(line) {}

Token::Type Token::type() const {
	return _type;
}

const std::string & Token::value() const {
	return _value;
}

size_t Token::line() const {
	return _line;
}

std::string Token::type_to_str(Type type) {
	std::string str;
	switch (type) {
	case Type::KEYWORD:
		str = "KEYWORD";
		break;
	case Type::DELIMITER:
		str = "DELIMITER";
		break;
	case Type::IDENT:
		str = "IDENT";
		break;
	case Type::CONSTANT:
		str = "CONSTANT";
		break;
	case Type::OPERATION_SIGN:
		str = "OPERATION_SIGN";
		break;
	default:
		throw std::invalid_argument("Token::type_to_str(): token type mismatch");
		break;
	}
	return str;
}

bool Token::valid() const {
	return this->_type != Type::NONE;
}