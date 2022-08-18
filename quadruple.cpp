#include "quadruple.h"
#include "quilt.h"

int Quadruple::totalTemp = 0;

extern vector<const char*> values;
extern FILE *yyout;

Quadruple::Quadruple(int _op, Quadruple* _operand1, Quadruple* _operand2, int _result)
{
	this->op = _op;
	this->operand1 = _operand1;
	this->operand2 = _operand2;
	this->result = _result;
}

void Quadruple::print()
{
	if (operand1 == NULL && operand2 == NULL)
	{
		fprintf(yyout, "t%d = Quilt(%s0);\n", result, values[op]);
	}	
	else if (operand2 == NULL)
	{
		operand1->print();
	
		fprintf(yyout, "t%d = %s(", result, values[op]);

		fprintf(yyout, "t%d", operand1->result);
		
		fprintf(yyout, ");\n");
	}
	else
	{		
		operand1->print();
		operand2->print();

		fprintf(yyout, "t%d = %s(", result, values[op]);
		
		fprintf(yyout, "t%d", operand1->result);

		fprintf(yyout, ", ");

		fprintf(yyout, "t%d", operand2->result);

		fprintf(yyout, ");\n");
	}
}
