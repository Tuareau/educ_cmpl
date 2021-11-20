#include "ident.h"

Ident::Ident()
	: _name(""), _value(""), _type(Type::NONE) {}

Ident::Ident(Type type, const std::string & value, const std::string & name)
	: _name(name), _value(value), _type(type) {}

Ident::Type Ident::type() const {
	return this->_type;
}

const std::string & Ident::value() const {
	return this->_value;
}

std::string Ident::type_to_str(Type type) {
	std::string str;
	switch (type) {
	case Type::BOOL:
		str = "BOOL";
		break;
	case Type::INTEGER:
		str = "INTEGER";
		break;
	case Type::STRING:
		str = "STRING";
		break;
	case Type::FUNC:
		str = "FUNC";
		break;
	case Type::NONE:
		str = "NONE";
		break;
	default:
		throw std::invalid_argument("Ident<T>::type_to_str(): ident type mismatch");
		break;
	}
	return str;
}
Ident::Type Ident::str_to_type(std::string str) {
	if (str == "bool")
		return Ident::Type::BOOL;
	else 	if (str == "integer")
		return Ident::Type::INTEGER;
	else 	if (str == "string")
		return Ident::Type::STRING;
	else 	if (str == "FUNC")
		return Ident::Type::FUNC;
	else
		return Ident::Type::NONE;

}