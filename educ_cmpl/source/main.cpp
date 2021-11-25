#include <iostream>
#include <string>

#include "lexical_analyzer.h"

int main(int argc, char * argv[]) {
	if (argc != 2) {
		std::cout << "USAGE: prog filename.txt";
		return 1;
	}

	std::string filename = argv[1];

	LexicalAnalyzer la;
	la.construct_token_table(filename);
	la.print_token_table(std::cout);
	la.construct_ident_table();
	la.print_ident_table(std::cout);
	la.construct_constant_table();
	la.print_constant_table(std::cout);

	return 0;
}