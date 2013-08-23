#ifndef VARTABLE_H
#define VARTABLE_H
#include <iostream>
#include <string>
#include <vector>
#include <assert.h>

using namespace std;
typedef struct var_row_t{
	string var_name;
	int varIndex;
} var_row;

class VarTable
{
private:
	vector<var_row> Var_Table;

public:
	VarTable(void);
	~VarTable(void);

	// If varName is not in the VarTable, inserts varName into the VarTable and returns its index.
	// Else, returns its index and the table remains unchanged.
	int insertVar(string varName);

	// Returns the name of a variable at VarTable [index]					
	// If ‘index?is out of range, error
	string getVarName(int index);

	// If varName is in VarTable, returns its index; else, returns -1
	int getVarIndex(string varName);

	// Returns the total number of variables in VarTable
	int getSize();

	// Print VarTable
	void printVarTable();
};
#endif