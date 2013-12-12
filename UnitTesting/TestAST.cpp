#include "TestAST.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestAST::setUp()
{
}

void TestAST::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestAST );

void TestAST::testCreateNode()
{
	TNode* new_node;
	TNode* new_node_2;
	TNode* new_node_3;
	TNode* new_node_4;
	TNode* new_node_5;
	TNode* new_node_6;
	vector<TNode*> nodes;

	// Test create nodes
	new_node = ast.createRootNode("root", -1, 10);
	CPPUNIT_ASSERT_EQUAL(-1, ast.getContent(new_node));
	CPPUNIT_ASSERT_EQUAL(10, ast.getLineNo(new_node));
	CPPUNIT_ASSERT(ast.getType(new_node).compare("root") == 0);
	CPPUNIT_ASSERT(ast.getLeftChild(new_node) == NULL);
	CPPUNIT_ASSERT(ast.getRightChild(new_node) == NULL);

	new_node_2 = ast.createNode("var", 7, 10);
	CPPUNIT_ASSERT_EQUAL(7, ast.getContent(new_node_2));
	CPPUNIT_ASSERT_EQUAL(10, ast.getLineNo(new_node_2));
	CPPUNIT_ASSERT(ast.getType(new_node_2).compare("var") == 0);
	CPPUNIT_ASSERT(ast.getLeftChild(new_node_2) == NULL);
	CPPUNIT_ASSERT(ast.getRightChild(new_node_2) == NULL);

	// Test create wrong node
	new_node_3 = ast.createRootNode("root", -1, 10);
	CPPUNIT_ASSERT(new_node_3 == NULL);

	new_node_4 = ast.createNode("opt", -1, 10);
	CPPUNIT_ASSERT_EQUAL(-1, ast.getContent(new_node_4));
	CPPUNIT_ASSERT_EQUAL(10, ast.getLineNo(new_node_4));

	// Test make children
	ast.makeLeftChild(new_node, new_node_2);
	ast.makeRightChild(new_node, new_node_4);

	new_node_5 = ast.getLeftChild(new_node);
	CPPUNIT_ASSERT_EQUAL(7, ast.getContent(new_node_5));
	CPPUNIT_ASSERT_EQUAL(10, ast.getLineNo(new_node_5));

	new_node_6 = ast.getRightChild(new_node);
	CPPUNIT_ASSERT_EQUAL(-1, ast.getContent(new_node_6));
	CPPUNIT_ASSERT_EQUAL(10, ast.getLineNo(new_node_6));

	// Test getAST() method
	nodes = ast.getAST();
	CPPUNIT_ASSERT_EQUAL((unsigned)1, nodes.size());

	ast.createRootNode("root", -1, 11);
	nodes = ast.getAST();
	CPPUNIT_ASSERT_EQUAL((unsigned)2, nodes.size());
}