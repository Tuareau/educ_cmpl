#include "constant.h"

Constant::Constant()
	: _value(""), _type(Type::NONE) {}

Constant::Constant(Type type, const std::string & value)
	: _value(value), _type(type) {}

Constant::Type Constant::type() const {
	return this->_type;
}

const std::string & Constant::value() const {
	return this->_value;
}

std::string Constant::type_to_str(Type type) {
	std::string str;
	switch (type) {
	case Type::INTEGER:
		str = "INTEGER";
		break;
	case Type::STRING:
		str = "STRING";
		break;
	case Type::NONE:
		str = "NONE";
		break;
	default:
		throw std::invalid_argument("Constant::type_to_str(): constant type mismatch");
		break;
	}
	return str;
}