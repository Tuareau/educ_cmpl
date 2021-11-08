# educ_cmpl
educational compiler

## language grammar

	G = { Vt, Vn, R, S }

	Vt = {
		PROGRAMM, END,
		integer, bool, string,
		WHILE, DO, EXITWHILE, ENDWHILE,
		FUNC, ENDF, return, write,
		if, then, endif,
		true, false,
		(, ), ;, ., ”, ”, _, \, /
		,
		[A..Z], [a..z], [0..9]
		=, <, >, >=, <=, != ,==,
		*, /, %, +, -,
		/*, */
		$
	}

	Vn = {
		<PROGRAM_BODY>
		<LANG_CONSTRUCT_SEQUENCE>
		<LANG_CONSTRUCT>
		<VAR_DECL>
		<FUNC_DEF>
		<ARYTHM_EXPR>
		<FUNC_CALL>
		<LANG_OPERATOR>
		<COMMENT>

		<IDENT>
		<LITER>
		<CHAR_SEQUENCE>
		<CHAR>
		<DIGIT>
		<TYPE>

		<FUNC_PARAMS>
		<FUNC_PARAM>
		<FUNC_BODY>
		<FUNC_CONSTRUCT_SEQUENCE>
		<FUNC_CONSTRUCT>
		<RETURN_OP>
		<RETURN_EXPR>
		<FUNC_CALL>
		<ARGS>
		<ARG>


		<ARITHM_OPERATION>
		<OP>
		<OPERAND>

		<CONST_EXPR>
		<CONST_INT>
		<DIGITS>
		<CONST_BOOL>
		<CONST_STRING>
		<SYMBOLS>
		<SYMBOL>
		<SEPARATOR>
	
		<LOGICAL_EXPR>
		<LOGICAL_OPERATION>
		<LOGICAL_OP>
		<LOGICAL_STATEMENT>

		<WHILE_OP>	
		<IF_OP>	
		<OPERATIONS_SEQUENCE>
		<OPERATION>	
	}	

	R = {
		* program *
		S ::= “PROGRAMM” ”;” <PROGRAM_BODY> “END” “.”

		* program body *
		<PROGRAM_BODY> ::= <LANG_CONSTRUCT_SEQUENCE>

		* language constructions *
		<LANG_CONSTRUCT_SEQUENCE> ::= <LANG_CONSTRUCT> | <LANG_CONSTRUCT> <LANG_CONSTRUCT_SEQUENCE>
		<LANG_CONSTRUCT> ::=
			<VAR_DECL> | 
			<FUNC_DEF> | 
			<ARYTHM_EXPR> “;” | 
			<FUNC_CALL> “;” | 
			<LANG_OPERATOR> |
			<COMMENT>

		* variable or function parameter declaration *
		<VAR_DECL> ::= <TYPE> <IDENT> ”;”
		<TYPE> ::= “integer” | “string” | “bool”

		* identificator *
		<IDENT> ::= <LITER> | <LITER> <CHAR_SEQUENCE>
		<LITER> ::= [a..z] | [A..Z] 
		<CHAR_SEQUENCE> ::= <CHAR> | <CHAR> <CHAR_SEQUENCE>
		<CHAR> ::= <LITER> | <DIGIT>
		<DIGIT> ::= [0..9]


		* function definition *
		<FUNC_DEF> ::= <FUNC_DECL> <FUNC_BODY> “ENDF” “;”
		<FUNC_DECL> ::= “FUNC” <IDENT> ”(“ <FUNC_PARAMS> ”)” “;”
		<FUNC_PARAMS> ::= <FUNC_PARAM> | <FUNC_PARAM> “,” <FUNC_PARAMS>
		<FUNC_PARAM> ::= <TYPE> <IDENT>

		* function body *
		<FUNC_BODY> ::= <FUNC_CONSTRUCT_SEQUENCE>
		<FUNC_CONSTRUCT_SEQUENCE> ::=
		<FUNC_CONSTRUCT> | <FUNC_CONSTRUCT> <FUNC_CONSTRUCT_SEQUENCE>
		<FUNC_CONSTRUCT> ::=
			<VAR_DECL> | 
			<ARYTHM_EXPR> | 
			<FUNC_CALL> ”;” | 
			<LANG_OPERATOR> |
			<RETURN_OP>

		* function constructions *
		<RETURN_OP> ::= “return” <RETURN_EXPR> ”;”
		<RETURN_EXPR> ::= <FUNC_CALL> | <ARYTHM_EXPR> | <LOGICAL_EXPR> | <IDENT>

		* function call *
		<FUNC_CALL> ::= <IDENT> ”(“ <ARGS> ”)”
		<ARGS> ::= <ARG> | <ARG> “,” <ARGS>
		<ARG> ::= <IDENT> | <CONST_EXPR>

		* arythmetic expression *
		<ARYTHM_EXPR> ::=
		<IDENT> “=” <IDENT> | 
		<IDENT> “=” <ARITHM_OPERATION> | 
		<IDENT> “=” <LOGICAL_EXPR> |
		<IDENT> “=” <CONST_EXPR>

		* arythmetic operation *
		<ARITHM_OPERATION> ::= <OPERAND> <OP> <OPERAND> | <OPERAND> <OP> <ARITHM_OPERATION> 
		<OP> ::= “+” | “-“ | “/” | “*” | “%”
		<OPERAND> ::= <IDENT> | <CONST_EXPR> | <FUNC_CALL>

		* constant expression *
		<CONST_EXPR> ::=
			<CONST_INT> | 
			<CONST_BOOL> |
			<CONST_STRING> 
		<CONST_INT> ::= <DIGITS>
		<DIGITS> ::= <DIGIT> | <DIGIT> <DIGITS>
		<CONST_BOOL> ::= true | false
		<CONST_STRING> ::= “ <SYMBOLS> ”
		<SYMBOLS> ::= <SYMBOL> | <SYMBOL> <SYMBOLS> | “$”
		<SYMBOL> ::= <CHAR> | <SEPARATOR> | \n | \” | *space*
		<SEPARATOR> ::= “(“ | “)” | “;” | “.” | “‘” | “’” | ” | ” | “_” | “\” | “/”

		* logical expression *
		<LOGICAL_EXPR> ::= <LOGICAL_OPERATION> | <LOGICAL_STATEMENT> | “(” <LOGICAL_OPERATION> “)”
		<LOGICAL_OPERATION> ::= <OPERAND> <LOGICAL_OP> <OPERAND>
		<LOGICAL_OP> ::= “>” | “<” | “==” | “>=” | “<=” | “!=”
		<LOGICAL_STATEMENT> ::= <IDENT> | <CONST_INT> | <CONST_BOOL> | <FUNC_CALL> 

		* language operators *
		<LANG_OPERATOR> ::= <WHILE_OP> | <IF_OP> | <WRITE_OP>

		<WRITE_OP> ::= “write” “(“ <ARGS> “)” “;”

		<WHILE_OP> ::= “WHILE” <LOGICAL_EXPR> “DO” <OPERATIONS_SEQUENCE> “ENDWHILE” “;”
	
		<IF_OP> ::=
			“if” <LOGICAL_EXPR> “then” <OPERATIONS_SEQUENCE> “endif” ”;”
			“if” <LOGICAL_EXPR> “then” <OPERATIONS_SEQUENCE> “else” <OPERATIONS_SEQUENCE> “endif” “;”
	

		* operations in while and if operators *
		<OPERATIONS_SEQUENCE> ::= <OPERATION> | <OPERATION> <OPERATIONS_SEQUENCE>
		<OPERATION> ::=
			“EXITWHILE” “;” |
			<LANG_OPERATOR> |
			<FUNC_CALL> “;” |
			<ARYTHM_EXPR> “;” |
			<VAR_DECL> |
			<COMMENT>
 
		* c-style comments *
		<COMMENT> ::= “/*” <SYMBOLS> “*/”	

	}

## Program Example

	PROGRAMM;

		integer x;
		bool y;
		string z;
		x = 10;
		z = ”www”;
		WHILE 1
			DO
				x = x - 1; /* comment */
				if x == 5 then 
					y = (length(z) > 5); 
					EXITWHILE;
				else y = (length(z) < 5) 
				endif;
		ENDWHILE;

		write(y);

		FUNC length(string s);
			return strlen(s);
		ENDF;

		FUNC strlen(string s);
			integer n;
			string t;
			t = ””;
			n = 0;
			WHILE 1
				DO
					if t == s then EXITWHILE;
					endif;
					t = t + ”w”;
					n = n + 1;
			ENDWHILE;
			return n;
		ENDF;
	END.