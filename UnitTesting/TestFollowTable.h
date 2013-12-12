#ifndef FollowTableTest_h
#define FollowTableTest_h

#include <cppunit/extensions/HelperMacros.h>
#include "FollowTable.h"
class TestFollowTable : public CPPUNIT_NS::TestFixture 
{ 
	CPPUNIT_TEST_SUITE( TestFollowTable ); 
	CPPUNIT_TEST( testInsert );
	CPPUNIT_TEST_SUITE_END();

private:
	FollowTable table;
public:
	void setUp();
	void tearDown();

	void testInsert();
};
#endif
    