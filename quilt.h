#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

enum CONSTANTS {a0, a1, a2, a3, b0, b1, b2, b3, a, b, Turn, Sew, HdRow, TlRow, HdCol, TlCol, val, fun, param};

using namespace std;

class Quilt {
public:
	vector<vector<int> > result;		/* Stores quilt in matrix form */
	
	Quilt();
	Quilt(const Quilt&);
	Quilt(int c);	
	void reSize(int rows, int cols);	/* Resizes the quilt matrix*/
	int numRows() const;				/* Returns the number of rows in quilt matrix */
	int numColumns() const;			/* Returns the number of columns in quilt matrix */
	void nextElement(int i, int j);		/* Writes the next element at position (i, j) of quilt matrix */	
	void printQuilt() const;			/* Prints the quilt in matrix form */
	void print_quilt();
};

Quilt turn(Quilt q);			
Quilt sew(Quilt q1, Quilt q2);
Quilt hdRow(Quilt q);
Quilt tlRow(Quilt q);
Quilt hdCol(Quilt q);
Quilt tlCol(Quilt q); 
