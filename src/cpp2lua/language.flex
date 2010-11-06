%{
/*
 * Lex file for the Cpp2Lua program.
 */
 
#include <stdio.h>
#include <memory.h>
#include <string>

#include "astcontents.h"
#include "astclass.h"
#include "language.h"

extern void yyerror(const char *str);
%}

%x C_COMMENT

%%

[\t \n]+
#include					{ return INCLUDE; }
class 						{ return CLASS; }
public						{ return PUBLIC; }
const						{ return CONST; }
void						{ return VOID; }
bool						{ return BOOL; }
float						{ return FLOAT; }
":"							{ return COLON; }

"/*"            { BEGIN(C_COMMENT); } 
<C_COMMENT>"*/" { BEGIN(INITIAL); } 
<C_COMMENT>.    { } 

[A-Za-z][A-Za-z0-9]* 		    	{ yylval.string = new std::string(yytext,yyleng); return IDENTIFIER; }
"\""([^\n\"\\]*(\\[.\n])*)*"\""     { yylval.string = new std::string(yytext,yyleng); return LITERAL; }

%%
