#pragma once
#ifndef TestAST_h
#define TestAST_h
#include <cppunit/extensions/HelperMacros.h>
#include "AST.h"

class TestAST : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE( TestAST );
	CPPUNIT_TEST( testCreateNode );
	CPPUNIT_TEST_SUITE_END();

private:
	AST ast;

public:
	void setUp();
	void tearDown();
	
	void testCreateNode();
};
#endif

