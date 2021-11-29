#include "interpreter.h"
#include "token.h"
#include "lexical_analyzer.h"
#include "rpn_element.h"

std::string Interpreter::_condition_operator = { "!COND" };
std::string Interpreter::_endline_operator = { "ENDL" };

void Interpreter::initialize_memory() {
	auto token = this->_la->get_next_token();
	while (token.valid()) {
		switch (token.type()) {
		case Token::Type::IDENT:
		{
			const auto & ident = this->_la->ident_ref(token.value());
			if (ident.type() == Ident::Type::INTEGER) {
				this->_int_values[ident.name()] = 0;
			}
			else if (ident.type() == Ident::Type::STRING) {
				this->_str_values[ident.name()] = std::string();
			}
			else if (ident.type() == Ident::Type::BOOL) {
				this->_bool_values[ident.name()] = false;
			}
			break;
		}
		case Token::Type::CONSTANT:
		{			
			const auto & constant = this->_la->constant_ref(token.value());
			const auto & value = constant.value();
			if (constant.type() == Constant::Type::INTEGER) {
				std::stringstream sstream;
				sstream << value;
				int num;
				sstream >> num;
				this->_int_values[value] = num;
			}
			else if (constant.type() == Constant::Type::STRING) {
				auto content = value.substr(1, value.size() - 2); // from "www" to www as std::string
				this->_str_values[value] = content;
			}
			break;
		}
		default:
			break;
		}
		token = this->_la->get_next_token();
	}

}

void Interpreter::print_memory(std::ostream & os) const {	
	const auto type_indent = std::setw(15);
	const auto name_indent = std::setw(20);
	const auto value_indent = std::setw(20);
	os << "\n" << name_indent << "MEMORY TABLE" << "\n\n";
	os << type_indent << "TYPE" << value_indent << "NAME" << value_indent << "VALUE" << std::endl;
	for (auto & key_val : this->_int_values) {
		os << type_indent << "INTEGER";
		os << name_indent << key_val.first;
		os << value_indent << key_val.second;
		os << std::endl;
	}
	for (auto & key_val : this->_str_values) {
		os << type_indent << "STRING";
		os << name_indent << key_val.first;
		os << value_indent << key_val.second;
		os << std::endl;
	}
	for (auto & key_val : this->_bool_values) {
		os << type_indent << "BOOL";
		os << name_indent << key_val.first;
		os << value_indent << key_val.second;
		os << std::endl;
	}
}

void Interpreter::construct_notation_output() {
	std::stack<RpnElement> stack;
	auto token = this->_la->get_next_token();
	while (token.valid()) {
		switch (token.type()) {
		case Token::Type::IDENT: 
		{
			const auto & ident_name = token.value();
			if (this->_la->contains_ident(ident_name)) {
				const auto & ident = this->_la->ident_ref(ident_name);
				if (ident.type() == Ident::Type::FUNC) {
					const auto element = RpnElement(RpnElement::Type::FUNC, ident.name());
					stack.push(element);
				}
				else {
					const auto element = RpnElement(RpnElement::Type::OPERAND_IDENT, ident.name());
					this->_reverse_notation.push_back(element);
				}
			}
			break;
		}
		case Token::Type::CONSTANT:
		{
			auto element = RpnElement(RpnElement::Type::OPERAND_CONSTANT, token.value());
			this->_reverse_notation.push_back(element);
			break;
		}
		case Token::Type::KEYWORD:
		{
			if (this->is_const_logic_keyword(token)) {
				auto element = RpnElement(RpnElement::Type::OPERAND_CONSTANT, token.value());
				this->_reverse_notation.push_back(element);
			}
			else if (this->is_title_keyword(token)) {
				auto element = RpnElement(RpnElement::Type::LABEL, token.value());
				this->_reverse_notation.push_back(element);
			}
			else if (this->is_type_keyword(token)) {
				while (this->_la->get_next_token().value() != ";") {
					continue;
				}
			}
			else if (this->is_func_keyword(token)) {
				// i don't know what to do
			}
			else if (this->is_operator_keyword(token)) {
				auto & name = token.value();
				if (name == "then" || "DO") {
					while (!stack.empty()) {
						this->_reverse_notation.push_back(stack.top());
						stack.pop();
					}
				}
				this->note_operator(token);
			}
			else if (token.value() == "write") {
				stack.push(RpnElement(RpnElement::Type::OPERATOR, token.value()));
			}
			else {
				throw std::logic_error("Interpreter::construct_notation_output(): unknown type of KEYWORD");
			}

			break;
		}
		case Token::Type::OPERATION_SIGN:
		{
			auto op1 = token.value();
			while (!stack.empty()) {
				if (stack.top().type() == RpnElement::Type::OPERATOR) {
					auto op2 = stack.top().name();
					auto op2_priority = this->_operations_priorities.at(op2);
					auto op1_priority = this->_operations_priorities.at(op1);
					if (op2_priority >= op1_priority) {
						auto element = RpnElement(RpnElement::Type::OPERATOR, op2);
						this->_reverse_notation.push_back(element);
						stack.pop();
					}
					else {
						break;
					}
				}
				else if (stack.top().type() == RpnElement::Type::FUNC) {
					auto element = RpnElement(RpnElement::Type::FUNC, stack.top().name());
					this->_reverse_notation.push_back(element);
					stack.pop();
				}
				else {
					break;
				}
			}
			auto element = RpnElement(RpnElement::Type::OPERATOR, op1);
			stack.push(element);
			break;
		}
		case Token::Type::DELIMITER:
		{
			if (token.value() == "(") {
				stack.push(RpnElement(RpnElement::Type::DELIMITER, token.value()));
			}
			else if (token.value() == ";") {
				while (!stack.empty()) {
					this->_reverse_notation.push_back(stack.top());
					stack.pop();
				}
			}
			else if (token.value() == ")") {
				auto parenthesis_found = false;
				while (!stack.empty()) {
					if (stack.top().name() != "(") {
						this->_reverse_notation.push_back(stack.top());						
					}
					else {
						parenthesis_found = true;
						stack.pop();
						break;
					}
					stack.pop();
				}
				if (!parenthesis_found) {
					throw std::logic_error("Interpreter::construct_notation_output(): closing parenthesis not found");
				}
			}

			break;
		}
		case Token::Type::NONE:
			throw std::logic_error("Interpreter::construct_notation_output(): token type was NONE");
			break;
		default:
			throw std::logic_error("Interpreter::construct_notation_output(): token type mismatch");
			break;		
		}

		token = this->_la->get_next_token();
	}

	while (!stack.empty()) {
		this->_reverse_notation.push_back(stack.top());
		stack.pop();
	}
	this->_la->reset_tokens_stream();
}

void Interpreter::print_notation(std::ostream & os) const {
	const auto title_indent = std::setw(20);
	const auto value_indent = std::setw(25);
	os << "\n" << title_indent << "RPN TABLE" << "\n\n";
	os << title_indent << "ELEMENT TYPE" << value_indent << "ELEMENT VALUE" << std::endl;
	for (auto & element : this->_reverse_notation) {
		os << title_indent << RpnElement::type_as_str(element.type());
		os << value_indent << element.name();
		os << std::endl;
	}
}

bool Interpreter::is_operator_keyword(const Token & token) const {
	for (const auto & keyword : this->_operator_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_title_keyword(const Token & token) const {
	for (const auto & keyword : this->_title_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_func_keyword(const Token & token) const {
	for (const auto & keyword : this->_func_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_const_logic_keyword(const Token & token) const {
	for (const auto & keyword : this->_const_logic_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

bool Interpreter::is_type_keyword(const Token & token) const {
	for (const auto & keyword : this->_type_keywords) {
		if (keyword == token.value()) {
			return true;
		}
	}
	return false;
}

void Interpreter::note_operator(const Token & token) {
	const auto & keyword = token.value();
	if (keyword == "WHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("WHILE"));
	}
	else if (keyword == "DO") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ENDWHILE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::OPERATOR, this->_condition_operator);
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("DO"));
	}
	else if (keyword == "if") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("IF"));
	}
	else if (keyword == "then") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ELSE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::OPERATOR, this->_condition_operator);
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("THEN"));
	}
	else if (keyword == "EXITWHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$ENDWHILE"));
	}
	else if (keyword == "else") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ELSE"));
	}
	else if (keyword == "endif") {
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ENDIF"));
	}
	else if (keyword == "ENDWHILE") {
		this->_reverse_notation.emplace_back(RpnElement::Type::GOTO, std::string("$WHILE"));
		this->_reverse_notation.emplace_back(RpnElement::Type::LABEL, std::string("ENDWHILE"));
	}
	else if (keyword == "endl") {
		this->_reverse_notation.emplace_back(RpnElement::Type::OPERATOR, this->_endline_operator);
	}
	else {
		throw std::logic_error("Interpreter::note_operator(): operator's keyword mismatch");
	}
}

void Interpreter::execute_notation() {

	std::stack<RpnElement> stack;

	const auto begin = this->_reverse_notation.begin();
	const auto end = this->_reverse_notation.end();
	auto iter = begin;
	while (iter != end) {
		switch (iter->type()) {
		case RpnElement::Type::OPERAND_CONSTANT:
		{
			stack.push(*iter);
			++iter;
			break;
		}
		case RpnElement::Type::OPERAND_IDENT:
		{
			stack.push(*iter);
			++iter;
			break;
		}
		case RpnElement::Type::GOTO:
		{
			const auto & element = *iter;
			++iter;
			if (iter->name() != this->_condition_operator) {
				iter = this->shift_to_label(iter, element);
			}
			else {
				stack.push(element);
			}
			break;
		}
		case RpnElement::Type::OPERATOR:
		{
			if (iter->name() == this->_condition_operator) {
				const auto label = stack.top();
				stack.pop();
				const auto cond = stack.top();				
				stack.pop();
				std::stringstream sstream;
				sstream << cond.name();
				int value = 0;
				if (cond.name() == "false") {
					iter = this->shift_to_label(iter, label);
				}
				else if (sstream >> value) {
					if (!value) {
						iter = this->shift_to_label(iter, label);
					}
				}
			}
			else if (iter->name() == this->_endline_operator) {
				std::cout << std::endl;
			}
			else {
				auto element = stack.top().name();
				if (this->_int_values.contains(element)) {
					this->execute_integer_operation(iter->name(), stack);
				}
				else if (this->_str_values.contains(element)) {
					this->execute_string_operation(iter->name(), stack);
				}
				else if (this->_bool_values.contains(element)) {
					this->execute_logic_operation(iter->name(), stack);
				}
			}
			++iter;
			break;
		}
		case RpnElement::Type::LABEL:
		{
			++iter;
			break;
		}
		default:
			throw std::logic_error("Interpreter::execute_notation(): rpn-element type mismatch");
			break;
		}
	}
}

std::vector<RpnElement>::iterator Interpreter::shift_to_label(std::vector<RpnElement>::iterator pos, const RpnElement & label) {
	auto iter = pos;
	if (label.name() == "$ENDWHILE") {
		while (iter->name() != "ENDWHILE") { // TODO: check for mistakes
			++iter;
		}
	}
	else if (label.name() == "$ELSE") {
		while (iter->name() != "ELSE" && iter->name() != "ENDIF") {
			++iter;
		}
	}
	else if (label.name() == "$WHILE") {
		while (iter->name() != "WHILE") {
			--iter;
		}
	}
	return iter;
}

void Interpreter::execute_integer_operation(const std::string & op, std::stack<RpnElement> & stack) {

	auto assign = [](auto & lhs, const auto & rhs) { lhs = rhs; };
	auto add = [](const auto & lhs, const auto & rhs) { return lhs + rhs; };
	auto substract = [](const auto & lhs, const auto & rhs) { return lhs - rhs; };
	auto divide = [](const auto & lhs, const auto & rhs) {
		if (!rhs) {
			throw std::invalid_argument("divide lambda: division by zero");
		}
		return lhs / rhs;
	};
	auto multiply = [](const auto & lhs, const auto & rhs) { return lhs * rhs; };
	auto equal = [](const auto & lhs, const auto & rhs) { return lhs == rhs; };
	auto more = [](const auto & lhs, const auto & rhs) { return lhs > rhs; };
	auto less = [](const auto & lhs, const auto & rhs) { return lhs < rhs; };
	auto write = [](const auto & par) { std::cout << par; };

	auto op1 = stack.top().name();
	stack.pop();

	if (op == "=") {
		auto op2 = stack.top().name();
		stack.pop();
		assign(this->_int_values.at(op2), this->_int_values.at(op1));
	}
	else if (op == "*") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = multiply(this->_int_values.at(op2), this->_int_values.at(op1));
		std::stringstream sstream;
		std::string name;
		sstream << result;
		sstream >> name;
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
		this->_int_values[name] = result;
	}
	else if (op == "/") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = divide(this->_int_values.at(op2), this->_int_values.at(op1));
		std::stringstream sstream;
		std::string name;
		sstream << result;
		sstream >> name;
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
		this->_int_values[name] = result;
	}
	else if (op == "+") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = add(this->_int_values.at(op2), this->_int_values.at(op1));
		std::stringstream sstream;
		std::string name;
		sstream << result;
		sstream >> name;
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
		this->_int_values[name] = result;
	}
	else if (op == "-") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = substract(this->_int_values.at(op2), this->_int_values.at(op1));
		std::stringstream sstream;
		std::string name;
		sstream << result;
		sstream >> name;
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
		this->_int_values[name] = result;
	}
	else if (op == "==") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = equal(this->_int_values.at(op2), this->_int_values.at(op1));
		std::string name;
		if (result) {
			name = "true";
		}
		else {
			name = "false";
		}
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == ">") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = more(this->_int_values.at(op2), this->_int_values.at(op1));
		std::string name;
		if (result) {
			name = "true";
		}
		else {
			name = "false";
		}
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "<") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = less(this->_int_values.at(op2), this->_int_values.at(op1));
		std::string name;
		if (result) {
			name = "true";
		}
		else {
			name = "false";
		}
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "write") {
		write(this->_int_values.at(op1));
	}
	else {
		throw std::logic_error("Interpreter::execute_operation(): unknown operation");
	}
}

void Interpreter::execute_string_operation(const std::string & op, std::stack<RpnElement> & stack) {
	auto assign = [](auto & lhs, const auto & rhs) { lhs = rhs; };
	auto add = [](const auto & lhs, const auto & rhs) { return lhs + rhs; };
	auto equal = [](const auto & lhs, const auto & rhs) { return lhs == rhs; };
	auto more = [](const auto & lhs, const auto & rhs) { return lhs > rhs; };
	auto less = [](const auto & lhs, const auto & rhs) { return lhs < rhs; };
	auto write = [](const auto & par) { std::cout << par; };

	auto op1 = stack.top().name();
	stack.pop();

	if (op == "=") {
		auto op2 = stack.top().name();
		RpnElement::Type op2_type;
		stack.pop();
		assign(this->_str_values.at(op2), this->_str_values.at(op1));
	}
	else if (op == "+") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = add(this->_str_values.at(op2), this->_str_values.at(op1));
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, result));
		this->_str_values['"' + result + '"'] = result;
	}
	else if (op == "==") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = equal(this->_str_values.at(op2), this->_str_values.at(op1));
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == ">") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = more(this->_str_values.at(op2), this->_str_values.at(op1));
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "<") {
		auto op2 = stack.top().name();
		stack.pop();
		auto result = less(this->_str_values.at(op2), this->_str_values.at(op1));
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "write") {
		write(this->_str_values.at(op1));
	}
	else {
		throw std::logic_error("Interpreter::execute_operation(): unknown operation");
	}
}

void Interpreter::execute_logic_operation(const std::string & op, std::stack<RpnElement> & stack) {
	auto assign = [](auto & lhs, const auto & rhs) { lhs = rhs; };
	auto equal = [](const auto & lhs, const auto & rhs) { return lhs == rhs; };
	auto more = [](const auto & lhs, const auto & rhs) { return lhs > rhs; };
	auto less = [](const auto & lhs, const auto & rhs) { return lhs < rhs; };
	auto write = [](const auto & par) { std::cout << std::boolalpha << par; };

	auto op1 = stack.top().name();
	stack.pop();

	if (op == "=") {
		auto op2 = stack.top().name();
		stack.pop();
		bool op1_val = (op1 == "true") ? true : false;
		assign(this->_bool_values.at(op2), op1_val);
	}
	else if (op == "==") { // ident == keyword(true || false)
		auto op2 = stack.top().name();
		stack.pop();
		bool op1_val = (op1 == "true") ? true : false;
		auto result = equal(this->_int_values.at(op2), op1_val);
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == ">") { // ident > keyword(true || false)
		auto op2 = stack.top().name();
		stack.pop();
		bool op1_val = (op1 == "true") ? true : false;
		auto result = more(this->_int_values.at(op2), op1_val);
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "<") { // ident < keyword(true || false)
		auto op2 = stack.top().name();
		stack.pop();
		bool op1_val = (op1 == "true") ? true : false;
		auto result = less(this->_int_values.at(op2), op1_val);
		std::string name = (result) ? "true" : "false";
		stack.push(RpnElement(RpnElement::Type::OPERAND_CONSTANT, name));
	}
	else if (op == "write") {
		if (op1 == "true" || op1 == "false") {
			write(op1);
		}
		else {
			write(this->_bool_values.at(op1));
		}
	}
	else {
		throw std::logic_error("Interpreter::execute_operation(): unknown operation");
	}
}

bool Interpreter::is_operation_higher_priority(const std::string& oper1, const std::string& oper2) {
	std::map<std::string, OperationsPriority> priorities = {
	{ "=", OperationsPriority::LOW },
	{ "==", OperationsPriority::MIDDLE },
	{ "<", OperationsPriority::MIDDLE },
	{ ">", OperationsPriority::MIDDLE },
	{ "+", OperationsPriority::MIDDLE },
	{ "-", OperationsPriority::MIDDLE },
	{ "*", OperationsPriority::HIGH },
	{ "/", OperationsPriority::HIGH },
	{ "write", OperationsPriority::HIGH },
	};

	if (priorities.contains(oper1) && priorities.contains(oper2)) {
		if (priorities[oper1] > priorities[oper2]) {
			return true;
		}
		else {
			return false;
		}
	}
	else {
		throw std::invalid_argument("Interpreter::is_operation_higher_priority(): no such operator/operators");
	}
}