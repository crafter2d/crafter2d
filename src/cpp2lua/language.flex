%{
/*
 * Lex file for the Cpp2Lua program.
 */
 
#include <stdio.h>
#include <memory.h>
#include <string>

class ASTContents;
class ASTInclude;
class ASTClass;
class ASTBody;
class ASTDeclaration;
class ASTVariables;
class ASTExpression;
class ASTType;

#include "language.h"

extern void yyerror(const char *str);

#define SAVE_TOKEN yylval.string = new std::string(yytext,yyleng)
%}

%option noyywrap

%x C_COMMENT

%%

[\t \n]+
#include					{ return INCLUDE; }
class 						{ return CLASS; }
public						{ return PUBLIC; }
const						{ return CONST; }
void						{ return VOID; }
bool						{ return BOOL; }
int							{ return INT; }
float						{ return FLOAT; }
char						{ return CHAR; }
true						{ return TRUE; }
false						{ return FALSE; }

";"							{ return SCOLON; }
"{"							{ return LBRACKET; }
"}"							{ return RBRACKET; }
"("							{ return LPAREN; }
")"							{ return RPAREN; }
"~"							{ return TILDE; }
","							{ return COMMA; }
":"							{ return COLON; }
"::"						{ return DOUBLECOLON; }

"&"							{ return REFERENCE; }
"*"							{ return POINTER; }

"="							{ return EQUAL; }

"/*"            { BEGIN(C_COMMENT); } 
<C_COMMENT>"*/" { BEGIN(INITIAL); } 
<C_COMMENT>.    { } 

[A-Za-z][A-Za-z0-9]* 		    	{ SAVE_TOKEN; return IDENTIFIER; }
"\""([^\n\"\\]*(\\[.\n])*)*"\""     { SAVE_TOKEN; return LITERAL; }

[0-9]+\.[0-9]*						{ SAVE_TOKEN; return REAL; }
[0-9]+								{ SAVE_TOKEN; return INTEGER; }

%%
