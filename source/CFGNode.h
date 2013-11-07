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

	void setProcedure(int proc);
	int getProcedure();
	int thisProcedure;

	// for inter-procedure
	int callNode;
	int calledNode;
	void setCalledNode();
	void setCallNode();
	bool isCallNode();
	bool isCalledNode();
	int numOfChildrenBip;
	int numOfParentBip;
	vector<CFGNode *> childBipList;
	vector<CFGNode *> parentBipList;
	void addChildBip(CFGNode *node);
	void addParentBip(CFGNode *node);
};

#endif