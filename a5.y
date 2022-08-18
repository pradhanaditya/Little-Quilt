%{
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <map>
#include "exp.h"

extern "C" void yyerror(char *s);

extern int yylex(void);
extern FILE *yyin;
extern FILE *yyout;

using namespace std;	

Exp_Node *exp;
bool printTokens = false;
bool formalParams = true;

stack<char *> names;
vector<SymTab *> stStack;
stack<int> callStack;
map<int, Quilt>  varResultMap;
map<int, Exp_Node *> funcExprMapping;
SymEntry *symEntry = new SymEntry();

enum options {help, parse, tokens, qast, eval, symtab, compile, d, file};

int optionSelected(char *arg);
void displayAllOptions();
void setOutputFileInfo(int option, char *fileName);
void action(int option);
bool isOptionLegal(int option);
void displayMessage(int option, bool isDSelected);
SymTab* stackTop();
bool searchElement(char *name);
int searchSymEntry(char *name);
%}

%start Start

%union{
   Exp_Node *exp;
   char *name;
}

%token A
%token B
%token LEFT_BRACKET
%token RIGHT_BRACKET
%token TURN
%token SEW
%token COMMA
%token HDROW
%token TLROW
%token HDCOL
%token TLCOL
%token LET
%token IN
%token END
%token FUN
%token VAL
%token EQUALS
%token NAME
%type <exp> Expression
%type <exp> Declarations
%type <exp> Declaration
%type <exp> Formals
%type <exp> Actuals
%%

Start :	Expression  {exp = $1;}
	;

Expression : TURN LEFT_BRACKET Expression RIGHT_BRACKET {$$ = new TURN_AST($3);}
		     |                      
		     SEW LEFT_BRACKET Expression COMMA Expression RIGHT_BRACKET {$$ = new SEW_AST($3, $5);}
		     |
		     HDROW LEFT_BRACKET Expression RIGHT_BRACKET	{$$ = new HDROW_AST($3);}
		     |
		     TLROW LEFT_BRACKET Expression RIGHT_BRACKET	{$$ = new TLROW_AST($3);}
		     |
		     HDCOL LEFT_BRACKET Expression RIGHT_BRACKET	{$$ = new HDCOL_AST($3);}
		     |
		     TLCOL LEFT_BRACKET Expression RIGHT_BRACKET	{$$ = new TLCOL_AST($3);}
	         |
		     LET {stStack.push_back(new SymTab());} Declarations IN  Expression {stStack.pop_back();} END		{$$ = new LET_AST($3, $5);}
		     |
		     NAME {if(!searchElement(names.top())) {
		     	fprintf(yyout, "Syntax Error: Function %s not defined!\n", names.top()); return 1;
		     }
		     callStack.push(searchSymEntry(names.top()));
		 	 } 
		     LEFT_BRACKET Actuals RIGHT_BRACKET	{
		     	int symEntryIndex = searchSymEntry(names.top());
		     	Exp_Node *funcExpr = funcExprMapping[symEntryIndex];
		     	$$ = new CALL_AST(symEntry, symEntryIndex, $4, funcExpr); 	
		     	callStack.pop(); 
		     	names.pop();
		     } 
	 	     |
		     NAME {if(!searchElement(names.top())) {
		     	fprintf(yyout, "Syntax Error: Variable %s not defined!\n", names.top()); return 1;
		     }
		 	 }						{$$ = new CALL_AST(symEntry, searchSymEntry(names.top()), NULL, NULL); names.pop();}	
		     |
		     A						{$$ = new A_AST();}
		     |
		     B						{$$ = new B_AST();}
		     ;

Declarations : 	Declaration Declarations	{$$ = new DECLS_AST($1, $2);}
	    	    |
				Declaration 				{$$ = new DECLS_AST($1, NULL); formalParams = false;} 
		   		;

Declaration : FUN NAME 
			  {
			  	int symEntryIndex = symEntry->addEntry(names.top(), fun); 
				stackTop()->insert(symEntryIndex);
				callStack.push(symEntryIndex);
			  } 
			  LEFT_BRACKET {stStack.push_back(new SymTab());} Formals RIGHT_BRACKET EQUALS Expression
			  {
			  	callStack.pop();
			  	stStack.pop_back();
			  	$$ = new FUN_AST(names.top(), $6, $9);
			  	funcExprMapping[searchSymEntry(names.top())] = $$;
			  	names.pop();
			  }
	  	      |
		      VAL NAME EQUALS  {int symEntryIndex = symEntry->addEntry(names.top(), val);stackTop()->insert(symEntryIndex); }  Expression	  
		     {
		       	$$ = new VAL_AST(names.top(), $5);
		       	$5->evaluate();
		       	Quilt q = $5->q; 
		       	varResultMap[searchSymEntry(names.top())] = q;
		        names.pop();
		     }
		      ;

Formals :	NAME {int symEntryIndex = symEntry->addEntry(names.top(), param, callStack.top()); stackTop()->insert(symEntryIndex); symEntry->addFormalParam(callStack.top(), symEntryIndex);} COMMA Formals					   {$$ = new FORMAL_AST(names.top(), $4, false); names.pop();}
			|
			NAME {int symEntryIndex = symEntry->addEntry(names.top(), param, callStack.top()); stackTop()->insert(symEntryIndex);  $$ = new FORMAL_AST(names.top(), NULL, true); names.pop(); symEntry->addFormalParam(callStack.top(), symEntryIndex);}
		    ;

Actuals :	Expression COMMA Actuals			   
		    {
		    	$$ = new ACTUALS_AST($1, $3, formalParams);
			}
			|
			Expression 
			{
				$$ = new ACTUALS_AST($1, NULL, formalParams);
			}
		    ;
%%

int main(int argc, char *argv[])
{
	bool selectedOption[9] = {false};
	char *fileName = strdup("out");
	bool allOptionsUnselected = true;

	if (argc > 1)
	{		
		for (int i = 1; i < argc; i++)
		{
			int option = optionSelected(argv[i]);		
			
			if (option == file && selectedOption[option])
			{
				cout << "compiler error: File: : Only one input file name can be provided";
				displayAllOptions();
				return 0;
			}
			else if (option == file)
			{
				yyin = fopen(argv[i], "r");
				fileName = argv[i];
			}
			else
			{
				if (!isOptionLegal(option))
				{
					cout << "compiler error: File:  : Unknown option '" << argv[i] << "'" << endl;
			  		displayAllOptions();
					return 0;				
				}
			}
			selectedOption[option] = true;
		}
	}

	if (selectedOption[help])
	{
		action(help);
		return 0;
	}

	if (selectedOption[tokens])
	{
		if (!selectedOption[d])
		{
			setOutputFileInfo(tokens, fileName);
			displayMessage(tokens, selectedOption[d]);
			cout << fileName << endl;
		}		
		else
		{
			displayMessage(tokens, selectedOption[d]);
		}
		action(tokens);
	}

	yyparse();
	
	if (argc == 0)
	{
		action(eval);
	}	
	else
	{
		for (int option = help; option < file; option++)
		{
			if (option == tokens) 
			{
				allOptionsUnselected = false;
				continue;
			}

			if (option != d && selectedOption[option])
			{
				allOptionsUnselected = false;
			}		
			if (selectedOption[option]) 
			{
				if (!selectedOption[d] && option != parse && option != symtab) {
					setOutputFileInfo(option, fileName);
					displayMessage(option, selectedOption[d]);					
					cout << fileName << endl;
				}
			        else if (option != parse && option != symtab) 
				{
					displayMessage(option, selectedOption[d]);
				}
				action(option);
			}
		}
	}

	if (allOptionsUnselected)
	{
		if (!selectedOption[d]) {
			setOutputFileInfo(eval, fileName);
			displayMessage(eval, selectedOption[d]);					
			cout << fileName << endl;
		}
	        else
		{
			displayMessage(eval, selectedOption[d]);
		}
		
		action(eval);
	}

	fclose(yyin);
	fclose(yyout);
	return 0;
}

int optionSelected(char *arg)
{
	int option;
	
	if (strcmp(arg, "-help") == 0) {
		option = help;
	}
	else if (strcmp(arg, "-parse") == 0) {
		option = parse;
	}
	else if (strcmp(arg, "-tokens") == 0) {
		option = tokens;
	}
	else if (strcmp(arg, "-qast") == 0) {
		option = qast;
	}
	else if (strcmp(arg, "-eval") == 0) {
		option = eval;
	}
	else if (strcmp(arg, "-symtab") == 0) {
		option = symtab;
	}
	else if (strcmp(arg, "-compile") == 0) {
		option = compile;
	}
	else if (strcmp(arg, "-d") == 0) {
		option = d;
	}
	else if (arg[0] != '-') {
		option = file;
	}
	else {
		option = -1;
	}

	return option;
}

void displayAllOptions()
{
	cout << "\n\tUsage: compiler [options] [file]\n" <<
		"\t\t\tOptions:\n" <<
		"\t\t\t-help     Show this help\n" <<
		"\t\t\t-parse    Stop processing with parsing\n" <<
		"\t\t\t-tokens   Show the tokens in file.toks (or out.toks)\n" <<
		"\t\t\t-qast     Show the abstract syntax trees in file.ast (or out.ast)\n" <<
		"\t\t\t-eval     Interpret the program and show a trace of the execution in file.matrix (or out.matrix)\n" << 
		"\t\t\t-symtab   Show the symbol table of declarations in file.sym (or out.sym)\n" <<
		"\t\t\t-compile  Compile the program and generate C++ code in file.lqc.cpp (or out.lqc.cpp)\n" <<
		"\t\t\t-d        Demo version. Use stdout for the outputs instead of files\n\n" << endl;
}

void setOutputFileInfo(int option, char *fileName)
{
	switch(option)
	{
	   case tokens:   fileName = strcat(fileName, ".toks"); 	
		          break;

	   case qast:     fileName = strcat(fileName, ".ast");
		          break;
	
	   case eval:     fileName = strcat(fileName, ".matrix");
    			  break;
	   
	   case compile:  fileName = strcat(fileName, ".lqc.cpp");
			  break;
			
	   default:  	  break;
	}

	yyout = fopen(fileName, "w");
}

void action(int option)
{
	switch(option)
	{
	   case help:     displayAllOptions();
		          break;

	   case parse:    cout << "Not yet implemented!" << endl;
			  break;

	   case tokens:   printTokens = true;
		          break;

	   case qast:     exp->print();
		          break;
	
	   case eval:     exp->evaluate();
		     	  exp->printMatrix();
		          break;

	   case symtab:   cout << "Not yet implemented!" << endl;
		          break;

	   case compile:  exp->generateCPPCode();
		          break;
			
	   default:  	  break;
	}
}

bool isOptionLegal(int option)
{
	if (option == help || option == parse || option == tokens || option == qast || option == eval || option == 					     symtab || option == compile || option == d)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void displayMessage(int option, bool isDSelected)
{
	switch(option)
	{
	   case tokens:   if (isDSelected)
			  {
				cout << "--------------------\nPrinting the Tokens\n--------------------\n" << endl;
			  }
			  else
			  {
				cout << "Printing the tokens in file ";
			  }  	
		          break;

	   case qast:     if (isDSelected)
			  {
				cout << "----------------\nPrinting the AST\n----------------\n" << endl;
			  }
			  else
			  {
				cout << "Printing the AST in file ";
			  } 
		          break;
	
	   case eval:     if (isDSelected)
			  {
				cout << "----------------------------------\nPrinting the quilt in matrix form\n" << 					"----------------------------------\n" << endl;
			  }
			  else
			  {
				cout << "Printing the quilt in matrix form in file ";
			  } 
			  break;
	   
	   case compile:  if (isDSelected)
			  {
				cout << "----------------------\nPrinting the C++ code\n----------------------\n" << endl;

			  }
			  else
			  {
				cout << "Printing the C++ code in file ";
			  } 
		          break;
			
	   default:  	  break;
	}
}

SymTab* stackTop()
{
	if (!stStack.empty())
	{
		return stStack[stStack.size() - 1];
	}

	return NULL;
}

bool searchElement(char *name)
{
	for (int i = stStack.size() - 1; i >= 0; i--)
	{
		if (stStack[i]->lookup(symEntry, name))
		{
			return true;
		}
	}

	return false;
}

int searchSymEntry(char *name)
{
	bool found = false;

	for (int i = stStack.size() - 1; i >= 0; i--)
	{
		found = stStack[i]->lookup(symEntry, name);

		if (found)
		{
			if (callStack.empty())
			{	
				return stStack[i]->getSymEntryIndex(symEntry, name);		
			}
			else
			{
				return stStack[i]->getSymEntryIndex(symEntry, name, callStack.top());
			}
		}
	}

	return -1;
}

void yyerror (char *s) {
   fprintf (stderr, "%s\n", s);
}
