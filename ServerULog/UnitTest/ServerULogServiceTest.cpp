#include "stdafx.h"
#include "ServerULogServiceTest.h"


CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(ServerULogServiceTest, ServerULogServiceTest::GetSuiteName());


ServerULogServiceTest::ServerULogServiceTest(void)
{
}

ServerULogServiceTest::~ServerULogServiceTest(void)
{
}

void ServerULogServiceTest::testFindGameIDAndFileName()
{

}

std::string ServerULogServiceTest::GetSuiteName()
{
	return "ServerULogServiceTest";
}

/*
*	ע�⣺CPlusTestCase::GetSuite()����һ��ָ��CppUnit::Test��ָ��.
*  ���ָ������������Ե���㡣
*  CppUnit::TestFactoryRegistry::getRegistry()����TestSuite�����ַ���TestFactoryRegistry����,
*  Ȼ����ù������makeTest()��TestSuite������װ����������һ����״�Ĳ��Խṹ��
*/

CppUnit::Test* ServerULogServiceTest::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(ServerULogServiceTest::GetSuiteName());
	return reg.makeTest();
}
