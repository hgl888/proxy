#ifndef _IGA_COMMON_H_
#define _IGA_COMMON_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// IGACommon.h : header file
//

typedef char  int8;
typedef short int16;
typedef int   int32;

typedef unsigned char  uint8;
typedef unsigned short uint16;
typedef unsigned int   uint32;

// дֻ�����ڴ�
void WriteMemory(LPCVOID lpAddress, LPCVOID lpData, uint32 nBytes);

// �麯�������

// ת�������麯����Ķ��󣬵�PIGAVOBJECT����
#define CPVO2PIVO(pAnyObj) ((PIGAVOBJECT)(LPVOID)(pAnyObj))

// ָ���麯�����ָ��
typedef uint32* IGAVFT;
// ָ����������ָ��
typedef void* IGAANYOBJECT;
// �����麯����Ľṹ��
typedef struct tagIGAVOBJECT {
	IGAVFT vft;
} IGAVOBJECT, *PIGAVOBJECT;

// ��ö�����麯������
int32 GetVFNumber(PIGAVOBJECT pVObject);

// �滻�麯����
// �� pObject ���麯���������� pVFT �С�
// iVftSize��pVFT �п��Դ�ŵĺ���ָ��ĸ���
int32 CopyVFT(IGAVFT pVFT, int32 iVFTSize, PIGAVOBJECT pVObject);

// ���ƣ� VOID ReplaceVFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject);
// ���ܣ� �滻�麯����
// ������ �� pTargetObject ���麯�����滻Ϊ pObject ���麯����
int32 ReplaceVFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject);

// ���ƣ� VOID ReplaceN_VFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject, int32 iVFTSize);
// ���ܣ� �滻�麯����
// ������ �� pTargetObject ���麯�����滻Ϊ pObject ���麯����
//	       iVFTSize �滻���麯������
int32 ReplaceN_VFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject, int32 iVFTSize);

#endif // #ifndef _IGA_COMMON_H_