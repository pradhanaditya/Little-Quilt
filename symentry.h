#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>

using namespace std;

class SymEntry {
	 //Pointers to symbol entries of formal parameters

public:
	vector<char *> name;
	vector<int> type;
	vector<vector<int> > formalParams;
	vector<int> funSymEntry;
	vector<int> actualArgCounter;
	int addEntry(char *name, int type);
	int addEntry(char *name, int type, int funSymEntry);
	char* getName(int symEntryIndex);
	bool lookup(char *name);
	int getType(int symEntryIndex);
	void addFormalParam(int funIndex, int paramIndex);
	int findFuncOfParam(int paramIndex);
	int findPosOfArg(int funcSymEntry, int paramIndex);
	int numberOfParams(int funcSymEntry);
	void printSymEntry();
};