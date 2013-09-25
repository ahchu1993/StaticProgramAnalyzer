#include "CFGNode.h"


CFGNode::CFGNode(int statementNumber)
{
	stmtNum = statementNumber;
	numOfChildren = 0;
	numOfParent = 0;
}

void CFGNode::addChild(CFGNode *node)
{
	childList.push_back(node);
	numOfChildren++;
}
void CFGNode::addParent(CFGNode *node)
{
	parentList.push_back(node);
	numOfParent++;
}


