#pragma once

#ifndef _IDENT_H_
#define _IDENT_H_

#include <string>
#include <stdexcept>

class Ident
{
public:
	enum class Type { BOOL, INTEGER, STRING, FUNC, NONE };

private:
	Type _type;
	std::string _value;
	std::string _name;

public:
	Ident();
	Ident(Type type, const std::string & name, const std::string & value = std::string());

	Ident & operator=(const Ident & other) = default;

	Type type() const;
	const std::string & value() const;
	const std::string & name() const;

	static std::string type_to_str(Type type);
	static Ident::Type str_to_type(const std::string & str);
};

#endif