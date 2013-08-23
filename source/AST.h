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

	// Create root node for the AST, type must be "ROOT"
	// content is -1
	TNode* createRootNode(string type, int content, int lineNo);

	// FOR CS3202, USE -2 -3 -4 -5 FOR OPERATOR
	// Create normal node for the AST, type can be "OPT", "VAR" or "CONST"
	// content is -1 for "OPT", varIndex for "VAR" and constValue for "CONST"
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
