#include "CFG.h"

CFG::CFG(){
	
}



vector<int> CFG::getNext(int stmtNo)
{
	resultList.clear();
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> childrenList = currentNode->childList;
	for(unsigned int i=0;i<childrenList.size();i++)
		resultList.push_back(childrenList[i]->stmtNum);
	sort (resultList.begin(), resultList.end());
	return resultList;
}

vector<int> CFG::getPrev(int stmtNo){
	resultList.clear();
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> parentList = currentNode->parentList;
	for(unsigned int i=0;i<parentList.size();i++)
		resultList.push_back(parentList[i]->stmtNum);
	sort (resultList.begin(), resultList.end());
	return resultList;
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
		}else return;

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
}

bool CFG::contains(vector<int> list, int stmtNo)
{
	for(unsigned int i=0;i<list.size();i++)
		if(stmtNo==list[i])
			return true;
	return false;
}

