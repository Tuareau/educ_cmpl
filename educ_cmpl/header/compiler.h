#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <fstream>
#include <vector>

class LexicalAnalyzer;

class Token;
class Ident;

class Interpreter;

class SyntaxAnalyzer;

class Compiler
{
private:
	Interpreter * _interpreter;
	LexicalAnalyzer * _lexical_analyzer;

public:
	//void compile(const std::string & filename);

};

#endif