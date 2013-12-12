#pragma once
#ifndef TestCallTable_h
#define TestCallTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "CallTable.h"

class TestCallTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestCallTable );
	CPPUNIT_TEST( testInsertCall );
	CPPUNIT_TEST_SUITE_END();

private:
	CallTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertCall();
};
#endif

