#include "lexical_analyzer.h"

LexicalAnalyzer::LexicalAnalyzer()
	: _tokens_stream_pos(0) {}

void LexicalAnalyzer::construct_token_table(const std::string & filename) {

	std::fstream fin;
	fin.open(filename, std::ios_base::in);
	if (!fin.good()) {
		throw std::ios_base::failure("LexicalAnalyzer::construct_token_table(): couldn't open file");
	}

	size_t index = 0;
	int symbol, err_symbol;
	this->_state = State::START;

	Token::Type token_type;
	std::string token_value;

	symbol = fin.get();
	while (fin.good() && !fin.eof() /*&& fin.peek() != EOF*/) {
		switch (this->_state) {
			case State::START:
			{
				while (std::isspace(symbol)) {
					symbol = fin.get();
				}

				if (std::isalpha(symbol) || symbol == '_') {
					this->_state = State::IDENT;
				}
				else if (std::isdigit(symbol) || (symbol == '\"')) {
					this->_state = State::CONSTANT;
				}
				else if (this->is_operation_sign(symbol)) {
					this->_state = State::OPERATION_SIGN;
				}
				else if ((symbol == '/')) {
					this->_state = State::COMMENT;
				}
				else {
					this->_state = State::DELIMITER;
				}
				break;
			}

			case State::OPERATION_SIGN:
			{
				if (symbol == '=') { // assign or logical '=='
					token_type = Token::Type::OPERATION_SIGN;
					token_value = std::string((std::stringstream() << static_cast<char>(symbol)).str());

					int next = fin.get();
					if (next == '=') { // logical '=='
						token_value += next;
					}

					this->_token_table[index++] = Token(token_type, token_value);

					if (next == '=') {
						symbol = fin.get();
					}
					else {
						symbol = next;
					}
					this->_state = State::START;
				}
				else if (this->is_operation_sign(symbol)) {
					if (symbol == '/') { // comment check
						auto next = fin.get();
						if (next == '*') {
							this->_state = State::COMMENT;
						}
						fin.unget();
					}
					else {
						token_type = Token::Type::OPERATION_SIGN;
						token_value = std::string((std::stringstream() << static_cast<char>(symbol)).str());

						this->_token_table[index++] = Token(token_type, token_value);

						symbol = fin.get();
						this->_state = State::START;
					}
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
				if (std::isdigit(symbol)) { // number	
					symbol = fin.get();
					while (std::isdigit(symbol)) {
						token_value += symbol;
						symbol = fin.get();						
					}
				}
				else if (symbol == '\"') { // literal
					symbol = fin.get();
					while (symbol != '\"') {
						token_value += symbol;
						symbol = fin.get();
					}
					token_value += symbol;
					symbol = fin.get();
				}

				this->_token_table[index++] = Token(token_type, token_value);

				this->_state = State::START;
				break;
			}

			case State::DELIMITER:
			{
				if (this->is_delimiter(symbol)) {
					token_type = Token::Type::DELIMITER;
					token_value = std::string((std::stringstream() << static_cast<char>(symbol)).str());

					this->_token_table[index++] = Token(token_type, token_value);

					symbol = fin.get();
					this->_state = State::START;
				}
				else { // unknown symbol
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
				while (std::isalpha(symbol) || std::isdigit(symbol) || symbol == '_') {
					token_value += symbol;
					symbol = fin.get();
				}

				if (this->is_keyword(token_value)) {
					token_type = Token::Type::KEYWORD;
				}
				else {
					token_type = Token::Type::IDENT;
				}
				this->_token_table[index++] = Token(token_type, token_value);

				this->_state = State::START;
				break;
			}

			case State::COMMENT:
			{
				if (symbol == '/') {
					int next = fin.get();
					if (next == '*') {
						while (true) {
							while ((next = fin.get()) != '*');
							next = fin.get();
							if (next == '/') {
								break;
							}
						}
						symbol = fin.get();
					}
					else {
						fin.unget();
					}
				}

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

void LexicalAnalyzer::print_token_table(std::ostream & os) const {
	const auto indent = std::setw(20);
	os << "\n" << indent << "TOKEN TABLE" << "\n\n";
	os << indent << "TOKEN TYPE" << indent << "TOKEN VALUE" << std::endl;
	for (const auto & token : this->_token_table) {
		os << indent << Token::type_to_str(token.second.type());
		os << indent << token.second.value();
		os << std::endl;
	}
}

bool LexicalAnalyzer::is_operation_sign(const char c) const {
	const auto begin = this->_operation_signs.begin();
	const auto end = this->_operation_signs.end();
	std::string symbol;
	symbol += c;
	if (std::find(begin, end, symbol) != end) {
		return true;
	}
	else {
		return false;
	}
}

bool LexicalAnalyzer::is_delimiter(const char c) const {
	const auto begin = this->_delimiters.begin();
	const auto end = this->_delimiters.end();
	std::string symbol;
	symbol += c;
	if (std::find(begin, end, symbol) != end) {
		return true;
	}
	else {
		return false;
	}
}

bool LexicalAnalyzer::is_keyword(const std::string & str) const {
	const auto begin = this->_keywords.begin();
	const auto end = this->_keywords.end();
	if (std::find(begin, end, str) != end) {
		return true;
	}
	else {
		return false;
	}
}

Token LexicalAnalyzer::get_next_token() {
	this->_tokens_stream_pos += 1;
	const auto pos = this->_tokens_stream_pos;
	if (this->_token_table.contains(pos)) {
		const auto & token = this->_token_table.at(pos);
		return Token(token);
	}
	return Token();
}

void LexicalAnalyzer::unget_token() {
	if (this->_tokens_stream_pos > 0) {
		this->_tokens_stream_pos -= 1;
	}
}

void LexicalAnalyzer::reset_tokens_stream() {
	this->_tokens_stream_pos = 0;
}

bool LexicalAnalyzer::token_stream_ended() const {
	return this->_tokens_stream_pos >= this->_token_table.size();
}

size_t LexicalAnalyzer::tokens_count() const {
	return this->_token_table.size();
}

Token LexicalAnalyzer::get_token(size_t key) const {
	if (this->_token_table.contains(key)) {
		const auto & token = this->_token_table.at(key);
		return Token(token);
	}
	return Token();
}

void LexicalAnalyzer::construct_ident_table() {
	std::string scope;
	for (const auto & token_iter : this->_token_table) {	
		const auto & [pos, token] = token_iter; // current token and its position
		auto next_token = this->get_token(pos + 1);
		if (token.value() == "ENDF") { // endf: reset scope
			scope.clear();
		}
		if (this->is_decl_keyword(token.value())) { // bool, string, integer or func
			if (next_token.valid()) {
				const auto ident_name = scope + next_token.value();
				if (this->_ident_table.contains(ident_name)) { // declaration repeated
					std::cout << "\n\tREUSE ident: '" << ident_name
						<< "' type: " << token.value() << std::endl;
				}
				else { // right declaration, new ident with scope
					this->_ident_table[ident_name] = Ident(Ident::str_to_type(token.value()), ident_name);
				}
			}
		}
		if (token.value() == "FUNC" && next_token.valid()) { // func: increase scope
			const auto & func_name = next_token.value();
			scope += func_name + "::";
		}
	}
}

bool LexicalAnalyzer::is_decl_keyword(const std::string & str) const {
	if (str == "bool" || str == "integer" || str == "string" || str == "FUNC") {
		return true;
	}
	else {
		return false;
	}
}

void LexicalAnalyzer::print_ident_table(std::ostream & os) const {
	const auto title_indent = std::setw(20);
	const auto value_indent = std::setw(25);
	os << "\n" << title_indent << "IDENT TABLE" << "\n\n";
	os << title_indent << "IDENT TYPE" << value_indent << "IDENT NAME" << std::endl;
	for (const auto & ident : this->_ident_table) {
		os << title_indent << Ident::type_to_str(ident.second.type());
		os << value_indent << ident.second.name();
		os << std::endl;
	}
}

void LexicalAnalyzer::construct_constant_table() {
	for (const auto & token_iter : this->_token_table) {
		const auto & [pos, token] = token_iter;
		if (token.type() == Token::Type::CONSTANT) {
			auto lexeme = token.value();
			if (!this->_constant_table.contains(lexeme)) {
				if (lexeme.at(0) == '"') {
					this->_constant_table[lexeme] = Constant(Constant::Type::STRING, lexeme);
				}
				else if (std::isdigit(lexeme.at(0))) {
					this->_constant_table[lexeme] = Constant(Constant::Type::INTEGER, lexeme);
				}
				else {
					throw std::logic_error("LexicalAnalyzer::construct_constant_table(): couldn't recognize constant");
				}
			}
		}
	}
}

void LexicalAnalyzer::print_constant_table(std::ostream & os) const {
	const auto title_indent = std::setw(20);
	const auto value_indent = std::setw(25);
	os << "\n" << title_indent << "CONSTANTS TABLE" << "\n\n";
	os << title_indent << "CONSTANT TYPE" << value_indent << "CONSTANT VALUE" << std::endl;
	for (const auto & constant : this->_constant_table) {
		os << title_indent << Constant::type_to_str(constant.second.type());
		os << value_indent << constant.second.value();
		os << std::endl;
	}
}