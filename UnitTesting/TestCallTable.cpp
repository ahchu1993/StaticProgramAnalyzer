#include "TestCallTable.h"

#include <cppunit/config/SourcePrefix.h>

#include <iostream>
#include <string>

void TestCallTable::setUp()
{
}

void TestCallTable::tearDown()
{
}

CPPUNIT_TEST_SUITE_REGISTRATION( TestCallTable );

void TestCallTable::testInsertCall()
{
	table.insert(1, "First", "Second");
	table.insert(2, "First", "Third");
	table.insert(14, "Second", "Fourth");
	table.insert(55, "Second", "Third");

	table.print();
}