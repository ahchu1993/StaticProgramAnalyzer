#include "UseTable.h"

UseTable::UseTable(void)
{
}

int UseTable::insertUseStmt(int stmtNo, int varIndex, string DE)
{
	assert(stmtNo>=0 && varIndex >= 0 && (DE.compare("assign") == 0 || DE.compare("call") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0));

	use_stmt_row temp_row;

	for (unsigned i = 0; i<UseStmtTable.size(); i++)
	{
		temp_row = UseStmtTable.at(i);
		if (temp_row.stmtNo == stmtNo && temp_row.DE.compare(DE) != 0)
		{
			cout<< "The stmt number is already in the UseTable and is of different DE\n";
			return -1;
		}
		if (temp_row.stmtNo == stmtNo && temp_row.varIndex == varIndex)
		{
			return 1;
		}
	}

	use_stmt_row new_row;
	new_row.stmtNo = stmtNo;
	new_row.varIndex = varIndex;
	new_row.DE = DE;
	UseStmtTable.push_back(new_row);

	return 1;
}

int UseTable::insertUseProc(int procIndex, int varIndex)
{
	assert (procIndex >= 0 && varIndex >= 0);

	use_proc_row temp_row;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_row = UseProcTable.at(i);
		if (temp_row.procIndex == procIndex && temp_row.varIndex == varIndex)
		{
			return 1;
		}
	}

	use_proc_row new_row;
	new_row.procIndex = procIndex;
	new_row.varIndex = varIndex;
	UseProcTable.push_back(new_row);

	return 1;
}

vector<int> UseTable::getUsedStmt(int stmtNo)
{
	assert (stmtNo >= 0);

	use_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_row = UseStmtTable.at(i);
		if (temp_row.stmtNo == stmtNo)
		{
			results.push_back(temp_row.varIndex);
		}
	}

	return results;
}

vector<int> UseTable::getUsedProc(int procIndex)
{
	assert (procIndex >= 0);

	use_proc_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_row = UseProcTable.at(i);
		if (temp_row.procIndex == procIndex)
		{
			results.push_back(temp_row.varIndex);
		}
	}

	return results;
}

vector<int> UseTable::getUsedList(int varIndex, string DE)
{
	assert(varIndex >= 0 && (DE.compare("assign") == 0 || DE.compare("call") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0 || DE.compare("stmt") == 0 || DE.compare("prog_line") == 0));
	
	vector<int> usedList;
	use_stmt_row temp_row;

	if (DE.compare("assign") == 0 || DE.compare("call") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0)
	{
		for (unsigned i =0; i<UseStmtTable.size(); i++)
		{
			temp_row = UseStmtTable.at(i);
			if (temp_row.varIndex == varIndex && temp_row.DE.compare(DE) == 0)
			{
				usedList.push_back(temp_row.stmtNo);
			}
		}
	} else if (DE.compare("stmt") == 0 || DE.compare("prog_line") == 0)
	{
		for (unsigned i =0; i<UseStmtTable.size(); i++)
		{
			temp_row = UseStmtTable.at(i);
			if (temp_row.varIndex == varIndex)
			{
				usedList.push_back(temp_row.stmtNo);
			}
		}
	}

	return usedList;
}

void UseTable::printUseTable()
{
	use_stmt_row temp_row;

	cout<< "UseStmtTable:" << endl;
	cout<< "stmtNo" << "\t" << "varIndex" << "\t" << "DE" << endl;
	for (unsigned i = 0; i < UseStmtTable.size(); i++)
	{
		temp_row = UseStmtTable.at(i);
		cout << temp_row.stmtNo << "\t" << temp_row.varIndex << "\t" << temp_row.DE << endl;
	}

	use_proc_row temp_row_2;

	cout<< "UseProcTable:" << endl;
	cout<< "procIndex" << "\t" << "varIndex" << endl;
	for (unsigned i = 0; i < UseProcTable.size(); i++)
	{
		temp_row_2 = UseProcTable.at(i);
		cout << temp_row_2.procIndex << "\t" << temp_row_2.varIndex << endl;
	}
}