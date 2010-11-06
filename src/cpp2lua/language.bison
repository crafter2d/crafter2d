%{
#include <stdio.h>
#include <string>
#include "astcontents.h"
#include "astclass.h"

extern int yylex (void);
void yyerror (char const *);

ASTContents* pcontents = 0;
%}

%union {
	ASTContents* contents;
	std::string* string;
}

%token INCLUDE
%token CLASS
%token PUBLIC
%token CONST
%token VOID
%token FLOAT
%token BOOL
%token IDENTIFIER
%token LITERAL
%token COLON

%start contents

%type <contents> contents statements

%%

contents: statements { pcontents = $1; }

statements: statement { $$ = new ASTContents(); $$->add($<contents>1); }
	|	statements statement
	;

statement:
		INCLUDE LITERAL { printf("found include %s", yyval.string); delete yyval.string; }
	|   class
	;
	
class:
		CLASS identifier { printf("found class %s", yylval.string); delete yyval.string;}
	|	CLASS identifier COLON PUBLIC { printf("found inherited class %s", yyval.string); delete yyval.string; }
	;

identifier:
	IDENTIFIER
	;
%%

void yyerror(const char *str)
{
}
 
int _yywrap()
{
        return 1;
}
