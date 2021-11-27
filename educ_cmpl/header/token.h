#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>
#include <stdexcept>

class Token
{
public:
	enum class Type { KEYWORD, DELIMITER, IDENT, CONSTANT, OPERATION_SIGN, NONE, };

private:
	Type _type;
	size_t _line;
	std::string _value;

public:
	Token();
	explicit Token(Type type, size_t line, std::string value = std::string());
	Token(const Token & other) = default;

	Token & operator=(const Token & other) = default;

	Type type() const;
	const std::string & value() const;
	size_t line() const;

	static std::string type_to_str(Type type);

	bool valid() const;

};

#endif