#pragma once

#ifndef _RPN_ELEMENT_H_
#define _RPN_ELEMENT_H_

#include <string>

class RpnElement
{
public:
	enum class Type {
		OPERAND_IDENT,
		OPERAND_CONSTANT,
		OPERATOR,
		LABEL,
		GOTO,
		FUNC,
		DELIMITER,
	};

private:
	Type _type;
	std::string _name;

public:
	RpnElement() = delete;
	RpnElement(Type type, const std::string & name)
		: _type(type), _name(name) {}

	const std::string name() const {
		return this->_name;
	}

	RpnElement::Type type() const {
		return this->_type;
	}

	static std::string type_as_str(Type type) {
		std::string str;
		switch (type) {
		case Type::FUNC:
			str = "FUNC";
			break;
		case Type::GOTO:
			str = "GOTO";
			break;
		case Type::LABEL:
			str = "LABEL";
			break;
		case Type::OPERAND_IDENT:
			str = "OPERAND_IDENT";
			break;
		case Type::OPERAND_CONSTANT:
			str = "OPERAND_CONSTANT";
			break;
		case Type::OPERATOR:
			str = "OPERATOR";
			break;
		case Type::DELIMITER:
			str = "DELIMITER";
			break;
		default:
			throw std::invalid_argument("RpnElement::type_as_str(): element type mismatch");
			break;
		}
		return str;
	}
};

#endif
