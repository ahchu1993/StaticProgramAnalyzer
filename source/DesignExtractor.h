#pragma once
#include "PKB.h"
#include "CFGNode.h"
class DesignExtractor
{
public:
	DesignExtractor(PKB *pkb);
	void buildCFG();
	vector<CFGNode *> CFGHeaderList;
	vector<CFGNode *> CFGNodes;
	void printCFG();
private:
	PKB *pkb;
	void buildTree(int procIndex);
	CFGNode* buildLink(int stmtNum);
	int currentIndex;
	CFGNode* findNext(int stmtNo);
	void printfTree(CFGNode *node);
	vector<int> visited;
};
