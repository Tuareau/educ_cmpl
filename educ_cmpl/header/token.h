#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <string>

class IdentTable;

class Token
{
public:
	enum class Type { KEYWORD, DELIMITER, IDENT, CONSTANT, OPERATION_SIGN };

private:
	Type type;
	std::string value;

public:
	explicit Token(Type type, std::string value = std::string());

	Type type() const;
	const std::string & value() const;

};

#endif