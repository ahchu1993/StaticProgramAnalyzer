#pragma once
#ifndef TestModifyTable_h
#define TestModifyTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "ModifyTable.h"

class TestModifyTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestModifyTable );
	CPPUNIT_TEST( testInsertModifyStmt );
	CPPUNIT_TEST( testInsertModifyProc );
	CPPUNIT_TEST_SUITE_END();

private:
	ModifyTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertModifyStmt();
	void testInsertModifyProc();
};
#endif

