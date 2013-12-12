#ifndef ParentTableTest_h
#define ParentTableTest_h

#include <cppunit/extensions/HelperMacros.h>
#include "ParentTable.h"
class TestParentTable : public CPPUNIT_NS::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( TestParentTable ); 
	CPPUNIT_TEST( testInsert );
	CPPUNIT_TEST_SUITE_END();

private:
	ParentTable table;
public:
	void setUp();
	void tearDown();

	void testInsert();

};
#endif
    