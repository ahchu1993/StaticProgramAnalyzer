#pragma once
#ifndef TestStmtTable_h
#define TestStmtTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "StmtTable.h"

class TestStmtTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestStmtTable );
	CPPUNIT_TEST( testInsertStmt );
	CPPUNIT_TEST_SUITE_END();

private:
	StmtTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertStmt();
};
#endif