#include "CFG.h"

CFG::CFG(){
	
}



vector<int> CFG::getNext(int stmtNo)
{
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> childrenList = currentNode->childList;
	for(unsigned int i=0;i<childrenList.size();i++)
		results.push_back(childrenList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}

vector<int> CFG::getPrev(int stmtNo){
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> parentList = currentNode->parentList;
	for(unsigned int i=0;i<parentList.size();i++)
		results.push_back(parentList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}

bool CFG::isNext(int stmtNo1, int stmtNo2)
{
	vector <int> childrenList = getNext(stmtNo1);
	for(unsigned int i=0;i<childrenList.size();i++)
	{
		if(childrenList[i]==stmtNo2)
			return true;
	}
	return false;
}
bool CFG::isNextStar(int stmtNo1,int stmtNo2)
{
	vector<int> nextStarList = getNextStar(stmtNo1);
	if(contains(nextStarList,stmtNo2))
		return true;
	return false;
}
vector<int> CFG::getNextStar(int stmtNo)
{
	for(unsigned int i=0;i<visited.size();i++)
		visited[i]=0;
	resultList.clear();
	getNextStarRecursive(stmtNo);
	sort (resultList.begin(), resultList.end());
	
	return resultList;
}
vector<int> CFG::getPrevStar(int stmtNo)
{
	for(unsigned int i=0;i<visited.size();i++)
		visited[i]=0;
	resultList.clear();
	getPrevStarRecursive(stmtNo);
	sort (resultList.begin(), resultList.end());
	return resultList;
}
void CFG::getPrevStarRecursive(int stmtNo)
{
	vector<int> prevList = getPrev(stmtNo);
	for(unsigned int i=0;i<prevList.size();i++){
		int prevStmtNo = prevList[i];

		// add size in case over flow
		while(visited.size()<=(unsigned int)prevStmtNo){
			visited.push_back(0);
		}
		// loop detection
		if(visited[prevStmtNo]==0){
			visited[prevStmtNo]=1;
		}else return;
		
		resultList.push_back(prevStmtNo);
		getPrevStarRecursive(prevStmtNo);
	}
}
void CFG::getNextStarRecursive(int stmtNo)
{
	//cout<<"stmtNo "<<stmtNo<<endl;
	vector<int> childrenList = getNext(stmtNo);

	for(unsigned int i=0;i<childrenList.size();i++){
		int childStmtNo = childrenList[i];
		// add size in case over flow
		while(visited.size()<=(unsigned int)childStmtNo){
			visited.push_back(0);
		}
		// loop detection
		if(visited[childStmtNo]==0){
			visited[childStmtNo]=1;
		}else{
			continue;
		}
		resultList.push_back(childStmtNo);
		getNextStarRecursive(childStmtNo);
	}
}
void CFG::buildCFGParentList(int stmtNo)
{
	while((unsigned int)visited.size()<=(unsigned int)stmtNo)
		visited.push_back(0);
	if(visited[stmtNo]==1){
		return;
	}else visited[stmtNo]=1;
	CFGNode *currentNode = CFGNodes[stmtNo];
	for(unsigned int i=0;i<currentNode->childList.size();i++){
		CFGNode * child = currentNode->childList[i];
		child->addParent(currentNode);
		buildCFGParentList(child->stmtNum);
	}
	if(currentNode->isCallNode()){ // for BIP
		for(unsigned int i=0;i<currentNode->childBIPList.size();i++){
			CFGNode * child = currentNode->childBIPList[i];
			child->addParentBIP(currentNode);
			//buildCFGParentList(child->stmtNum);
		}
	}
}

bool CFG::contains(vector<int> list, int stmtNo)
{
	for(unsigned int i=0;i<list.size();i++)
		if(stmtNo==list[i])
			return true;
	return false;
}


vector<int> CFG::getNextBIP(int stmtNo)
{
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> childrenList;
	if(currentNode->isCallNode())
		childrenList= currentNode->childBIPList;
	else childrenList = currentNode->childList;

	for(unsigned int i=0;i<childrenList.size();i++)
		results.push_back(childrenList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}
vector<int> CFG::getPrevBIP(int stmtNo){
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> parentList;
	if(currentNode->isCalledNode())
		parentList= currentNode->parentList;
	else parentList = currentNode->parentBIPList;

	for(unsigned int i=0;i<parentList.size();i++)
		results.push_back(parentList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}
bool CFG::isNextBIP(int stmtNo1, int stmtNo2)
{
	vector <int> childrenList = getNextBIP(stmtNo1);
	for(unsigned int i=0;i<childrenList.size();i++)
	{
		if(childrenList[i]==stmtNo2)
			return true;
	}
	return false;
}
bool CFG::isNextStarBIP(int stmtNo1,int stmtNo2)
{
	vector<int> nextStarList = getNextStarBIP(stmtNo1);
	if(contains(nextStarList,stmtNo2))
		return true;
	return false;
}
vector<int> CFG::getNextStarBIP(int stmtNo)
{
	for(unsigned int i=0;i<visited.size();i++)
		visited[i]=0;
	resultList.clear();
	getNextStarBIPRecursive(stmtNo);
	sort (resultList.begin(), resultList.end());

	return resultList;
}
vector<int> CFG::getPrevStarBIP(int stmtNo)
{
	for(unsigned int i=0;i<visited.size();i++)
		visited[i]=0;
	resultList.clear();
	getPrevStarBIPRecursive(stmtNo);
	sort (resultList.begin(), resultList.end());
	return resultList;
}
void CFG::getPrevStarBIPRecursive(int stmtNo)
{
	vector<int> prevList = getPrevBIP(stmtNo);
	for(unsigned int i=0;i<prevList.size();i++){
		int prevStmtNo = prevList[i];

		// add size in case over flow
		while(visited.size()<=(unsigned int)prevStmtNo){
			visited.push_back(0);
		}
		// loop detection
		if(visited[prevStmtNo]==0){
			visited[prevStmtNo]=1;
		}else return;

		resultList.push_back(prevStmtNo);
		getPrevStarBIPRecursive(prevStmtNo);
	}
}
void CFG::getNextStarBIPRecursive(int stmtNo)
{
	vector<int> childrenList = getNextBIP(stmtNo);
	for(unsigned int i=0;i<childrenList.size();i++){
		int childStmtNo = childrenList[i];

		// add size in case over flow
		while(visited.size()<=(unsigned int)childStmtNo){
			visited.push_back(0);
		}
		// loop detection
		if(visited[childStmtNo]==0){
			visited[childStmtNo]=1;
		}else return;
		resultList.push_back(childStmtNo);
		getNextStarBIPRecursive(childStmtNo);
	}
}
// dont need, put it in buildCFGParentList
/*
void CFG::buildCFGParentListBIP(int stmtNo)
{
	while((unsigned int)visited.size()<=(unsigned int)stmtNo)
		visited.push_back(0);
	if(visited[stmtNo]==1){
		return;
	}else visited[stmtNo]=1;
	CFGNode *currentNode = CFGNodes[stmtNo];
	for(unsigned int i=0;i<currentNode->childBIPList.size();i++){
		CFGNode * child = currentNode->childList[i];
		child->addParentBIP(currentNode);
		buildCFGParentListBIP(child->stmtNum);
	}
}*/