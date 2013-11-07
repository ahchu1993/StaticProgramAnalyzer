#include "CFGNode.h"
#include <iostream>
#include <cstdio>
CFGNode::CFGNode(int statementNumber)
{
	stmtNum = statementNumber;
	numOfChildren = 0;
	numOfParent = 0;
	numOfChildrenBip=0;
	numOfParentBip=0;
	callNode=0;
	calledNode=0;
	thisProcedure=-1;
}
void CFGNode::setProcedure(int proc)
{
	thisProcedure=proc;
}
int CFGNode::getProcedure()
{
	return thisProcedure;
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
void CFGNode::addChildBip(CFGNode *node)
{
	childBipList.push_back(node);
	numOfChildrenBip++;
}
void CFGNode::addParentBip(CFGNode *node)
{
	parentBipList.push_back(node);
	numOfParentBip++;
}



