#include "CFGNode.h"


CFGNode::CFGNode(int statementNumber)
{
	stmtNum = statementNumber;
	numOfChildren = 0;
}

void CFGNode::addChild(CFGNode *node)
{
	childList.push_back(node);
	numOfChildren++;
}


