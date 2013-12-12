#include "TestUseTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestUseTable::setUp()
{
}

void TestUseTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestUseTable );

void TestUseTable::testInsertUseStmt()
{
	int flag;

	// Test insert data
	flag = table.insertUseStmt(30, 2, "assign");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	vector<int> ans = table.getUsedStmt(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));

	flag = table.insertUseStmt(34, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	flag = table.insertUseStmt(34, 4, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getUsedStmt(34);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(4, ans.at(1));

	// Test insert same data
	flag = table.insertUseStmt(30, 2, "while");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertUseStmt(34, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	
	// Test getUsedList() method
	flag = table.insertUseStmt(35, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.insertUseStmt(37, 3, "call");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	ans = table.getUsedList(3, "if");
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(35, ans.at(1));

	ans = table.getUsedList(3, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(37, ans.at(2));

	ans = table.getUsedList(3, "prog_line");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(37, ans.at(2));
}

void TestUseTable::testInsertUseProc()
{
	int flag;
	
	// Test insert data
	flag = table.insertUseProc(30, 2);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	vector<int> ans = table.getUsedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));

	flag = table.insertUseProc(30, 5);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getUsedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(5, ans.at(1));

	// Test insert same data
	flag = table.insertUseProc(30, 2);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getUsedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
}