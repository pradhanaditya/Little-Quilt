#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
#include "symentry.h"

using namespace std;

class SymTab {
	vector<int> symEntryList;

public:
	void insert(int symEntryIndex);
	bool lookup(SymEntry *symEntry, char *name);
	int getSymEntryIndex(SymEntry *symEntry, char *name);
	int getSymEntryIndex(SymEntry *symEntry, char *name, int currentFuncIndex);
};
