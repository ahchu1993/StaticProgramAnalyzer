#ifndef MODIFYTABLE_H
#define MODIFYTABLE_H
#include <vector>
#include <iostream>
#include <string>
#include <algorithm>
#include <assert.h>
#include "Pair.h"
#include <sstream>

using namespace std;

typedef struct modify_stmt_row_t{
	int stmtNo;
	int varIndex;
	string DE;
} modify_stmt_row;

typedef struct modify_proc_row_t{
	int procIndex;
	int varIndex;
} modify_proc_row;

class ModifyTable{

private:
	vector<modify_stmt_row> ModifyStmtTable;
	vector<modify_proc_row> ModifyProcTable;

public:
	ModifyTable();

	// Insert modify relationship to modify_stmt table
	// Return -1 if unsuccessful, 1 if successful
	int insertModifyStmt(int stmtNo, int varIndex, string DE);

	// Insert modify relationship to modify_proc table
	// Return -1 if unsuccessful, 1 if successful
	int insertModifyProc(int procIndex, int varIndex);

	// Return the index of variables modified in statememnt stmtNo
	vector<int> getModifiedStmt(int stmtNo);

	// Return the index of variables modified in procedure procNo
	vector<int> getModifiedProc(int procIndex);

	// Return list of stmt of type DE modifying variable having index
	vector<int> getModifiedList(int varIndex, string DE);

	// Print ModifyTable
	void printModifyTable();

	/*************************************** New APIs *******************************************/

	// Return index of all procedures in ModifyTable
	vector<int> getModifyProcList();

	// Return index of all stmt in ModifyTable
	vector<int> getModifyStmtList();

	// Return index of all stmt of type DE in ModifyTable
	vector<int> getModifyDEList(string DE);

	// Return index of all variables in ModifyTable
	vector<int> getModifyVarList();

	// Return pairs satisfying set1 and set2
	vector<Pair<string,string>> getModifyPairList(vector<int> set1, vector<int> set2);

	// Return whether there exist a pair satisfying set1 and set2
	bool checkModify(vector<int> set1, vector<int> set2);
};
#endif