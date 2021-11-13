#include "lexical_analyzer.h"

void LexicalAnalyzer::construct_token_table(const std::string & filename) {

	std::fstream fin;
	fin.open(filename, std::ios_base::in);
	if (!fin.good()) {
		throw std::ios_base::failure("LexicalAnalyzer::construct_token_table(): couldn't open file");
	}

	size_t index = 0;
	int symbol, err_symbol;
	this->_state = State::START;
	symbol = fin.get();

	Token::Type token_type;
	std::string token_value;

	while (fin.good() && !fin.eof() && fin.peek() != EOF) {
		switch (this->_state) {
			case State::START:
			{
				while (std::isspace(symbol)) {
					symbol = fin.get();
				}

				if (std::isalpha(symbol)) {
					this->_state = State::IDENT;
				}
				else if (std::isdigit(symbol) || (symbol == '\"')) {
					this->_state = State::CONSTANT;
				}
				else if ((symbol == '=') || (symbol == '<') || 
					(symbol == '>') || (symbol == '-') || (symbol == '+')) 
				{
					this->_state = State::OPERATION_SIGN;
				}
				else if ((symbol == '$')) {
					this->_state = State::COMMENT;
				}
				else {
					this->_state = State::DELIMITER;
				}
				break;
			}

			case State::OPERATION_SIGN:
			{
				if ((symbol == '<') || (symbol == '>') || (symbol == '=') || 
					(symbol == '-') || (symbol == '+'))
				{
					token_type = Token::Type::OPERATION_SIGN;
					token_value = std::string((std::stringstream() << static_cast<char>(symbol)).str());

					this->_token_table[index] = Token(token_type, token_value);
					index++;

					symbol = fin.get();
					this->_state = State::START;
				}
				else {
					err_symbol = symbol;
					this->_state = State::ERROR;
				}
				break;
			}

			case State::CONSTANT:
			{
				token_type = Token::Type::CONSTANT;
				token_value = symbol;
				if (std::isdigit(symbol)) {						
					symbol = fin.get();
					while (std::isdigit(symbol)) {
						token_value += symbol;
						symbol = fin.get();						
					}
				}
				else if (symbol == '\"') {
					symbol = fin.get();
					while (symbol != '\"') {
						token_value += symbol;
						symbol = fin.get();
					}
					token_value += symbol;
				}

				this->_token_table[index] = Token(token_type, token_value);
				index++;

				symbol = fin.get();
				this->_state = State::START;
				break;
			}

			case State::DELIMITER:
			{
				if ((symbol == '(') || (symbol == ')') || (symbol == ';') || (symbol == ',')) {
					token_type = Token::Type::DELIMITER;
					token_value = std::string((std::stringstream() << static_cast<char>(symbol)).str());

					this->_token_table[index] = Token(token_type, token_value);
					index++;

					symbol = fin.get();
					this->_state = State::START;
				}
				else {
					err_symbol = symbol;
					symbol = fin.get();
					this->_state = State::ERROR;
				}
				break;
			}

			case State::ERROR:
			{
				std::cout << "\nUnknown character\n" << static_cast<char>(err_symbol);
				this->_state = State::START;
				break;
			}

			case State::IDENT:
			{
				token_value = symbol;
				symbol = fin.get();
				while (std::isalpha(symbol) || std::isdigit(symbol)) {
					token_value += symbol;
					symbol = fin.get();
				}

				auto key_word_iter = std::find(this->_keywords.begin(), this->_keywords.end(), token_value);
				if (key_word_iter != this->_keywords.end()) {
					token_type = Token::Type::KEYWORD;
				}
				else {
					token_type = Token::Type::IDENT;
				}
				this->_token_table[index] = Token(token_type, token_value);
				index++;

				this->_state = State::START;
				break;
			}

			case State::COMMENT:
			{
				while (symbol != '$') {
					symbol = fin.get();
				}

				symbol = fin.get();
				this->_state = State::START;
				break;
			}

			default:
			{
				throw std::logic_error("LexicalAnalyzer::construct_token_table: analyzer state type mismatch");
				break;
			}
		}
	} 
}

void LexicalAnalyzer::print_token_table() const {
	const auto indent = std::setw(20);
	std::cout << "\n" << indent << "TOKEN TABLE" << "\n\n";
	std::cout << indent << "TOKEN TYPE" << indent << "TOKEN VALUE" << std::endl;
	for (const auto & token : this->_token_table) {
		std::cout << indent << Token::type_to_str(token.second.type());
		std::cout << indent << token.second.value();
		std::cout << std::endl;
	}
}