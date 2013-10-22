#include "CFGNode.h"


CFGNode::CFGNode(int statementNumber)
{
	stmtNum = statementNumber;
	numOfChildren = 0;
	numOfParent = 0;
	numOfChildrenBIP=0;
	numOfParentBIP=0;
	callNode=0;
	calledNode=0;
}
void CFGNode::setCallNode(){
	callNode=1;
}
void CFGNode::setCalledNode(){
	calledNode=1;
}
bool CFGNode::isCallNode()
{
	if(callNode==1) return true;
	else return false;
}
bool CFGNode::isCalledNode()
{
	if(calledNode==1) return true;
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
void CFGNode::addChildBIP(CFGNode *node)
{
	childBIPList.push_back(node);
	numOfChildrenBIP++;
}
void CFGNode::addParentBIP(CFGNode *node)
{
	parentBIPList.push_back(node);
	numOfParentBIP++;
}



