#ifndef CALLTABLE_H
#define CALLTABLE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "procTable.h"
#include "Pair.h"

using namespace std;
typedef struct call_row_t{
	string callingProc;
	vector<string> calledProcs;
}call_row;
class CallTable
{
private:
	vector<call_row> callTable;
public:
	CallTable(void);
	void insert(string proc1, string proc2);
	bool isCalled(string proc1, string proc2);
	vector<string> getCallsList(string procName);
	vector<string> getCalledList(string procName);
	vector<string> getCall(string procName);
	vector<string> getCalled(string procName);
	vector<Pair> getCallPairList(vector<string> set1, vector<string> set2);
	int getSize();
	void print();
};

#endif