#include "symtab.h"
#include <cstring>
extern FILE *yyout;

//int val = 16;
//int fun = 17;
int param = 18;

void SymTab::insert(int symEntryIndex)
{
	symEntryList.push_back(symEntryIndex);
}

bool SymTab::lookup(SymEntry *symEntry, char *name)
{
	bool found = false;

	for (int i = symEntryList.size() - 1; i >= 0; i--)
	{
		if (strcmp(symEntry->getName(symEntryList[i]), name) == 0)
		{
			found = true;
			break;
		}
	}

	return found;
}

int SymTab::getSymEntryIndex(SymEntry *symEntry, char *name)
{
	int index;

	for (int i = symEntryList.size() - 1; i >= 0; i--)
	{
		if (strcmp(symEntry->getName(symEntryList[i]), name) == 0)
		{
			index = symEntryList[i];
			break;
		}
	}

	return index;
}

int SymTab::getSymEntryIndex(SymEntry *symEntry, char *name, int currentFuncIndex)
{
	int index;

	for (int i = symEntryList.size() - 1; i >= 0; i--)
	{
		if (strcmp(symEntry->getName(symEntryList[i]), name) == 0)
		{
			index = symEntryList[i];

			if (symEntry->getType(index) == param && symEntry->funSymEntry[index] != currentFuncIndex)
			{
				continue;
			}
			break;
		}
	}

	return index;
}

/*
---------------------------------------
Program which doesn't work correctly
---------------------------------------
let
fun f(x) = turn(x)
fun s(x) = turn(f(x))
in
s(a)
end
*/