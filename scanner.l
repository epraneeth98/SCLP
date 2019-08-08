%{
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include "parser.tab.h"
extern int yylinen = 1;
%}



digit [0-9]+
names [_a-zA-Z][a-zA-Z0-9]*
operator [-+*/]
metachar [,;(){}:?]
%%

"&&"    { store_token_name("AND",yytext,yylinen); return AND; }
"||"    { store_token_name("OR",yytext,yylinen); return OR; }
"!"     { store_token_name("NOT",yytext,yylinen); return NOT; }
"<"		{ store_token_name("LESS_THAN",yytext,yylinen); return LESS_THAN; }
">"		{ store_token_name("GREATER_THAN",yytext,yylinen); return GREATER_THAN; }
"<="		{ store_token_name("LESS_THAN_EQUAL",yytext,yylinen); return LESS_THAN_EQUAL; }
">="		{ store_token_name("GREATER_THAN_EQUAL",yytext,yylinen); return GREATER_THAN_EQUAL; }
"=="	{ store_token_name("EQUAL",yytext,yylinen); return EQUAL; }
"!="	{ store_token_name("NOT_EQUAL",yytext,yylinen); return NEQ; }
if  	{ store_token_name("IF",yytext,yylinen); return IF; }
else	{ store_token_name("ELSE",yytext,yylinen); return ELSE; }
while	{ store_token_name("WHILE",yytext,yylinen); return WHILE; }
print  	{ store_token_name("PRINT",yytext,yylinen); return PRINT; }
do	{ store_token_name("DO",yytext,yylinen); return DO; }
int			{ store_token_name("INTEGER",yytext,yylinen); return INTEGER; }
float		{ store_token_name("FLOAT",yytext,yylinen);return FLOAT;}
void		{ store_token_name("VOID",yytext,yylinen);return VOID;}
{metachar}  { store_token_name("META CHAR",yytext,yylinen); return yytext[0];}
{operator}  { store_token_name("ARITHOP",yytext,yylinen); return yytext[0];}
{digit}     { store_token_name("NUM",yytext,yylinen);yylval.number = atoi(yytext);return NUM;}
{digit}"."{digit}    { store_token_name("FNUM",yytext,yylinen);yylval.fnumber = atof(yytext);  return FNUM;}
=			{ store_token_name("ASSIGN_OP",yytext,yylinen); return ASSIGN;}
{names} { store_token_name("NAME",yytext,yylinen); yylval.string_value=new string(yytext); string main = "main";string x =*(yylval.string_value); 
						if(x.compare(main)!=0) {*(yylval.string_value) += "_";} return NAME;}
"//"[^\n]*\n    {yylinen++;}
[\t ]        ;
[\n]          {yylinen++;} 

%%

