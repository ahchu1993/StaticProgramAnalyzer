#include "StmtTable.h"
StmtTable::StmtTable(void)
{
}

StmtTable::~StmtTable(void)
{
}

int StmtTable::insertStmt(int stmtNo, string type)
{
	//assert(stmtNo>=0 && (type.compare("assign") == 0 || type.compare("call") == 0 || type.compare("while") == 0 || type.compare("if") == 0));
	stmt_row temp_row;

	for (unsigned i = 0; i<Stmt_Table.size(); i++)
	{
		temp_row = Stmt_Table.at(i);
		if (temp_row.stmt_no == stmtNo)
		{
			cout<< "The stmt number is already in the StmtTable.\n";
			return -1;
		}
	}

	stmt_row new_row;
	new_row.stmt_no = stmtNo;
	new_row.type = type;
	Stmt_Table.push_back(new_row);

	return 1;
}

vector<int> StmtTable:: getStmtNo(string type)
{
	assert(type.compare("assign") == 0 || type.compare("call") == 0 || type.compare("while") == 0 || type.compare("if") == 0 || type.compare("stmt") == 0 || type.compare("prog_line") == 0);

	vector<int> ans;
	stmt_row temp_row;

	if (type.compare("assign") == 0 || type.compare("call") == 0 || type.compare("while") == 0 || type.compare("if") == 0)
	{
		for (unsigned i = 0; i<Stmt_Table.size(); i++)
		{
			temp_row = Stmt_Table.at(i);
			if (temp_row.type.compare(type) == 0)
			{
				ans.push_back(temp_row.stmt_no);
			}
		}
	} else if (type.compare("stmt") == 0 || type.compare("prog_line") == 0)
	{
		for (unsigned i = 0; i<Stmt_Table.size(); i++)
		{
			temp_row = Stmt_Table.at(i);
			ans.push_back(temp_row.stmt_no);
		}
	}

	return ans;
}

string StmtTable::getStmtType(int stmtNo)
{
	assert(stmtNo >= 0);

	stmt_row temp_row;

	for (unsigned i = 0; i<Stmt_Table.size(); i++)
	{
		temp_row = Stmt_Table.at(i);
		if (temp_row.stmt_no == stmtNo)
		{
			return temp_row.type;
		}
	}

	return "";
}

int StmtTable::getSize()
{
	return Stmt_Table.size();
}

void StmtTable::printStmtTable()
{
	stmt_row temp_row;

	cout<< "StmtTable:" << endl;
	cout<< "stmt_no" << "\t" << "type" << endl;
	for (unsigned i = 0; i < Stmt_Table.size(); i++)
	{
		temp_row = Stmt_Table.at(i);
		cout << temp_row.stmt_no << "\t" << temp_row.type << endl;
	}
}

/*************************************** New APIs *******************************************/
set<string> StmtTable::getAllStatements(){
	set<string> result;
	stmt_row temp_row;
	ostringstream convert;
	string str;

	for (unsigned i = 0; i < Stmt_Table.size(); i++)
	{
		temp_row = Stmt_Table.at(i);
		convert.str("");
		convert<<temp_row.stmt_no;
		str = convert.str();
		result.insert(str);
	}

	return result;
}