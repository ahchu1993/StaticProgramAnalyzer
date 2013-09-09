#include "CallTable.h"
#include <algorithm>
CallTable::CallTable() {
}

void CallTable::insert(string proc1, string proc2){
	for(it=callTable.begin();it<callTable.end();it++){
		if((*it).callingProc.compare(proc1)==0){
			if(find((*it).calledProcs.begin(), (*it).calledProcs.end(), proc2) == (*it).calledProcs.end()) {
				(*it).calledProcs.push_back(proc2);
			}
			return;
		}
	}
	call_row row;
	row.callingProc = proc1;
	row.calledProcs.push_back(proc2);
	callTable.push_back(row);
}
bool CallTable::isCalled(string proc1, string proc2){
	vector<string>::iterator ite;
	for(it=callTable.begin();it<callTable.end();it++){
		if((*it).callingProc.compare(proc1)==0){
			for(ite=(*it).calledProcs.begin(); ite<(*it).calledProcs.end();ite++){
				if((*ite).compare(proc2)==0)
					return true;
			}
		}
	}
	return false;
}

vector<string> CallTable::getCallsList(string procName){
	vector<string> result;
	vector<string>::iterator ite;
	for(it=callTable.begin();it<callTable.end();it++){
		for(ite=(*it).calledProcs.begin(); ite<(*it).calledProcs.end();ite++){
			if((*ite).compare(procName)==0){
				result.push_back((*it).callingProc);
			}
		}
	}
	return result;
}

vector<string> CallTable::getCalledList(string procName){
	vector<string> result;

	for(it=callTable.begin();it<callTable.end();it++){
		if((*it).callingProc.compare(procName)==0){
			result = (*it).calledProcs;
		}
	}
	return result;
}
int CallTable::getSize(){
	return callTable.size();
}
void CallTable::print(){
	vector<string>::iterator ite;
	for(it=callTable.begin();it<callTable.end();it++){
		cout << (*it).callingProc << ":";
		for(ite=(*it).calledProcs.begin(); ite<(*it).calledProcs.end();ite++){
			cout << *ite << " ";
		}
		cout << "\n";
	}
}