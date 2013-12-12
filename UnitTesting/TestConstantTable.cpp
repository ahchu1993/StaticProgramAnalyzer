#include "TestConstantTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestConstantTable::setUp()
{
}

void TestConstantTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestConstantTable );

void TestConstantTable::testInsertConst()
{
	vector<int> ans;

	// Test insert data
	table.insertConst(1, 3);
	table.insertConst(1, 4);
	table.insertConst(1, 3);
	table.insertConst(2, 3);

	ans = table.getConst(1);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(3, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(4, ans.at(1));
	
	ans = table.getStmt(3);
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(2, ans.at(1));

	ans = table.getAllStmt();
	CPPUNIT_ASSERT_EQUAL((unsigned)2, ans.size());
	CPPUNIT_ASSERT_EQUAL(1, ans.at(0));
	CPPUNIT_ASSERT_EQUAL(2, ans.at(1));
}