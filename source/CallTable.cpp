#include "CallTable.h"
#include <algorithm>
CallTable::CallTable() {
}

void CallTable::insert(string proc1, string proc2){
	for(unsigned i=0; i<callTable.size(); i++){
		if(callTable.at(i).callingProc.compare(proc1) == 0){
			if(find(callTable.at(i).calledProcs.begin(), callTable.at(i).calledProcs.end(), proc2) != callTable.at(i).calledProcs.end()){
				return;
			}else{
				callTable.at(i).calledProcs.push_back(proc2);
				return;
			}
		}	
	}
	
	call_row row;
	row.callingProc = proc1;
	row.calledProcs.push_back(proc2);
	callTable.push_back(row);
}
bool CallTable::isCalled(string proc1, string proc2){
	for(unsigned i=0; i<callTable.size(); i++){
		if(callTable.at(i).callingProc.compare(proc1) == 0){
			if(find(callTable.at(i).calledProcs.begin(), callTable.at(i).calledProcs.end(), proc2) != callTable.at(i).calledProcs.end())
				return true;
			return false;
		}
	}
	return false;
	
}

vector<string> CallTable::getCallsList(string procName){
	vector<string> result;
	for(unsigned i=0; i<callTable.size(); i++){
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			if(callTable.at(i).calledProcs.at(j).compare(procName) == 0 || procName.compare("_") == 0){
				result.push_back(callTable.at(i).callingProc);
			}
		}
	}
	return result;
}

vector<string> CallTable::getCalledList(string procName){
	vector<string> result;
	for(unsigned i=0; i<callTable.size(); i++){
		if(callTable.at(i).callingProc.compare(procName) == 0 || procName.compare("_") == 0){
			result.insert(result.end(), callTable.at(i).calledProcs.begin(), callTable.at(i).calledProcs.end());
		}
	}
	return result;
}

vector<string> CallTable::getCall(string procName){
	vector<string> result;
	for(unsigned i=0; i<callTable.size(); i++){
		if(callTable.at(i).callingProc.compare(procName) == 0 || procName.compare("_") == 0)
			result.push_back(callTable.at(i).callingProc);
	}
	return result;
}

vector<string> CallTable::getCalled(string procName){
	vector<string> result;
	for(unsigned i=0; i<callTable.size(); i++){
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			if(callTable.at(i).calledProcs.at(j).compare(procName) == 0 || procName.compare("_") == 0)
				result.push_back(callTable.at(i).calledProcs.at(j));
		}
	}
	return result;
}

vector<Pair> CallTable::getCallPairList(vector<string> set1, vector<string> set2){
	vector<Pair> result;
	for(unsigned i=0; i<set1.size(); i++){
		for(unsigned j=0; j<set2.size(); j++){
			if(isCalled(set1.at(i), set2.at(j)))
			result.push_back(Pair(set1.at(i), set2.at(j)));
		}
	}

	return result;
}
int CallTable::getSize(){
	return callTable.size();
}
void CallTable::print(){
	vector<string>::iterator ite;
	for(unsigned i=0; i<callTable.size(); i++){
		cout << callTable.at(i).callingProc << ":";
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			cout << callTable.at(i).calledProcs.at(j) << " ";
		}
		cout << "\n";
	}
}