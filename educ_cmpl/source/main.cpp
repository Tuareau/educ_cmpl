#include <iostream>
#include <string>

#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "interpreter.h"

int main(int argc, char * argv[]) {
	if (argc != 2) {
		std::cout << "USAGE: interpreter filename";
		return 1;
	}

	std::string filename = argv[1];

	LexicalAnalyzer la(filename);
	la.construct_token_table();
	la.print_token_table(std::cout);

	//SyntaxAnalyzer sa(&la);
	//auto is_correct = sa.analyze_syntax();

	//if (is_correct) {

		la.construct_ident_table();
		la.print_ident_table(std::cout);
		la.construct_constant_table();
		la.print_constant_table(std::cout);

		Interpreter ipr(&la);
		ipr.construct_notation_output();
		ipr.print_notation(std::cout);
		ipr.initialize_memory();
		ipr.print_memory(std::cout);
		ipr.execute_notation();
	//}

	return 0;
}