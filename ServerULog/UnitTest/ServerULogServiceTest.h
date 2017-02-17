#pragma once

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class ServerULogServiceTest : public CppUnit::TestCase  
{
	CPPUNIT_TEST_SUITE(ServerULogServiceTest);		//����һ�����԰�
	CPPUNIT_TEST(testFindGameIDAndFileName);					//����һ����������

	CPPUNIT_TEST_SUITE_END();	
public:
	ServerULogServiceTest(void);
	virtual ~ServerULogServiceTest(void);

	void testFindGameIDAndFileName();

	static std::string GetSuiteName();
	static CppUnit::Test* GetSuite();
};
