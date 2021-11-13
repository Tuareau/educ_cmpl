#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <fstream>
#include <vector>

class LexicalAnalyzer;

class Token;
class TokenTable;

class Ident;
class IdentTable;

class SyntaxAnalyzer;

class CodeGenerator;

class Compiler
{
private:
	IdentTable * ident_table;
	LexicalAnalyzer * _lexical_analyzer;

public:
	//void compile(const std::string & filename);

};

#endif