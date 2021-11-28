	R = {
		* program *
		S ::= ìPROGRAMMî î;î <PROGRAM_BODY> ìENDî ì.î

		* program body *
		<PROGRAM_BODY> ::= while <LANG_CONSTRUCT>

		* language constructions *
		<LANG_CONSTRUCT> ::=
			<VAR_DECL> î;î | 
			<ARYTHM_EXPR> ì;î | 
			<LANG_OPERATOR> î;î

		* variable or function parameter declaration *
		<VAR_DECL> ::= <TYPE> <IDENT>
		<TYPE> ::= ìintegerî | ìstringî | ìboolî

		<ARG> ::= <IDENT> | <CONST>

		* arythmetic expression *
		<ARYTHM_EXPR> ::=
		<IDENT> ì=î <OPERAND> | 
		<IDENT> ì=î –¿«¡Œ– ¿–»‘Ã≈“»◊≈— Œ√Œ ¬€–¿∆≈Õ»ﬂ —Œ — Œ¡ ¿Ã» <ARITHM_OPERATION>
		<IDENT> ì=î <LOGICAL_EXPR> |

		* arythmetic operation *
		<ARITHM_OPERATION> ::= ?
		<ARITHM_OP> ::= ì+î | ì-ì | ì/î | ì*î |
		<OPERAND> ::= <IDENT> | <CONST>

		* logical expression *
		<LOGICAL_EXPR> ::= <LOGICAL_OPERATION> | <LOGICAL_STATEMENT> | ì(î <LOGICAL_OPERATION> ì)î
		<LOGICAL_OPERATION> ::= <OPERAND> <LOGICAL_OP> <OPERAND>
		<LOGICAL_OP> ::= ì>î | ì<î | ì==î
		<LOGICAL_STATEMENT> ::= <IDENT> | <CONST>

		* language operators *
		<LANG_OPERATOR> ::= <WHILE_OP> | <IF_OP> | <WRITE_OP> | <ENDL_OP>

		<WRITE_OP> ::= ìwriteî ì(ì while <ARG> , ì)î

		<ENDL_OP> ::= ìendlî

		<WHILE_OP> ::= ìWHILEî <LOGICAL_EXPR> ìDOî while <OPERATION> ìENDWHILEî
	
		<IF_OP> ::=
			ìifî <LOGICAL_EXPR> ìthenî while <OPERATION> ìendifî
			ìifî <LOGICAL_EXPR> ìthenî while <OPERATION> ìelseî while <OPERATION> ìendifî
	

		* operations in while and if operators *
		<OPERATION> ::=
			ìEXITWHILEî ì;î |
			<LANG_OPERATOR> ì;î |
			<ARYTHM_EXPR> ì;î |
			<VAR_DECL> ì;î |	

