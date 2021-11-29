#include <iostream>
#include <fstream>
#include <string>

#include "lexical_analyzer.h"
#include "syntax_analyzer.h"
#include "interpreter.h"

int main(int argc, char * argv[]) {
	if (argc != 2) {
		std::cout << "\n>>> usage: interpreter filename\n";
		return 1;
	}

	std::string filename = argv[1];
	std::fstream fhandler;
	fhandler.open(filename);
	if (!fhandler.good()) {
		std::cout << "\n>>> file not found";
		std::cout << "\n>>> usage: interpreter filename\n";
		return 1;
	}

	std::ofstream fout_tokens("tokens.txt", std::ios_base::trunc);
	std::ofstream fout_idents("idents.txt", std::ios_base::trunc);
	std::ofstream fout_constants("constants.txt", std::ios_base::trunc);
	std::ofstream fout_rpn("rpn.txt", std::ios_base::trunc);
	std::ofstream fout_init_memory("memory.txt", std::ios_base::trunc);

	std::cout << "\n>>> build started...\n\n";

	LexicalAnalyzer la(filename);
	auto no_wrong_symbols = la.construct_token_table();
	la.print_token_table(fout_tokens);

	if (!no_wrong_symbols) {
		std::cout << "\n>>> could not start interpretation due to unknow characters\n";
		return 0;
	}

	SyntaxAnalyzer sa(&la);
	auto is_correct = sa.analyze_syntax();

	if (!is_correct) {
		std::cout << "\n>>> could not start interpretation due to errors\n";
		return 0;
	}

	auto no_repeated_declarations = la.construct_ident_table();
	if (!no_repeated_declarations) {
		std::cout << "\n>>> could not start interpretation due to repeated declarations\n";		
		return 0;
	}

	la.print_ident_table(fout_idents);
	la.construct_constant_table();
	la.print_constant_table(fout_constants);

	Interpreter ipr(&la);
	ipr.construct_notation_output();
	ipr.print_notation(fout_rpn);
	ipr.initialize_memory();
	ipr.print_memory(fout_init_memory);
	ipr.execute_notation();

	std::cout << "\n\n>>> successfuly executed\n";
	return 0;
}