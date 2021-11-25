#pragma once

#ifndef _CONSTANT_H_
#define _CONSTANT_H_

#include <string>
#include <stdexcept>

class Constant
{
public:
	enum class Type { INTEGER, STRING, NONE };

private:
	Type _type;
	std::string _value;

public:
	Constant();
	Constant(Type type, const std::string & value);

	Constant & operator=(const Constant & other) = default;

	Type type() const;
	const std::string & value() const;

	static std::string type_to_str(Type type);
};

#endif