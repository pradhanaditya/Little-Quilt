#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Quadruple {
	public:	
	static int totalTemp;	
	int op;
	Quadruple *operand1;
	Quadruple *operand2;
	int result;

	Quadruple(int _op, Quadruple* _operand1, Quadruple* _operand2, int _result);
	void print();
};
