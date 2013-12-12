 #include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

#include "TestParentTable.h"

void 
	TestParentTable::setUp()
{
}

void 
	TestParentTable::tearDown()
{
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestParentTable );

void TestParentTable::testInsert()
{  
	table.insert(1, "assign", 2, "if");
	table.insert(2, "if", 3, "while");
	table.insert(3, "while", 5, "while");
	table.insert(3, "while", 7, "call");

	CPPUNIT_ASSERT_EQUAL(1, table.getParent(2));
	CPPUNIT_ASSERT_EQUAL(2, table.getParent(3));
	CPPUNIT_ASSERT_EQUAL(3, table.getParent(5));
	CPPUNIT_ASSERT_EQUAL(3, table.getParent(7));
	CPPUNIT_ASSERT_EQUAL(-1, table.getParent(9));

	table.print();

	return;
}
