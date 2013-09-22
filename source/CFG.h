#ifndef CFG_H
#define CFG_H
#include "CFGNode.h"
class CFG
{
public:
	CFG(void);
	vector<CFGNode *> CFGHeaderList;//root nood
	vector<CFGNode *> CFGNodes;	

	vector<int> getNext(int stmtNo);

	void buildCFGParentList(int stmtNo);
	vector<int> visited;
};

#endif