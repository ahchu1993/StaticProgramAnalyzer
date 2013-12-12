#pragma once
#ifndef TestVarTable_h
#define TestVarTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "VarTable.h"

class TestVarTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestVarTable );
	CPPUNIT_TEST( testInsertVar );
	CPPUNIT_TEST_SUITE_END();

private:
	VarTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertVar();
};
#endif

