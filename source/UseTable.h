#ifndef USETABLE_H
#define USETABLE_H
#include <vector>
#include <string>
#include "iostream"
#include <assert.h>

using namespace std;
typedef struct use_stmt_row_t{
	int stmtNo;
	int varIndex;
	string DE;
} use_stmt_row;

typedef struct use_proc_row_t{
	int procIndex;
	int varIndex;
} use_proc_row;

class UseTable{
private:
	vector<use_stmt_row> UseStmtTable;
	vector<use_proc_row> UseProcTable;

public:
	UseTable();

	// Insert use relationship to use_stmt table
	// Return -1 if unsuccessful,1 if successful
	int insertUseStmt(int stmtNo, int varIndex, string DE);

	// Insert use relationship to use_proc table
	// Return -1 if unsuccessful, 1 if successful
	int insertUseProc(int procIndex, int varIndex);

	// Return the index of variable used in statememnt stmtNo
	vector<int> getUsedStmt(int stmtNo);

	// Return the index of variables used in procedure procNo
	vector<int> getUsedProc(int procIndex);

	// Return list of stmt of type DE using variable having index
	vector<int> getUsedList(int varIndex, string DE);

	// Print UseTable
	void printUseTable();
};
#endif