#include "AST.h"


AST::AST(void)
{
}

AST::~AST(void)
{
}

TNode* AST::createRootNode(string type, int content, int lineNo)
{
	assert(type.compare("root") == 0 && (content == -1 || content == -2 || content == -3) && lineNo >= 0);

	TNode* temp_node;
	TNode* new_node = NULL;

	for (unsigned i = 0; i < Trees.size(); i++)
	{
		temp_node = Trees.at(i);
		if (AST::getLineNo(temp_node) == lineNo)
		{
			cout<< "This assignment has already been added.\n";
			return new_node;
		}
	}

	new_node =  new TNode(type, content, lineNo);
	Trees.push_back(new_node);
	return new_node;
}

TNode* AST::createNode(string type, int content, int lineNo)
{
	assert((type.compare("opt") == 0 || type.compare("var") == 0 || type.compare("const") == 0) && content >= -4 && lineNo >= 0);

	TNode* new_node =  new TNode(type, content, lineNo);
	return new_node;
}

void AST::makeLeftChild(TNode* parent, TNode* leftChild)
{
	assert(parent != NULL && leftChild != NULL);

	(*parent).makeLeftChild(leftChild);
}

void AST::makeRightChild(TNode* parent, TNode* rightChild)
{
	assert(parent != NULL && rightChild != NULL);

	(*parent).makeRightChild(rightChild);
}

TNode* AST::getLeftChild(TNode* parent)
{
	assert(parent != NULL);

	return (*parent).getLeftChild();
}

TNode* AST::getRightChild(TNode* parent)
{
	assert(parent != NULL);

	return (*parent).getRightChild();
}

int AST::getContent(TNode* node)
{
	assert(node != NULL);

	return (*node).getContent();
}

int AST::getLineNo(TNode* node)
{
	assert(node != NULL);

	return (*node).getLineNo();
}

string AST::getType(TNode* node)
{
	assert(node != NULL);

	return (*node).getType();
}

vector<TNode*> AST::getAST()
{
	return Trees;
}

void AST::printAST()
{
	TNode* temp_node;

	cout<< "AST:" << endl;
	cout<< "Format: " << "type" << "\t" << "content" << "\t" << "lineNo" << endl;
	for (unsigned i = 0; i < Trees.size(); i++)
	{
		cout << "AST " << i + 1 << "th:" << endl;
		temp_node = Trees.at(i);
		AST::printASTRecursive(temp_node);
	}
}

void AST::printASTRecursive(TNode* node)
{
	TNode* left;
	TNode* right;

	left = AST::getLeftChild(node);
	if (left != NULL)
	{
		AST::printASTRecursive(left);
	}

	cout << AST::getType(node) << "\t" << AST::getContent(node) << "\t" << AST::getLineNo(node) << endl;

	right = AST::getRightChild(node);
	if (right != NULL)
	{
		AST::printASTRecursive(right);
	}
}