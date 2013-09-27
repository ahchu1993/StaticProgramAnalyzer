#ifndef PROCTABLE_H
#define PROCTABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <stdio.h>
#include <assert.h>

using namespace std;
typedef struct proc_row_t{
	string proc_name;
	int procIndex;
} proc_row;
class ProcTable
{

private:
	vector<proc_row> Proc_Table;

public:
	ProcTable(void);
	~ProcTable(void);

	// If procName is not in the ProcTable, inserts procName into the ProcTable and returns its index.
	// Else, returns its index and the table remains unchanged.
	int insertProc(string procName);

	// Returns the name of a procedure at ProcTable [index]
	// If index is out of range, error
	string getProcName(int index);

	// If procName is in ProcTable, returns its index; else, returns -1
	int getProcIndex(string procName);

	// Returns the total number of procedures in ProcTable
	int getSize();

	// Print ProcTable
	void printProcTable();

	/*************************************** New APIs *******************************************/

	// Return list of all proc index
	vector<int> getProcList();
};
#endif