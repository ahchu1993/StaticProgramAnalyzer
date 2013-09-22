#include "CFG.h"

CFG::CFG(){
	
}



vector<int> CFG::getNext(int stmtNo)
{
	vector<int> resultList;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> childrenList = currentNode->childList;
	for(int i=0;i<childrenList.size();i++)
		resultList.push_back(childrenList[i]->stmtNum);
	return resultList;
}

void CFG::buildCFGParentList(int stmtNo)
{
	while(visited.size()<=stmtNo)
		visited.push_back(0);
	if(visited[stmtNo]==1){
		return;
	}else visited[stmtNo]=1;
	CFGNode *currentNode = CFGNodes[stmtNo];
	for(int i=0;i<currentNode->childList.size();i++){
		CFGNode * child = currentNode->childList[i];
		child->addParent(currentNode);
		buildCFGParentList(child->stmtNum);
	}
}