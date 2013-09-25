#include "ModifyTable.h"

ModifyTable::ModifyTable(void)
{
}

int ModifyTable::insertModifyStmt(int stmtNo, int varIndex, string DE)
{
	assert(stmtNo>=0 && varIndex >= 0 && (DE.compare("assign") == 0 || DE.compare("call") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0));
	
	bool flag = false;

	modify_stmt_row temp_row;

	for (unsigned i = 0; i<ModifyStmtTable.size(); i++)
	{
		temp_row = ModifyStmtTable.at(i);
		if (temp_row.stmtNo == stmtNo && temp_row.DE.compare(DE) != 0)
		{
			cout<< "The stmt number is already in the ModifyTable and is of different DE\n";
			return -1;
		}
		if (temp_row.stmtNo == stmtNo && temp_row.DE.compare("assign") == 0 && temp_row.varIndex != varIndex)
		{
			cout<< "The stmt number is already in the ModifyTable and modifies different variable\n";
			return -1;
		} else if (temp_row.stmtNo == stmtNo && temp_row.DE.compare("assign") == 0 && temp_row.varIndex == varIndex)
		{
			cout<< "The data is already in the ModifyTable\n";
			return -1;
		} else if (temp_row.stmtNo == stmtNo && temp_row.DE.compare("assign") != 0 && temp_row.varIndex == varIndex)
		{
			return 1;
		}
	}

	modify_stmt_row new_row;
	new_row.stmtNo = stmtNo;
	new_row.varIndex = varIndex;
	new_row.DE = DE;
	ModifyStmtTable.push_back(new_row);

	return 1;
}

int ModifyTable::insertModifyProc(int procIndex, int varIndex)
{
	assert (procIndex >= 0 && varIndex >= 0);

	modify_proc_row temp_row;

	for (unsigned i =0; i<ModifyProcTable.size(); i++)
	{
		temp_row = ModifyProcTable.at(i);
		if (temp_row.procIndex == procIndex && temp_row.varIndex == varIndex)
		{
			return 1;
		}
	}

	modify_proc_row new_row;
	new_row.procIndex = procIndex;
	new_row.varIndex = varIndex;
	ModifyProcTable.push_back(new_row);

	return 1;
}

vector<int> ModifyTable::getModifiedStmt(int stmtNo)
{
	assert (stmtNo >= 0);

	modify_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyStmtTable.size(); i++)
	{
		temp_row = ModifyStmtTable.at(i);
		if (temp_row.stmtNo == stmtNo)
		{
			results.push_back(temp_row.varIndex);
		}
	}

	return results;
}

vector<int> ModifyTable::getModifiedProc(int procIndex)
{
	assert (procIndex >= 0);

	modify_proc_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyProcTable.size(); i++)
	{
		temp_row = ModifyProcTable.at(i);
		if (temp_row.procIndex == procIndex)
		{
			results.push_back(temp_row.varIndex);
		}
	}

	return results;
}

vector<int> ModifyTable::getModifiedList(int varIndex, string DE)
{
	assert(varIndex >= 0 && (DE.compare("assign") == 0 || DE.compare("assign") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0 || DE.compare("stmt") == 0 || DE.compare("prog_line") == 0));

	vector<int> modifiedList;

	modify_stmt_row temp_row;

	if (DE.compare("assign") == 0 || DE.compare("call") == 0 || DE.compare("while") == 0 || DE.compare("if") == 0)
	{
		for (unsigned i = 0; i<ModifyStmtTable.size(); i++)
		{
			temp_row = ModifyStmtTable.at(i);
			if (temp_row.varIndex == varIndex && temp_row.DE.compare(DE) == 0)
			{
				modifiedList.push_back(temp_row.stmtNo);
			}
		}
	} else if (DE.compare("stmt") == 0 || DE.compare("prog_line") == 0)
	{
		for (unsigned i =0; i<ModifyStmtTable.size(); i++)
		{
			temp_row = ModifyStmtTable.at(i);
			if (temp_row.varIndex == varIndex)
			{
				modifiedList.push_back(temp_row.stmtNo);
			}
		}
	}

	return modifiedList;
}

void ModifyTable::printModifyTable()
{
	modify_stmt_row temp_row;

	cout<< "ModifyStmtTable:" << endl;
	cout<< "stmtNo" << "\t" << "varIndex" << "\t" << "DE" << endl;
	for (unsigned i = 0; i < ModifyStmtTable.size(); i++)
	{
		temp_row = ModifyStmtTable.at(i);
		cout << temp_row.stmtNo << "\t" << temp_row.varIndex << "\t" << temp_row.DE << endl;
	}

	modify_proc_row temp_row_2;

	cout<< "ModifyProcTable:" << endl;
	cout<< "procIndex" << "\t" << "varIndex" << endl;
	for (unsigned i = 0; i < ModifyProcTable.size(); i++)
	{
		temp_row_2 = ModifyProcTable.at(i);
		cout << temp_row_2.procIndex << "\t" << temp_row_2.varIndex << endl;
	}
}

/*************************************** New APIs *******************************************/

vector<int> ModifyTable::getModifyProcList(){
	modify_proc_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyProcTable.size(); i++)
	{
		temp_row = ModifyProcTable.at(i);
		results.push_back(temp_row.procIndex);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> ModifyTable::getModifyStmtList(){
	modify_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyStmtTable.size(); i++)
	{
		temp_row = ModifyStmtTable.at(i);
		results.push_back(temp_row.stmtNo);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> ModifyTable::getModifyDEList(string DE){
	modify_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyStmtTable.size(); i++)
	{
		temp_row = ModifyStmtTable.at(i);
		if(temp_row.DE.compare(DE) == 0){
			results.push_back(temp_row.stmtNo);
		}
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> ModifyTable::getModifyVarList(){
	modify_proc_row temp_proc_row;
	modify_stmt_row temp_stmt_row;
	vector<int> results;

	for (unsigned i =0; i<ModifyProcTable.size(); i++)
	{
		temp_proc_row = ModifyProcTable.at(i);
		results.push_back(temp_proc_row.varIndex);
	}

	for (unsigned i =0; i<ModifyStmtTable.size(); i++)
	{
		temp_stmt_row = ModifyStmtTable.at(i);
		results.push_back(temp_stmt_row.varIndex);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<Pair> ModifyTable::getModifyPairList(vector<int> set1, vector<int> set2){
	modify_proc_row temp_proc_row;
	modify_stmt_row temp_stmt_row;
	bool flag1 = true;
	bool flag2 = true;
	ostringstream convert;
	string str1, str2;
	vector<Pair> results;

	for (unsigned i =0; i<ModifyProcTable.size(); i++)
	{
		temp_proc_row = ModifyProcTable.at(i);
		flag1 = false;
		flag2 = false;
		for (unsigned j = 0; j < set1.size(); j++){
			if(temp_proc_row.procIndex == set1.at(j)){
				flag1 = true;
				break;
			}
		}
		for (unsigned j = 0; j < set2.size(); j++){
			if(temp_proc_row.varIndex == set2.at(j)){
				flag2 = true;
				break;
			}
		}
		if(flag1 && flag2){
			convert<<temp_proc_row.procIndex;
			str1 = convert.str();
			convert<<temp_proc_row.varIndex;
			str2 = convert.str();
			Pair myPair(str1, str2);
			results.push_back(myPair);
		}
	}

	for (unsigned i =0; i<ModifyStmtTable.size(); i++)
	{
		temp_stmt_row = ModifyStmtTable.at(i);
		flag1 = false;
		flag2 = false;
		for (unsigned j = 0; j < set1.size(); j++){
			if(temp_stmt_row.stmtNo == set1.at(j)){
				flag1 = true;
				break;
			}
		}
		for (unsigned j = 0; j < set2.size(); j++){
			if(temp_stmt_row.varIndex == set2.at(j)){
				flag2 = true;
				break;
			}
		}
		if(flag1 && flag2){
			convert<<temp_stmt_row.stmtNo;
			str1 = convert.str();
			convert<<temp_stmt_row.varIndex;
			str2 = convert.str();
			Pair myPair(str1, str2);
			results.push_back(myPair);
		}
	}

	return results;
}