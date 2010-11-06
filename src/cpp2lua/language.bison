%{
int yylex (void);
void yyerror (char const *);
%}

%token INCLUDE
%token CLASS
%token PUBLIC
%token CONST
%token VOID
%token FLOAT
%token BOOL
%token IDENTIFIER

%start statements

%%

statements: /* empty */
	|	statements statement
	;

statement:
		INCLUDE { printf("found include"); }
	|   CLASS identifier { printf("found class %s", yylval); }
	;
	
identifier:
	IDENTIFIER
	;
%%

void yyerror(const char *str)
{
}
 
int yywrap()
{
        return 1;
}
