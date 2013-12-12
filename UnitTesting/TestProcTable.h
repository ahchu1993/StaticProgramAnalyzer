#pragma once
#ifndef TestProcTable_h
#define TestProcTable_h
#include <cppunit/extensions/HelperMacros.h>
#include "ProcTable.h"

class TestProcTable : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestProcTable );
	CPPUNIT_TEST( testInsertProc );
	CPPUNIT_TEST_SUITE_END();

private:
	ProcTable table;

public:
	void setUp();
	void tearDown();
	
	void testInsertProc();
};
#endif

