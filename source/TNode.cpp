#include "TNode.h"

TNode::TNode(string type, int content, int line){
	TNode::type = type;
	TNode::content = content;
	TNode::lineNo = line;
	TNode::leftChild = NULL;
	TNode::rightChild = NULL;
}

void TNode::makeLeftChild(TNode* leftChild){
	TNode::leftChild = leftChild;
}

void TNode::makeRightChild(TNode* rightChild){
	TNode::rightChild = rightChild;
}

TNode* TNode::getLeftChild(){
	return TNode::leftChild;
}

TNode* TNode::getRightChild(){
	return TNode::rightChild;
}

int TNode::getContent()
{
	return TNode::content;
}

int TNode::getLineNo()
{
	return TNode::lineNo;
}

string TNode::getType()
{
	return TNode::type;
}