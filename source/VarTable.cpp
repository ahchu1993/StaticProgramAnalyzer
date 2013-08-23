#include "VarTable.h"
VarTable::VarTable(void)
{
}

VarTable::~VarTable(void)
{
}

int VarTable::insertVar(string varName)
{
	var_row temp_row;

	for (unsigned i = 0; i<Var_Table.size(); i++)
	{
		temp_row = Var_Table.at(i);
		if (temp_row.var_name.compare(varName) == 0)
		{
			return temp_row.varIndex;
		}
	}

	var_row new_row;
	new_row.varIndex = Var_Table.size();
	new_row.var_name = varName;
	Var_Table.push_back(new_row);

	return new_row.varIndex;
}

string VarTable::getVarName(int index)
{
	assert(index >= 0);

	if ((unsigned)index >= Var_Table.size())
	{
		cout << "There's no variable with this index.\n";
		return "";
	}

	var_row ans_row = Var_Table.at(index);
	return ans_row.var_name;
}

int VarTable::getVarIndex(string varName)
{
	var_row temp_row;

	for (unsigned i = 0; i<Var_Table.size(); i++)
	{
		temp_row = Var_Table.at(i);
		if (temp_row.var_name.compare(varName) == 0)
		{
			return temp_row.varIndex;
		}
	}

	return -1;
}

int VarTable::getSize()
{
	return Var_Table.size();
}

void VarTable::printVarTable()
{
	var_row temp_row;

	cout<< "VarTable:" << Var_Table.size()<<endl;
	//cout<< "varIndex" << "\t" << "var_name" << endl;
	for (unsigned i = 0; i < Var_Table.size(); i++)
	{
		temp_row = Var_Table.at(i);
		cout<< temp_row.varIndex << "\t" << temp_row.var_name << endl;
	}
}