%{
#include "nametable.h"
#include "y.tab.h"
extern int lineno;
%}

strname	[A-Z][A-Za-z0-9_]*
varname [a-z_][A-Za-z0-9_]*
ws	[ \t]+
nl	\n

%%

struct	{ return STRUCT; }
int	{ return INT; }
bool	{ return BOOL; }
string	{ return STRING; }
array	{ return ARRAY; }
enum	{ return ENUM; }

{ws}	  ;
{nl}      { lineno++; }
{strname} { yylval.s = strdup(yytext); return STRNAME; }
{varname} { yylval.s = strdup(yytext); return VARNAME; }
.	  { return yytext[0]; }
