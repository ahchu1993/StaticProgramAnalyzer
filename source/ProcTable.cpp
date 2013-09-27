#include "ProcTable.h"

ProcTable::ProcTable(void)
{
}

ProcTable::~ProcTable(void)
{
}

int ProcTable::insertProc(string procName)
{
	proc_row temp_row;

	for (unsigned i = 0; i<Proc_Table.size(); i++)
	{
		temp_row = Proc_Table.at(i);
		if (temp_row.proc_name.compare(procName) == 0)
		{
			return temp_row.procIndex;
		}
	}

	proc_row new_row;
	new_row.procIndex = Proc_Table.size();
	new_row.proc_name = procName;
	Proc_Table.push_back(new_row);

	return new_row.procIndex;
}

string ProcTable::getProcName(int index)
{
	assert(index >= 0);

	if ((unsigned)index >= Proc_Table.size())
	{
		cout << "There's no variable with this index.\n";
		return "";
	}

	proc_row ans_row = Proc_Table.at(index);
	return ans_row.proc_name;
}

int ProcTable::getProcIndex(string procName)
{
	proc_row temp_row;

	for (unsigned i = 0; i<Proc_Table.size(); i++)
	{
		temp_row = Proc_Table.at(i);
		if (temp_row.proc_name.compare(procName) == 0)
		{
			return temp_row.procIndex;
		}
	}

	return -1;
}

int ProcTable::getSize()
{
	return Proc_Table.size();
}


void ProcTable::printProcTable()
{
	proc_row temp_row;

	cout<< "ProcTable:" << endl;
	cout<< "procIndex" << "\t" << "proc_name" << endl;
	for (unsigned i = 0; i < Proc_Table.size(); i++)
	{
		temp_row = Proc_Table.at(i);
		cout<< temp_row.procIndex << "\t" << temp_row.proc_name << endl;
	}
}

vector<int> ProcTable::getProcList(){
	vector<int> results;
	proc_row temp_row;

	for (unsigned i=0; i<Proc_Table.size(); i++){
		temp_row = Proc_Table.at(i);
		results.push_back(temp_row.procIndex);
	}

	return results;
}