grammar as;

options 
{
    output		= AST;
    language		= C;
    ASTLabelType	= pANTLR3_BASE_TREE;
    backtrack           = true;
}

tokens
{
    IFACE_MEMBER;
    IFACE_VOID_MEMBER;
    TYPEINFOS;
    TYPEINFO;
    COMPOUNT;
    BLOCK;
    MODIFIERS;
    CONSTRUCTOR_DECL;
    FUNCTION_DECL;
    FUNCTION_ARGUMENTS;
    VOID_FUNCDECL;
    VARIABLE_DECL;
    ARRAYINIT;
    LOCALVARDECL;
    STMT_EXPR;
    CAST;
    ACCESS;
    ARRAYACCESS;
    CLASSACCESS;
    FOR_INIT;
    LITERAL;
    EXPRESSION;
    UNARY;
    TYPE;
    TYPELIST;
    TYPEARGUMENT;
    ARGUMENTS;
    ARRAYARGUMENTS;
    ANNOTATIONS;
    ANNOTATION;
}

script 	:	declaration
	;
	
declaration
	:	package_decl?
		use_decl*
		type_decl
	;
	
package_decl
	:	T_PACKAGE identifier ('.' identifier)* ';'			-> ^(T_PACKAGE identifier+)
	;
	
use_decl:	USE identifier ('.' identifier)* ('.' use_identifier)? ';' 			-> ^(USE identifier+ use_identifier?)
	;
	
use_identifier
	:	identifier
	|	MUL
	;
	
type_decl
	:	class_decl
	|	interface_decl
	;
	
// class declaration

class_decl
	:	modifiers CLASS identifier typeinfos? class_inheritance '{' class_member* '}'	-> ^(CLASS modifiers identifier typeinfos? class_inheritance class_member*)
	;

class_inheritance
	:	(EXTENDS type)? (IMPL type_list)?
	;
	
class_member
	:	class_func_decl
	|	class_var_decl
	;
	
class_var_decl
	:	modifiers type variables_decl ';' 					-> ^(VARIABLE_DECL modifiers type variables_decl)
	;
	
class_func_decl
	:	modifiers identifier member_decl_rest					-> ^(CONSTRUCTOR_DECL modifiers identifier member_decl_rest)
	|	annotations modifiers typeinfos? type identifier member_decl_rest 	-> ^(FUNCTION_DECL annotations modifiers typeinfos? type identifier member_decl_rest)
	|	annotations modifiers typeinfos? T_VOID identifier member_decl_rest	-> ^(VOID_FUNCDECL annotations modifiers typeinfos? identifier member_decl_rest)
	;
	
member_decl_rest
	:	func_arguments (block | ';')
	;

// interface declaration

interface_decl
	:	modifiers INTRFACE identifier typeinfos? iface_inheritance '{' iface_member* '}' 	-> ^(INTRFACE modifiers identifier typeinfos? iface_inheritance iface_member*)
	;
	
iface_inheritance
	:	(EXTENDS type_list)?
	;
	
iface_member
	:	modifiers type identifier iface_func_field				-> ^(IFACE_MEMBER modifiers type identifier iface_func_field)
	|	modifiers T_VOID identifier iface_func_field				-> ^(IFACE_VOID_MEMBER modifiers identifier iface_func_field)
	;
	
iface_func_field
	:	('=' expression)? ';'
	|	func_arguments ';'
	;
	
// arguments & generic decl

typeinfos
	:	'<' typeinfo (',' typeinfo)* '>'					-> ^(TYPEINFOS typeinfo+)
	;

typeinfo:	identifier								-> ^(TYPEINFO identifier)
	|	identifier (EXTENDS|SUPER)^ identifier
	;
	
func_arguments
	:	'(' (func_arg_decl (',' func_arg_decl)*)? ')'				-> ^(FUNCTION_ARGUMENTS func_arg_decl*)
	;
	
func_arg_decl
	:	type identifier ('=' expression)?
	;
	
variables_decl
	:	identifier ('=' variable_init)?
	;
	
variable_init
	:	array_init
	|	expression
	;
	
array_init
	:	'{' variable_init (',' variable_init)* '}'				-> ^(ARRAYINIT variable_init+)
	;
	
modifiers
	:	modifier*								-> ^(MODIFIERS modifier*)
	;
	
modifier:	STATIC
	|	FINAL
	|	NATIVE
	|	ABSTRACT
	|	PUBLIC
	|	PRIVATE
	|	PROT
	;
	
// annotations

annotations
	:	annotation*								-> ^(ANNOTATIONS annotation*)
	;
	
annotation
	:	'@' identifier annotation_arguments?					-> ^(ANNOTATION identifier annotation_arguments)
	;
	
annotation_arguments
	:	'(' ')'
	;

// statements
	
block	:	'{' block_stmt* '}' -> ^(BLOCK block_stmt*)
	;

block_stmt
	:	local_var_decl
	|	stmt
	;
	
stmt	:	block
	|	IF compound_expression stmt (options {greedy=true;} : ELSE stmt)?	-> ^(IF compound_expression stmt (stmt)?)
	|	FOR '(' for_declaration ')' stmt					-> ^(FOR stmt for_declaration)
	|	FOREACH '(' type identifier ':' expression ')' stmt			-> ^(FOREACH type identifier expression stmt)
	|	WHILE compound_expression stmt						-> ^(WHILE compound_expression stmt)
	|	DO stmt WHILE compound_expression					-> ^(DO stmt compound_expression)
	|	SWITCH compound_expression '{' switch_case* '}'				-> ^(SWITCH compound_expression switch_case*)
	|	RETURN expression? ';'							-> ^(RETURN expression?)
	|	TRY block catch_block* finally_block?					-> ^(TRY block catch_block* finally_block?)
	|	THROW expression ';'							-> ^(THROW expression)
	|	T_ASSERT expression (':' expression)? ';'				-> ^(T_ASSERT expression expression?)
	|	BREAK^ ';'
	|	CONTINUE^ ';'
	|	stmt_expression
	;
	
for_declaration
	:	for_init? SEP expression? SEP expression_list?
	;
	
for_init:	type identifier '=' expression						-> ^(LOCALVARDECL type identifier expression)
	;
		
catch_block
	:	CATCH '(' type identifier ')' block					-> ^(CATCH type identifier block)
	;
	
finally_block
	:	FINALLY block								-> ^(FINALLY block)
	;
	
switch_case
	:	CASE unary_expression ':' block_stmt*					-> ^(CASE unary_expression block_stmt*)
	|	DEFAULT ':' block_stmt*							-> ^(DEFAULT block_stmt*)
	;
	
local_var_decl
	:	type identifier ('=' variable_init)? ';' 				-> ^(LOCALVARDECL type identifier variable_init?)
	;
	
// expressions
	
stmt_expression
	:	expression ';'								-> ^(STMT_EXPR expression)
	;
	
compound_expression
	:	'(' expression ')'							-> ^(COMPOUNT expression)
	;
	
expression_list
	:	expression (',' expression)*
	;
	
expression
	:	or_expression (assignment_operator expression)?				-> ^(EXPRESSION or_expression (assignment_operator expression)?)
	;
	
assignment_operator
	:	'='
	|	'+='
	|	'-='
	;
	
or_expression
	:	and_expression (OR^ and_expression)*
	;
	
and_expression
	:	bit_or_expression (AND^ bit_or_expression)*
	;
	
bit_or_expression
	:	bit_xor_expression (BITOR^ bit_xor_expression)*
	;
	
bit_xor_expression
	:	bit_and_expression (BITXOR^ bit_and_expression)*
	;
	
bit_and_expression
	:	instanceof_expression (BITAND^ instanceof_expression)*
	;

instanceof_expression
	:	equal_expression (INST_OF^ type)?
	;
	
equal_expression
	:	relational_expression ((EQUAL|UNEQUAL)^ relational_expression)*
	;
	
relational_expression
	:	shift_expression (relational_operator^ shift_expression)?
	;

relational_operator
	:	SEQUAL
	| 	SMALLER
	| 	GREATER
	|	GEQUAL
	;
	
shift_expression
	:	additive_expression ((SHIFTL|SHIFTR)^ additive_expression)*
	;
	
additive_expression
	:	multiplicative_expression ((PLUS|MINUS)^ multiplicative_expression)*
	;

multiplicative_expression
	:	unary_expression ((MUL|DIV|REM)^ unary_expression)*
	;
	
unary_expression
	:	'(' type ')' unary_expression						-> ^(CAST type unary_expression)
	|	unary_pre_operator? primary selector* unary_post_oparator? 		-> ^(UNARY unary_pre_operator? primary selector* unary_post_oparator?)
	;
	
unary_pre_operator
	:	PLUS
	|	MINUS
	|	NOT
	|	BITCOMPL
	|	'++'
	|	'--'
	;
	
unary_post_oparator
	:	'++'
	|	'--'
	;

primary	:	compound_expression
	|	literal									-> ^(LITERAL literal)
	|	identifier type_argument? arguments?					-> ^(ACCESS identifier type_argument? arguments?)
	|	NEW^ type new_arguments
	|	SUPER^ arguments?
	|	THIS^ arguments?
	;
	
selector:	'.' identifier type_argument? arguments?				-> ^(ACCESS identifier type_argument? arguments?)
	|	'.' CLASS								-> ^(CLASSACCESS)
	|	('[' expression ']')+							-> ^(ARRAYACCESS expression+)
	;
	
new_name:	identifier	-> ^(TYPE identifier)
	|	basictype	-> ^(TYPE basictype)
	;
	
new_arguments
	:	arguments
	|	array_argument
	;
	
array_argument
	:	('[' expression ']')+							-> ^(ARRAYARGUMENTS expression+)
	;
		
arguments
	: 	'(' expression_list? ')'						-> ^(ARGUMENTS expression_list?)
	;
	
type_list
	:	type (',' type)*							-> ^(TYPELIST type+)
	;
	
type	:	identifier type_argument? array_decl*					-> ^(TYPE identifier type_argument? array_decl*)
	|	basictype array_decl*							-> ^(TYPE basictype array_decl*)
	;
	
type_argument
	:	'<' type (',' type)? '>'						-> ^(TYPEARGUMENT type+)
	;
	
array_decl
	:	('[' ']')
	;
	
basictype
	:	T_CHAR
	|	T_INT
	|	T_REAL
	|	T_STRING
	|	T_BOOL
	;
	
literal	:	STRING
	|	CHAR
	|	numeric_literal
	|	boolean_literal
	|	LITNULL
	;

numeric_literal
	:	INT
	| 	FLOAT
	;
	
boolean_literal
	:	LITTRUE
	|	LITFALSE
	;
	
identifier
	:	ID
	;

T_PACKAGE	:	'package';
USE	:	'use';
CLASS	:	'class';
INTRFACE:	'interface';
SUPER	:	'super';
THIS	:	'this';
EXTENDS	:	'extends';
IMPL	:	'implements';
NEW	:	'new';
INST_OF	:	'instanceof';
T_ASSERT:	'assert';
FOR	:	'for';
FOREACH	:	'foreach';
WHILE	:	'while';
DO	:	'do';
SWITCH	:	'switch';
CASE	:	'case';
DEFAULT	:	'default'; 
BREAK	:	'break';
CONTINUE:	'continue';
RETURN	:	'return';
IF	:	'if';
ELSE	:	'else';
TRY	:	'try';
CATCH	:	'catch';
FINALLY	:	'finally';
THROW	:	'throw';

LITNULL	:	'null';
LITTRUE :	'true';
LITFALSE:	'false';

AND	:	'&&';
OR	:	'||';
MUL	:	'*';
DIV	:	'/';
REM	:	'%';
PLUS	:	'+';
MINUS	:	'-';
BITCOMPL:	'~';
NOT	:	'!';
BITAND	:	'&';
BITOR	:	'|';
BITXOR	:	'^';
INCR	:	'++';
DECR	:	'--';
EQUAL	:	'==';
UNEQUAL	:	'!=';
SEQUAL	:	'<' '=';
SMALLER	:	'<';
GREATER	:	'>';
GEQUAL	:	'>' '=';
SHIFTL	:	'<' '<';
SHIFTR	:	'>' '>';

T_CHAR	:	'char';
T_INT	:	'int';
T_REAL	:	'real';
T_STRING:	'string';
T_BOOL	:	'boolean';
T_VOID	:	'void';

STATIC	:	'static';
FINAL	:	'final';
ABSTRACT:	'abstract';
NATIVE	:	'native';
SYNC	:	'synchronized';
PUBLIC	:	'public';
PROT	:	'protected';
PRIVATE	:	'private';

SEP	:	';';

ID  :	('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'0'..'9'|'_')*
    ;

INT :	'0'..'9'+
    ;

FLOAT
    :   ('0'..'9')+ '.' ('0'..'9')* EXPONENT?
    |   '.' ('0'..'9')+ EXPONENT?
    |   ('0'..'9')+ EXPONENT
    ;

WHITESPACE
    : ( '\t' | ' ' | '\r' | '\n'| '\u000C' )+ { $channel = HIDDEN; }
    ;
    
STRING
    :  '"' ( ESC_SEQ | ~('\\'|'"') )* '"'
    ;
    
CHAR:  '\'' (ESC_SEQ | ~('\\'|'"') ) '\''
    ;
    
COMMENT
    :   '/*' ( options {greedy=false;} : . )* '*/' {$channel=HIDDEN;}
    ;

LINE_COMMENT
    : '//' ~('\n'|'\r')* '\r'? '\n' {$channel=HIDDEN;}
    ;

fragment
EXPONENT : ('e'|'E') ('+'|'-')? ('0'..'9')+ ;

fragment
HEX_DIGIT : ('0'..'9'|'a'..'f'|'A'..'F') ;

fragment
ESC_SEQ
    :   '\\' ('b'|'t'|'n'|'f'|'r'|'\"'|'\''|'\\')
    |   UNICODE_ESC
    |   OCTAL_ESC
    ;

fragment
OCTAL_ESC
    :   '\\' ('0'..'3') ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7') ('0'..'7')
    |   '\\' ('0'..'7')
    ;

fragment
UNICODE_ESC
    :   '\\' 'u' HEX_DIGIT HEX_DIGIT HEX_DIGIT HEX_DIGIT
    ;
