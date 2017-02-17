// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#define WIN32_LEAN_AND_MEAN		
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3d9types.h>
#include <d3dx9tex.h>
#include <string>
#include "Hideme.h"
#include "IGAWin.h"

#define  OUTPUT_DEBUG 0
//-----------------------------------------------
//3Dģ��
typedef DWORD (*FUN_IGA3DINIT)( LPDIRECT3DDEVICE9  );
typedef void  (*FUN_IGA3DGetMatrixData)( D3DTRANSFORMSTATETYPE ,CONST D3DMATRIX*  );
typedef void  (*FUN_IGA3DRender)();
typedef void  (*FUN_IGA3DDestory)();
typedef void  (*FUN_ISetFristDraw)();

enum RenderPos
{
	Per_SetTransform,	//���þ���ǰ
	Post_SetTransform,	//���þ����
	Per_Present,		//��תǰ
	Post_Present		//��ת��
};
//-------------------------------------------------