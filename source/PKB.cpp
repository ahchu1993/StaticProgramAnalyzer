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
vector<pair<string, string>> PKB::getCall(string arg1, string arg1Type, string arg2, string arg2Type){	
	vector<string> set1, set2;
	if(arg1Type.compare("procedure") == 0 || arg1Type.compare("_") == 0){
		set1 = callTable.getCall("_");
	}else if(arg1Type.compare("String") == 0){
		set1 = callTable.getCall(arg1);
	}

	if(arg2Type.compare("procedure") == 0 || arg2Type.compare("_") == 0){
		set2 = callTable.getCalled("_");
	}else if(arg2Type.compare("String") == 0){
		set2 = callTable.getCalled(arg2);
	}
	vector<pair<string, string>> result = callTable.getCallPairList(set1, set2);

	for(unsigned i=0; i<result.size(); i++){
		stringstream ss1,ss2;
		string proc1_string = result.at(i).first;
		string proc2_string = result.at(i).second;
		int proc1 =	procTable.getProcIndex(proc1_string);
		int proc2 =	procTable.getProcIndex(proc2_string);
		ss1 << proc1;
		ss2 << proc2;
		proc1_string = ss1.str();
		proc2_string = ss2.str();
		result.at(i).first= proc1_string;
		result.at(i).second = proc2_string;
	}
	return result;
}

bool PKB::checkCall(string arg1, string arg1Type, string arg2, string arg2Type){	
	vector<string> set1, set2;
	if(arg1Type.compare("procedure") == 0 || arg1Type.compare("_") == 0){
		set1 = callTable.getCall("_");
	}else if(arg1Type.compare("String") == 0){
		set1 = callTable.getCall(arg1);
	}

	if(arg2Type.compare("procedure") == 0 || arg2Type.compare("_") == 0){
		set2 = callTable.getCalled("_");
	}else if(arg2Type.compare("String") == 0){
		set2 = callTable.getCalled(arg2);
	}
	return callTable.checkCall(set1, set2);
}
void PKB::insert(string proc1, string proc2){
	callTable.insert(proc1,proc2);
}
bool PKB::isCalled(string proc1, string proc2){
	return callTable.isCalled(proc1,proc2);
}

vector<string> PKB::getCallsList(string procName){
	return callTable.getCallsList(procName);
}
vector<string> PKB::getCalledList(string procName){
	return callTable.getCalledList(procName);
}
void PKB::printCallTable(){
	callTable.print();
}
//Api for Khue
vector<int> PKB::getCallsList(int procIndex){
	vector<int> result;
	string procName = procTable.getProcName(procIndex);
	vector<string> callList = callTable.getCallsList(procName);
	
	for(unsigned i=0; i<callList.size(); i++){
		result.push_back(procTable.getProcIndex(callList.at(i)));
	}
	return result;
}
/************************************************** ParentTable *************************************************/
vector<pair<string, string>> PKB::getParent(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1, set2;
	if(arg1Type.compare("prog_line") == 0 || arg1Type.compare("stmt") == 0 || arg1Type.compare("_") == 0){
		set1 = parentTable.getParentList("_");
	}else if(arg1Type.compare("while") == 0 || arg1Type.compare("if") == 0){
		set1 = parentTable.getParentList(arg1Type);
	}

	if(arg2Type.compare("prog_line") == 0 || arg2Type.compare("stmt") == 0 || arg2Type.compare("_") == 0){
		set2 = parentTable.getChildrenList("_");
	}else if(arg2Type.compare("while") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("assign") == 0){
		set2 = parentTable.getChildrenList(arg2Type);
	}
	
	return parentTable.getParentPairList(set1, set2);
}
bool PKB::checkParent(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1, set2;
	if(arg1Type.compare("prog_line") == 0 || arg1Type.compare("stmt") == 0 || arg1Type.compare("_") == 0){
		set1 = parentTable.getParentList("_");
	}else if(arg1Type.compare("while") == 0 || arg1Type.compare("if") == 0){
		set1 = parentTable.getParentList(arg1Type);
	}

	if(arg2Type.compare("prog_line") == 0 || arg2Type.compare("stmt") == 0 || arg2Type.compare("_") == 0){
		set2 = parentTable.getChildrenList("_");
	}else if(arg2Type.compare("while") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("assign") == 0){
		set2 = parentTable.getChildrenList(arg2Type);
	}
	
	return parentTable.checkParent(set1, set2);
}
void PKB::insert(int stm1, string DE1, int stm2, string DE2){
	parentTable.insert(stm1, DE1, stm2, DE2);
}
int PKB::getParent (int stm){
	return parentTable.getParent(stm);
}
vector<int> PKB::getParentT(int stmt){
	return parentTable.getParentT(stmt);
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
vector<pair<string, string>> PKB::getFollow(string arg1, string arg1Type, string arg2, string arg2Type){	
	vector<int> set1, set2;
	if(arg1Type.compare("prog_line") == 0 || arg1Type.compare("stmt") == 0 || arg1Type.compare("_") == 0){
		set1 = followTable.getFollowsList("_");
	}else if(arg1Type.compare("while") == 0 || arg1Type.compare("if") == 0){
		set1 = followTable.getFollowsList(arg1Type);
	}

	if(arg2Type.compare("prog_line") == 0 || arg2Type.compare("stmt") == 0 || arg2Type.compare("_") == 0){
		set2 = followTable.getFollowedList("_");
	}else if(arg2Type.compare("while") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("assign") == 0){
		set2 = followTable.getFollowedList(arg2Type);
	}
	
	return followTable.getFollowPairList(set1, set2);
}
bool PKB::checkFollow(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1, set2;
	if(arg1Type.compare("prog_line") == 0 || arg1Type.compare("stmt") == 0 || arg1Type.compare("_") == 0){
		set1 = followTable.getFollowsList("_");
	}else if(arg1Type.compare("while") == 0 || arg1Type.compare("if") == 0){
		set1 = followTable.getFollowsList(arg1Type);
	}

	if(arg2Type.compare("prog_line") == 0 || arg2Type.compare("stmt") == 0 || arg2Type.compare("_") == 0){
		set2 = followTable.getFollowedList("_");
	}else if(arg2Type.compare("while") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("assign") == 0){
		set2 = followTable.getFollowedList(arg2Type);
	}
	
	return followTable.checkFollow(set1, set2);
}
void PKB::insertFollow(int stm1, string DE1, int stm2, string DE2){
	followTable.insertFollow(stm1, DE1, stm2, DE2);
}
int PKB::findFollowed(int stm){
	return followTable.findFollowed(stm);
}
vector<int> PKB::findFollowedT(int stmt, string DE){
	return followTable.findFollowedT(stmt, DE);
}
int PKB::findFollows(int stm){
	return followTable.findFollows(stm);
}
vector<int> PKB::findFollowsT(int stmt, string DE){
	return followTable.findFollowsT(stmt, DE);
}
bool PKB::isFollowed(int stm1,int stm2){
	return followTable.isFollowed(stm1,stm2);
}
void PKB::printFollowTable(){
	followTable.print();
}

/************************************************** ModifyTable *************************************************/
vector<pair<string, string>> PKB::getModify(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = modifyTable.getModifyProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = modifyTable.getModifyStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = modifyTable.getModifyDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg1);
		if(procIndex != -1){
			set1.push_back(procIndex);
		}
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = modifyTable.getModifyVarList();
	} else if (arg2Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg2);
		if (procIndex != -1){
			set2.push_back(procIndex);
		} else{
			varIndex = varTable.getVarIndex(arg2);
			if (varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return modifyTable.getModifyPairList(set1, set2);
}

vector<pair<string, string>> PKB::getModifySpecific(vector<string> arg1List, string arg1Type, vector<string> arg2List, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex, stmtNo;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		// arg1List will be vector of proc name
		for (unsigned i = 0; i<arg1List.size(); i++){
			procIndex = procTable.getProcIndex(arg1List.at(i));
			if(procIndex != -1){
				set1.push_back(procIndex);
			}
		}
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0 || arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		for (unsigned i = 0; i<arg1List.size(); i++){
			istringstream(arg1List.at(i))>>stmtNo;
			set1.push_back(stmtNo);
		}
	} else if (arg1Type.compare("integer") == 0){
		if(arg1List.size() == (unsigned)1){
			istringstream(arg1List.at(0))>>stmtNo;
			set1.push_back(stmtNo);
		}
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		// arg2List will be vector of var name
		for (unsigned i = 0; i<arg2List.size(); i++){
			varIndex = varTable.getVarIndex(arg2List.at(i));
			if(varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return modifyTable.getModifyPairList(set1, set2);
}

bool PKB::checkModify(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = modifyTable.getModifyProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = modifyTable.getModifyStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = modifyTable.getModifyDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg1);
		if(procIndex != -1){
			set1.push_back(procIndex);
		}
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = modifyTable.getModifyVarList();
	} else if (arg2Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg2);
		if (procIndex != -1){
			set2.push_back(procIndex);
		} else{
			varIndex = varTable.getVarIndex(arg2);
			if (varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return modifyTable.checkModify(set1, set2);
}

// Update ModifyTable using CallTable
void PKB::updateModify(){
	vector<int> procList;
	int currentProcIndex;
	int tempProcIndex;
	int tempVarIndex;
	vector<int> callingProc;
	vector<int> modifiedVar;

	procList = procTable.getProcList();
	for (unsigned i=0; i<procList.size(); i++){
		currentProcIndex = procList.at(i);
		callingProc = getCallsList(currentProcIndex);
		modifiedVar = modifyTable.getModifiedProc(currentProcIndex);
		for (unsigned j=0; j<callingProc.size(); j++){
			tempProcIndex = callingProc.at(j);
			for (unsigned k=0; k<modifiedVar.size(); k++){
				tempVarIndex = modifiedVar.at(k);
				modifyTable.insertModifyProc(tempProcIndex, tempVarIndex);
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
	modifyTable.printModifyTable();
}

/************************************************** UseTable *************************************************/
vector<pair<string, string>> PKB::getUse(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = useTable.getUseProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = useTable.getUseStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = useTable.getUseDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg1);
		if(procIndex != -1){
			set1.push_back(procIndex);
		}
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = useTable.getUseVarList();
	} else if (arg2Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg2);
		if (procIndex != -1){
			set2.push_back(procIndex);
		} else{
			varIndex = varTable.getVarIndex(arg2);
			if (varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return useTable.getUsePairList(set1, set2);
}

vector<pair<string, string>> PKB::getUseSpecific(vector<string> arg1List, string arg1Type, vector<string> arg2List, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex, stmtNo;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		// arg1List will be vector of proc name
		for (unsigned i = 0; i<arg1List.size(); i++){
			procIndex = procTable.getProcIndex(arg1List.at(i));
			if(procIndex != -1){
				set1.push_back(procIndex);
			}
		}
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0 || arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		for (unsigned i = 0; i<arg1List.size(); i++){
			istringstream(arg1List.at(i))>>stmtNo;
			set1.push_back(stmtNo);
		}
	} else if (arg1Type.compare("integer") == 0){
		if(arg1List.size() == (unsigned)1){
			istringstream(arg1List.at(0))>>stmtNo;
			set1.push_back(stmtNo);
		}
	}

	// Get the set of possible values for argument 2
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		// arg2List will be vector of var name
		for (unsigned i = 0; i<arg2List.size(); i++){
			varIndex = varTable.getVarIndex(arg2List.at(i));
			if(varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return useTable.getUsePairList(set1, set2);
}

bool PKB::checkUse(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;
	int procIndex, varIndex;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = useTable.getUseProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = useTable.getUseStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = useTable.getUseDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg1);
		if(procIndex != -1){
			set1.push_back(procIndex);
		}
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = useTable.getUseVarList();
	} else if (arg2Type.compare("string") == 0){
		procIndex = procTable.getProcIndex(arg2);
		if (procIndex != -1){
			set2.push_back(procIndex);
		} else{
			varIndex = varTable.getVarIndex(arg2);
			if (varIndex != -1){
				set2.push_back(varIndex);
			}
		}
	}

	return useTable.checkUse(set1, set2);
}

// Update UseTable using CallTable
void PKB::updateUse(){
	vector<int> procList;
	int currentProcIndex;
	int tempProcIndex;
	int tempVarIndex;
	vector<int> callingProc;
	vector<int> usedVar;

	procList = procTable.getProcList();
	for (unsigned i=0; i<procList.size(); i++){
		currentProcIndex = procList.at(i);
		callingProc = getCallsList(currentProcIndex);
		usedVar = useTable.getUsedProc(currentProcIndex);
		for (unsigned j=0; j<callingProc.size(); j++){
			tempProcIndex = callingProc.at(j);
			for (unsigned k=0; k<usedVar.size(); k++){
				tempVarIndex = usedVar.at(k);
				useTable.insertUseProc(tempProcIndex, tempVarIndex);
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
	useTable.printUseTable();
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

/************************************************** CFG - Zhao Yang *************************************************/
void PKB::buildCFG()
{
	currentIndex=1;
	for(int i=0;i< getSizeStmtTable()+1;i++)
	{
		CFGNode * newNode = new CFGNode(i); // 0 is dummy
		cfg.CFGNodes.push_back(newNode);
		
		visited.push_back(0);
	}
	cout<<"size proc table "<< getSizeProcTable()<<endl;
	int procNum =  getSizeProcTable();
	for(int i=0;i<procNum;i++){
		buildTree(i);
		getchar();
	}

	// build cfgparentlist   ->double linked list
	for(unsigned int i=0;i<cfg.CFGHeaderList.size();i++)
		cfg.buildCFGParentList(cfg.CFGHeaderList[i]->stmtNum);
}

void PKB::buildTree(int procIndex)
{
	cout<<"current index "<<currentIndex<<endl;
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

	cout<<"stmtType: "<<stmtType<<"  "<<stmtNo<<endl;
	//cout<<"followed: "<< findFollowed(1)<<endl; // find after
	//cout<<"follows: "<< findFollows(1)<<endl;  // find prev

	if(stmtType.compare("while")==0){

		CFGNode *whileNode=cfg.CFGNodes[stmtNo];

		whileNode->addChild(buildLink(stmtNo+1)); // must be his child

		int followedIndex =  findFollowed(stmtNo);
		if(followedIndex>0){
			whileNode->addChild(buildLink(followedIndex));
		}

		return whileNode;
	}else if(stmtType.compare("if")==0){
		CFGNode *ifNode=cfg.CFGNodes[stmtNo];

		vector<int> childrenList =  getChildren(stmtNo,"stmt"); // *** why?
		cout<<"child "<<childrenList.size()<<"  stmtNo: "<<stmtNo<<endl;
		
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

		cout<<"haha found: "<<"  "<<afterElseStmtNo<<endl;

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
	cout<<" CFGList size "<<cfg.CFGHeaderList.size()<<endl;
	for(unsigned int i=0;i<cfg.CFGHeaderList.size();i++){
		cout<<" procedure "<<(i+1)<<endl;
		CFGNode* rootNode = cfg.CFGHeaderList[i];
		printfTree(rootNode);
	}
	getchar();
	cout<<"CFG END"<<endl;
	getchar();
}
void PKB::printfTree(CFGNode *node)
{
	int stmtNo = node->stmtNum;
	if(visited[stmtNo]==0)
		visited[stmtNo]=1;
	else return;
	for(unsigned int i=0;i<node->childList.size();i++)
	{
		cout<<node->stmtNum<<" to "<<node->childList[i]->stmtNum<<endl;
		printfTree(node->childList[i]);
	}
	if(node->childList.size()==0)
	{
		cout<<node->stmtNum<<" to "<<"NO-WHERE"<<endl;
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
	vector<int> nextStarList = cfg.getNextStar(stmtNo);
	return nextStarList;
}
vector<int> PKB::getPrevT(int stmtNo)
{
	vector<int> prevStarList = cfg.getPrevStar(stmtNo);
	return prevStarList;
}

vector<pair<string,string>> PKB::getNext(string arg1, string arg1Type, string arg2, string arg2Type){
	// synonym(prog_line|stmt|assign|if|while), _, integer
	//					synonym(prog_line|stmt|assign|if|while), _, integer
	vector<pair<string,string>> result;
	vector<int> list1;
	vector<int> list2;
	int type =0;
	// Get the set of possible values for argument 1
	if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		type=1;
		list1 = getStmtNo("stmt");
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		type=2;
		list1 = getStmtNo(arg1Type);
	} else if (arg1Type.compare("integer") == 0){
		type=3;
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		list1.push_back(stmtNo);
	} else if(arg1Type.compare("_")==0){
		type=4;
		// ***** what?
		list1 = getStmtNo("stmt");
	}

	//************ need optimize according to arg2type

	// Get the set of possible values for argument 2
	if (arg2Type.compare("stmt") == 0 || arg2Type.compare("prog_line") == 0){
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextList = getNext(stmtNo1);
			for(unsigned int j=0;j<nextList.size();j++){
				int stmtNo2 = nextList[j];
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	} else if (arg2Type.compare("assign") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("while") == 0){
		list2 = getStmtNo(arg2Type);
		if(type!=3){
			for(unsigned int j=0;j<list2.size();j++){
				int stmtNo2=list2[j];
				vector<int> prevList = getPrev(stmtNo2);
				for(unsigned int i=0;i<prevList.size();i++){
					int stmtNo1 = prevList[i];
					if(contains(list1,stmtNo1)){
						pair<string,string> p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
					}
				}
			}
		}else{ // arg1type==integer
			for(unsigned int i=0;i<list1.size();i++){
				int stmtNo1 = list1[i];
				vector<int> nextList = getNext(stmtNo1);
				for(unsigned int j=0;j<nextList.size();j++){
					int stmtNo2 = nextList[j];
					if(contains(list2,stmtNo2)){
						pair<string,string> p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
					}
				}
			}
		}
	} else if (arg2Type.compare("integer") == 0){
		int stmtNo2;
		istringstream(arg2)>>stmtNo2;
		vector<int> prevList = getPrev(stmtNo2);
		for(unsigned int i=0;i<prevList.size();i++){
			int stmtNo1 = prevList[i];
			if(contains(list1,stmtNo1)){
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	} else if(arg2Type.compare("_")==0){
		// ***** what?
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextList = getNext(stmtNo1);
			for(unsigned int j=0;j<nextList.size();j++){
				int stmtNo2 = nextList[j];
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	}



	return result;
}
bool PKB::checkNext(string arg1, string arg1Type, string arg2, string arg2Type){
	// synonym(prog_line|stmt|assign|if|while), _, integer
	//					synonym(prog_line|stmt|assign|if|while), _, integer
	vector<pair<string,string>> result;
	vector<int> list1;
	vector<int> list2;
	int type =0;
	// Get the set of possible values for argument 1
	if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		type=1;
		list1 = getStmtNo("stmt");
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		type=2;
		list1 = getStmtNo(arg1Type);
	} else if (arg1Type.compare("integer") == 0){
		type=3;
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		list1.push_back(stmtNo);
	} else if(arg1Type.compare("_")==0){
		type=4;
		// ***** what?
		list1 = getStmtNo("stmt");
	}

	//************ need optimize according to arg2type

	// Get the set of possible values for argument 2
	if (arg2Type.compare("stmt") == 0 || arg2Type.compare("prog_line") == 0){
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextList = getNext(stmtNo1);
			for(unsigned int j=0;j<nextList.size();j++){
				int stmtNo2 = nextList[j];
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	} else if (arg2Type.compare("assign") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("while") == 0){
		list2 = getStmtNo(arg2Type);
		if(type!=3){
			for(unsigned int j=0;j<list2.size();j++){
				int stmtNo2=list2[j];
				vector<int> prevList = getPrev(stmtNo2);
				for(unsigned int i=0;i<prevList.size();i++){
					int stmtNo1 = prevList[i];
					if(contains(list1,stmtNo1)){
						pair<string,string>p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
						return true;
					}
				}
			}
		}else{ // arg1type==integer
			for(unsigned int i=0;i<list1.size();i++){
				int stmtNo1 = list1[i];
				vector<int> nextList = getNext(stmtNo1);
				for(unsigned int j=0;j<nextList.size();j++){
					int stmtNo2 = nextList[j];
					if(contains(list2,stmtNo2)){
						pair<string,string>p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
						return true;
					}
				}
			}
		}
	} else if (arg2Type.compare("integer") == 0){
		int stmtNo2;
		istringstream(arg2)>>stmtNo2;
		vector<int> prevList = getPrev(stmtNo2);
		for(unsigned int i=0;i<prevList.size();i++){
			int stmtNo1 = prevList[i];
			if(contains(list1,stmtNo1)){
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	} else if(arg2Type.compare("_")==0){
		// ***** what?
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextList = getNext(stmtNo1);
			for(unsigned int j=0;j<nextList.size();j++){
				int stmtNo2 = nextList[j];
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	}



	return false;
}
bool PKB::contains(vector<int> list, int stmtNo)
{
	for(unsigned int i=0;i<list.size();i++)
		if(stmtNo==list[i])
			return true;
	return false;
}
vector<pair<string,string>> PKB::getNextT(string arg1, string arg1Type, string arg2, string arg2Type)
{
	// synonym(prog_line|stmt|assign|if|while), _, integer
	//					synonym(prog_line|stmt|assign|if|while), _, integer
	vector<pair<string,string>> result;
	vector<int> list1;
	vector<int> list2;
	int type =0;
	// Get the set of possible values for argument 1
	if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		type=1;
		list1 = getStmtNo("stmt");
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		type=2;
		list1 = getStmtNo(arg1Type);
	} else if (arg1Type.compare("integer") == 0){
		type=3;
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		list1.push_back(stmtNo);
	} else if(arg1Type.compare("_")==0){
		type=4;
		// ***** what?
		list1 = getStmtNo("stmt");
	}

	//************ need optimize according to arg2type

	// Get the set of possible values for argument 2
	if (arg2Type.compare("stmt") == 0 || arg2Type.compare("prog_line") == 0){
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextTList = getNextT(stmtNo1);
			for(unsigned int j=0;j<nextTList.size();j++){
				int stmtNo2 = nextTList[j];
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	} else if (arg2Type.compare("assign") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("while") == 0){
		list2 = getStmtNo(arg2Type);
		if(type!=3){
			for(unsigned int j=0;j<list2.size();j++){
				int stmtNo2=list2[j];
				vector<int> prevTList = getPrevT(stmtNo2);
				for(unsigned int i=0;i<prevTList.size();i++){
					int stmtNo1 = prevTList[i];
					if(contains(list1,stmtNo1)){
						pair<string,string> p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
					}
				}
			}
		}else{ // arg1type==integer
			for(unsigned int i=0;i<list1.size();i++){
				int stmtNo1 = list1[i];
				vector<int> nextTList = getNextT(stmtNo1);
				for(unsigned int j=0;j<nextTList.size();j++){
					int stmtNo2 = nextTList[j];
					if(contains(list2,stmtNo2)){
						pair<string,string> p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
					}
				}
			}
		}
	} else if (arg2Type.compare("integer") == 0){
		int stmtNo2;
		istringstream(arg2)>>stmtNo2;
		vector<int> prevTList = getPrevT(stmtNo2);
		for(unsigned int i=0;i<prevTList.size();i++){
			int stmtNo1 = prevTList[i];
			if(contains(list1,stmtNo1)){
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	} else if(arg2Type.compare("_")==0){
		// ***** what?
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextTList = getNextT(stmtNo1);
			for(unsigned int j=0;j<nextTList.size();j++){
				int stmtNo2 = nextTList[j];
				pair<string,string> p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
			}
		}
	}



	return result;
}
bool PKB::checkNextT(string arg1, string arg1Type, string arg2, string arg2Type)
{
	// synonym(prog_line|stmt|assign|if|while), _, integer
	//					synonym(prog_line|stmt|assign|if|while), _, integer
	vector<pair<string,string>> result;
	vector<int> list1;
	vector<int> list2;
	int type =0;
	// Get the set of possible values for argument 1
	if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		type=1;
		list1 = getStmtNo("stmt");
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		type=2;
		list1 = getStmtNo(arg1Type);
	} else if (arg1Type.compare("integer") == 0){
		type=3;
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		list1.push_back(stmtNo);
	} else if(arg1Type.compare("_")==0){
		type=4;
		// ***** what?
		list1 = getStmtNo("stmt");
	}

	//************ need optimize according to arg2type

	// Get the set of possible values for argument 2
	if (arg2Type.compare("stmt") == 0 || arg2Type.compare("prog_line") == 0){
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextTList = getNextT(stmtNo1);
			for(unsigned int j=0;j<nextTList.size();j++){
				int stmtNo2 = nextTList[j];
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	} else if (arg2Type.compare("assign") == 0 || arg2Type.compare("if") == 0 || arg2Type.compare("while") == 0){
		list2 = getStmtNo(arg2Type);
		if(type!=3){
			for(unsigned int j=0;j<list2.size();j++){
				int stmtNo2=list2[j];
				vector<int> prevTList = getPrevT(stmtNo2);
				for(unsigned int i=0;i<prevTList.size();i++){
					int stmtNo1 = prevTList[i];
					if(contains(list1,stmtNo1)){
						pair<string,string>p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
						return true;
					}
				}
			}
		}else{ // arg1type==integer
			for(unsigned int i=0;i<list1.size();i++){
				int stmtNo1 = list1[i];
				vector<int> nextTList = getNextT(stmtNo1);
				for(unsigned int j=0;j<nextTList.size();j++){
					int stmtNo2 = nextTList[j];
					if(contains(list2,stmtNo2)){
						pair<string,string>p(stmtNo1+"", stmtNo2+"");
						result.push_back(p);
						return true;
					}
				}
			}
		}
	} else if (arg2Type.compare("integer") == 0){
		int stmtNo2;
		istringstream(arg2)>>stmtNo2;
		vector<int> prevTList = getPrevT(stmtNo2);
		for(unsigned int i=0;i<prevTList.size();i++){
			int stmtNo1 = prevTList[i];
			if(contains(list1,stmtNo1)){
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	} else if(arg2Type.compare("_")==0){
		// ***** what?
		for(unsigned int i=0;i<list1.size();i++){
			int stmtNo1 = list1[i];
			vector<int> nextTList = getNextT(stmtNo1);
			for(unsigned int j=0;j<nextTList.size();j++){
				int stmtNo2 = nextTList[j];
				pair<string,string>p(stmtNo1+"", stmtNo2+"");
				result.push_back(p);
				return true;
			}
		}
	}



	return false;
}


/************************************************** Affect -zy *************************************************/
vector<int> PKB::getAffectList(int stmtNo)
{
	string var = "";
	vector<int> result;
	return result;
}
// not used
bool PKB::isAffect(int stmtNo1, int stmtNo2)
{
	int currentLine = stmtNo1;
	// this is wrong !!!!!!!!************
	int modifiedVarIndex = getModifiedStmt(stmtNo1)[0];

	vector<int> modifiedStmts = getUsedList(modifiedVarIndex,"assign");
	//cout<<"var "<<getVarName(modifiedVarIndex)<<endl;
	if(!contains(modifiedStmts,stmtNo2)){  // used in stmtNo2
		//for(int i=0;i<modifiedStmts.size();i++)
			//cout<<"!! mofie "<<modifiedStmts[i]<<"  "<<endl;
		return false;
	}
	if(!isNextT(stmtNo1,stmtNo2)){    // next*
		//cout<<"!! next* "<<endl;
		return false;
	}
	bool modifiedInBetween = false;
	visited.clear();
	vector<int> nextList = getNext(stmtNo1);
	for(unsigned int i=0;i<nextList.size();i++){
		int nextStmt=nextList[i];
		modifiedInBetween = isMofiedBetween(modifiedVarIndex,nextStmt,stmtNo2);
		//cout<<"?? "<<modifiedInBetween<<endl;
		if(!modifiedInBetween)
			return true;
	}
	
	return false;
}
// not used
bool PKB::isMofiedBetween(int modifiedVarIndex,int currentLine,int target)
{
	if(currentLine==target)
		return false;
	bool result=false;
	vector<int> nextList = getNext(currentLine);
	for(unsigned int i=0;i<nextList.size();i++){
		int nextStmtNo = nextList[i];
		if(nextStmtNo==target){
			return false;
		}
		if(nextStmtNo<=0) continue;
		// add size in case over flow
		while(visited.size()<=(unsigned int)nextStmtNo){
			visited.push_back(0);
		}
		// loop detection
		if(visited[nextStmtNo]==0){
			visited[nextStmtNo]=1;
		}else continue;// didnt go to the target path

		string nextStmtType = getStmtType(nextStmtNo);
		if(nextStmtType.compare("assign")==0){
			int varIndex = getModifiedStmt(nextStmtNo)[0];
			if(varIndex==modifiedVarIndex) // modified
				continue;
			bool re = isMofiedBetween(modifiedVarIndex,nextStmtNo,target);
			if(!re) return false;
		}else if(nextStmtType.compare("call")==0){
			//******* wait API, just check if this call modifies var or not
			//vector <int> varList = getModifiedProc();
		}else { // while or if
			bool re = isMofiedBetween(modifiedVarIndex,nextStmtNo,target);
			if(!re) return false;
		}
	}
	return true;
}

/************************************************** Flatten - Zhao Yang *************************************************/
void PKB::flattenAST()
{
	vector<TNode*> listOfRootNode = ast.getAST();
	getchar();
	
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

		/*cout<<endl;
		cout<<"type: "<<type<<endl;
		cout<<"lineNum: "<<lineNum<<endl;
		cout<<"varRef: "<<varRef<<endl;
		cout<<"postfixExpr: --"<<postfixExpr<<"--"<<endl;
		cout<<endl; */

		postfixNode* node = new postfixNode(type,lineNum,varRef,postfixExpr);

		postfixExprList[lineNum] = node;
	}

	getchar();
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
