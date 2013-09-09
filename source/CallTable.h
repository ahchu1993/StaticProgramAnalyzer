#ifndef CALLTABLE_H
#define CALLTABLE_H

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include "procTable.h"

using namespace std;
typedef struct call_row_t{
	string callingProc;
	vector<string> calledProcs;
}call_row;
class CallTable
{
private:
	vector<call_row> callTable;
	vector<call_row>::iterator it;
public:
	CallTable(void);
	void insert(string proc1, string proc2);
	bool isCalled(string proc1, string proc2);
	vector<string> getCallsList(string procName);
	vector<string> getCalledList(string procName);
	int getSize();
	void print();
};

#endif