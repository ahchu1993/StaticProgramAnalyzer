#include "TestProcTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestProcTable::setUp()
{
}

void TestProcTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestProcTable );

void TestProcTable::testInsertProc()
{
	int flag;
	string proc;

	// Test insert data
	flag = table.insertProc("Alpha");
	CPPUNIT_ASSERT_EQUAL(0, flag);
	
	flag = table.insertProc("Beta");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.insertProc("Alpha");
	CPPUNIT_ASSERT_EQUAL(0, flag);

	// Test getProcName() method
	proc = table.getProcName(0);
	CPPUNIT_ASSERT(proc.compare("Alpha") == 0);

	proc = table.getProcName(2);
	CPPUNIT_ASSERT(proc.compare("") == 0);

	// Test getProcIndex() method
	flag = table.getProcIndex("Beta");
	CPPUNIT_ASSERT_EQUAL(1, flag);

	flag = table.getProcIndex("Gamma");
	CPPUNIT_ASSERT_EQUAL(-1, flag);

	// Test getSize() method
	flag = table.getSize();
	CPPUNIT_ASSERT_EQUAL(2, flag);
}