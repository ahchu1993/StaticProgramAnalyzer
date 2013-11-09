#include "CallTable.h"
#include <algorithm>
#include "Util.h"
CallTable::CallTable() {
}

void CallTable::insert(int stmNo, string proc1, string proc2){
	for(unsigned i=0; i<callTable.size(); i++){
		if(callTable.at(i).callingProc.compare(proc1) == 0){
			callTable.at(i).calledProcs.push_back(proc2);
			callTable.at(i).callStmNo.push_back(stmNo);
			return;
		}	
	}
	
	call_row row;
	row.callingProc = proc1;
	row.calledProcs.push_back(proc2);
	row.callStmNo.push_back(stmNo);
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


vector<string> CallTable::getCalledT(string proc){
	vector<string> result;
	vector<string> calledList = getCalledList(proc);
	if(calledList.size() == 0)
		return result;
	for(unsigned i=0; i<calledList.size(); i++){
		vector<string> tmp = getCalledT(calledList.at(i));
		result.push_back(calledList.at(i));
		result.insert(result.end(), tmp.begin(), tmp.end());
	}
	return Util::removeDuplicate(result);

}
vector<string> result;
vector<string> CallTable::getTopoCall(){
	result.clear();
	string firstProc;
	for(unsigned i=0; i<callTable.size(); i++){
		if(getCallsList(callTable.at(i).callingProc).size() == 0){
			firstProc = callTable.at(i).callingProc;
			getTopoCallRecur(firstProc);
		}
	}
	for(unsigned i=0; i<result.size(); i++){
		cout << result.at(i);
	}
	return result;
}
void CallTable::getTopoCallRecur(string startProc){
	if(find(result.begin(), result.end(), startProc) != result.end()){
		return;
	}

	vector<string> neighbor= getCalledList(startProc);
	for(unsigned i=0; i<neighbor.size(); i++){
		string proc = neighbor.at(i);
		getTopoCallRecur(proc);

	}

	result.push_back(startProc);
}
vector<string> CallTable::getCallsT(string proc){
	vector<string> result;
	vector<string> callsList = getCallsList(proc);
	if(callsList.size() == 0)
		return result;
	for(unsigned i=0; i<callsList.size(); i++){
		vector<string> tmp = getCallsT(callsList.at(i));
		result.push_back(callsList.at(i));
		result.insert(result.end(), tmp.begin(), tmp.end());
	}
	return Util::removeDuplicate(result);
}

vector<int> CallTable::getCallsStmT(string proc){
	vector<int> result;
	//vector<string> callsList = getCallsList(proc);
	//vector<int> callsStmList = getCallsStmtList(proc);
	/*vector<int> callsStmList;
	for(unsigned i=0; i<callsList.size(); i++){
		if(callTable.at(i).callingProc == callsList.at(i)){
			callsStmList.push_back(callTable.at(i).callStmNo);
		}
	}
	if(callsList.size() == 0)
		return result;
	for(unsigned i=0; i<callsList.size(); i++){
		vector<int> tmp = getCallsStmT(callsList.at(i));
		result.push_back(callsStmList.at(i));
		result.insert(result.end(), tmp.begin(), tmp.end());
	}*/
	for(unsigned i=0; i<callTable.size(); i++){
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			if(callTable.at(i).calledProcs.at(j) == proc){
				result.push_back(callTable.at(i).callStmNo.at(j));
			}
		}
	}
	return Util::removeDuplicateInt(result);
}

vector<int> CallTable::getCallsStmtList(string procName){
	vector<int> result;
	for(unsigned i=0; i<callTable.size(); i++){
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			if(callTable.at(i).calledProcs.at(j).compare(procName) == 0 || procName.compare("_") == 0){
				result.push_back(callTable.at(i).callStmNo.at(j));
			}
		}
	}
	return result;
}
bool CallTable::checkCall(vector<string> set1, vector<string> set2){
	vector<pair<string, string>> result;
	for(unsigned i=0; i<set1.size(); i++){
		for(unsigned j=0; j<set2.size(); j++){
			if(isCalled(set1.at(i), set2.at(j)))
				return true;
		}
	}

	return false;
}

int CallTable::getSize(){
	return callTable.size();
}
void CallTable::print(){
	vector<string>::iterator ite;
	for(unsigned i=0; i<callTable.size(); i++){
		cout << callTable.at(i).callingProc << " : ";
		for(unsigned j=0; j<callTable.at(i).calledProcs.size(); j++){
			cout << callTable.at(i).calledProcs.at(j) << ", ";
		}
		cout << "\n";
		cout << "Call stmts are:";
		for(unsigned j=0; j<callTable.at(i).callStmNo.size(); j++){
			cout << callTable.at(i).callStmNo.at(j) << ", ";
		}
		cout << "\n";
	}
	getTopoCall();
}