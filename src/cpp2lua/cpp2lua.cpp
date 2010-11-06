
#include <stdio.h>


// bison : bison -d language.bison -o language.c
// flex  : flex -I language.flex

extern "C" FILE* yyin;

extern "C" int yyparse();

int main(int argc, char *argv[])
{
   yyin = fopen("game.cpp", "r");
   yyparse();

   fclose(yyin);
   return 0;
}
