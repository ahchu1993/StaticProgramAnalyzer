#include "TestStmtTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestStmtTable::setUp()
{
}

void TestStmtTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestStmtTable );

void TestStmtTable::testInsertStmt()
{
	int flag;
	string type;
	vector<int> stmtList;

	// Test insert data
	flag = table.insertStmt(1, "assign");
	CPPUNIT_ASSERT_EQUAL(1, flag);
	
	flag = table.insertStmt(1, "assign");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertStmt(1, "if");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	flag = table.insertStmt(2, "assign");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.insertStmt(3, "if");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	// Test getStmtType() method
	type = table.getStmtType(1);
	CPPUNIT_ASSERT(type.compare("assign") == 0);

	type = table.getStmtType(3);
	CPPUNIT_ASSERT(type.compare("if") == 0);

	// Test getStmtNo() method
	stmtList = table.getStmtNo("assign");
	CPPUNIT_ASSERT(stmtList.size() == 2);
	CPPUNIT_ASSERT_EQUAL(1, stmtList.at(0));
	CPPUNIT_ASSERT_EQUAL(2, stmtList.at(1));

	stmtList = table.getStmtNo("if");
	CPPUNIT_ASSERT(stmtList.size() == 1);
	CPPUNIT_ASSERT_EQUAL(3, stmtList.at(0));

	stmtList = table.getStmtNo("stmt");
	CPPUNIT_ASSERT(stmtList.size() == 3);

	stmtList = table.getStmtNo("prog_line");
	CPPUNIT_ASSERT(stmtList.size() == 3);

	// Test getSize() method
	flag = table.getSize();
	CPPUNIT_ASSERT_EQUAL(3, flag);
}