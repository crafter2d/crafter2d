%{
#include <stdio.h>
#include <string>

#include "astcontents.h"
#include "astclass.h"
#include "astinclude.h"
#include "astbody.h"
#include "astfunction.h"
#include "astvariables.h"
#include "astvariable.h"
#include "astexpression.h"
#include "astinteger.h"
#include "astfloat.h"
#include "astboolean.h"
#include "asttype.h"

extern int yylex (void);
void yyerror (char const *str) { printf("ERROR: %s\n", str); }

ASTContents* pcontents = 0;
%}

%union {
	ASTContents* 	pcontents;
	ASTInclude*  	pinclude;
	ASTClass*    	pclass;
	ASTBody*	 	pbody;
	ASTVariables* 	pvariables;
	ASTDeclaration*	pdeclaration;
	ASTExpression*	pexpression;
	ASTType*	 	ptype;
	std::string* 	pident;
	
	std::string* 	string;
}

%token INCLUDE
%token CLASS
%token PUBLIC
%token CONST
%token VOID INT FLOAT BOOL CHAR
%token TRUE FALSE
%token SCOLON COLON DOUBLECOLON
%token TILDE
%token COMMA
%token EQUAL
%token LBRACKET RBRACKET
%token LPAREN RPAREN
%token REFERENCE POINTER

%token <string> IDENTIFIER LITERAL INTEGER REAL

%type <pcontents> contents stmts
%type <pinclude> include
%type <pclass> class
%type <pident> ident
%type <pbody> body 
%type <ptype> type type_id
%type <pdeclaration> decl func var
%type <pvariables> func_args
%type <pexpression> expr numeric

%start contents

%%

contents: 
		stmts { pcontents = $1; }

stmts: 
		stmt { $$ = new ASTContents(); $$->add($<pcontents>1); }
	|	stmts stmt { $1->add($<pcontents>2); }
	;

stmt:
		include
	|   class
	;

include:
		INCLUDE LITERAL { $$ = new ASTInclude($2); delete $2; }
	;
	
class:
		CLASS ident LBRACKET body RBRACKET SCOLON { $$ = new ASTClass($2); $$->add($4); delete $2;}
	|	CLASS ident COLON PUBLIC ident LBRACKET body RBRACKET SCOLON { $$ = new ASTClass($2, $5); $$->add($7); delete $2; delete $5; }
	;
	
body:
		decl { $$ = new ASTBody(); $$->add($1); }
	|	body decl { $1->add($2); }
	;
	
decl:
		var SCOLON
	|	func SCOLON
	;
	
var:
		type ident { $$ = new ASTVariable($1, $2); delete $2; }
	|	type ident EQUAL expr { $$ = new ASTVariable($1, $2, $4); delete $2; }
	;
	
func:
		ident LPAREN func_args RPAREN { $$ = new ASTFunction(ASTFunction::eConstructor, $1, $3); delete $1; }
	|	TILDE ident LPAREN RPAREN  { $$ = new ASTFunction(ASTFunction::eDestructor, $2); delete $2; }
	|	type ident LPAREN func_args RPAREN { $$ = new ASTFunction($1, $2, $4); delete $2; }
	|	type ident LPAREN func_args RPAREN CONST { $$ = new ASTFunction($1, $2, $4, true); delete $2; }
	;
	
func_args:
		/* blank */ { $$ = new ASTVariables(); }
	|	var { $$ = new ASTVariables(); $$->add($1); }
	|	func_args COMMA var { $1->add($3); }
	;
	
expr:
		numeric
	|	LITERAL	{ $$ = new ASTExpression(); }
	;

type:
		type_id	{ $$ = $1; }
	|	CONST type_id { $$ = $2; $$->isConst(true); }
	|	type_id REFERENCE	{ $$ = $1; $$->isReference(); }
	|	type_id POINTER		{ $$ = $1; $$->isPointer(); }
	| 	CONST type_id REFERENCE	{ $$ = $2; $$->isConst(true); $$->isReference(); }
	| 	CONST type_id POINTER	{ $$ = $2; $$->isConst(true); $$->isPointer(); }
	;
	
type_id:
		VOID	{ $$ = new ASTType(ASTType::eVoid); }
	|	INT 	{ $$ = new ASTType(ASTType::eInt); }
	| 	FLOAT	{ $$ = new ASTType(ASTType::eFloat); }
	|	BOOL	{ $$ = new ASTType(ASTType::eBool); }
	|	CHAR	{ $$ = new ASTType(ASTType::eChar); }
	|	ident	{ $$ = new ASTType($1); delete $1; }
	|	ident DOUBLECOLON ident { $$ = new ASTType($3,$1); delete $1; delete $3; }
	;
	
ident:
		IDENTIFIER { $$ = new std::string(*$1); delete $1 }
	;
	
numeric:
		INTEGER	{ $$ = new ASTInteger(atoi($1->c_str())); delete $1; }
	|	REAL	{ $$ = new ASTFloat(atof($1->c_str())); delete $1; }
	|	TRUE	{ $$ = new ASTBoolean(true); }
	|	FALSE	{ $$ = new ASTBoolean(false); }
	;

%%

