// IGACommon.cpp : source file
//

#include "stdafx.h"
#include "IGACommon.h"

// дֻ�����ڴ�
void WriteMemory(LPCVOID lpAddress, LPCVOID lpData, uint32 nBytes)
{
	MEMORY_BASIC_INFORMATION mbi;
	::VirtualQuery(lpAddress, &mbi, sizeof(MEMORY_BASIC_INFORMATION));  
	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect);

	::CopyMemory(const_cast<void*>(lpAddress), lpData, nBytes);
	
	DWORD dwOldProtect;
	::VirtualProtect(mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &dwOldProtect);
}

// ��ö�����麯������
int32 GetVFNumber(PIGAVOBJECT pVObject)
{
    int32 i;
    IGAVFT pVft;

	pVft = pVObject->vft;
	if (pVft == NULL)
		return 0;

	i = 0;
	while (1) {
        if (NULL == pVft[i]) {
			return i;
        }
		i++;
    }
	return i;
}

// �滻�麯����
// �� pObject ���麯���������� pVFT �С�
// iVftSize��pVFT �п��Դ�ŵĺ���ָ��ĸ���
int32 CopyVFT(IGAVFT pVFT, int32 iVFTSize, PIGAVOBJECT pVObject)
{
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	int32 iVFNum = GetVFNumber(pVObject);
	iVFNum = min(iVFNum, iVFTSize);
	for (int32 i = 0; i < iVFNum; i++) {
		pVFT[i] = pVObject->vft[i];
	}
	return iVFNum;
}

// ���ƣ� VOID ReplaceVFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject);
// ���ܣ� �滻�麯����
// ������ �� pTargetObject ���麯�����滻Ϊ pObject ���麯����
int32 ReplaceVFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject)
{
	if (pTargetVObject == NULL || pTargetVObject->vft == NULL)
		return 0;
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	INT nVfNumber;
	INT nTvfNumber = GetVFNumber(pTargetVObject);
	INT nRvfNumber = GetVFNumber(pVObject);

	nVfNumber = min(nTvfNumber, nRvfNumber);
	for (int i = 0; i < nVfNumber; i++) {
		WriteMemory(&pTargetVObject->vft[i], &pVObject->vft[i], sizeof(uint32));
	}
	return nVfNumber;
}

// ���ƣ� VOID ReplaceN_VFT(IGAANYOBJECT pTargetObject, IGAANYOBJECT pObject, int32 iVFTSize);
// ���ܣ� �滻�麯����
// ������ �� pTargetObject ���麯�����滻Ϊ pObject ���麯����
//	       iVFTSize �滻���麯������
int32 ReplaceN_VFT(PIGAVOBJECT pTargetVObject, PIGAVOBJECT pVObject, int32 iVFTSize)
{
	if (pTargetVObject == NULL || pTargetVObject->vft == NULL)
		return 0;
	if (pVObject == NULL || pVObject->vft == NULL)
		return 0;

	INT nVfNumber;
	INT nTvfNumber = GetVFNumber(pTargetVObject);
	INT nRvfNumber = iVFTSize;

	nVfNumber = min(nTvfNumber, nRvfNumber);
	for (int i = 0; i < nVfNumber; i++) {
		WriteMemory(&pTargetVObject->vft[i], &pVObject->vft[i], sizeof(uint32));
	}
	return nVfNumber;
}