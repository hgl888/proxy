#include "SkillInfoTable.h"
#include "../GameDefine.h"





SKILL_INFO * GetSkillInfo(DWORD dwID)
{
	for (int i = 0; i < MAX_SKILL; i ++)
	{
		if (g_SkillInfo[i].dwID == dwID)
		{
			return &g_SkillInfo[i];
		}
	}
	return NULL;
}

SKILL_INFO g_SkillInfo[MAX_SKILL] = 
{
	{L"����֮��",L"���ȼ�1��",L"�׵���\n�ȼ�1\n����ֵ 40\n�˺� 70 - 85\n",UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_1,1},
	{L"��ǽ",L"���ȼ�1��",L"����׿���",UI_TEXTURE_ID::GAMEUI_TEX_SKILL_STATIC_2,2},
};
