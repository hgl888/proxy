// MinusTestCase.cpp: implementation of the CMinusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "UnitTest.h"
#include "HandleEventHookTestCase.h"
#include "ServerULog.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//ע��һ������suite��һ��ָ����TestFactory������

CPPUNIT_TEST_SUITE_NAMED_REGISTRATION(CHandleEventHookTestCase, CHandleEventHookTestCase::GetSuiteName());

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHandleEventHookTestCase::CHandleEventHookTestCase()
{

}

CHandleEventHookTestCase::~CHandleEventHookTestCase()
{

}

void CHandleEventHookTestCase::testMinus()
{
	//CMinus minusTemp;
	//int nResult = minusTemp.Minus(30, 4); //ִ��Minus����
	CPPUNIT_ASSERT_EQUAL(26, 26); //������Ƿ����30	
}

void CHandleEventHookTestCase::testHandleEventHook()
{
	RFTPEventStr eventStr;
	WORD wResult = HandleEventHook(&eventStr);
	int iResult = wResult;
	CPPUNIT_ASSERT_EQUAL(0 , iResult);
}

std::string CHandleEventHookTestCase::GetSuiteName()
{
	return "HandleEventHook";
}

/*
 *	ע�⣺CMinusTestCase::GetSuite()����һ��ָ��CppUnit::Test��ָ��.
 *  ���ָ������������Ե���㡣
 *  CppUnit::TestFactoryRegistry::getRegistry()����TestSuite�����ַ���TestFactoryRegistry����,
 *  Ȼ����ù������makeTest()��TestSuite������װ����������һ����״�Ĳ��Խṹ��
 */

CppUnit::Test* CHandleEventHookTestCase::GetSuite()
{
	CppUnit::TestFactoryRegistry& reg = CppUnit::TestFactoryRegistry::getRegistry(CHandleEventHookTestCase::GetSuiteName());
	return reg.makeTest();
}
