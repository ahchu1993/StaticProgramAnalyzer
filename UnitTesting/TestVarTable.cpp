#include "TestVarTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestVarTable::setUp()
{
}

void TestVarTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestVarTable );

void TestVarTable::testInsertVar()
{
	int flag;
	string variable;

	// Test insert data
	flag = table.insertVar("x");
	CPPUNIT_ASSERT_EQUAL(0, flag);
	
	flag = table.insertVar("y");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.insertVar("x");
	CPPUNIT_ASSERT_EQUAL(0, flag);

	// Test getVarName() method
	variable = table.getVarName(0);
	CPPUNIT_ASSERT(variable.compare("x") == 0);

	variable = table.getVarName(2);
	CPPUNIT_ASSERT(variable.compare("") == 0);

	// Test getVarIndex() method
	flag = table.getVarIndex("y");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.getVarIndex("temp");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	// Test getSize() method
	flag = table.getSize();
	CPPUNIT_ASSERT_EQUAL(2, flag);
}