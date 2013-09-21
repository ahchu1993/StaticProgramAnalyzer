#pragma once

#include<stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm> 
#include "DesignExtractor.h"
using namespace std;

DesignExtractor::DesignExtractor (PKB *p) 
{
//	int findFollowed(int stm);
//	vector<int> findFollowedT(int stmt, string DE);
//	int findFollows(int stm);
//	vector<int> findFollowsT(int stmt, string DE);
//	bool isFollowed(int stm1,int stm2);
//	void printFollowTable();
	pkb=p;
	currentIndex=1;
	for(int i=0;i<pkb->getSizeStmtTable()+1;i++)
	{
		CFGNode * newNode = new CFGNode(i); // 0 is dummy
		CFGNodes.push_back(newNode);
		
		visited.push_back(0);
	}
	cout<<"??????"<<endl;
}

void DesignExtractor::buildCFG()
{
	cout<<"size proc table "<<pkb->getSizeProcTable()<<endl;
	int procNum = pkb->getSizeProcTable();
	for(int i=0;i<procNum;i++){
		buildTree(i);
		getchar();
	}
}

void DesignExtractor::buildTree(int procIndex)
{
	cout<<"current index "<<currentIndex<<endl;
	CFGHeaderList.push_back(CFGNodes[currentIndex]);
	buildLink(currentIndex);
}
// top down approach
CFGNode* DesignExtractor::buildLink(int stmtNo)
{
	if(visited[stmtNo]==0)
		visited[stmtNo]=1;
	else return CFGNodes[stmtNo];
	currentIndex++;
	string stmtType =  pkb->getStmtType(stmtNo);

	cout<<"stmtType: "<<stmtType<<"  "<<stmtNo<<endl;
	//cout<<"followed: "<<pkb->findFollowed(1)<<endl; // find after
	//cout<<"follows: "<<pkb->findFollows(1)<<endl;  // find prev

	if(stmtType.compare("while")==0){

		CFGNode *whileNode=CFGNodes[stmtNo];

		whileNode->addChild(buildLink(stmtNo+1)); // must be his child

		int followedIndex = pkb->findFollowed(stmtNo);
		if(followedIndex>0){
			whileNode->addChild(buildLink(followedIndex));
		}else whileNode->addChild(findNext(stmtNo));

		return whileNode;
	}else if(stmtType.compare("if")==0){
		CFGNode *ifNode=CFGNodes[stmtNo];

		vector<int> childrenList = pkb->getChildren(stmtNo,"stmt"); // *** why?
		cout<<"child "<<childrenList.size()<<"  stmtNo: "<<stmtNo<<endl;
		
		// find else stmtList
		int afterElseStmtNo=0;
		std::sort (childrenList.begin(), childrenList.end());
		for(int i=0;i<childrenList.size()-1;i++){
			int index1=childrenList[i];
			int index2=childrenList[i+1];
			if(!pkb->isFollowed(index1,index2)){
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
		
		CFGNode *node=CFGNodes[stmtNo];

		int afterStmtNo = stmtNo+1;
		if(pkb->isFollowed(stmtNo,afterStmtNo)){
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

CFGNode* DesignExtractor::findNext(int stmtNo)
{
	int parentIndex = pkb->getParent(stmtNo);	
	if(parentIndex<=0){
		return CFGNodes[0];
	}
	else{
		string parentType = pkb->getStmtType(parentIndex);
		if(parentType.compare("while")==0){
			return CFGNodes[parentIndex];
		}else if(parentType.compare("if")==0){
			int afterParentNo = pkb->findFollowed(parentIndex);
			if(afterParentNo>0){ // exits
				return buildLink(afterParentNo);  // may be duplicated
			}else {
				return findNext(parentIndex);
			}
		}
	}

	return CFGNodes[0];
}

void DesignExtractor::printCFG()
{
	for(int i=0;i<visited.size();i++){
		visited[i]=0;
	}
	cout<<" CFGList size "<<CFGHeaderList.size()<<endl;
	for(int i=0;i<CFGHeaderList.size();i++){
		cout<<" procedure "<<(i+1)<<endl;
		CFGNode* rootNode = CFGHeaderList[i];
		printfTree(rootNode);
	}
	getchar();
	cout<<"CFG END"<<endl;
	getchar();
}
void DesignExtractor::printfTree(CFGNode *node)
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