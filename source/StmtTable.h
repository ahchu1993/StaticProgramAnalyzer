#pragma once

#ifndef STMTTABLE_H
#define STMTTABLE_H

#include <iostream>
#include <string>
#include <vector>
#include <assert.h>
#include <set>
#include <sstream>

using namespace std;

typedef struct stmt_row_t{
	int stmt_no;
	string type;
	string controlVar;
} stmt_row;

class StmtTable
{
private:
	vector<stmt_row> Stmt_Table;

public:
	StmtTable(void);
	~StmtTable(void);

	// If stmtNo is not in the StmtTable, inserts stmtNo into the StmtTable and return 1 to indicate success
	// Else, returns -1.
	// type can only be "ASSIGN", "CALL", "IF", "WHILE"
	int insertStmt(int stmtNo, string type);

	// Returns the list of all stmt of type type
	vector<int> getStmtNo(string type);

	// Return the type of given stmt
	string getStmtType(int stmtNo);

	// Returns the total number of stmt in StmtTable
	int getSize();

	// Print StmtTable
	void printStmtTable();

	/*************************************** New APIs *******************************************/
	set<string> getAllStatements();
	// Return 1 to indicate success, -1 otherwise
	int insertControlVar(int stmtNo, string controlVar);
	string getControlVariable(int stmtNo);
};
#endif