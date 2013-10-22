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

	// for normail next
	vector<int> getNext(int stmtNo);
	vector<int> getPrev(int stmtNo);
	vector<int> getNextStar(int stmtNo);
	vector<int> getPrevStar(int stmtNo);
	bool isNext(int stmtNo1, int stmtNo2);
	bool isNextStar(int stmtNo1,int stmtNo2);
	void buildCFGParentList(int stmtNo);

	
	// for next BIP  
	vector<int> getNextBIP(int stmtNo);
	vector<int> getPrevBIP(int stmtNo);
	vector<int> getNextStarBIP(int stmtNo);
	vector<int> getPrevStarBIP(int stmtNo);
	bool isNextBIP(int stmtNo1, int stmtNo2);
	bool isNextStarBIP(int stmtNo1,int stmtNo2);
	//void buildCFGParentListBIP(int stmtNo);

	vector<int> visited;

private:
	vector<int> resultList;
	// normail next
	void getNextStarRecursive(int stmtNo);
	void getPrevStarRecursive(int stmtNo);

	// next BIP
	void getNextStarBIPRecursive(int stmtNo);
	void getPrevStarBIPRecursive(int stmtNo);

	bool contains(vector<int> list, int stmtNo);
};

#endif