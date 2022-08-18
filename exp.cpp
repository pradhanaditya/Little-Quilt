#include "exp.h"
#include <algorithm>
using namespace std;

int spaceCount = 0;
extern map<int, Quilt>  varResultMap;
extern SymEntry *symEntry;
extern map<int, Exp_Node *> funcExprMapping;
extern FILE *yyout;

vector<vector<Quilt> > actualQuilts;
map<int, Quilt> paramMap;

Exp_Node::Exp_Node() { }
Exp_Node::Exp_Node(const Exp_Node &e)
{
	Quilt q1;
	q1 = e.q;
	this->q = q1;
	this->quadruple = e.quadruple;
	this->tempNumber = e.tempNumber;
}

void Exp_Node::printSpaces(int nSpaces)
{
	for (int i = 0; i < nSpaces; i++)
	{
		fprintf(yyout, " ");
	}
}

void Exp_Node::generateCPPCode()
{
	fprintf(yyout, "#include \"quilt.h\" \n#include <cstdlib> \n\nusing namespace std;\n\nint main()\n{\nQuilt ");
	
	for (int i = 0; i < Quadruple::totalTemp; i++)
	{
		fprintf(yyout, "t%d", i);
		if (i == Quadruple::totalTemp - 1)
		{
			fprintf(yyout, ";\n");
		}
		else
		{
			fprintf(yyout, ", ");
		}
	}
	quadruple->print();
	
	if (Quadruple::totalTemp != 0)
	{
		fprintf(yyout, "\nt%d.print_quilt();\n", Quadruple::totalTemp - 1);
	}	
	fprintf(yyout, "\nreturn 0;\n");
	fprintf(yyout, "}\n");
}

void Exp_Node::printMatrix()
{
	q.printQuilt();
}

A_AST::A_AST() {
	quadruple = new Quadruple(a, NULL, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void A_AST::print() {
	printSpaces(spaceCount);
	fprintf(yyout, "(A\n");
	printSpaces(spaceCount);
	fprintf(yyout, ")\n");
}		

void A_AST::evaluate() {
	q = Quilt(a0);
}

B_AST::B_AST() {
	quadruple = new Quadruple(b, NULL, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void B_AST::print() {
	printSpaces(spaceCount);
	fprintf(yyout, "(B\n");
	printSpaces(spaceCount);
	fprintf(yyout, ")\n");
}

void B_AST::evaluate() {
	q = Quilt(b0);
}

TURN_AST::TURN_AST(Exp_Node *c) {
	child = c;
	quadruple = new Quadruple(Turn, child->quadruple, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void TURN_AST::print() {
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(TURN\n");
	spaceCount += 4;
	child->print();	
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");
}

void TURN_AST::evaluate() {
	child->evaluate();	
	Quilt q1 = child->q;
	q = turn(q1);
}

SEW_AST::SEW_AST(Exp_Node *l, Exp_Node *r) {
	left = l;
	right = r;
	quadruple = new Quadruple(Sew, left->quadruple, right->quadruple, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void SEW_AST::print() {
	int localSpaceCount = spaceCount;	
	printSpaces(spaceCount);
	fprintf(yyout, "(SEW\n");

	spaceCount += 3;	

	left->print();

	printSpaces(localSpaceCount + 3);
	fprintf(yyout, ",\n");

	spaceCount = localSpaceCount + 3;
	right->print();

	printSpaces(localSpaceCount);
	fprintf(yyout, ")\n");
}

void SEW_AST::evaluate() {
	right->evaluate();
	Quilt q2 = right->q;
	left->evaluate();
	Quilt q1 = left->q;
	q = sew(q1, q2);
}

HDROW_AST::HDROW_AST(Exp_Node *c)
{
	child = c;
	quadruple = new Quadruple(HdRow, child->quadruple, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void HDROW_AST::print()
{
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(HDROW\n");
	spaceCount += 5;
	child->print();	
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");
}

void HDROW_AST::evaluate()
{
	child->evaluate();
	q = hdRow(child->q);
}

TLROW_AST::TLROW_AST(Exp_Node *c)
{
	child = c;
	quadruple = new Quadruple(TlRow, child->quadruple, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void TLROW_AST::print()
{
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(TLROW\n");
	spaceCount += 5;
	child->print();	
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");
}

void TLROW_AST::evaluate()
{
	child->evaluate();
	q = tlRow(child->q);	
}

HDCOL_AST::HDCOL_AST(Exp_Node *c)
{
	child = c;
	quadruple = new Quadruple(HdCol, child->quadruple, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void HDCOL_AST::print()
{
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(HDCOL\n");
	spaceCount += 5;
	child->print();	
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");
}

void HDCOL_AST::evaluate()
{
	child->evaluate();
	q = hdCol(child->q);
}

TLCOL_AST::TLCOL_AST(Exp_Node *c)
{
	child = c;
	quadruple = new Quadruple(TlCol, child->quadruple, NULL, Quadruple::totalTemp);
	Quadruple::totalTemp++;
}

void TLCOL_AST::print()
{
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(TLCOL\n");
	spaceCount += 5;
	child->print();	
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");
}

void TLCOL_AST::evaluate()
{
	child->evaluate();
	q = tlCol(child->q);	
}

LET_AST::LET_AST(Exp_Node *decls, Exp_Node *exprs)
{
	this->decls = decls;
	this->exprs = exprs;
}

void LET_AST::print()
{
	int localSpaceCount = spaceCount;	
	printSpaces(localSpaceCount);
	fprintf(yyout, "(LET\n");

	spaceCount = localSpaceCount + 3;	

	printSpaces(localSpaceCount + 3);
	fprintf(yyout, "(DECLS\n");
	spaceCount = localSpaceCount + 5;

	decls->print();

	printSpaces(localSpaceCount + 3);
	fprintf(yyout, ")\n");
	printSpaces(localSpaceCount + 3);
	fprintf(yyout, ",\n");

	spaceCount = localSpaceCount + 3;
	
	printSpaces(localSpaceCount + 3);
	fprintf(yyout, "(IN-EXPR\n");
	spaceCount += 5;		
	exprs->print();

	printSpaces(localSpaceCount + 3);
	fprintf(yyout, ")\n");
	printSpaces(localSpaceCount);
	fprintf(yyout, ")\n");
}

void LET_AST::evaluate()
{
	exprs->evaluate();
	Quilt q1 = exprs->q;
	this->q = q1;
}

CALL_AST::CALL_AST(SymEntry *symEntry, int symEntryIndex, Exp_Node *actuals, Exp_Node *funcExpr)
{
	this->symEntry = symEntry;
	this->symEntryIndex = symEntryIndex;
	this->actuals = actuals;
	this->funcExpr = funcExpr;
}

void CALL_AST::print()
{
	int localSpaceCount = spaceCount;

	int varType = this->symEntry->getType(this->symEntryIndex);
	if (varType == val || varType == param)	//actuals will always be 'NULL' in this case
	{
		if (varType == param) {
			printSpaces(localSpaceCount);
			fprintf(yyout, "(Formal Param\n\n");
			printSpaces(localSpaceCount + 4);
			fprintf(yyout, "%s\n\n", symEntry->getName(this->symEntryIndex));
			printSpaces(localSpaceCount);
			fprintf(yyout, ")\n");
		}
		else
		{
			printSpaces(localSpaceCount);
			fprintf(yyout, "(CALL VAL\n");
			printSpaces(localSpaceCount + 4);
			fprintf(yyout, "(%s\n", symEntry->getName(this->symEntryIndex));
			printSpaces(localSpaceCount + 4);
			fprintf(yyout, ")\n");
			printSpaces(localSpaceCount);	
			fprintf(yyout, ")\n");
		}	
	}
	else if (varType == fun)
	{
		printSpaces(localSpaceCount);
		fprintf(yyout, "(CALL FUN %s\n", symEntry->getName(this->symEntryIndex));
		spaceCount += 4;
		actuals->print();
		printSpaces(localSpaceCount);	
		fprintf(yyout, ")\n");
	}
}

void CALL_AST::evaluate()
{
	int varType = this->symEntry->getType(this->symEntryIndex);

	if (varType == val)
	{
		this->q = varResultMap[this->symEntryIndex];
	}
	else if (varType == param)
	{
		int funcEntry = symEntry->findFuncOfParam(this->symEntryIndex);
		int argPosition = symEntry->findPosOfArg(funcEntry, this->symEntryIndex);
		int size = actualQuilts.size();
		this->q = paramMap[this->symEntryIndex];
	}
	else if (varType == fun)
	{	
		int nParams = symEntry->numberOfParams(this->symEntryIndex);
		vector<bool> b(nParams);
		vector<Quilt> v;

		actualQuilts.push_back(v);
		
		if (this->actuals != NULL)
			this->actuals->evaluate();	

		for (int i = 0; i < nParams; i++)
		{
			paramMap[symEntry->formalParams[this->symEntryIndex][i]] = actualQuilts[actualQuilts.size() - 1][i];
		}

		this->funcExpr->evaluate();
		Quilt q = this->funcExpr->q;
		this->q = q;
		
		actualQuilts.pop_back();
	}
}

DECLS_AST::DECLS_AST(Exp_Node *child1, Exp_Node *child2)
{											
	this->child1 = child1;
	this->child2 = child2;
}

void DECLS_AST::print()
{
	int localSpaceCount = spaceCount;
	child1->print();
	spaceCount = localSpaceCount;
	if (child2 != NULL)
	{
		child2->print();
	}	
}	

void DECLS_AST::evaluate() { }

FUN_AST::FUN_AST() { }

FUN_AST::FUN_AST(char *name, Exp_Node *formals, Exp_Node *expr)
{
	this->name = name;
	this->formals = formals;
	this->expr = expr;
}

void FUN_AST::print()
{
	int localSpaceCount = spaceCount;	
	printSpaces(spaceCount);
	fprintf(yyout, "(FUN %s = \n", this->name);

//	spaceCount += 4;	

	printSpaces(localSpaceCount + 4);
	fprintf(yyout, "(PARAMS\n");
	
	//spaceCount += 4;
	printSpaces(localSpaceCount + 8);
	fprintf(yyout, "(");
	formals->print();
	fprintf(yyout, ")\n");
	printSpaces(localSpaceCount + 4);	
	fprintf(yyout, ")\n");
	
	printSpaces(localSpaceCount + 4);
	fprintf(yyout, ",\n");

	spaceCount = localSpaceCount + 8;
	
	printSpaces(localSpaceCount + 4);
	fprintf(yyout, "(EXPR\n");
			
	expr->print();

	printSpaces(localSpaceCount + 4);
	fprintf(yyout, ")\n");
	printSpaces(localSpaceCount);
	fprintf(yyout, ")\n");
}

void FUN_AST::evaluate()
{
	expr->evaluate();
	Quilt q1 = expr->q;
	this->q = q1;
}

VAL_AST::VAL_AST(char *name, Exp_Node *expr)
{
	this->name = name;
	this->expr = expr;
}

void VAL_AST::print()
{
	int localSpaceCount = spaceCount;
	printSpaces(localSpaceCount);
	fprintf(yyout, "(VAL %s =\n", this->name);
	spaceCount += 4;
	expr->print();
	printSpaces(localSpaceCount);	
	fprintf(yyout, ")\n");	
}

void VAL_AST::evaluate()
{
	expr->evaluate();
	Quilt q1 = expr->q;
	this->q = q1;
}

FORMAL_AST::FORMAL_AST(char *name, Exp_Node *child, bool lastParam)
{
	this->name = name;
	this->child = child;
	this->lastParam = lastParam;
}

void FORMAL_AST::print()
{
	if (this->lastParam)
	{
		fprintf(yyout, "%s", this->name);	
	}
	else
	{
		fprintf(yyout, "%s,", this->name);
		child->print();
	}	
}

void FORMAL_AST::evaluate() { }

ACTUALS_AST::ACTUALS_AST(Exp_Node *expr, Exp_Node *actuals, bool formalParams)
{
	this->expr = expr;
	this->actuals = actuals;
	this->formalParams = formalParams;
}

void ACTUALS_AST::print()
{
	int localSpaceCount = spaceCount;
	if (formalParams)
	{
		expr->print();
		if (actuals != NULL)
		{
			printSpaces(localSpaceCount);
			fprintf(yyout, ",\n");
			actuals->print();
		}
	}
	else
	{
		if (actuals != NULL)
		{			
			printSpaces(localSpaceCount);	
			fprintf(yyout, "(ACTUAL PARAMS\n");
			spaceCount += 4;
		}
		expr->print();

		if (actuals != NULL)
		{
			spaceCount = localSpaceCount;
			actuals->print();
			printSpaces(localSpaceCount + 4);
			fprintf(yyout, ",\n");
			printSpaces(localSpaceCount);
			fprintf(yyout, ")\n");			
		}
	}
}

void ACTUALS_AST::evaluate()
{
	this->expr->evaluate();
	Quilt q1 = this->expr->q;
	int size = actualQuilts.size();
	
	actualQuilts[size - 1].push_back(q1);	
	
	if (this->actuals != NULL)
	{
		this->actuals->evaluate();
	}
}