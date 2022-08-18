#include "symentry.h"

int fun = 17;
int val = 16;

int SymEntry::addEntry(char *name, int type)
{
	int symEntryIndex = this->name.size();
	this->name.push_back(name);
	this->type.push_back(type);
	this->funSymEntry.push_back(-1);
	this->actualArgCounter.push_back(0);
	return symEntryIndex;
}

int SymEntry::addEntry(char *name, int type, int funSymEntry)
{
	int symEntryIndex = this->name.size();
	this->name.push_back(name);
	this->type.push_back(type);
	this->funSymEntry.push_back(funSymEntry);
	this->actualArgCounter.push_back(0);
	return symEntryIndex;
}

bool SymEntry::lookup(char *name)
{
	int n = this->name.size();
	bool found = false;

	for (int i = 0; i < n; i++)
	{
		if (this->name[i] == name)
		{
			found = true;
			break;
		}
	}

	return found;
}

char* SymEntry::getName(int symEntryIndex) 
{
	return this->name[symEntryIndex];
}

int SymEntry::getType(int symEntryIndex)
{
	return this->type[symEntryIndex];
}

void SymEntry::addFormalParam(int funIndex, int paramIndex)
{
	this->formalParams.resize(this->name.size());
	this->formalParams[funIndex].push_back(paramIndex);
}

int SymEntry::findFuncOfParam(int paramIndex)
{
	int funcSymEntry;

	for (int i = 0; i < type.size(); i++)
	{
		if (this->type[i] == fun)
		{
			for (int j = 0; j < formalParams[i].size(); j++)
			{
				if (formalParams[i][j] == paramIndex)
				{
					funcSymEntry = i;
				}
			}
		}
	}

	return funcSymEntry;
}

int SymEntry::findPosOfArg(int funcSymEntry, int paramIndex)
{
	int argPosition;
	for (int i = 0; i < formalParams[funcSymEntry].size(); ++i)
	{
		if (formalParams[funcSymEntry][i] == paramIndex)
		{
			argPosition = i;
		}
	}

	return argPosition;
}

int SymEntry::numberOfParams(int funcSymEntry)
{
	return formalParams[funcSymEntry].size();
}

void SymEntry::printSymEntry()
{
	for (int i = 0; i < type.size(); i++)
	{
		cout << "Index: " << i << "\tName: " << name[i] << "\tType: " << type[i];
		if (type[i] == fun)
		{
			cout << "\tFormalParams: ";
			for (int j = 0; j < formalParams[i].size(); j++)
			{
				cout << formalParams[i][j] << " ";
			}
		}
		cout << endl;
	}
}