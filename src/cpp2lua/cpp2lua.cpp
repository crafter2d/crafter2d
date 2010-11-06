
#include <stdio.h>
#include "astcontents.h"

// bison : bison -d language.bison -o language.c
// flex  : flex -I language.flex

extern FILE* yyin;

extern int yyparse();

extern ASTContents* pcontents;

int main(int argc, char *argv[])
{
   yyin = fopen("game.cpp", "r");
   yyparse();
   fclose(yyin);

   return 0;
}
