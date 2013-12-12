#pragma once
#ifndef TestUseTable_h
#define TestUseTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "UseTable.h"

class TestUseTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestUseTable );
	CPPUNIT_TEST( testInsertUseStmt );
	CPPUNIT_TEST( testInsertUseProc );
	CPPUNIT_TEST_SUITE_END();

private:
	UseTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertUseStmt();
	void testInsertUseProc();
};
#endif

