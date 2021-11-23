#pragma once

#ifndef _RPN_ELEMENT_H_
#define _RPN_ELEMENT_H_

#include <string>

class RpnElement
{
public:
	enum class Type {
		OPERAND,
		OPERATOR,
		LABEL,
		GOTO,
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
};

#endif
