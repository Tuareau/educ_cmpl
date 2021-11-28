	R = {
		* program *
		S ::= �PROGRAMM� �;� <PROGRAM_BODY> �END� �.�

		* program body *
		<PROGRAM_BODY> ::= while <LANG_CONSTRUCT>

		* language constructions *
		<LANG_CONSTRUCT> ::=
			<VAR_DECL> �;� | 
			<ARYTHM_EXPR> �;� | 
			<LANG_OPERATOR> �;�

		* variable or function parameter declaration *
		<VAR_DECL> ::= <TYPE> <IDENT>
		<TYPE> ::= �integer� | �string� | �bool�

		<ARG> ::= <IDENT> | <CONST>

		* arythmetic expression *
		<ARYTHM_EXPR> ::=
		<IDENT> �=� <OPERAND> | 
		<IDENT> �=� ������ ��������������� ��������� �� �������� <ARITHM_OPERATION>
		<IDENT> �=� <LOGICAL_EXPR> |

		* arythmetic operation *
		<ARITHM_OPERATION> ::= ?
		<ARITHM_OP> ::= �+� | �-� | �/� | �*� |
		<OPERAND> ::= <IDENT> | <CONST>

		* logical expression *
		<LOGICAL_EXPR> ::= <LOGICAL_OPERATION> | <LOGICAL_STATEMENT> | �(� <LOGICAL_OPERATION> �)�
		<LOGICAL_OPERATION> ::= <OPERAND> <LOGICAL_OP> <OPERAND>
		<LOGICAL_OP> ::= �>� | �<� | �==�
		<LOGICAL_STATEMENT> ::= <IDENT> | <CONST>

		* language operators *
		<LANG_OPERATOR> ::= <WHILE_OP> | <IF_OP> | <WRITE_OP> | <ENDL_OP>

		<WRITE_OP> ::= �write� �(� while <ARG> , �)�

		<ENDL_OP> ::= �endl�

		<WHILE_OP> ::= �WHILE� <LOGICAL_EXPR> �DO� while <OPERATION> �ENDWHILE�
	
		<IF_OP> ::=
			�if� <LOGICAL_EXPR> �then� while <OPERATION> �endif�
			�if� <LOGICAL_EXPR> �then� while <OPERATION> �else� while <OPERATION> �endif�
	

		* operations in while and if operators *
		<OPERATION> ::=
			�EXITWHILE� �;� |
			<LANG_OPERATOR> �;� |
			<ARYTHM_EXPR> �;� |
			<VAR_DECL> �;� |	

