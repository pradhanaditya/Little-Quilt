#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "quilt.h"
#include "quadruple.h"
#include "symtab.h"
using namespace std;

class Exp_Node { 
public:
Quilt q;
Quadruple *quadruple;
int tempNumber;

Exp_Node();
Exp_Node(const Exp_Node &);
virtual void print() = 0;
virtual void evaluate() = 0;
void printMatrix();
void generateCPPCode();
void printSpaces(int spaces);
};

class A_AST: public Exp_Node {
public:
A_AST();
void print();
void evaluate();
};

class B_AST: public Exp_Node {
public:
B_AST();
void print();
void evaluate();
};

class TURN_AST: public Exp_Node {
protected:
Exp_Node *child;

public:
TURN_AST(Exp_Node *c);
void print();
void evaluate();
};

class SEW_AST: public Exp_Node {
protected:
Exp_Node *left;
Exp_Node *right;

public:
SEW_AST(Exp_Node *l, Exp_Node *r);
void print();
void evaluate();
};

class HDROW_AST: public Exp_Node {
protected:
Exp_Node *child;

public:
HDROW_AST(Exp_Node *c);
void print();
void evaluate();
};

class TLROW_AST: public Exp_Node {
protected:
Exp_Node *child;

public:
TLROW_AST(Exp_Node *c);
void print();
void evaluate();
};

class HDCOL_AST: public Exp_Node {
protected:
Exp_Node *child;

public:
HDCOL_AST(Exp_Node *c);
void print();
void evaluate();
};

class TLCOL_AST: public Exp_Node {
protected:
Exp_Node *child;

public:
TLCOL_AST(Exp_Node *c);
void print();
void evaluate();
};

class LET_AST: public Exp_Node {
Exp_Node *decls;
Exp_Node *exprs;

public:
LET_AST(Exp_Node *decls, Exp_Node *exprs);
void print();
void evaluate();
};

class CALL_AST: public Exp_Node {
SymEntry *symEntry; 
int symEntryIndex;
Exp_Node *actuals;
Exp_Node *funcExpr;

public:
CALL_AST(SymEntry *symEntry, int symEntryIndex, Exp_Node *actuals, Exp_Node *funcExpr);
void print();
void evaluate();
};

class DECLS_AST: public Exp_Node {
Exp_Node *child1;
Exp_Node *child2;

public:
DECLS_AST(Exp_Node *child1, Exp_Node *child2);
void print();
void evaluate();
};

class FUN_AST: public Exp_Node {
public:
char *name;
Exp_Node *formals;
Exp_Node *expr;
FUN_AST();
FUN_AST(char *name, Exp_Node *formals, Exp_Node *expr);
void print();
void evaluate();
};

class VAL_AST: public Exp_Node {
char *name;
Exp_Node *expr;

public:
VAL_AST(char *name, Exp_Node *expr);
void print();
void evaluate();
};

class FORMAL_AST: public Exp_Node {
char *name;
Exp_Node *child;
bool lastParam;

public:
FORMAL_AST(char *name, Exp_Node *child, bool lastParam);
void print();
void evaluate();
};

class ACTUALS_AST: public Exp_Node {
public:
Exp_Node *expr;
Exp_Node *actuals;
vector<Exp_Node *> actualArguments;
bool formalParams;
ACTUALS_AST(Exp_Node *expr, Exp_Node *actuals, bool formalParams);
void print();
void evaluate();
};
