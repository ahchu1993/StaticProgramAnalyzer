#include "PKB.h"

PKB::PKB(){

}

/************************************************** AST *************************************************/
TNode* PKB::createRootNode(string type, int content, int lineNo){
	return ast.createRootNode(type, content, lineNo);
}
TNode* PKB::createNode(string type, int content, int lineNo){
	return ast.createNode(type,content,lineNo);
}
void PKB::makeLeftChild(TNode* parent, TNode* leftChild){
	ast.makeLeftChild(parent, leftChild);
}
void PKB::makeRightChild(TNode* parent, TNode* rightChild){
	ast.makeRightChild(parent, rightChild);
}
TNode* PKB::getLeftChild(TNode* parent){
	return ast.getLeftChild(parent);
}
TNode* PKB::getRightChild(TNode* parent){
	return ast.getRightChild(parent);
}
int PKB::getContent(TNode* node)
{
	return ast.getContent(node);
}
int PKB::getLineNo(TNode* node)
{
	return ast.getLineNo(node);
}
string PKB::getType(TNode* node)
{
	return ast.getType(node);
}
vector<TNode*> PKB::getAST()
{
	return ast.getAST();
}
void PKB::printAST()
{
	ast.printAST();
}

/************************************************** CallTable *************************************************/
vector<pair<string, string>> PKB::getCalls(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){	
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
		string caller = *it;	
		vector<string> callee = getCalledList(caller);
		
		for(unsigned int i=0;i<callee.size();i++){
			it2 = s2.find(callee.at(i));
			if(it2!=s2.end()){
				pair<string,string> p(caller,*it2);
				result.push_back(p);
			}
		}	
	}	
	return result;
}

vector<pair<string, string>> PKB::getCallsT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s1.begin();it!=s1.end();it++){
		string caller = *it;	
		vector<string> callee = callTable.getCalledT(caller);
		
		for(unsigned int i=0;i<callee.size();i++){
			it2 = s2.find(callee.at(i));
			if(it2!=s2.end()){
				pair<string,string> p(caller,*it2);
				result.push_back(p);
			}
		}	
	}	
	return result;
}
bool PKB::checkCalls(string arg1, string arg1Type, string arg2, string arg2Type){	
	if(arg1=="_"&&arg2=="_"){
		int size = callTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="string"){

		vector<string> callers = getCallsList(arg2);
		if(callers.size()>0) return true;
		else return false;

	}else if(arg1Type=="string"&&arg2=="_"){
		
		vector<string> callees = getCalledList(arg1);		
		if(callees.size()>0) return true;
		else return false;

	}else{
		bool called = isCalled(arg1,arg2);
		if(called)
			return true;
		else return false;
	}
}

bool PKB::checkCallsT(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = callTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="string"){

		vector<string> callers = callTable.getCallsT(arg2);
		if(callers.size()>0) return true;
		else return false;

	}else if(arg1Type=="string"&&arg2=="_"){
		
		vector<string> callees = callTable.getCalledT(arg1);		
		if(callees.size()>0) return true;
		else return false;

	}else{
		bool called = isCalled(arg1,arg2);
		if(called)
			return true;
		else return false;
	}
	return true;
}
//Insert into call table
//stmNo: stmNo in proc1 that called proc2
void PKB::insert(int stmNo, string proc1, string proc2){
	callTable.insert(stmNo, proc1, proc2);
}
bool PKB::isCalled(string proc1, string proc2){
	return callTable.isCalled(proc1,proc2);
}
//return all procedures that called procName directly
vector<string> PKB::getCallsList(string procName){
	return callTable.getCallsList(procName);
}
//return all procedures that were called by procName directly
vector<string> PKB::getCalledList(string procName){
	return callTable.getCalledList(procName);
}
void PKB::printCallTable(){
	callTable.print();
}
//Api for Khue
//Input: procIndex
//Output: vector of all stmNo that call* procIndex
vector<int> PKB::getCallsStmT(int procIndex){
	string procName = procTable.getProcName(procIndex);
	return callTable.getCallsStmT(procName);
}
vector<int> PKB::getCallsList(int procIndex){
	vector<int> result;
	string procName = procTable.getProcName(procIndex);
	vector<string> callList = callTable.getCallsList(procName);
	
	for(unsigned i=0; i<callList.size(); i++){
		result.push_back(procTable.getProcIndex(callList.at(i)));
	}
	return result;
}

//Input:procedure index
//Output: vector of all proc indexes that call* input procedure
vector<int> PKB::getCallsT(int procIndex){
	vector<int> result;
	string procName = procTable.getProcName(procIndex);
	vector<string> procNameList = callTable.getCallsT(procName);

	for(unsigned i=0; i<procNameList.size(); i++){
		result.push_back(procTable.getProcIndex(procNameList.at(i)));
	}

	return result;
}
//Input:procedure index
//Output: vector of all proc indexes that were call* by input procedure
vector<int> PKB::getCalledT(int procIndex){
	vector<int> result;
	string procName = procTable.getProcName(procIndex);
	vector<string> procNameList = callTable.getCalledT(procName);

	for(unsigned i=0; i<procNameList.size(); i++){
		result.push_back(procTable.getProcIndex(procNameList.at(i)));
	}

	return result;
}

/************************************************** ParentTable *************************************************/
vector<pair<string, string>> PKB::getParent(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s2.begin();it!=s2.end();it++){

		string child = *it;
		int c = Util::convertStringToInt(child);
		int p = getParent(c);
		string parent = Util::convertIntToString(p);
		it1 = s1.find(parent);
		if(it1!=s1.end()){
			pair<string,string> p(parent,child);
			result.push_back(p);
		}
		
	}
	
	return result;
}

vector<int> PKB::getParentT(int stmt){
	return parentTable.getParentT(stmt);
}

vector<pair<string, string>> PKB::getParentT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s2.begin();it!=s2.end();it++){

		string child = *it;
		int c = Util::convertStringToInt(child);
		vector<int> parents = getParentT(c);
		for(unsigned int i=0;i<parents.size();i++){
			int p = parents.at(i);
			string p_string = Util::convertIntToString(p);
			it1 = s1.find(p_string);
			if(it1!=s1.end()){
				pair<string,string> p(p_string,child);
				result.push_back(p);
			}
		}	
	}
	
	return result;
}

bool PKB::checkParent(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = parentTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int c = Util::convertStringToInt(arg2);
		int p = getParent(c);
		if(p>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int p = Util::convertStringToInt(arg1);
		string type = getStmtType(p);
		vector<int> children = getChildren(p,type);
		if(children.size()>0) return true;
		else return false;
	}else{
		int c = Util::convertStringToInt(arg2);
		int p = getParent(c);
		int parent = Util::convertStringToInt(arg1);
		if(parent==p)
			return true;
		else return false;
	}
	
}

bool PKB::checkParentT(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = parentTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int c = Util::convertStringToInt(arg2);
		vector<int> p = getParentT(c);
		if(p.size()>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int p = Util::convertStringToInt(arg1);
		string type = getStmtType(p);
		vector<int> children = getChildrenT(p,type);
		if(children.size()>0) return true;
		else return false;
	}else{
		int c = Util::convertStringToInt(arg2);
		vector<int> p = getParentT(c);
		int parent = Util::convertStringToInt(arg1);
		for(unsigned int i=0;i<p.size();i++){
			if(p.at(i)==parent)
				return true;	
		}
		return false;
	}
}
void PKB::insert(int stm1, string DE1, int stm2, string DE2){
	parentTable.insert(stm1, DE1, stm2, DE2);
}
int PKB::getParent (int stm){
	return parentTable.getParent(stm);
}

vector<int> PKB::getChildren(int stm, string DE){
	return parentTable.getChildren(stm, DE);
}
vector<int> PKB::getChildrenT(int stm, string DE){
	return parentTable.getChildrenT(stm, DE);
}
void PKB::printParentTable(){
	parentTable.print();
}

/************************************************** FollowTable *************************************************/
vector<pair<string, string>> PKB::getFollow(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){	
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s1.begin();it!=s1.end();it++){

		string first = *it;
		int f = Util::convertStringToInt(first);
		int s = findFollowed(f);
		string second = Util::convertIntToString(s);
		it1 = s2.find(second);
		if(it1!=s2.end()){
			pair<string,string> p(first,second);
			result.push_back(p);
		}
		
	}
	
	return result;
}
vector<pair<string, string>> PKB::getFollowT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	for(set<string>::iterator it = s1.begin();it!=s1.end();it++){

		string first = *it;
		int f = Util::convertStringToInt(first);
		vector<int> s = findFollowedT(f);
		
		for(unsigned int i=0;i<s.size();i++){
			string second = Util::convertIntToString(s.at(i));
			it1 = s2.find(second);
			if(it1!=s2.end()){
				pair<string,string> p(first,second);
				result.push_back(p);
			}
		}		
	}
	
	return result;
}

bool PKB::checkFollow(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = followTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int second = Util::convertStringToInt(arg2);
		int f = findFollows(second);
		if(f>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int f = Util::convertStringToInt(arg1);
		int s = findFollowed(f);
		if(s>0) return true;
		else return false;
	}else{
		int s = Util::convertStringToInt(arg2);
		int f = findFollows(s);
		int first = Util::convertStringToInt(arg1);
		if(first==f)
			return true;
		else return false;
	}
}

bool PKB::checkFollowT(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = followTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int second = Util::convertStringToInt(arg2);
		vector<int> f = findFollowsT(second);
		if(f.size()>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int f = Util::convertStringToInt(arg1);
		vector<int> s = findFollowedT(f);
		if(s.size()>0) return true;
		else return false;
	}else{
		int s = Util::convertStringToInt(arg2);
		vector<int> f = findFollowsT(s);
		int first = Util::convertStringToInt(arg1);
		for(unsigned int i=0;i<f.size();i++){		
			if(f.at(i) == first)
				return true;
		}
		
		return false;
	}
}

void PKB::insertFollow(int stm1, string DE1, int stm2, string DE2){
	followTable.insertFollow(stm1, DE1, stm2, DE2);
}
int PKB::findFollowed(int stm){
	return followTable.findFollowed(stm);
}
vector<int> PKB::findFollowedT(int stmt){
	return followTable.findFollowedT(stmt);
}
int PKB::findFollows(int stm){
	return followTable.findFollows(stm);
}
vector<int> PKB::findFollowsT(int stmt){
	return followTable.findFollowsT(stmt);
}
bool PKB::isFollowed(int stm1,int stm2){
	return followTable.isFollowed(stm1,stm2);
}
void PKB::printFollowTable(){
	followTable.print();
}

/************************************************** ModifyTable *************************************************/
vector<pair<string, string>> PKB::getModify(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	if(arg1Type=="procedure"||arg1Type =="string"){
		vector<modify_proc_row> ModifyProcTable = modifyTable.getModifyProcTable();
		for(unsigned int i=0;i<ModifyProcTable.size();i++){
			modify_proc_row temp = ModifyProcTable.at(i);
			int p_index = temp.procIndex;
			int v_index = temp.varIndex;
			string proc = procTable.getProcName(p_index);
			string var = varTable.getVarName(v_index);
			
			it1 = s1.find(proc);
			it2 = s2.find(var);

			if(it1!=s1.end()&&it2!=s2.end()){
				pair<string,string> p(proc,var);
				result.push_back(p);
			}
		}
	}else {
		vector<modify_stmt_row> ModifyStmtTable = modifyTable.getModifyStmtTable();
		for(unsigned int i=0;i<ModifyStmtTable.size();i++){
			
			modify_stmt_row temp = ModifyStmtTable.at(i);

			if(temp.DE == arg1Type||arg1Type=="stmt"||arg1Type=="prog_line"||arg1Type=="integer"){ // assign,if,while,stmt
				int stmtNo = temp.stmtNo;
				int v_index = temp.varIndex;
				string stmt = Util::convertIntToString(stmtNo);
				string var = varTable.getVarName(v_index);
				
				it1 = s1.find(stmt);
				it2 = s2.find(var);

				if(it1!=s1.end()&&it2!=s2.end()){
					pair<string,string> p(stmt,var);
					result.push_back(p);
				}
			}			
		}
	}
	return result;
}

bool PKB::checkModify(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1Type =="string"){
		vector<modify_proc_row> ModifyProcTable = modifyTable.getModifyProcTable();
		for(unsigned int i=0;i<ModifyProcTable.size();i++){
			modify_proc_row temp = ModifyProcTable.at(i);
			int p_index = temp.procIndex;
			int v_index = temp.varIndex;
			string proc = procTable.getProcName(p_index);
			string var = varTable.getVarName(v_index);
			
			if(arg2=="_"){
				if(arg1==proc) 
					return true;
			}else if(arg1==proc&&arg2==var){
				return true;
			}
		}
		return false;
	}else {
		vector<modify_stmt_row> ModifyStmtTable = modifyTable.getModifyStmtTable();
		for(unsigned int i=0;i<ModifyStmtTable.size();i++){
			
			modify_stmt_row temp = ModifyStmtTable.at(i);
				
			int stmtNo = temp.stmtNo;
			int v_index = temp.varIndex;
			string stmt = Util::convertIntToString(stmtNo);
			string var = varTable.getVarName(v_index);

			if(arg2=="_"){
				if(arg1==stmt) 
					return true;
			}else if(arg1 == stmt && arg2 == var){
				return true;
			}	
		}
		return false;
	}
}

// Update ModifyTable using CallTable
void PKB::updateModify(){
	vector<int> procList;
	int currentProcIndex;
	int tempProcIndex;
	int tempStmtNo;
	int tempVarIndex;
	vector<int> callingProc;
	vector<int> callingStmt;
	vector<int> modifiedVar;

	procList = procTable.getProcList();
	for (unsigned i=0; i<procList.size(); i++){
		currentProcIndex = procList.at(i);

		callingProc = getCallsT(currentProcIndex);
		callingStmt = getCallsStmT(currentProcIndex);

		modifiedVar = modifyTable.getModifiedProc(currentProcIndex);

		for (unsigned j=0; j<callingProc.size(); j++){
			tempProcIndex = callingProc.at(j);
			for (unsigned k=0; k<modifiedVar.size(); k++){
				tempVarIndex = modifiedVar.at(k);
				modifyTable.insertModifyProc(tempProcIndex, tempVarIndex);
			}
		}

		for (unsigned j =0; j<callingStmt.size(); j++){
			tempStmtNo = callingStmt.at(j);
			for (unsigned k=0; k<modifiedVar.size(); k++){
				tempVarIndex = modifiedVar.at(k);
				modifyTable.insertModifyStmt(tempStmtNo, tempVarIndex, "call");
			}
		}
	}
}

int PKB::insertModifyStmt(int stmtNo, int varIndex, string DE){
	return modifyTable.insertModifyStmt(stmtNo, varIndex, DE);
}
int PKB::insertModifyProc(int procIndex, int varIndex){
	return modifyTable.insertModifyProc(procIndex, varIndex);
}
vector<int> PKB::getModifiedStmt(int stmtNo){
	return modifyTable.getModifiedStmt(stmtNo);
}
vector<int> PKB::getModifiedProc(int procIndex){
	return modifyTable.getModifiedProc(procIndex);
}
vector<int> PKB::getModifiedList(int varIndex, string DE){
	return modifyTable.getModifiedList(varIndex, DE);
}
void PKB::printModifyTable()
{
	vector<modify_proc_row> ModifyProcTable = modifyTable.getModifyProcTable();
	vector<modify_stmt_row> ModifyStmtTable = modifyTable.getModifyStmtTable();
	modify_proc_row temp_proc_row;
	modify_stmt_row temp_stmt_row;
	string type;
	int procIndex, stmtNo, varIndex;

	//cout<< "ModifyProcTable:" << endl;
	//cout<< "procName" << "\t" << "varName" << endl;
	for (unsigned i = 0; i<ModifyProcTable.size(); i++){
		temp_proc_row = ModifyProcTable.at(i);
		procIndex = temp_proc_row.procIndex;
		varIndex = temp_proc_row.varIndex;
		//cout<<procTable.getProcName(procIndex)<<"\t"<<varTable.getVarName(varIndex)<<endl;
	}

	//cout<< "ModifyStmtTable:" << endl;
	//cout<< "stmtNo" << "\t" << "Type" << "\t" << "varName" << endl;
	for (unsigned i = 0; i<ModifyStmtTable.size(); i++){
		temp_stmt_row = ModifyStmtTable.at(i);
		stmtNo = temp_stmt_row.stmtNo;
		type = temp_stmt_row.DE;
		varIndex = temp_stmt_row.varIndex;
		//cout<<stmtNo<<"\t"<<type<<"\t"<<varTable.getVarName(varIndex)<<endl;
	}
}

/************************************************** UseTable *************************************************/
vector<pair<string, string>> PKB::getUse(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	
	vector<pair<string,string>> result;
	
	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> s1 = *arg1_set;
	set<string> s2 = *arg2_set;

	if(arg1Type=="procedure"||arg1Type =="string"){
		vector<use_proc_row> UseProcTable = useTable.getUseProcTable();
		for(unsigned int i=0;i<UseProcTable.size();i++){
			use_proc_row temp = UseProcTable.at(i);
			int p_index = temp.procIndex;
			int v_index = temp.varIndex;
			string proc = procTable.getProcName(p_index);
			string var = varTable.getVarName(v_index);
			
			it1 = s1.find(proc);
			it2 = s2.find(var);

			if(it1!=s1.end()&&it2!=s2.end()){
				pair<string,string> p(proc,var);
				result.push_back(p);
			}
		}
	}else {
		vector<use_stmt_row> UseStmtTable = useTable.getUseStmtTable();
		for(unsigned int i=0;i<UseStmtTable.size();i++){
			
			use_stmt_row temp = UseStmtTable.at(i);
			
			if(temp.DE == arg1Type||arg1Type=="stmt"||arg1Type=="prog_line"||arg1Type=="integer"){ // assign,if,while,stmt
				int stmtNo = temp.stmtNo;
				int v_index = temp.varIndex;
				string stmt = Util::convertIntToString(stmtNo);
				string var = varTable.getVarName(v_index);
				
				it1 = s1.find(stmt);
				it2 = s2.find(var);

				if(it1!=s1.end()&&it2!=s2.end()){
					pair<string,string> p(stmt,var);
					result.push_back(p);
				}
			}		
		}
	}
	return result;
}

bool PKB::checkUse(string arg1, string arg1Type, string arg2, string arg2Type){	
	if(arg1Type =="string"){
		vector<use_proc_row> UseProcTable = useTable.getUseProcTable();
		for(unsigned int i=0;i<UseProcTable.size();i++){
			use_proc_row temp = UseProcTable.at(i);
			int p_index = temp.procIndex;
			int v_index = temp.varIndex;
			string proc = procTable.getProcName(p_index);
			string var = varTable.getVarName(v_index);
			
			if(arg2=="_"){
				if(arg1==proc) 
					return true;
			}else if(arg1==proc&&arg2==var){
				return true;
			}
		}
		return false;
	}else {
		vector<use_stmt_row> UseStmtTable = useTable.getUseStmtTable();
		for(unsigned int i=0;i<UseStmtTable.size();i++){
			
			use_stmt_row temp = UseStmtTable.at(i);
				
			int stmtNo = temp.stmtNo;
			int v_index = temp.varIndex;
			string stmt = Util::convertIntToString(stmtNo);
			string var = varTable.getVarName(v_index);

			if(arg2=="_"){
				if(arg1==stmt) 
					return true;
			}else if(arg1 == stmt && arg2 == var){
				return true;
			}	
		}
		return false;
	}
}

// Update UseTable using CallTable
void PKB::updateUse(){
	vector<int> procList;
	int currentProcIndex;
	int tempProcIndex;
	int tempStmtNo;
	int tempVarIndex;
	vector<int> callingProc;
	vector<int> callingStmt;
	vector<int> usedVar;

	procList = procTable.getProcList();
	for (unsigned i=0; i<procList.size(); i++){
		currentProcIndex = procList.at(i);

		callingProc = getCallsT(currentProcIndex);
		callingStmt = getCallsStmT(currentProcIndex);

		usedVar = useTable.getUsedProc(currentProcIndex);

		for (unsigned j=0; j<callingProc.size(); j++){
			tempProcIndex = callingProc.at(j);
			for (unsigned k=0; k<usedVar.size(); k++){
				tempVarIndex = usedVar.at(k);
				useTable.insertUseProc(tempProcIndex, tempVarIndex);
			}
		}

		for (unsigned j =0; j<callingStmt.size(); j++){
			tempStmtNo = callingStmt.at(j);
			for (unsigned k=0; k<usedVar.size(); k++){
				tempVarIndex = usedVar.at(k);
				useTable.insertUseStmt(tempStmtNo, tempVarIndex, "call");
			}
		}
	}
}

int PKB::insertUseStmt(int stmtNo, int varIndex, string DE){
	return useTable.insertUseStmt(stmtNo, varIndex, DE);
}
int PKB::insertUseProc(int procIndex, int varIndex){
	return useTable.insertUseProc(procIndex, varIndex);
}
vector<int> PKB::getUsedStmt(int stmtNo){
	return useTable.getUsedStmt(stmtNo);
}
vector<int> PKB::getUsedProc(int procIndex){
	return useTable.getUsedProc(procIndex);
}
vector<int> PKB::getUsedList(int varIndex, string DE){
	return useTable.getUsedList(varIndex, DE);
}
void PKB::printUseTable()
{
	vector<use_proc_row> UseProcTable = useTable.getUseProcTable();
	vector<use_stmt_row> UseStmtTable = useTable.getUseStmtTable();
	use_proc_row temp_proc_row;
	use_stmt_row temp_stmt_row;
	string type;
	int procIndex, stmtNo, varIndex;

	//cout<< "UseProcTable:" << endl;
	//cout<< "procName" << "\t" << "varName" << endl;
	for (unsigned i = 0; i<UseProcTable.size(); i++){
		temp_proc_row = UseProcTable.at(i);
		procIndex = temp_proc_row.procIndex;
		varIndex = temp_proc_row.varIndex;
		//cout<<procTable.getProcName(procIndex)<<"\t"<<varTable.getVarName(varIndex)<<endl;
	}

	//cout<< "UseStmtTable:" << endl;
	//cout<< "stmtNo" << "\t" << "Type" << "\t" << "varName" << endl;
	for (unsigned i = 0; i<UseStmtTable.size(); i++){
		temp_stmt_row = UseStmtTable.at(i);
		stmtNo = temp_stmt_row.stmtNo;
		type = temp_stmt_row.DE;
		varIndex = temp_stmt_row.varIndex;
		//cout<<stmtNo<<"\t"<<type<<"\t"<<varTable.getVarName(varIndex)<<endl;
	}
}

/************************************************** VarTable *************************************************/
int PKB::insertVar(string varName){
	return varTable.insertVar(varName);
}
string PKB::getVarName(int index){
	return varTable.getVarName(index);
}
int PKB::getVarIndex(string varName){
	return varTable.getVarIndex(varName);
}
int PKB::getSizeVarTable(){
	return varTable.getSize();
}
void PKB::printVarTable()
{
	varTable.printVarTable();
}
set<string> PKB::getAllVars(){
	set<string> r;
	r =  varTable.getAllVariables();
	return r;
}

/************************************************** ProcTable *************************************************/
int PKB::insertProc(string procName){
	return procTable.insertProc(procName);
}
string PKB::getProcName(int index){
	return procTable.getProcName(index);
}
int PKB::getProcIndex(string procName){
	return procTable.getProcIndex(procName);
}
int PKB::getSizeProcTable(){
	return procTable.getSize();
}
void PKB::printProcTable()
{
	procTable.printProcTable();
}
set<string> PKB::getAllProcs(){
	return procTable.getAllProcedures();
}

/************************************************** StmtTable *************************************************/
int PKB::insertStmt(int stmtNo, string type)
{
	return stmtTable.insertStmt(stmtNo, type);
}
vector<int> PKB::getStmtNo(string type)
{
	return stmtTable.getStmtNo(type);
}
int PKB::getSizeStmtTable()
{
	return stmtTable.getSize();
}
string PKB::getStmtType(int stmtNo)
{
	return stmtTable.getStmtType(stmtNo);
}
void PKB::printStmtTable()
{
	stmtTable.printStmtTable();
}
set<string> PKB::getAllStmts(){
	return stmtTable.getAllStatements();
}

/************************************************** ConstantTable *************************************************/
void PKB::insertConst(int stmtNo, int number)
{
	constTable.insertConst(stmtNo, number);
}

vector<int> PKB::getConst(int stmtNo)
{
	return constTable.getConst(stmtNo);
}

vector<int> PKB::getStmtForConst(int number)
{
	return constTable.getStmt(number);
}

vector<int> PKB::getAllStmtForConst()
{
	return constTable.getAllStmt();
}

void PKB::printConstTable()
{
	constTable.printConstTable();
}

set<string> PKB::getAllConstants(){
	set<string> r;
	r = constTable.getAllConstants();
	return r;
}

/************************************************** CFG - Zhao Yang *************************************************/
void PKB::buildCFG()
{
	currentIndex=1;
	for(int i=0;i< getSizeStmtTable()+1;i++)
	{
		CFGNode * newNode = new CFGNode(i); // 0 is dummy
		string type = getStmtType(i);
		if(type.compare("call")==0)
			newNode->setCallNode();
		cfg.CFGNodes.push_back(newNode);
		
		visited.push_back(0);
	}
	//cout<<"size proc table "<< getSizeProcTable()<<endl;
	int procNum =  getSizeProcTable();
	for(int i=0;i<procNum;i++){
		buildTree(i);  // build cfg's next
		//getchar();
	}

	// build cfgparentlist   ->double linked list
	for(unsigned int i=0;i<cfg.CFGHeaderList.size();i++)
		cfg.buildCFGParentList(cfg.CFGHeaderList[i]->stmtNum);
}

void PKB::buildTree(int procIndex)
{
	//cout<<"current index "<<currentIndex<<endl;
	cfg.CFGHeaderList.push_back(cfg.CFGNodes[currentIndex]);
	buildLink(currentIndex);
}
// top down approach
CFGNode* PKB::buildLink(int stmtNo)
{
	if(visited[stmtNo]==0)
		visited[stmtNo]=1;
	else return cfg.CFGNodes[stmtNo];
	currentIndex++;
	string stmtType =   getStmtType(stmtNo);

	if(stmtType.compare("call")==0){
		CFGNode *currentNode = cfg.CFGNodes[stmtNo];
		currentNode->setCallNode();
		
		// ****go to the call proceudre!@! for BIP
		// how to get the first stmt using procedure call stmtNo;
		
	}


	if(stmtType.compare("while")==0){

		CFGNode *whileNode=cfg.CFGNodes[stmtNo];

		whileNode->addChild(buildLink(stmtNo+1)); // must be his child

		int followedIndex =  findFollowed(stmtNo);
		if(followedIndex>0){
			whileNode->addChild(buildLink(followedIndex));
		}else{
			whileNode->addChild(findNext(stmtNo));
		}

		return whileNode;
	}else if(stmtType.compare("if")==0){
		CFGNode *ifNode=cfg.CFGNodes[stmtNo];

		vector<int> childrenList =  getChildren(stmtNo,"stmt"); // *** why?
		//cout<<"child "<<childrenList.size()<<"  stmtNo: "<<stmtNo<<endl;
		
		// find else stmtList
		int afterElseStmtNo=0;
		std::sort (childrenList.begin(), childrenList.end());
		for(unsigned int i=0;i<childrenList.size()-1;i++){
			int index1=childrenList[i];
			int index2=childrenList[i+1];
			if(! isFollowed(index1,index2)){
				afterElseStmtNo = index2;
				break;
			}
		}

		//cout<<"haha found: "<<"  "<<afterElseStmtNo<<endl;

		ifNode->addChild(buildLink(stmtNo+1)); // then
		ifNode->addChild(buildLink(afterElseStmtNo)); // else

		return ifNode;
	}else{  // call or assign
		// detect  self and ++stmtNo is follow or, otherwize find parent, then follow..
		
		CFGNode *node=cfg.CFGNodes[stmtNo];

		int afterStmtNo = stmtNo+1;
		if( isFollowed(stmtNo,afterStmtNo)){
			node->addChild(buildLink(afterStmtNo));
		}else{
			CFGNode* nextNode= findNext(stmtNo);
			if(nextNode->stmtNum==0)
				return node;
			else node->addChild(nextNode);
		}
		return node;
	}

	return NULL;
}

CFGNode* PKB::findNext(int stmtNo)
{
	int parentIndex =  getParent(stmtNo);	
	if(parentIndex<=0){
		return cfg.CFGNodes[0];
	}
	else{
		string parentType =  getStmtType(parentIndex);
		if(parentType.compare("while")==0){
			return cfg.CFGNodes[parentIndex];
		}else if(parentType.compare("if")==0){
			int afterParentNo =  findFollowed(parentIndex);
			if(afterParentNo>0){ // exits
				return buildLink(afterParentNo);  // may be duplicated
			}else {
				return findNext(parentIndex);
			}
		}
	}

	return cfg.CFGNodes[0];
}

void PKB::printCFG()
{
	for(unsigned int i=0;i<visited.size();i++){
		visited[i]=0;
	}
	//cout<<" CFGList size "<<cfg.CFGHeaderList.size()<<endl;
	for(unsigned int i=0;i<cfg.CFGHeaderList.size();i++){
		if(i!=cfg.CFGHeaderList.size()-1)continue; //***

		//cout<<" procedure "<<(i+1)<<endl;
		CFGNode* rootNode = cfg.CFGHeaderList[i];
		printfTree(rootNode);
	}
	//getchar();
	//cout<<"CFG END"<<endl;
	//getchar();
}
void PKB::printfTree(CFGNode *node)
{
	int stmtNo = node->stmtNum;
	if(visited[stmtNo]==0)
		visited[stmtNo]=1;
	else return;
	for(unsigned int i=0;i<node->childList.size();i++)
	{
		//cout<<node->stmtNum<<" to "<<node->childList[i]->stmtNum<<endl;
		printfTree(node->childList[i]);
	}
	if(node->childList.size()==0)
	{
		//cout<<node->stmtNum<<" to "<<"NO-WHERE"<<endl;
	}
}

/************************************************** Next *************************************************/
vector<int> PKB::getNext(int stmtNo){
	return cfg.getNext(stmtNo);
}

vector<int> PKB::getPrev(int stmtNo){
	return cfg.getPrev(stmtNo);
}
bool PKB::isNext(int stmtNo1,int stmtNo2)
{
	if(cfg.isNext(stmtNo1,stmtNo2))
		return true;
	return false;
}

bool PKB::isNextT(int stmtNo1, int stmtNo2)
{
	if(cfg.isNextStar(stmtNo1,stmtNo2))
		return true;
	return false;
}

vector<int> PKB::getNextT(int stmtNo)
{	
	printf("Enter next star \n");
	clock_t t;
	t = clock();
	vector<int> nextStarList = cfg.getNextStar(stmtNo);
	t = clock() - t;
	////cout<<"This affectT takes "<<finish<<endl;
	printf ("It took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
	return nextStarList;
}
vector<int> PKB::getPrevT(int stmtNo)
{
	vector<int> prevStarList = cfg.getPrevStar(stmtNo);
	return prevStarList;
}

bool PKB::checkNext(string arg1, string arg1Type, string arg2, string arg2Type){
	if(arg1=="_"&&arg2=="_"){
		int size = stmtTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int second = Util::convertStringToInt(arg2);
		vector<int> f = getPrev(second);
		if(f.size()>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int f = Util::convertStringToInt(arg1);
		vector<int> s = getNext(f);
		if(s.size()>0) return true;
		else return false;
	}else{
		int first = Util::convertStringToInt(arg1);
		int second = Util::convertStringToInt(arg2);
		
		return isNext(first,second);
	}
}
vector<pair<string, string>> PKB::getNext(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type)
{
	//what? _/integer

	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	for(it1=arg1List.begin();it1!=arg1List.end();it1++){
		string index1 = *it1;
		int stmtNo1 = atoi(index1.c_str());
		vector<int> childrenList = getNext(stmtNo1);
		for(it2=arg2List.begin();it2!= arg2List.end();it2++){
			string index2 = *it2;
			int stmtNo2 = atoi(index2.c_str());
			if(contains(childrenList,stmtNo2)){
				pair<string,string> p(index1,index2);
				result.push_back(p);
			}
		}
	}
	return result;
}
vector<pair<string, string>> PKB::getNextT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type)
{

	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	//** check size , next/prev

	for(it1=arg1List.begin();it1!=arg1List.end();it1++){
		string index1 = *it1;
		////cout<<"search "<<index1<<endl;
		int stmtNo1 = atoi(index1.c_str());
		vector<int> childrenList = getNextT(stmtNo1);
		
	

		for(it2=arg2List.begin();it2!= arg2List.end();it2++){
			string index2 = *it2;
			int stmtNo2 = atoi(index2.c_str());
			if(contains(childrenList,stmtNo2)){
				
				pair<string,string> p(index1,index2);
				result.push_back(p);
			}
		}
	}
	return result;
}
bool PKB::contains(vector<int> list, int stmtNo)
{
	for(unsigned int i=0;i<list.size();i++)
		if(stmtNo==list[i])
			return true;
	return false;
}
bool PKB::contains(list<int> list, int stmtNo)
{
	for (std::list<int>::iterator it=list.begin(); it!=list.end(); ++it)
		if(*it==stmtNo)
			return true;
	return false;
}

bool PKB::checkNextT(string arg1, string arg1Type, string arg2, string arg2Type)
{
	if(arg1=="_"&&arg2=="_"){
		int size = stmtTable.getSize();
		if(size>0)
			return true;
		else return false;
	}else if(arg1=="_"&&arg2Type=="integer"){
		int second = Util::convertStringToInt(arg2);
		vector<int> f = getPrevT(second);
		if(f.size()>0) return true;
		else return false;
	}else if(arg1Type=="integer"&&arg2=="_"){
		int f = Util::convertStringToInt(arg1);
		vector<int> s = getNextT(f);
		if(s.size()>0) return true;
		else return false;
	}else{
		int first = Util::convertStringToInt(arg1);
		int second = Util::convertStringToInt(arg2);
		
		return isNextT(first,second);
	}
}

string PKB::toString(int num){
	stringstream ss;
	ss << num;
	return ss.str();
}

/************************************************** Affect -zy *************************************************/
vector<int> PKB::getAffectList(int stmtNo)
{
	affectList.clear();
	string type = getStmtType(stmtNo);
	if(type.compare("assign")!=0)
		return affectList;
	int varIndex = getModifiedStmt(stmtNo)[0];
	
	visited.clear();

	vector<int> childrenList = getNext(stmtNo);

	for(int i=0;i<childrenList.size();i++){
		int childStmt = childrenList[i];
		recusiveBuildAffectList(childStmt,varIndex);
	}

	return affectList;
}
vector<int> PKB::listToVector(list<int> lis)
{
	vector<int> vec;
	for (std::list<int>::iterator it=lis.begin(); it!=lis.end(); ++it)
		vec.push_back(*it);
	return vec;
}
vector<int> PKB::getAffectedList(int stmtNo)
{
	affectedList.clear();
	string type = getStmtType(stmtNo);
	if(type.compare("assign")!=0)
		return affectedList;
	vector<int> varIndexes = getUsedStmt(stmtNo); // all var-s that modified this stmt

	visited.clear();
	vector<int> parentList = getPrev(stmtNo);
	for(int i=0;i<parentList.size();i++){
		int parentStmt = parentList[i];
		recusiveBuildAffectedList(parentStmt,varIndexes);
	}
	return affectedList;
}
void PKB::recusiveBuildAffectedList(int stmtNo, vector<int> varIndexes)
{
	while(visited.size()<=stmtNo+1)
		visited.push_back(0);
	//num of loops
	if(visited[stmtNo]==1)
		return;
	else visited[stmtNo]=visited[stmtNo]+1;

	string type = getStmtType(stmtNo);
	if(type.compare("assign")!=0){

	}else{
		int modifiedVar = getModifiedStmt(stmtNo)[0];
		if(contains(varIndexes,modifiedVar)){
			if(!contains(affectedList,stmtNo)){
				affectedList.push_back(stmtNo);
				visited.clear();
			}
			vector<int>::iterator it = std::find(varIndexes.begin(),varIndexes.end(),modifiedVar);
			if (it != varIndexes.end()) {
				
				string var = getVarName(*it);
				if(varIndexes.size()>1)
					varIndexes.erase(it);
				else varIndexes.clear();
			}
		}
	}
	if(varIndexes.size()==0)return;

	vector<int> parentList = getPrev(stmtNo);
	for(int i=0;i<parentList.size();i++){
		int parentStmt = parentList[i];
		////cout<<"no "<<stmtNo<<" child "<<childStmt<<endl;
		recusiveBuildAffectedList(parentStmt,varIndexes);
	}

}
void PKB::recusiveBuildAffectList(int stmtNo, int varIndex)
{
	while(visited.size()<=stmtNo+1)
		visited.push_back(0);
	//num of loops
	if(visited[stmtNo]==1)
		return;
	else visited[stmtNo]=visited[stmtNo]+1;

	vector<int> usedVarList = getUsedStmt(stmtNo);
	string stmtType =  getStmtType(stmtNo);
	
	//**** check if it's call stmt

	// problematic!!!! while situation, i only need assignment vars
	if(stmtType.compare("assign")==0&&contains(usedVarList,varIndex)){
		if(!contains(affectList,stmtNo)){
			
			affectList.push_back(stmtNo);
			visited.clear();
		}
	}
	int currentVar= -1;
	if(getModifiedStmt(stmtNo).size()>0)
		currentVar= getModifiedStmt(stmtNo)[0]; 

	if(stmtType.compare("assign")==0&&currentVar==varIndex){// this var is being modified
		return ;
	}else{
		vector<int> childrenList = getNext(stmtNo);
		for(int i=0;i<childrenList.size();i++){
			int childStmt = childrenList[i];
			////cout<<"no "<<stmtNo<<" child "<<childStmt<<endl;
			recusiveBuildAffectList(childStmt,varIndex);
		}
	}
}

vector<pair<string, string>> PKB::getAffects(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type)
{
	//cout<<"Into affects "<<endl;
	clock_t t;
	t = clock();
	// _/integer??.
	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	// if set1 is smaller , we find next
	if(arg1_set->size()<=arg2_set->size())
	{
		for(it1=arg1List.begin();it1!=arg1List.end();it1++){
			vector<int> list1;
			int index1;
			istringstream ( *it1 ) >> index1;

			list1 = getAffectList(index1);
			for(it2=arg2List.begin();it2!=arg2List.end();it2++){
				int index2;
				istringstream (*it2) >> index2;
				if(contains(list1,index2)){
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}else{
		for(it2=arg2List.begin();it2!=arg2List.end();it2++){
			vector<int> list2;
			int index2;
			istringstream ( *it2 ) >> index2;
			list2=getAffectedList(index2);
			for(it1=arg1List.begin();it1!=arg1List.end();it1++){
				int index1;
				istringstream (*it1)>>index1;
				if(contains(list2,index1)){
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}
	t = clock() - t;
	////cout<<"This affectT takes "<<finish<<endl;
	printf ("It took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
	return result;
}



vector<pair<string, string>> PKB::getAffectsT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type)
{
	//cout<<"Into affectT"<<endl;

	clock_t t;
	t = clock();
	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	// if set1 is smaller , we find next
	if(arg1_set->size()<=arg2_set->size())
	{
		for(it1=arg1List.begin();it1!=arg1List.end();it1++){
			vector<int> list1;
			int index1;
			istringstream ( *it1 ) >> index1;

			string type = getStmtType(index1);
			if(type.compare("assign")!=0)
				continue;

			list1 = getAffectTList(index1);
			for(it2=arg2List.begin();it2!=arg2List.end();it2++){
				int index2;
				istringstream (*it2) >> index2;
				////cout<<"index1 "<<index1<<" index2 "<<index2<<" size "<<result.size()<<endl;
				if(contains(list1,index2)){
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}else{ // set2 is smaller , find prev
		for(it2=arg2List.begin();it2!=arg2List.end();it2++){
			vector<int> list2;
			int index2;
			istringstream ( *it2 ) >> index2;

			string type = getStmtType(index2);
			if(type.compare("assign")!=0)
				continue;

			list2 = getAffectedTList(index2);
			for(it1=arg1List.begin();it1!=arg1List.end();it1++){
				int index1;
				istringstream (*it1) >> index1;
				
				if(contains(list2,index1)){
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}
	t = clock() - t;
	////cout<<"This affectT takes "<<finish<<endl;
	printf ("It took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
	return result;

}

bool PKB::checkAffects(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	//cout<<"into check affect"<<endl;
	clock_t t;
	t = clock();
	// _/integer??.
	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	// if set1 is smaller , we find next
	if(arg1_set->size()<=arg2_set->size())
	{
		for(it1=arg1List.begin();it1!=arg1List.end();it1++){
			vector<int> list1;
			int index1;
			istringstream ( *it1 ) >> index1;

			list1 = getAffectList(index1);
			for(it2=arg2List.begin();it2!=arg2List.end();it2++){
				int index2;
				istringstream (*it2) >> index2;
				if(contains(list1,index2)){
					return true;
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}else{
		for(it2=arg2List.begin();it2!=arg2List.end();it2++){
			vector<int> list2;
			int index2;
			istringstream ( *it2 ) >> index2;
			list2=getAffectedList(index2);
			for(it1=arg1List.begin();it1!=arg1List.end();it1++){
				int index1;
				istringstream (*it1)>>index1;
				if(contains(list2,index1)){
					return true;
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}
	t = clock() - t;
	////cout<<"This affectT takes "<<finish<<endl;
	printf ("It took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);
	//return result;
	
	return false;
}

bool PKB::checkAffectsT(set<string>* arg1_set, string arg1Type, set<string>* arg2_set, string arg2Type){
	
	//cout<<"into check affect*"<<endl;
	clock_t t;
	t = clock();
	vector<pair<string,string>> result;

	set<string>::iterator it1;
	set<string>::iterator it2;
	set<string> arg1List = *arg1_set;
	set<string> arg2List = *arg2_set;

	// if set1 is smaller , we find next
	if(arg1_set->size()<=arg2_set->size())
	{
		for(it1=arg1List.begin();it1!=arg1List.end();it1++){
			vector<int> list1;
			int index1;
			istringstream ( *it1 ) >> index1;

			string type = getStmtType(index1);
			if(type.compare("assign")!=0)
				continue;

			list1 = getAffectTList(index1);
			for(it2=arg2List.begin();it2!=arg2List.end();it2++){
				int index2;
				istringstream (*it2) >> index2;
				////cout<<"index1 "<<index1<<" index2 "<<index2<<" size "<<result.size()<<endl;
				if(contains(list1,index2)){
					return true;
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}else{ // set2 is smaller , find prev
		for(it2=arg2List.begin();it2!=arg2List.end();it2++){
			vector<int> list2;
			int index2;
			istringstream ( *it2 ) >> index2;

			string type = getStmtType(index2);
			if(type.compare("assign")!=0)
				continue;

			list2 = getAffectedTList(index2);
			for(it1=arg1List.begin();it1!=arg1List.end();it1++){
				int index1;
				istringstream (*it1) >> index1;

				if(contains(list2,index1)){
					return true;
					pair<string,string> p (*it1,*it2);
					result.push_back(p);
				}
			}
		}
	}
	t = clock() - t;
	////cout<<"This affectT takes "<<finish<<endl;
	printf ("It took (%f seconds).\n",((float)t)/CLOCKS_PER_SEC);

	return false;
}


bool PKB::checkAffects(string arg1, string arg1Type, string arg2, string arg2Type)
{
	int stmtNo1,stmtNo2;
	istringstream (arg1) >> stmtNo1;
	istringstream (arg2) >> stmtNo2;
	
	vector<int> affectResult = getAffectList(stmtNo1);
	for(int i=0;i<affectResult.size();i++){
		if(stmtNo2==affectResult[i])
			return true;  
	}
	return false;

	//****** will be deleted
}
bool PKB::checkAffectsT(string arg1, string arg1Type, string arg2, string arg2Type)
{
	//******** will be deleted
	int stmtNo1,stmtNo2;
	istringstream (arg1) >> stmtNo1;
	istringstream (arg2) >> stmtNo2;

	vector<int> affectTResult = getAffectTList(stmtNo1);
	for(int i=0;i<affectTResult.size();i++){
		if(stmtNo2==affectTResult[i])
			return true;  
	}
	return false;
}
vector<int> PKB::getAffectTList(int stmtNo)
{


	visited.clear();
	affectTList.clear();
	int varIndex = getModifiedStmt(stmtNo)[0];
	vector<int> varIndexList;
	varIndexList.push_back(varIndex);

	vector<int> childrenList = getNext(stmtNo);
	for(int i=0;i<childrenList.size();i++){
		int childStmt = childrenList[i];
		recusiveBuildAffectTList(childStmt,varIndexList);
	}
	//DWORD finish = GetTickCount()-start;


	return affectTList;
}
vector<int> PKB::getAffectedTList(int stmtNo)
{

	visited.clear();
	affectedTList.clear();

	string type = getStmtType(stmtNo);
	if(type.compare("assign")!=0)
		return affectedTList;

	vector<int> varIndexes = getUsedStmt(stmtNo);


	vector<int> parentList = getPrev(stmtNo);
	for(int i=0;i<parentList.size();i++){
		int parentStmt = parentList[i];
		recusiveBuildAffectedTList(parentStmt,varIndexes);
	}
	//DWORD finish = GetTickCount()-start;
	return affectedTList;
}
void PKB::recusiveBuildAffectedTList(int stmtNo, vector<int> varIndexes)
{
	////cout<<"STMTNO "<<stmtNo<<endl; 
	//for(int i=0;i<varIndexes.size();i++){
		////cout<<"used  "<<getVarName(varIndexes[i])<<endl;
	//}
	while(visited.size()<=stmtNo)
		visited.push_back(0);
	//num of loops
	if(visited[stmtNo]>=1)//********** need to change to NUM, each time modify, num++;
		return;
	else visited[stmtNo]=visited[stmtNo]+1;
	
	string stmtType = getStmtType(stmtNo);

	int modifiedVar = getModifiedStmt(stmtNo)[0];
	if(stmtType.compare("assign")==0&&contains(varIndexes,modifiedVar)){
		if(!contains(affectedList,stmtNo)){
			affectedTList.push_back(stmtNo);
			//visited.clear();


		}

		vector<int> usedVarList = getUsedStmt(stmtNo);
		for(int i=0;i<usedVarList.size();i++){
			int usedVar = usedVarList[i];
			if(!contains(varIndexes,usedVar)){
				varIndexes.push_back(usedVar);

				////cout<<"add new used var "<<getVarName(usedVar)<<endl;
				//getchar();
				visited.clear();
			}
		}
	}

	vector<int> parentList = getPrev(stmtNo);
	for(int i=0;i<parentList.size();i++){
		int parentStmt = parentList[i];
		recusiveBuildAffectedTList(parentStmt,varIndexes);
	}

}
bool PKB::intersect(vector<int> list1, vector<int> list2){
	for(int i=0;i<list1.size();i++)
		for(int j=0;j<list2.size();j++){
			if(list1[i]==list2[j])
				return true;
		}
	return false;
}
// need to change it to , int stmtNo(current line), vector<int> varList(all varIndex that are using!!!)
void PKB::recusiveBuildAffectTList(int stmtNo, vector<int> varIndexList)
{
	////cout<<"STMTNO "<<stmtNo<<"  varIndex  "<<varIndexList[0]<<endl; // how to solve loop!!

	while(visited.size()<=stmtNo)
		visited.push_back(0);
	//num of loops
	if(visited[stmtNo]==1)//********** need to change to NUM, each time modify, num++;
		return;
	else visited[stmtNo]=visited[stmtNo]+1;
	
	string stmtType = getStmtType(stmtNo);

	vector<int> usedVarList = getUsedStmt(stmtNo);
	if(stmtType.compare("assign")==0&&intersect(usedVarList,varIndexList)){
		if(!contains(affectTList,stmtNo)){
			affectTList.push_back(stmtNo);
			visited.clear();
		}

		int currentVar = getModifiedStmt(stmtNo)[0];
		if(!contains(varIndexList,currentVar))
			varIndexList.push_back(currentVar);
	}

	vector<int> childrenList = getNext(stmtNo);
	for(int i=0;i<childrenList.size();i++){
		int childStmt = childrenList[i];
		recusiveBuildAffectTList(childStmt,varIndexList);
	}

}

/************************************************** Flatten - Zhao Yang *************************************************/
void PKB::flattenAST()
{
	vector<TNode*> listOfRootNode = ast.getAST();
	
	
	for(unsigned i=0;i<listOfRootNode.size();i++){
		string type ="";
		int lineNum =-1;
		string varRef="#";
		string postfixExpr="#";

		TNode *thisTnode = listOfRootNode[i];
		// get type
		int content= (thisTnode->getContent());
		if(content==-1){
			type="assign";
		}else if(content==-2){
			type="if";
		}else if(content==-3){
			type="while";
		}//else if(content==-4) type = "call"; // call

		// get varRef
		int varIndex = thisTnode->getLeftChild()->getContent();
		varRef = getVarName(varIndex);

		lineNum = thisTnode->getLineNo();
		
		postfixExpr = createPostfix(thisTnode->getRightChild());

		/*//cout<<endl;
		//cout<<"type: "<<type<<endl;
		//cout<<"lineNum: "<<lineNum<<endl;
		//cout<<"varRef: "<<varRef<<endl;
		//cout<<"postfixExpr: --"<<postfixExpr<<"--"<<endl;
		//cout<<endl; */

		postfixNode* node = new postfixNode(type,lineNum,varRef,postfixExpr);

		postfixExprList[lineNum] = node;
	}

	
}
string PKB::createPostfix(TNode *node)
{
	string result = "";;
	if(!node)
		return "";
	if(node&&node->hasLeftChild==1)
		result += createPostfix(node->getLeftChild());
	if(node&&node->hasRightChild==1)
		result += createPostfix(node->getRightChild());
	if(node->getType().compare("opt")!=0){  
		if(node->getType().compare("const")!=0)// var node
			result += getVarName(node->getContent());
		else{ // const node
			int constFactor=node->getContent();
			stringstream ss;
			ss << constFactor;
			result += ss.str();
		}
	}
	else{
		if(node->getContent()==-1)
			result += "+";
		else if(node->getContent()==-2)
			result += "-";
		else if(node->getContent()==-3)
			result += "*";
		else if(node->getContent()==-4)
			result += "/";
	}
	result += " ";
	return result;
}


/************************************/
/*               Testing next/nextT/affects/affectT   -zy                                                   */
/************************************/

void PKB::printNext()
{
	int stmtNum = getSizeStmtTable();
	//cout<<"StmtNumber   "<<"  nextStmt"<<endl;
	for(int i=1;i<=stmtNum;i++){
		vector<int> nextList = getNext(i);
		sort (nextList.begin(), nextList.end());
		for(unsigned int j=0;j<nextList.size();j++){
			//cout<<i<<"         "<<nextList[j]<<endl;
		}
	}

}
void PKB::printNextT()
{
	int stmtNum = getSizeStmtTable();
	//cout<<"StmtNumber   "<<"  nextTStmt"<<endl;
	for(int i=1;i<=stmtNum;i++){
		vector<int> nextTList = getNextT(i);
		sort (nextTList.begin(), nextTList.end());
		for(unsigned int j=0;j<nextTList.size();j++){
			//cout<<i<<"         "<<nextTList[j]<<endl;
		}
	}

}
void PKB::printAffects()
{
	int stmtNum = getSizeStmtTable();
	//cout<<"StmtNumber   "<<"  affectsStmt"<<endl;
	for(int i=1;i<=stmtNum;i++){
		if(getStmtType(i).compare("assign")!=0)
			continue;
		vector<int> affectList = getAffectList(i);
		sort (affectList.begin(), affectList.end());
		for(unsigned int j=0;j<affectList.size();j++){
			//cout<<i<<"                   "<<affectList[j]<<endl;
		}
	}

}
void PKB::printAffectsT()
{
	// affect only assignment?>
	int stmtNum = getSizeStmtTable();
	//cout<<"StmtNumber   "<<"  affectsTStmt"<<endl;
	for(int i=1;i<=stmtNum;i++){
		if(getStmtType(i).compare("assign")!=0)
			continue;
		vector<int> affectTList = getAffectTList(i);
		sort (affectTList.begin(), affectTList.end());
		for(unsigned int j=0;j<affectTList.size();j++){
			//cout<<i<<"                   "<<affectTList[j]<<endl;
		}
	}

}
