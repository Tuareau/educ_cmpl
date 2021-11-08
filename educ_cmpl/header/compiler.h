#pragma once

#ifndef _COMPILER_H_
#define _COMPILER_H_

#include <fstream>

class LexicalAnalyzer;
class SyntaxAnalyzer;

class Token;
class TokenTable;
class IdentTable;

class CodeGenerator;

class Compiler
{
private:
	LexicalAnalyzer lexical_analyzer;

public:
	void compile(const std::fstream & file);
};

#endif