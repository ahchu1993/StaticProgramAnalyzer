#ifndef CFGNODE_h
#define CFGNODE_h

#include <vector>
using namespace std;
class CFGNode
{
public:
	CFGNode(int statementNumber=0);
	void addChild(CFGNode *node);
	void addParent(CFGNode *node);
	int stmtNum; // 0 means it's not a cfg node.. just a linking node
	int numOfChildren;
	int numOfParent;
	vector<CFGNode *> childList;
	vector<CFGNode *> parentList;
	int callNode;
	void setCallNode();
	bool isCallNode();
};

#endif