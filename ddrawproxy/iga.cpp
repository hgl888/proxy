// iga.cpp : ���� DLL Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include <windowsx.h>

LPDIRECTDRAW7          g_pDD                         =   NULL;   
LPDIRECTDRAWSURFACE7   g_pDDSPrimary                 =   NULL;       
DDCAPS                               g_ddcaps;   

bool   Is555=false;   //   �Ƿ���565ģʽ�����������Ҫ������д  

inline unsigned char GetRed(WORD color)   
{   
	if( Is555 )   
		return (color>>7) & 0xff;   
	else   
		return (color>>8) & 0xff;   
}   

inline   unsigned   char   GetGreen(WORD   color)   
{   
	if(   Is555   )   
		return   (color>>2)   &   0xff;   
	else   
		return   (color>>3)   &   0xff;   
}   

inline   unsigned   char   GetBlue(WORD   color)   
{   
	return   (color   &   0x1f)   <<   3;   
}   

//���캯��
IGA::IGA(LPDIRECTDRAW7 pOriginal)
{	
	m_pIGADraw = pOriginal;
	m_hBitmap = NULL;
}
//��������
IGA::~IGA()
{
} 

//   ���ܣ���һ��16λ��DirectDraw���棬��Ϊһ��24λBMPλͼ   �����������漴������   
//   ���룺����ָ�룬������ļ���   
//   ������Ƿ�ɹ�   
bool   IGA::SaveToBitmapFile(LPDIRECTDRAWSURFACE7   lpSurface,   char*   filename)   
{   
	OutputDebugString("DDRAWPROXY: SaveToBitmapFile.\r\n");
	WORD*   lpBuffer = NULL;             //   ����ָ��   
	int   nPitch = 0;             //   ������   
	int   nWidth = 0,   nHeight = 0;     //   ������   


//	DWORD RGB555Mask[4] = {0x7C00, 0x03E0, 0x001F, 0};
	DWORD RGB565Mask[4] = {0xF800, 0x07E0, 0x001F, 0};

	//   ���ļ�s   
	FILE*   fp;   
	if(   (fp=fopen(filename,   "wb"))   !=   NULL   )   
	{   
		//   ��������   
		DDSURFACEDESC2   ddsd;   
		ddsd.dwSize   =   sizeof(ddsd);   
		
		HRESULT   ddrval   =   IGA_IDirectDrawSurface7::m_pfnLock(lpSurface, NULL,   &ddsd,   DDLOCK_WAIT,   NULL);// lpSurface->Lock(   NULL,   &ddsd,   DDLOCK_WAIT,   NULL   );   
		if(   ddrval   ==   DD_OK   )   
		{   
			lpBuffer   =   (WORD   *)ddsd.lpSurface;   
			nWidth   =   ddsd.dwWidth;   
			nHeight   =   ddsd.dwHeight;   
			nPitch     =   ddsd.lPitch   >>   1;         //lPitch��ByteΪ��λ��GraphPitch��WORDΪ��λ������GraphPitch   =   lPitch   /   2;   
		}   

		//   �����ļ�ͷ   
		BITMAPFILEHEADER   FileHeader;   
		FileHeader.bfType   =   'MB';
		FileHeader.bfSize   =   nWidth   *   nHeight   *   2   +   0x36;   
		FileHeader.bfReserved1   =   0;   
		FileHeader.bfReserved2   =   0;   
		FileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGB565Mask);   
		fwrite(&FileHeader,   sizeof(BITMAPFILEHEADER),   1,   fp);   

		//   �����ļ���Ϣ   
		BITMAPINFOHEADER   Header;   
		Header.biSize   =   sizeof(BITMAPINFOHEADER);     //   �ṹ�Ĵ�С   
		Header.biWidth   =   nWidth;                     //   ��   
		Header.biHeight   =   -nHeight;                     //   ��   
		Header.biPlanes   =   1;                         //   �̶�   
		Header.biBitCount   =   16;                         //   ��ɫ��   
		Header.biCompression   =   BI_BITFIELDS;                 //   �Ƿ�ѹ��   
		Header.biSizeImage   =   nWidth   *   nHeight   *   2;     //   ͼƬ�Ĵ�С   
		Header.biXPelsPerMeter   =   0;   
		Header.biYPelsPerMeter   =   0;   
		Header.biClrUsed   =   0;   
		Header.biClrImportant   =   0;   
		fwrite(&Header,   Header.biSize,   1,   fp);   


		fwrite(&RGB565Mask, sizeof(RGB565Mask),   1,   fp);

		//   д���������(�������ϴ��)   
		fseek(fp,   FileHeader.bfOffBits,   SEEK_SET);

//		WORD   word;   
	//	lpBuffer   +=   nWidth   *2*   (nHeight   -   1);   
		for(int i=0; i < nHeight;   i++)   
		{   
			fwrite(lpBuffer, sizeof(WORD) * nWidth, 1, fp);

			//for(int   j=0;   j<nWidth;   j++)   
			//{   
			//	//word   =   *lpBuffer;   
			//	//fputc(   GetBlue(   word   ),   fp);     //   ��   
			//	//fputc(   GetGreen(   word   ),   fp);     //   ��   
			//	//fputc(   GetRed(   word   ),   fp);         //   ��   
			//	//lpBuffer++;   
			//}   
		//	lpBuffer   -=3*nWidth   ;     //   ָ��ת����һ�еĿ�ʼ   
			lpBuffer += nPitch;
		}

		fclose(fp);   

		//   ��������   
		ddrval   =  IGA_IDirectDrawSurface7::m_pfnUnlock(lpSurface, NULL);   
		return   true;
	}   

	return   false;   
} 


//   ���ܣ���һ��16λ��DirectDraw���棬��Ϊһ��24λBMPλͼ   �����������漴������   
//   ���룺����ָ�룬������ļ���   
//   ������Ƿ�ɹ�   
bool   IGA::SaveToBitmapFile(LPDIRECTDRAWSURFACE   lpSurface,   char*   filename)   
{   
	OutputDebugString("DDRAWPROXY: SaveToBitmapFile.\r\n");
	WORD*   lpBuffer = NULL;             //   ����ָ��   
	int   nPitch = 0;             //   ������   
	int   nWidth = 0,   nHeight = 0;     //   ������   


	//	DWORD RGB555Mask[4] = {0x7C00, 0x03E0, 0x001F, 0};
	DWORD RGB565Mask[4] = {0xF800, 0x07E0, 0x001F, 0};

	//   ���ļ�s   
	FILE*   fp;   
	if(   (fp=fopen(filename,   "wb"))   !=   NULL   )   
	{   
		//   ��������   
		DDSURFACEDESC   ddsd;   
		ddsd.dwSize   =   sizeof(ddsd);   

		HRESULT   ddrval   =   IGA_IDirectDrawSurface::m_pfnLock(lpSurface, NULL,   &ddsd,   DDLOCK_WAIT,   NULL);// lpSurface->Lock(   NULL,   &ddsd,   DDLOCK_WAIT,   NULL   );   
		if(   ddrval   ==   DD_OK   )   
		{   
			lpBuffer   =   (WORD   *)ddsd.lpSurface;   
			nWidth   =   ddsd.dwWidth;   
			nHeight   =   ddsd.dwHeight;   
			nPitch     =   ddsd.lPitch   >>   1;         //lPitch��ByteΪ��λ��GraphPitch��WORDΪ��λ������GraphPitch   =   lPitch   /   2;   
		}   

		//   �����ļ�ͷ   
		BITMAPFILEHEADER   FileHeader;   
		FileHeader.bfType   =   'MB';
		FileHeader.bfSize   =   nWidth   *   nHeight   *   2   +   0x36;   
		FileHeader.bfReserved1   =   0;   
		FileHeader.bfReserved2   =   0;   
		FileHeader.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + sizeof(RGB565Mask);   
		fwrite(&FileHeader,   sizeof(BITMAPFILEHEADER),   1,   fp);   

		//   �����ļ���Ϣ   
		BITMAPINFOHEADER   Header;   
		Header.biSize   =   sizeof(BITMAPINFOHEADER);     //   �ṹ�Ĵ�С   
		Header.biWidth   =   nWidth;                     //   ��   
		Header.biHeight   =   -nHeight;                     //   ��   
		Header.biPlanes   =   1;                         //   �̶�   
		Header.biBitCount   =   16;                         //   ��ɫ��   
		Header.biCompression   =   BI_BITFIELDS;                 //   �Ƿ�ѹ��   
		Header.biSizeImage   =   nWidth   *   nHeight   *   2;     //   ͼƬ�Ĵ�С   
		Header.biXPelsPerMeter   =   0;   
		Header.biYPelsPerMeter   =   0;   
		Header.biClrUsed   =   0;   
		Header.biClrImportant   =   0;   
		fwrite(&Header,   Header.biSize,   1,   fp);   


		fwrite(&RGB565Mask, sizeof(RGB565Mask),   1,   fp);

		//   д���������(�������ϴ��)   
		fseek(fp,   FileHeader.bfOffBits,   SEEK_SET);

		//		WORD   word;   
		//	lpBuffer   +=   nWidth   *2*   (nHeight   -   1);   
		for(int i=0; i < nHeight;   i++)   
		{   
			fwrite(lpBuffer, sizeof(WORD) * nWidth, 1, fp);

			//for(int   j=0;   j<nWidth;   j++)   
			//{   
			//	//word   =   *lpBuffer;   
			//	//fputc(   GetBlue(   word   ),   fp);     //   ��   
			//	//fputc(   GetGreen(   word   ),   fp);     //   ��   
			//	//fputc(   GetRed(   word   ),   fp);         //   ��   
			//	//lpBuffer++;   
			//}   
			//	lpBuffer   -=3*nWidth   ;     //   ָ��ת����һ�еĿ�ʼ   
			lpBuffer += nPitch;
		}

		fclose(fp);   

		//   ��������   
		ddrval   =  IGA_IDirectDrawSurface::m_pfnUnlock(lpSurface, NULL);   
		return   true;
	}   

	return   false;   
} 

//��ʾλͼ
//�����ֶ�ȡλͼ�ļ�����ʾλͼ:
void IGA::LoadImageResource() 
{

	OutputDebugString("DDRAWPROXY: LoadImageResource.\r\n");
	CHAR chOutputString[255];
	sprintf(chOutputString, "LoadImageResource called!");
	OutputDebugString(chOutputString);

	// TODO: Add your control notification handler code here
	/*******************ͨ����ȡλͼ�ļ�����ʾλͼ**********************/
//	CString strFileName;
//	strFileName.Format("%s","NetFriend.bmp");
	//��λͼ��ϢBITMAPINFO
//	BITMAPINFO *bitmapinfo=NULL;
//	BYTE *BmpData=NULL;
//	CFile file;//��ȡλͼ��Ϣ
	//�����ļ��Ĵ򿪷�ʽ
//	if(!file.Open(strFileName,Cfile::modeRead|Cfile::typeBinary))
//	{
//		return;
//	}
	////BITMAP�ļ�ͷ�ṹ��
	//BITMAPFILEHEADER BitmapHead;
	////��ȡλͼ�ļ�ͷ
	//if(file.Read(&BitmapHead,sizeof(BITMAPFILEHEADER))!=sizeof(BITMAPFILEHEADER))
	//{
	//	MessageBox("��ȡ�ļ�ͷʧ��!");
	//	return;
	//}
	//if(BitmapHead.bfType!=0x4d42)
	//{
	//	MessageBox("�Բ���,����ȡ�Ĳ���λͼ�ļ�!");
	//	return;
	//}
	////λͼ��Ϣ
	//BITMAPINFOHEADER  BitmapInfo;
	//if(file.Read(&BitmapInfo,sizeof(BITMAPINFOHEADER))!=sizeof(BITMAPINFOHEADER))
	//{
	//	MessageBox("��ȡλͼ��Ϣʧ��!");
	//	return;
	//}
	//if(BitmapInfo.biBitCount!=24)
	//{
	//	MessageBox("�Բ���,��ǰ����ֻ֧��24λλͼ��Ϣ!");
	//	return;
	//}
	//bitmapinfo=(BITMAPINFO*)new char[sizeof(BITMAPINFOHEADER)];
	//if(!bitmapinfo)
	//{
	//	MessageBox("�ڴ����ʧ��!");
	//	return;
	//}
	///*��BMPλͼ��Ϣͷ�е����ݶ�ȡ��λͼ��Ϣ�ṹ��ȥ.*/
	//memcpy(bitmapinfo,&BitmapInfo,sizeof(BITMAPINFOHEADER));
	///*�����õ�λͼ�ļ��Ĵ�С*/
	//DWORD dataByte=BitmapHead.bfSize-BitmapHead.bfOffBits;
	//BmpData=(BYTE*)new char[dataByte];
	//if(!BmpData)
	//{
	//	MessageBox("�ڴ����ʧ��!");
	//	delete bitmapinfo;
	//	delete BmpData;
	//	return;
	//}
	//if(file.Read(BmpData,dataByte)!=dataByte)
	//{
	//	MessageBox("��ȡλͼ����ʧ��!");
	//	return;
	//}
	//file.Close();


	extern HINSTANCE           gl_hOriginalDll;
	extern HINSTANCE           gl_hThisInstance;

	sprintf(chOutputString, "gl_hThisInstance = 0x%p", gl_hThisInstance);
	OutputDebugString(chOutputString);

//	m_hBitmap = (HBITMAP)::LoadImage(gl_hThisInstance, "D:\\a_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE);
	// Use LoadImage() to get the image loaded into a DIBSection
	m_hBitmap = (HBITMAP)LoadImage( NULL, "D:\\a_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmap != NULL)
	{
	//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "ͼƬ���سɹ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "ͼƬ����ʧ��(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapB1 = (HBITMAP)LoadImage( NULL, "D:\\b_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapB1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "ͼƬ���سɹ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "ͼƬ����ʧ��(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapC1 = (HBITMAP)LoadImage( NULL, "D:\\c_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "ͼƬ���سɹ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "ͼƬ����ʧ��(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapD1 = (HBITMAP)LoadImage( NULL, "D:\\d_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "ͼƬ���سɹ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "ͼƬ����ʧ��(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}

	m_hBitmapE1 = (HBITMAP)LoadImage( NULL, "D:\\e_1.bmp", IMAGE_BITMAP, 0, 0, LR_DEFAULTSIZE | LR_LOADFROMFILE );
	// m_hBitmap = ::LoadBitmap(NULL, "D:\\a_1.bmp");
	if (m_hBitmapC1 != NULL)
	{
		//	GetObject(hBitmap, sizeof(bm), &bm);
		sprintf(chOutputString, "ͼƬ���سɹ�@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@");
		OutputDebugString(chOutputString);
	}
	else
	{
		sprintf(chOutputString, "ͼƬ����ʧ��(0x%08X)", GetLastError());
		OutputDebugString(chOutputString);
	}
//	CClientDC *pDC=new CClientDC(this);
//	pDC->SetStretchBltMode(COLORONCOLOR);
//	StretchDIBits(pDC->GetSafeHdc(),200,0,BitmapInfo.biWidth,BitmapInfo.biHeight,  0,0,BitmapInfo.biWidth,BitmapInfo.biHeight,BmpData,bitmapinfo,DIB_RGB_COLORS,SRCCOPY);

}

#define     SelectBitmap(hdc, hbm)  ((HBITMAP)SelectObject((hdc), (HGDIOBJ)(HBITMAP)(hbm)))

bool  IGA::ReplaceSurface(LPDIRECTDRAWSURFACE7 lpSurface, HBITMAP hBitmap)
{
	OutputDebugString("DDRAWPROXY: ReplaceSurface.\r\n");

	CHAR chOutputString[255];

	HRESULT hr;

///***********************************************
	//HBITMAP hBitmap = m_hBitmap;

	//if (m_hBitmap == NULL)
	//{
	//	LoadImageResource();
	//	hBitmap = m_hBitmap;
	//}

	if (lpSurface == NULL || hBitmap == NULL)
		return false;


	///***********************************************

	BYTE* lpBuffer;             //   ����ָ��
	BYTE* lpBmpBuf;
//	WORD  wPixel = 0;
	int   nPitch = 0;                 //   ������   
	int   nWidth = 0, nHeight = 0;      //   ������   

	DDSURFACEDESC2 ddsd;
//	BITMAP         bm;

	
//	GetObject(hBitmap, sizeof(BITMAP), &bm);
	
	//LONG	nLength = GetBitmapBits(hBitmap, 0, NULL);
	//static CHAR * pp = NULL;
	//if(!pp)
	//{
	//	pp	= new CHAR [ nLength ];
	//	::GetBitmapBits(hBitmap, nLength, pp );
	//	sprintf(chOutputString, "pp: %p, Length: %d\r\n",pp, nLength);
	//	OutputDebugString(chOutputString);
	//}

//	bm.bmWidth;
//	bm.bmHeight;

//	sprintf(chOutputString, "bmWidth : %d, bmHeight : %d", bm.bmWidth, bm.bmHeight);
//	OutputDebugString(chOutputString);


	ZeroMemory(&ddsd, sizeof(DDSURFACEDESC2));
	ddsd.dwSize = sizeof(DDSURFACEDESC2);

	hr = IGA_IDirectDrawSurface7::m_pfnGetSurfaceDesc(lpSurface, &ddsd);
	if (SUCCEEDED(hr))
	{
		nWidth     =  ddsd.dwWidth;
		nHeight    =  ddsd.dwHeight;

	//	sprintf(chOutputString, "nWidth : %d, nHeight : %d", nWidth, nHeight);
	//	OutputDebugString(chOutputString);

		HDC hSurface = NULL;
		hr = IGA_IDirectDrawSurface7::m_pfnGetDC(lpSurface, &hSurface);
		if (FAILED(hr))
		{
			sprintf(chOutputString, "GetDC ʧ�� (hr = 0x%08X)", hr);
			OutputDebugString(chOutputString);
			return false;
		}

	//	sprintf(chOutputString, "hSurface : 0x%p", hSurface);
	//	OutputDebugString(chOutputString);


		HBITMAP hPrevBitmap;
		HDC hSrcDC = ::CreateCompatibleDC(hSurface);

		hPrevBitmap = SelectBitmap(hSrcDC, hBitmap);

		BitBlt(hSurface, 0, 0, ddsd.dwWidth, ddsd.dwHeight, hSrcDC, 0, 0, SRCCOPY);

		SelectBitmap(hSrcDC, hPrevBitmap);

		IGA_IDirectDrawSurface7::m_pfnReleaseDC(lpSurface, hSurface);
	}
	return true;
}


