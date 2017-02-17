// PlusTestCase.h: interface for the CPlusTestCase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_)
#define AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <cppunit/TestCase.h>
#include <CppUnit/extensions/HelperMacros.h>
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/TestAssert.h>

class CHandleClientEventTestCase : public CppUnit::TestCase  
{
	//ͨ���⼸���꣬���ǾͰ�CPlusTestCase��testAddע�ᵽ�˲����б���.
	CPPUNIT_TEST_SUITE(CHandleClientEventTestCase);		//����һ�����԰�
    CPPUNIT_TEST(testAdd);					//����һ����������
	CPPUNIT_TEST(testHandleClientEvent);
	CPPUNIT_TEST(testCreateXml);
	CPPUNIT_TEST(testReadXML);
	CPPUNIT_TEST(testXmlLoad);
	CPPUNIT_TEST(testGetTodayDate);
	CPPUNIT_TEST(testGetYesterDayDate);
    CPPUNIT_TEST_SUITE_END();	
public:
	CHandleClientEventTestCase();
	virtual ~CHandleClientEventTestCase();
	void testAdd(); //���Է���
	void testHandleClientEvent();
	void testCreateXml();
	void testReadXML();
	void testXmlLoad();
	void testGetTodayDate();
	void testGetYesterDayDate();

	static std::string GetSuiteName();

	//дһ��ע�ắ��, ʹ��������������һ��Test
	static CppUnit::Test* GetSuite();
};

#endif // !defined(AFX_PLUSTESTCASE_H__8BB9FA0E_349D_4DA7_A3E7_C313B81C2528__INCLUDED_)
