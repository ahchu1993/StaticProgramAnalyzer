#include "ConstantTable.h"
ConstantTable::ConstantTable(void)
{
}

ConstantTable::~ConstantTable(void)
{
}

void ConstantTable::insertConst(int stmtNo, int number)
{
	assert(stmtNo >= 0);

	const_row temp_row;

	for (unsigned i = 0; i < Const_Table.size(); i++)
	{
		temp_row = Const_Table.at(i);
		if (temp_row.stmt_no == stmtNo && temp_row.number == number)
		{
			return;
		}
	}

	const_row new_row;
	new_row.stmt_no = stmtNo;
	new_row.number = number;
	Const_Table.push_back(new_row);
}

vector<int> ConstantTable:: getConst(int stmtNo)
{
	assert(stmtNo >= 0);

	vector<int> ans;
	const_row temp_row;

	for (unsigned i = 0; i < Const_Table.size(); i++)
	{
		temp_row = Const_Table.at(i);
		if (temp_row.stmt_no == stmtNo)
		{
			ans.push_back(temp_row.number);
		}
	}

	return ans;
}

vector<int> ConstantTable:: getStmt(int number)
{
	assert(number >= 0);

	vector<int> ans;
	const_row temp_row;

	for (unsigned i = 0; i < Const_Table.size(); i++)
	{
		temp_row = Const_Table.at(i);
		if (temp_row.number == number)
		{
			ans.push_back(temp_row.stmt_no);
		}
	}

	return ans;
}

vector<int> ConstantTable::getAllStmt()
{
	vector<int> ans;
	const_row temp_row;
	bool isExisting;

	for (unsigned i = 0; i < Const_Table.size(); i++)
	{
		temp_row = Const_Table.at(i);
		isExisting = false;
		for (unsigned j = 0; j < ans.size(); j++)
		{
			if (ans.at(j) == temp_row.stmt_no)
			{
				isExisting = true;
				break;
			}
		}

		if (!isExisting)
		{
			ans.push_back(temp_row.stmt_no);
		}
	}

	return ans;
}

void ConstantTable::printConstTable()
{
	const_row temp_row;

	cout<< "ConstantTable:" << endl;
	cout<< "stmt_no" << "\t" << "constant" << endl;
	for (unsigned i = 0; i < Const_Table.size(); i++)
	{
		temp_row = Const_Table.at(i);
		cout << temp_row.stmt_no << "\t" << temp_row.number << endl;
	}
}

set<string> ConstantTable::getAllConstants(){
	set<string> r;
	for(unsigned int i=0;i<Const_Table.size();i++){
		int num = Const_Table.at(i).number;
		ostringstream convert;
		convert << num;
		r.insert(convert.str());
	}
	return r;
}