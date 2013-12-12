#pragma once
#ifndef TestPKBParser_h
#define TestPKBParser_h
#include <cppunit/extensions/HelperMacros.h>

#include "PKB.h"
#include "Parser.h"

class TestPKBParser : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestPKBParser );
	CPPUNIT_TEST( testParseAndGet );
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();
	
	void testParseAndGet();
};
#endif