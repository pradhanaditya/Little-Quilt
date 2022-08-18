#include "quilt.h"
using namespace std;

vector<const char*> values = {"a0", "a1", "a2", "a3", "b0", "b1", "b2", "b3", "a", "b", "turn", "sew", "hdRow", "tlRow", "hdCol", "tlCol"};
extern FILE *yyout;

Quilt::Quilt()
{
}

Quilt::Quilt(const Quilt& q)
{
	int nRows = q.numRows();
	int nCols = q.numColumns();
	
	result.resize(nRows);

	for (int i = 0; i < nRows; i++)
	{
		result[i].resize(nCols);
	}
	//cout << "Quilt passed in copy constructor: " << endl;
	//q.printQuilt();

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			this->result[i][j] = q.result[i][j];
		}
	}
}

Quilt::Quilt(int c)
{
	if (c == a0)
	{
		result.resize(1);
		result[0].resize(1);
		result[0][0] = a0;
	}
	else if (c == b0)
	{
		result.resize(1);
		result[0].resize(1);
		result[0][0] = b0;
	}
}

void Quilt::reSize(int rows, int cols)
{
	result.resize(cols);

	for (int i = 0; i < cols; i++)
	{
		result[i].resize(rows);
	}
}

int Quilt::numRows() const
{
	if (result.empty())
	{
		return 0;
	}
	return result.size();
}

int Quilt::numColumns() const
{
	if (result[0].empty())
	{
		return 0;
	}

	return result[0].size();
}

void Quilt::nextElement(int i, int j)
{
	int element = result[i][j];

	if (element == a0 || element == a1 || element == a2 || element == a3)
	{
		if (element == a3)
		{
			result[i][j] = a0;
		}
		else
		{
			result[i][j] = element + 1;
		}
	}	
	else if (element == b0 || element == b1 || element == b2 || element == b3)
	{
		if (element == b3)
		{
			result[i][j] = b0;
		}
		else
		{
			result[i][j] = element + 1;	
		}	
	}
}


void Quilt::printQuilt() const
{
	if (result.empty())
	{
		fprintf(yyout, "ROWS = 0\nCOLUMNS = 0\n");
		fprintf(yyout, "No entries in the matrix.\n");
		return ;
	}

	int nRows = this->numRows();
	int nCols = this->numColumns();
	fprintf(yyout, "ROWS = %d\n", nRows);
	fprintf(yyout, "COLUMNS = %d\n", nCols);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			fprintf(yyout, "%s ", values[result[i][j]]);
		}	
		fprintf(yyout, "\n");
	}
}

void Quilt::print_quilt()
{
	if (result.empty())
	{
		fprintf(stderr, "ROWS = 0\nCOLUMNS = 0\n");
		fprintf(stderr, "No entries in the matrix.\n");
		return ;
	}

	int nRows = this->numRows();
	int nCols = this->numColumns();
	fprintf(stderr, "ROWS = %d\n", nRows);
	fprintf(stderr, "COLUMNS = %d\n", nCols);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			fprintf(stderr, "%s ", values[result[i][j]]);
		}	
		fprintf(stderr, "\n");
	}
}

Quilt turn(Quilt q) 
{
	Quilt qTurn;

	int nRows = q.numRows();
	int nCols = q.numColumns();
	
	qTurn.reSize(nRows, nCols);

	for (int i = 0; i < nRows; i++)
	{
		for (int j = 0; j < nCols; j++)
		{
			qTurn.result[j][nRows - 1 - i] = q.result[i][j];
			qTurn.nextElement(j, nRows - 1 - i);
		}
	}

	return qTurn;		
}

Quilt sew(Quilt q1, Quilt q2)
{
	Quilt qSew;

	int nRows = q1.numRows();
	int nCols = q1.numColumns() + q2.numColumns();
	qSew.result.resize(nRows);
	
	for (int i = 0; i < nRows; i++)
	{
		qSew.result[i].resize(nCols);

		for (int j = 0; j < nCols; j++)
		{
			if (j < q1.result[0].size())
			{
				qSew.result[i][j] = (q1.result[i][j]);
			}
			else
			{
				qSew.result[i][j] = (q2.result[i][j - q1.result[0].size()]);	
			}
		}	
	}	
	return qSew;
}

Quilt hdRow(Quilt q)
{
	Quilt qhdRow;

	int cRows = q.numRows();
	int cCols = q.numColumns();

	qhdRow.result.resize(1);
	qhdRow.result[0].resize(cCols);

	int index = 0;

	if (cRows >= 1 && cCols >= 1)
	{
		for (int j = 0; j < cCols; j++)
		{
			qhdRow.result[0][j] = q.result[0][j];					
		}	
	}

	return qhdRow;
}

Quilt tlRow(Quilt q)
{
	Quilt qtlRow;

	int cRows = q.numRows();
	int cCols = q.numColumns();
	int rowIndex = 0;

	if (cRows > 1)
	{
		for (int i = 1; i < cRows; i++)
		{
			qtlRow.result.resize(qtlRow.result.size() + 1);
			for (int j = 0; j < cCols; j++)
			{
				qtlRow.result[rowIndex].resize(qtlRow.result[rowIndex].size() + 1);
				qtlRow.result[rowIndex][j] = q.result[i][j];
			}
			rowIndex++;
		}		
	}

	return qtlRow;
}

Quilt hdCol(Quilt q)
{
	Quilt qhdCol;

	int cRows = q.numRows();
	int cCols = q.numColumns();

	qhdCol.result.resize(cRows);

	for (int i = 0; i < cRows; i++)
	{
		qhdCol.result[i].resize(1);
	}

	if (cRows >= 1 && cCols >= 1)
	{
		for (int i = 0; i < cRows; i++)
		{
			if (q.result[i][0] != 0)
			{
				qhdCol.result[i][0] = q.result[i][0];
			}					
		}	
	}

	return qhdCol;
}

Quilt tlCol(Quilt q)
{
	Quilt qtlCol;

	int cRows = q.numRows();
	int cCols = q.numColumns();
	int colIndex;

	if (cCols > 1)
	{
		for (int i = 0; i < cRows; i++)
		{
			qtlCol.result.resize(qtlCol.result.size() + 1);		
			colIndex = 0;	
			for (int j = 1; j < cCols; j++)
			{
				qtlCol.result[i].resize(qtlCol.result[i].size() + 1);
				qtlCol.result[i][colIndex++] = q.result[i][j];
			}
		}
	}	

	return qtlCol;
}
