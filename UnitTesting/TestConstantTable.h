#pragma once
#ifndef TestConstantTable_h
#define TestConstantTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "ConstantTable.h"

class TestConstantTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestConstantTable );
	CPPUNIT_TEST( testInsertConst );
	CPPUNIT_TEST_SUITE_END();

private:
	ConstantTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertConst();
};
#endif