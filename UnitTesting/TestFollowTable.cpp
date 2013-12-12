#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

#include "TestFollowTable.h"

void 
	TestFollowTable::setUp()
{
}

void 
	TestFollowTable::tearDown()
{
	
}

// Registers the fixture into the 'registry'
CPPUNIT_TEST_SUITE_REGISTRATION( TestFollowTable );

void TestFollowTable::testInsert()
{
	table.insertFollow(1, "assign", 2, "if");
	table.insertFollow(2, "if", 3, "while");
	table.insertFollow(3, "while", 5, "while");

	CPPUNIT_ASSERT_EQUAL(2, table.findFollowed(1));
	CPPUNIT_ASSERT_EQUAL(3, table.findFollowed(2));
	CPPUNIT_ASSERT_EQUAL(5, table.findFollowed(3));
	CPPUNIT_ASSERT_EQUAL(-1, table.findFollowed(9));

	table.print();
	
	return;
}