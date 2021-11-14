#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>
#include <stdexcept>

//class IdentTable;

class Token
{
public:
	enum class Type { KEYWORD, DELIMITER, IDENT, CONSTANT, OPERATION_SIGN, NONE, };

private:
	Type _type;
	std::string _value;

public:
	Token();
	explicit Token(Type type, std::string value = std::string());

	Token & operator=(const Token & other) = default;

	Type type() const;
	const std::string & value() const;

	static std::string type_to_str(Type type);

};

#endif