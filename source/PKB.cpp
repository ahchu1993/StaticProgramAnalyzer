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
vector<Pair> PKB::getCall(string arg1, string arg1Type, string arg2, string arg2Type){	
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
	vector<Pair> result = callTable.getCallPairList(set1, set2);

	for(unsigned i=0; i<result.size(); i++){
		stringstream ss1,ss2;
		string proc1 = result.at(i).getFirst();
		string proc2 = result.at(i).getSecond();
		int proc1 =	procTable.getProcIndex(proc1);
		int proc2 =	procTable.getProcIndex(proc2);
		ss1 << proc1;
		ss2 << proc2;
		proc1 = ss1.str();
		proc2 = ss2.str();
		result.at(i).setFirst(proc1);
		result.at(i).setSecond(proc2);
	}
	return result;
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
vector<Pair> PKB::getParent(string arg1, string arg1Type, string arg2, string arg2Type){
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
vector<Pair> PKB::getModify(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = modifyTable.getModifyProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = modifyTable.getModifyStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = modifyTable.getModifyDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		// Waiting for confirmation
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = modifyTable.getModifyVarList();
	} else if (arg2Type.compare("string") == 0){
		// Waiting for confirmation
	}

	return modifyTable.getModifyPairList(set1, set2);
}

bool PKB::checkModify(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = modifyTable.getModifyProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = modifyTable.getModifyStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = modifyTable.getModifyDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		// Waiting for confirmation
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = modifyTable.getModifyVarList();
	} else if (arg2Type.compare("string") == 0){
		// Waiting for confirmation
	}

	return modifyTable.checkModify(set1, set2);
}

void PKB::updateModify(){
	// Update ModifyTable using CallTable
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
vector<Pair> PKB::getUse(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = useTable.getUseProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = useTable.getUseStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = useTable.getUseDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		// Waiting for confirmation
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = useTable.getUseVarList();
	} else if (arg2Type.compare("string") == 0){
		// Waiting for confirmation
	}

	return useTable.getUsePairList(set1, set2);
}

bool PKB::checkUse(string arg1, string arg1Type, string arg2, string arg2Type){
	vector<int> set1;
	vector<int> set2;

	// Get the set of possible values for argument 1
	if (arg1Type.compare("procedure") == 0){
		set1 = useTable.getUseProcList();
	} else if (arg1Type.compare("stmt") == 0 || arg1Type.compare("prog_line") == 0){
		set1 = useTable.getUseStmtList();
	} else if (arg1Type.compare("assign") == 0 || arg1Type.compare("if") == 0 || arg1Type.compare("while") == 0){
		set1 = useTable.getUseDEList(arg1Type);
	} else if (arg1Type.compare("string") == 0){
		// Waiting for confirmation
	} else if (arg1Type.compare("integer") == 0){
		int stmtNo;
		istringstream(arg1)>>stmtNo;
		set1.push_back(stmtNo);
	}

	// Get the set of possible values for argument 1
	if (arg2Type.compare("variable") == 0 || arg2Type.compare("_") == 0){
		set2 = useTable.getUseVarList();
	} else if (arg2Type.compare("string") == 0){
		// Waiting for confirmation
	}

	return useTable.checkUse(set1, set2);
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
	for(int i=0;i<cfg.CFGHeaderList.size();i++)
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
		for(int i=0;i<childrenList.size()-1;i++){
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
	for(int i=0;i<visited.size();i++){
		visited[i]=0;
	}
	cout<<" CFGList size "<<cfg.CFGHeaderList.size()<<endl;
	for(int i=0;i<cfg.CFGHeaderList.size();i++){
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
	for(int i=0;i<node->childList.size();i++)
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
	vector<CFGNode *> childrenList = cfg.CFGNodes[stmtNo1]->childList;
	for(int i=0;i<childrenList.size();i++){
		CFGNode *currentNode = childrenList[i];
		int currentStmt = currentNode->stmtNum;
		if(currentStmt==stmtNo2){
			return true;
		}
	}
	return false;
}
