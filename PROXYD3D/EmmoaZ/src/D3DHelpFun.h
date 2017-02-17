#ifndef _D3DHELP_FUN_H
#define _D3DHELP_FUN_H

#ifdef  USING_D3D9
    #include <d3d9.h>
    #include <d3dx9.h>
	#define     _D3DX_LIB_NAME    _T("D3DX9_29.dll")
    typedef IDirect3D9          IDirect3D;
	typedef IDirect3DDevice9    IDirect3DDevice;
	typedef IDirect3DTexture9   IDirect3DTexture;
	typedef IDirect3DSurface9   IDirect3DSurface;
	typedef LPDIRECT3DDEVICE9   LPDIRECT3DDEVICE;
	typedef IDirect3DBaseTexture9  IDirect3DBaseTexture;
	
	const GUID __IID_IDirect3DTexture = {0xe4cdd575, 0x2866, 0x4f01, {0xb1, 0x2e, 0x7e, 0xec, 0xe1, 0xec, 0x93, 0x58}};
    
#else if USING_D3D8
	#include <d3dx8.h>
	#define     _D3DX_LIB_NAME    _T("D3DX8.dll")
	typedef IDirect3D8          IDirect3D;
	typedef IDirect3DDevice8    IDirect3DDevice;
	typedef IDirect3DTexture8   IDirect3DTexture;	
	typedef IDirect3DSurface8   IDirect3DSurface;
	typedef LPDIRECT3DDEVICE8   LPDIRECT3DDEVICE;
	typedef IDirect3DBaseTexture8  IDirect3DBaseTexture;

	
	const GUID __IID_IDirect3DTexture = {0xe4cdd575, 0x2866, 0x4f01, {0xb1, 0x2e, 0x7e, 0xec, 0xe1, 0xec, 0x93, 0x58}};
							
	
#endif


namespace   IGA_D3D_HELP
{
	//�������ݵıȽ��滻
	BOOL Surface_DoCompare(const void *pSource, const void *pTarget, UINT nPicth, D3DSURFACE_DESC* pDesc,BOOL  bDebug = FALSE);
	BOOL Surface_DoReplace( CHAR *pTarget, const CHAR *pSource, UINT nPicth, D3DSURFACE_DESC* pDesc);

	//����Ƚ��滻
	BOOL Texture_DoCompare( IDirect3DTexture* pTexture1,IDirect3DTexture* pTexture2,UINT  uLevel);
	BOOL Texture_DoReplace( IDirect3DTexture* pTexture1,IDirect3DTexture* pTexture2,UINT  uLevel);

	//�����������
	BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture);
	BOOL CreateTexture(LPDIRECT3DDEVICE pDevice,LPCTSTR lpszFileName,IDirect3DTexture** ppTexture ,IDirect3DTexture*  pRefTexture);


	//���������ļ�
	BOOL  SaveTextureToFile(IDirect3DTexture* pTexture,LPCTSTR   lpszFileName);
	//������浽�ļ�
	BOOL  SaveSurfaceToFile(IDirect3DSurface* pSurface,LPCTSTR   lpszFileName);


	//�Ƿ����ڼ�������
	BOOL  IsCreatingTexture();
	//��ʼ��d3d��չ��
	BOOL  InitD3Dx();
	VOID  UnInitD3Dx();


	/*�豸���ü���*/
	VOID  AddDeviceRefCount(LPDIRECT3DDEVICE pDevice, long  lCount);
	VOID  DelDeviceRefCount(LPDIRECT3DDEVICE pDevice );

	VOID  SetDeviceRefCount(LPDIRECT3DDEVICE pDevice, long  dwCount);	
	long  GetDeviceRefCount( LPDIRECT3DDEVICE pDevice);

	//�ı��豸���õĺ�������ǰ����Ӽ�����ĵ����ü���
// 	DWORD BeginUpdate ( LPDIRECT3DDEVICE pDevice);
//     VOID  EndUpdate   ( LPDIRECT3DDEVICE pDevice,DWORD  dwCount );

	//�豸 release�ǵ����ͽ���
	VOID  DeviceReleaseLock    (LPDIRECT3DDEVICE pDevice);
	VOID  DeviceReleaseUnLock  (LPDIRECT3DDEVICE pDevice);
}

#endif