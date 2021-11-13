#include "token.h"

Token::Token()
	: _type(Type::NONE), _value("") {}

Token::Token(Type type, std::string value) 
	: _type(type), _value(value) {}

Token::Type Token::type() const {
	return _type;
}

const std::string & Token::value() const {
	return _value;
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