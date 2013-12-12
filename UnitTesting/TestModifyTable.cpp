#include "TestModifyTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestModifyTable::setUp()
{
}

void TestModifyTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestModifyTable );

void TestModifyTable::testInsertModifyStmt()
{
	int flag;
/*
	// Test insert wrong data
	flag = table.insertModifyStmt(30, 2, "ERROR");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertModifyStmt(30, -2, "ASSIGN");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertModifyStmt(-30, 2, "IF");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertModifyStmt(-30, -2, "CALL");
	CPPUNIT_ASSERT_EQUAL(-1, flag);
*/
	// Test insert data
	flag = table.insertModifyStmt(30, 2, "assign");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	vector<int> ans = table.getModifiedStmt(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));

	flag = table.insertModifyStmt(34, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	flag = table.insertModifyStmt(34, 4, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getModifiedStmt(34);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(4, ans.at(1));

	// Test insert same data
	flag = table.insertModifyStmt(30, 2, "while");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertModifyStmt(30, 2, "assign");
	CPPUNIT_ASSERT_EQUAL(-1, flag);
	
	flag = table.insertModifyStmt(30, 3, "assign");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertModifyStmt(34, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	
	// Test getModifiedList() method
	flag = table.insertModifyStmt(35, 3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.insertModifyStmt(37, 3, "call");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	ans = table.getModifiedList(3, "if");
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(35, ans.at(1));

	ans = table.getModifiedList(3, "stmt");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(37, ans.at(2));

	ans = table.getModifiedList(3, "prog_line");
	CPPUNIT_ASSERT_EQUAL((unsigned)3, ans.size());
	CPPUNIT_ASSERT_EQUAL(37, ans.at(2));
}

void TestModifyTable::testInsertModifyProc()
{
	int flag;
	
	// Test insert data
	flag = table.insertModifyProc(30, 2);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	vector<int> ans = table.getModifiedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)1, ans.size());
	CPPUNIT_ASSERT_EQUAL(2, ans.at(0));

	flag = table.insertModifyProc(30, 5);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getModifiedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(5, ans.at(1));

	// Test insert same data
	flag = table.insertModifyProc(30, 2);
	CPPUNIT_ASSERT_EQUAL(1, flag);
	ans = table.getModifiedProc(30);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
}