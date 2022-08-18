%{
#include <string>
#include <iostream>
#include <stack>
#include "exp.h"
#include "parse.tab.h"

using namespace std;

extern FILE *yyout;
extern bool printTokens;
extern stack<char *> names;
%}

%option noyywrap

ws	[ \t\n]*
lb	[(]
rb	[)]
letter	[a-zA-Z_]
digit	[0-9]

%%
a				{if(printTokens) fprintf(yyout, "\tToken Name: A\t\tToken: a\tLineno: %d\n", yylineno);				return A;}
b				{if(printTokens) fprintf(yyout, "\tToken Name: B\t\tToken: b\tLineno: %d\n", yylineno);				return B;}
{lb}			{if(printTokens) fprintf(yyout, "\tToken Name: '('\t\tToken: (\tLineno: %d\n", yylineno);			return LEFT_BRACKET;}
{rb}			{if(printTokens) fprintf(yyout, "\tToken Name: ')'\t\tToken: )\tLineno: %d\n", yylineno);			return RIGHT_BRACKET;}
turn			{if(printTokens) fprintf(yyout, "\tToken Name: TURN\tToken: turn\tLineno: %d\n", yylineno);			return TURN;}
sew				{if(printTokens) fprintf(yyout, "\tToken Name: SEW \tToken: sew \tLineno: %d\n", yylineno);			return SEW;}
hdRow			{if(printTokens) fprintf(yyout, "\tToken Name: HDROW    Token: hdRow    Lineno: %d\n", yylineno);	return HDROW;}
tlRow			{if(printTokens) fprintf(yyout, "\tToken Name: TLROW    Token: tlRow    Lineno: %d\n", yylineno);	return TLROW;}
hdCol			{if(printTokens) fprintf(yyout, "\tToken Name: HDCOL    Token: hdCol    Lineno: %d\n", yylineno);	return HDCOL;}
tlCol			{if(printTokens) fprintf(yyout, "\tToken Name: TLCOL    Token: tlCol    Lineno: %d\n", yylineno);	return TLCOL;}
,				{if(printTokens) fprintf(yyout, "\tToken Name: ','\t\tToken: ,\tLineno: %d\n", yylineno);			return COMMA;}
let				{if(printTokens) fprintf(yyout, "\tToken Name: LET\t\tToken: Let\tLineno: %d\n", yylineno);			return LET;}
in				{if(printTokens) fprintf(yyout, "\tToken Name: IN\t\tToken: in\tLineno: %d\n", yylineno);			return IN;}
end				{if(printTokens) fprintf(yyout, "\tToken Name: END\t\tToken: end\tLineno: %d\n", yylineno);			return END;}
fun				{if(printTokens) fprintf(yyout, "\tToken Name: FUN\t\tToken: fun\tLineno: %d\n", yylineno);			return FUN;}
val				{if(printTokens) fprintf(yyout, "\tToken Name: VAL\t\tToken: val\tLineno: %d\n", yylineno);			return VAL;}
=				{if(printTokens) fprintf(yyout, "\tToken Name: EQUALS\t\tToken: =\tLineno: %d\n", yylineno);		return EQUALS;}
{letter}({letter}|{digit})*		{
								if(printTokens) fprintf(yyout, "\tToken Name: NAME\t\tToken: Identifier,\tLineno: %d\n", yylineno);
				 				names.push(strdup(yytext));		
								return NAME;
								}

{ws}					;
.				{ 	
					string error_message;
					error_message =  "Illegal character " + *yytext;
					error_message += " on line " + yylineno;
					cout << error_message << endl;
				}
%%
