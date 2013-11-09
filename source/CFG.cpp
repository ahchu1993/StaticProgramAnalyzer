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
		if(!currentNode->isCallNode())
			child->addParentBip(currentNode);
		buildCFGParentList(child->stmtNum);
	}
	if(currentNode->isCallNode()){ // for Bip
		for(unsigned int i=0;i<currentNode->childBipList.size();i++){
			CFGNode * child = currentNode->childBipList[i];
			child->addParentBip(currentNode);
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


vector<int> CFG::getNextBip(int stmtNo)
{
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> childrenList;
	if(currentNode->childBipList.size()>0)
		childrenList= currentNode->childBipList;
	else if(currentNode->isCallNode())
		;
	else childrenList = currentNode->childList;

	for(unsigned int i=0;i<childrenList.size();i++)
		results.push_back(childrenList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}
vector<int> CFG::getPrevBip(int stmtNo){
	vector<int> results;
	CFGNode *currentNode = CFGNodes[stmtNo];
	vector<CFGNode *> parentList;
	if(currentNode->parentBipList.size()>0){
		parentList= currentNode->parentBipList;
	}else {
		parentList = currentNode->parentList;
	}
	for(unsigned int i=0;i<parentList.size();i++)
		results.push_back(parentList[i]->stmtNum);
	sort (results.begin(), results.end());
	return results;
}
bool CFG::isNextBip(int stmtNo1, int stmtNo2)
{
	vector <int> childrenList = getNextBip(stmtNo1);
	for(unsigned int i=0;i<childrenList.size();i++)
	{
		if(childrenList[i]==stmtNo2)
			return true;
	}
	return false;
}



	/*
	if(getNext(stmtNo).size()==0&&CFGNodes[stmtNo]->isCallNode()){ // last call
		vector<int> tem = getNextBip(stmtNo);
		for(int i=0;i<tem.size();i++)
			processNextStarProcedure(tem[i]);

	}
	cout<<"##"<<stmtNo<<endl;
	getchar();
	vector<int> childrenList = getNextBip(stmtNo);
	for(unsigned int i=0;i<childrenList.size();i++){
		int childStmtNo = childrenList[i];

		if(CFGNodes[childStmtNo]->childList.size()==0){// last stmt
			getNextStarBipRecursive(childStmtNo);
		}else if(CFGNodes[childStmtNo]->getProcedure()!=CFGNodes[stmtNo]->getProcedure()){
			// branch in
			processNextStarProcedure(childStmtNo);
			
			vector<int> nextChildList=getNext(stmtNo);
			for(int i=0;i<nextChildList.size();i++){
				cout<<"???  "<<stmtNo<<"  "<<nextChildList[i]<<endl;
				getNextStarBipRecursive(nextChildList[i]);
			}
		}else{
			getNextStarBipRecursive(childStmtNo);
		}
	}*/

// dont need, put it in buildCFGParentList
/*
void CFG::buildCFGParentListBip(int stmtNo)
{
	while((unsigned int)visited.size()<=(unsigned int)stmtNo)
		visited.push_back(0);
	if(visited[stmtNo]==1){
		return;
	}else visited[stmtNo]=1;
	CFGNode *currentNode = CFGNodes[stmtNo];
	for(unsigned int i=0;i<currentNode->childBipList.size();i++){
		CFGNode * child = currentNode->childList[i];
		child->addParentBip(currentNode);
		buildCFGParentListBip(child->stmtNum);
	}
}*/

vector<int> CFG::merge(vector<int> v1,vector<int> v2) // no duplicate
{
	v1.insert(v1.end(), v2.begin(), v2.end());
	std::sort(v1.begin(), v1.end());
	v1.erase(std::unique(v1.begin(), v1.end()), v1.end());
	return v1;
}