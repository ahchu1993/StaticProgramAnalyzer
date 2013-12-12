#include "TestPKBParser.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestPKBParser::setUp()
{
}

void TestPKBParser::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestPKBParser );

void TestPKBParser::testParseAndGet()
{
	PKB* pkb = new PKB;
	Parser parser(pkb);

	string input="";
	ifstream infile;

	infile.open("E:\\NUS\\sau khi sang Sing\\sach\\CPP Workspace\\CS3202\\Debug\\IntegrationTestSource.txt");
	string temp;

	while(!infile.eof()){
		getline(infile,temp);
		input = input+" "+temp;
		
	}

	vector<int> ans;
	vector<TNode*> nodes;

	// Parser parses input
	parser.parseInput(input);

	// Test ProcTable
	CPPUNIT_ASSERT_EQUAL(0, (*pkb).getProcIndex("Example"));
	CPPUNIT_ASSERT((*pkb).getProcName(0).compare("Example") == 0);
	CPPUNIT_ASSERT_EQUAL(1, (*pkb).getSizeProcTable());
	
	// Test VarTable
	CPPUNIT_ASSERT_EQUAL(0, (*pkb).getVarIndex("x"));
	CPPUNIT_ASSERT_EQUAL(1, (*pkb).getVarIndex("z"));
	CPPUNIT_ASSERT((*pkb).getVarName(2).compare("i") == 0);
	CPPUNIT_ASSERT_EQUAL(4, (*pkb).getSizeVarTable());

	// Test StmtTable
	CPPUNIT_ASSERT((*pkb).getStmtType(1).compare("assign") == 0);
	ans = (*pkb).getStmtNo("assign");
	CPPUNIT_ASSERT_EQUAL(9, (*pkb).getSizeStmtTable());
	CPPUNIT_ASSERT_EQUAL(3, ans.at(2));

	// Test ConstantTable
	ans = (*pkb).getConst(5);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));

	ans = (*pkb).getConst(1);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));

	ans = (*pkb).getConst(8);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, ans.size());

	ans = (*pkb).getStmtForConst(1);
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(5, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(6, ans.at(1));
	CPPUNIT_ASSERT_EQUAL(9, ans.at(2));

	ans = (*pkb).getAllStmtForConst();
	CPPUNIT_ASSERT_EQUAL((unsigned)6, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(2, ans.at(1));
	CPPUNIT_ASSERT_EQUAL(3, ans.at(2));
	CPPUNIT_ASSERT_EQUAL(5, ans.at(3));
	CPPUNIT_ASSERT_EQUAL(6, ans.at(4));
	CPPUNIT_ASSERT_EQUAL(9, ans.at(5));

	// Test ModifyTable
	ans = (*pkb).getModifiedStmt(1);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(0, ans.at(0));

	ans = (*pkb).getModifiedStmt(4);
	CPPUNIT_ASSERT_EQUAL((unsigned)4, ans.size());
	CPPUNIT_ASSERT_EQUAL(0, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(3, ans.at(2));
	CPPUNIT_ASSERT_EQUAL(2, ans.at(3));

	ans = (*pkb).getModifiedProc(0);
	CPPUNIT_ASSERT_EQUAL((unsigned)4, ans.size());
	CPPUNIT_ASSERT_EQUAL(0, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(2, ans.at(2));

	ans = (*pkb).getModifiedList(0, "assign");
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(5, ans.at(1));

	ans = (*pkb).getModifiedList(0, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(5, ans.at(1));
	CPPUNIT_ASSERT_EQUAL(4, ans.at(2));

	ans = (*pkb).getModifiedList(2, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());

	// Test UseTable
	ans = (*pkb).getUsedStmt(1);
	CPPUNIT_ASSERT_EQUAL((unsigned)0, ans.size());

	ans = (*pkb).getUsedStmt(4);
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(0, ans.at(1));
	CPPUNIT_ASSERT_EQUAL(1, ans.at(2));

	ans = (*pkb).getUsedProc(0);
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(0, ans.at(1));
	CPPUNIT_ASSERT_EQUAL(1, ans.at(2));

	ans = (*pkb).getUsedList(0, "while");
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());

	ans = (*pkb).getUsedList(0, "assign");
	CPPUNIT_ASSERT_EQUAL((unsigned)4, ans.size());
	
	ans = (*pkb).getUsedList(0, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)5, ans.size());

	// Test AST
	nodes = (*pkb).getAST();
	CPPUNIT_ASSERT_EQUAL((unsigned)9, nodes.size());
	
	TNode* temp_node = nodes.at(7);
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).getContent(temp_node));
	CPPUNIT_ASSERT_EQUAL(8, (*pkb).getLineNo(temp_node));
	
	TNode* left_node = (*pkb).getLeftChild(temp_node);
	CPPUNIT_ASSERT_EQUAL(1, (*pkb).getContent(left_node));
	CPPUNIT_ASSERT_EQUAL(8, (*pkb).getLineNo(left_node));

	TNode* right_node = (*pkb).getRightChild(temp_node);
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).getContent(right_node));
	CPPUNIT_ASSERT((*pkb).getType(right_node).compare("opt") == 0);

	TNode* left_node_2 = (*pkb).getLeftChild(right_node);
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).getContent(left_node_2));

	TNode* left_node_3 = (*pkb).getLeftChild(left_node_2);
	CPPUNIT_ASSERT_EQUAL(1, (*pkb).getContent(left_node_3));

	// Test ParentTable
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).getParent(1));
	CPPUNIT_ASSERT_EQUAL(4, (*pkb).getParent(5));

	ans = (*pkb).getChildren(4, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)5, ans.size());

	ans = (*pkb).getParentT(9);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());

	ans = (*pkb).getChildrenT(4, "assign");
	CPPUNIT_ASSERT_EQUAL((unsigned)5, ans.size());

	// Test FollowTable
	CPPUNIT_ASSERT_EQUAL(2, (*pkb).findFollowed(1));
	CPPUNIT_ASSERT_EQUAL(4, (*pkb).findFollowed(3));
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).findFollowed(4));

	CPPUNIT_ASSERT_EQUAL(7, (*pkb).findFollows(8));
	CPPUNIT_ASSERT_EQUAL(-1, (*pkb).findFollows(1));

	CPPUNIT_ASSERT((*pkb).isFollowed(4, 5) == false);
	CPPUNIT_ASSERT((*pkb).isFollowed(5, 6) == true);
}