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

/*************************************** New APIs *******************************************/

vector<int> UseTable::getUseProcList(){
	use_proc_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_row = UseProcTable.at(i);
		results.push_back(temp_row.procIndex);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> UseTable::getUseStmtList(){
	use_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_row = UseStmtTable.at(i);
		results.push_back(temp_row.stmtNo);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> UseTable::getUseDEList(string DE){
	use_stmt_row temp_row;
	vector<int> results;

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_row = UseStmtTable.at(i);
		if(temp_row.DE.compare(DE) == 0){
			results.push_back(temp_row.stmtNo);
		}
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<int> UseTable::getUseVarList(){
	use_proc_row temp_proc_row;
	use_stmt_row temp_stmt_row;
	vector<int> results;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_proc_row = UseProcTable.at(i);
		results.push_back(temp_proc_row.varIndex);
	}

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_stmt_row = UseStmtTable.at(i);
		results.push_back(temp_stmt_row.varIndex);
	}

	// Remove duplicate
	sort(results.begin(), results.end());
    results.erase(unique(results.begin(), results.end()), results.end());

	return results;
}

vector<pair<string,string>> UseTable::getUsePairList(vector<int> set1, vector<int> set2){
	use_proc_row temp_proc_row;
	use_stmt_row temp_stmt_row;
	bool flag1 = true;
	bool flag2 = true;
	ostringstream convert;
	string str1, str2;
	vector<pair<string,string>> results;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_proc_row = UseProcTable.at(i);
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
			pair<string,string> myPair(str1, str2);
			results.push_back(myPair);
		}
	}

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_stmt_row = UseStmtTable.at(i);
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
			pair<string,string> myPair(str1, str2);
			results.push_back(myPair);
		}
	}

	return results;
}

bool UseTable::checkUse(vector<int> set1, vector<int> set2){
	use_proc_row temp_proc_row;
	use_stmt_row temp_stmt_row;
	bool flag1 = true;
	bool flag2 = true;

	for (unsigned i =0; i<UseProcTable.size(); i++)
	{
		temp_proc_row = UseProcTable.at(i);
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
			return true;
		}
	}

	for (unsigned i =0; i<UseStmtTable.size(); i++)
	{
		temp_stmt_row = UseStmtTable.at(i);
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
			return true;
		}
	}

	return false;
}