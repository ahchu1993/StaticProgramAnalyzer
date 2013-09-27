#ifndef AST_H
#define AST_H
#include <vector>
#include "TNode.h"
#include <assert.h>
#include <iostream>

using namespace std;
class AST{

private:
	vector<TNode*> Trees;

	void printASTRecursive(TNode* node);

public:
	AST();
	~AST();

	// Create root node for the AST, type must be "root"
	// content is -1 for ASSIGN, -2 for IF, -3 for WHILE
	TNode* createRootNode(string type, int content, int lineNo);

	// Create normal node for the AST, type can be "opt", "var" or "const"
	// Type "opt", content is -1 for +, -2 for -, -3 for *, -4 for /
	// Type "var", content is varIndex
	// Type "const", content is constant value
	TNode* createNode(string type, int content, int lineNo);

	// Link parent and left child node
	void makeLeftChild(TNode* parent, TNode* leftChild);

	// Link parent and right child node
	void makeRightChild(TNode* parent, TNode* rightChild);

	// Get left child of the given node
	TNode* getLeftChild(TNode* parent);

	// Get right child of the given node
	TNode* getRightChild(TNode* parent);

	// Get content of the given node
	int getContent(TNode* node);

	// Get line number of the given node
	int getLineNo(TNode* node);

	// Get type of the given node
	string getType(TNode* node);

	// Get the ASTs
	vector<TNode*> getAST();

	// Print AST
	void printAST();
};
#endif
