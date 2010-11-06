%{
/*
 * Lex file for the Cpp2Lua program.
 */
 
#include <stdio.h>
#include <memory.h>
#include "language.h"

extern void yyerror(const char *str);
%}

%%

[\t \n]+
#include					{ return INCLUDE; }
class 						{ return CLASS; }
public						{ return PUBLIC; }
const						{ return CONST; }
void						{ return VOID; }
bool						{ return BOOL; }
float						{ return FLOAT; }

[A-Za-z][A-Za-z0-9]* 		{ yylval = strdup(yytext); return IDENTIFIER; }

%%
