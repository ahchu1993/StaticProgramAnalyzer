#ifndef CFG_H
#define CFG_H
#include "CFGNode.h"
#include <algorithm>
#include <iostream>
class CFG
{
public:
	CFG(void);
	vector<CFGNode *> CFGHeaderList;//root nood
	vector<CFGNode *> CFGNodes;	

	vector<int> getNext(int stmtNo);
	vector<int> getPrev(int stmtNo);
	vector<int> getNextStar(int stmtNo);
	vector<int> getPrevStar(int stmtNo);
	bool isNext(int stmtNo1, int stmtNo2);
	bool isNextStar(int stmtNo1,int stmtNo2);
	void buildCFGParentList(int stmtNo);

	

	vector<int> visited;

private:
	vector<int> resultList;
	void getNextStarRecursive(int stmtNo);
	void getPrevStarRecursive(int stmtNo);
	bool contains(vector<int> list, int stmtNo);
};

#endif