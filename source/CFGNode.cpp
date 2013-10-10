#include "CFGNode.h"


CFGNode::CFGNode(int statementNumber)
{
	stmtNum = statementNumber;
	numOfChildren = 0;
	numOfParent = 0;
	callNode=0;
}
void CFGNode::setCallNode(){
	callNode=1;
}
bool CFGNode::isCallNode()
{
	if(callNode==1) return true;
	else return false;
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


