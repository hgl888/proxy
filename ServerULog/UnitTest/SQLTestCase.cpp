#include "stdafx.h"
#include "SQLTestCase.h"
#include "CSQLConnection.h"

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CSQLTestCase, CSQLTestCase::GetSuiteName());

CSQLTestCase::CSQLTestCase(void)
{
}

CSQLTestCase::~CSQLTestCase(void)
{
}

void CSQLTestCase::testSqlConntion()
{

}


std::string CSQLTestCase::GetSuiteName()
{
	//CSQLConnection	  SQLOperate;
	//SQLOperate.Connect("serverULog","192.168.1.72","serveru","serveru");

	return "CSQLTest";
}

/*
*	ע�⣺CPlusTestCase::GetSuite()����һ��ָ��CppUnit::Test��ָ��.
*  ���ָ������������Ե���㡣
*  CppUnit::TestFactoryRegistry::getRegistry()����TestSuite�����ַ���TestFactoryRegistry����,
*  Ȼ����ù������makeTest()��TestSuite������װ����������һ����״�Ĳ��Խṹ��
*/

CppUnit::Test* CSQLTestCase::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CSQLTestCase::GetSuiteName());
	return reg.makeTest();
}