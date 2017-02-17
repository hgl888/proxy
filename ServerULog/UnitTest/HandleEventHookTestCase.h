// MinusTestCase.h: interface for the CMinusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_)
#define AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CHandleEventHookTestCase : public CppUnit::TestCase
{
	//ͨ���⼸���꣬���ǾͰ�CMinusTestCase��testMinusע�ᵽ�˲����б���.
	CPPUNIT_TEST_SUITE(CHandleEventHookTestCase);			//����һ�����԰�
    CPPUNIT_TEST(testMinus);					//����һ����������
	CPPUNIT_TEST(testHandleEventHook);
    CPPUNIT_TEST_SUITE_END();	
public:
	CHandleEventHookTestCase();
	virtual ~CHandleEventHookTestCase();

	void testMinus(); //���Է���
	void testHandleEventHook();
	

	static std::string GetSuiteName();
	//дһ��ע�ắ��, ʹ��������������һ��Test
	static CppUnit::Test* GetSuite();
};

#endif // !defined(AFX_MINUSTESTCASE_H__B81DD1E4_36C5_4BF6_94E2_7527340AE2A2__INCLUDED_)
