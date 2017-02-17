#include "stdafx.h"
#include "IGA8Core.h"
#include "IGACoreImpl.h"

#include <tinyxml.h>
#include <process.h>

#include <string>
#include <cctype>
#include <list>
#include <map>
#include <algorithm>

#include "formats.h"

class CIDA2D_ADS
{
public:
	std::string name;
	std::string type;
	std::string match_file;
	std::string adsfile;
	int level;
	int adslevel;

	LPDIRECT3DTEXTURE8 match_tex;
	LPDIRECT3DTEXTURE8 ads_tex;

	CIDA2D_ADS() : level(-1), adslevel(-1), match_tex(NULL), ads_tex(NULL) {

	}
};

typedef std::list<CIDA2D_ADS>                CIGA2DAdsList;
typedef std::map<IDirect3DTexture8*, BOOL>  CDXMapTextureExport;
typedef std::map<IDirect3DTexture8*, BOOL>  CDXMapTextureScan;

CIGA2DAdsList              g_listIGA2DAds;
CDXMapTextureExport        g_mapDXTextureExport;
CDXMapTextureScan          g_mapDXTextureScan;

// ��ò������ֽ���
UINT GetSurfaceLineBytes(D3DSURFACE_DESC* pDesc)
{
	UINT nLineBytes = 0;

	for( int i = 0; i < fmtInfoArraySize; i++ )
	{
		if( fmtInfoArray[i].fmt == pDesc->Format )
		{
			nLineBytes = (pDesc->Width * (fmtInfoArray[i].nBitsPerTexel / sizeof(BYTE))) / 8;
			break;
		}
	}
	return nLineBytes;
}

// �Ƚ�ͼ��
BOOL CompareTexture(DWORD dwLevel, IDirect3DTexture8* pTarget, DWORD dwMatchLevel, IDirect3DTexture8* pMatchTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC match_desc;

	DWORD i;
	INT  Pitch;
	UINT Width, Height;
	UINT LineBytes;
	D3DFORMAT Format;

//	TCHAR chDebugString[128];

	if (pTarget->GetType() != pMatchTexture->GetType())
		goto l_failed;
	if (dwLevel >= pTarget->GetLevelCount())
		goto l_failed;
	if (dwMatchLevel >= pMatchTexture->GetLevelCount())
		goto l_failed;

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pMatchTexture->GetLevelDesc(dwMatchLevel, &match_desc);
	if (FAILED(hr))
		goto l_failed;

	LineBytes = GetSurfaceLineBytes(&target_desc);
	if (LineBytes == 0)
		goto l_failed;

	if (target_desc.Width != match_desc.Width)
		goto l_failed;
	if (target_desc.Height != match_desc.Height)
		goto l_failed;
	if (target_desc.Format != match_desc.Format)
		goto l_failed;
//	if (target_desc.Type != match_desc.Type)
//		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT match_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr))
	{
		goto l_failed;
	}

	hr = pMatchTexture->LockRect(dwMatchLevel, &match_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr)) {
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	Width  =  target_desc.Width;
	Height = target_desc.Height;
	Format = target_desc.Format;

	switch (Format)
	{
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		Pitch = target_lockedrect.Pitch / 4;
		break;
	default:
		Pitch = target_lockedrect.Pitch;
		break;
	}

	// �Ƚϵ���
	BOOL bMatch = (memcmp(target_lockedrect.pBits, match_lockedrect.pBits, LineBytes) == 0);
	if (!bMatch)
		goto l_return;
	
	LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
	LPBYTE lpMatchBits  = (LPBYTE)match_lockedrect.pBits;
	for (i = 0; i < Height; i++)
	{
		if (memcmp(lpTargetBits, lpMatchBits, LineBytes) != 0) {
			bMatch = FALSE;
			break;
		}
		lpTargetBits += Pitch;
		lpMatchBits  += Pitch;
	}
 
// 	if (bMatch)
// 	{
// 		_stprintf(chDebugString, _T("�Ƚ������� OK\r\n"));
// 		OutputDebugString(chDebugString);
// 	}

l_return:
	pTarget->UnlockRect(dwLevel);
	pMatchTexture->UnlockRect(dwMatchLevel);
	return bMatch;

l_failed:
	return FALSE;
}

// �滻����
BOOL ReplaceTexture(IDirect3DDevice8* pDevice, DWORD dwLevel, IDirect3DTexture8* pTarget, DWORD dwAdsLevel, IDirect3DTexture8* pAdsTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC adstex_desc;

	DWORD i;
	INT  Pitch;
	UINT Width, Height;
	UINT LineBytes;
	D3DFORMAT Format;

	if (pTarget->GetType() != pAdsTexture->GetType())
		goto l_failed;
	if (dwLevel >= pTarget->GetLevelCount())
		goto l_failed;
	if (dwAdsLevel >= pAdsTexture->GetLevelCount())
		goto l_failed;

	hr = pTarget->GetLevelDesc(dwLevel, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pAdsTexture->GetLevelDesc(dwAdsLevel, &adstex_desc);
	if (FAILED(hr))
		goto l_failed;

	LineBytes = GetSurfaceLineBytes(&target_desc);
	if (LineBytes == 0)
		goto l_failed;

	if (target_desc.Width != adstex_desc.Width)
		goto l_failed;
	if (target_desc.Height != adstex_desc.Height)
		goto l_failed;
	if (target_desc.Format != adstex_desc.Format)
		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT adstex_lockedrect;

	hr = pTarget->LockRect(dwLevel, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
	if (FAILED(hr))
		goto l_failed;

	hr = pAdsTexture->LockRect(dwAdsLevel, &adstex_lockedrect, NULL, D3DLOCK_READONLY);
	if (FAILED(hr)) {
		pTarget->UnlockRect(dwLevel);
		goto l_failed;
	}

	Width  =  target_desc.Width;
	Height = target_desc.Height;
	Format = target_desc.Format;

	switch (Format)
	{
	case D3DFMT_DXT1:
	case D3DFMT_DXT2:
	case D3DFMT_DXT3:
	case D3DFMT_DXT4:
	case D3DFMT_DXT5:
		Pitch = target_lockedrect.Pitch / 4;
		break;
	default:
		Pitch = target_lockedrect.Pitch;
		break;
	}

	LPBYTE lpTargetBits = (LPBYTE)target_lockedrect.pBits;
	LPBYTE lpAdstexBits  = (LPBYTE)adstex_lockedrect.pBits;
	for (i = 0; i < Height; i++)
	{
		::CopyMemory(lpTargetBits, lpAdstexBits, LineBytes);

		lpTargetBits  += Pitch;
		lpAdstexBits  += Pitch;
	}

	pTarget->UnlockRect(dwLevel);
	pAdsTexture->UnlockRect(dwAdsLevel);
	return TRUE;

l_failed:
	return FALSE;
}

BOOL ReplaceTextureAllLevel(IDirect3DDevice8* pDevice, IDirect3DTexture8* pTarget, IDirect3DTexture8* pAdsTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC target_desc;
	D3DSURFACE_DESC adstex_desc;

	TCHAR chDebugString[128];

	if (pTarget->GetType() != pAdsTexture->GetType())
		goto l_failed;
	if (pTarget->GetLevelCount() <= 0)
		goto l_failed;
	if (pTarget->GetLevelCount() > pAdsTexture->GetLevelCount())
		goto l_failed;
	hr = pTarget->GetLevelDesc(0, &target_desc);
	if (FAILED(hr))
		goto l_failed;
	hr = pAdsTexture->GetLevelDesc(0, &adstex_desc);
	if (FAILED(hr))
		goto l_failed;

	if (target_desc.Width != adstex_desc.Width)
		goto l_failed;
	if (target_desc.Height != adstex_desc.Height)
		goto l_failed;
	if (target_desc.Format != adstex_desc.Format)
		goto l_failed;

	D3DLOCKED_RECT target_lockedrect;
	D3DLOCKED_RECT adstex_lockedrect;

	DWORD i, j;
	UINT Width, Height;
	INT Pitch;
	UINT LineBytes;
	D3DFORMAT Format;

	DWORD dwLevelCount = pTarget->GetLevelCount();
	for (i = 0; i < dwLevelCount; i++)
	{
		hr = pTarget->GetLevelDesc(i, &target_desc);
		if (FAILED(hr))
			goto l_failed;

		LineBytes = GetSurfaceLineBytes(&target_desc);
		if (LineBytes == 0)
			goto l_failed;

		hr = pTarget->LockRect(i, &target_lockedrect, NULL, D3DLOCK_NO_DIRTY_UPDATE);
		if (FAILED(hr))
			goto l_failed;

		hr = pAdsTexture->LockRect(i, &adstex_lockedrect, NULL, D3DLOCK_READONLY);
		if (FAILED(hr)) 
		{
			pTarget->UnlockRect(i);
			goto l_failed;
		}

		Width  = target_desc.Width;
		Height = target_desc.Height;
		Format = target_desc.Format;

		switch (Format)
		{
		case D3DFMT_DXT1:
		case D3DFMT_DXT2:
		case D3DFMT_DXT3:
		case D3DFMT_DXT4:
		case D3DFMT_DXT5:
			Pitch = target_lockedrect.Pitch / 4;
			break;
		default:
			Pitch = target_lockedrect.Pitch;
			break;
		}

		LPBYTE lpTargetBits  = (LPBYTE)target_lockedrect.pBits;
		LPBYTE lpAdstexBits  = (LPBYTE)adstex_lockedrect.pBits;
		for (j = 0; j < Height; j++)
		{
			::CopyMemory(lpTargetBits, lpAdstexBits, LineBytes);

			lpTargetBits  += Pitch;
			lpAdstexBits  += Pitch;
		}

		pAdsTexture->UnlockRect(i);
		pTarget->UnlockRect(i);
	}
	return TRUE;

l_failed:
	return FALSE;
}

IIGACore* GetIGACore() {
	static CIGACoreImpl impl;
	return &impl;
}

CIGACoreImpl::CIGACoreImpl()
{
	m_bEnableExport = FALSE;
	m_bEnableScan = TRUE;
	m_bEnablePlayVideo = TRUE;
	m_pIDirect3DDevice8 = NULL;
	strcpy(m_chExportPath, "E:\\exports\\image\\"); 
}

CIGACoreImpl::~CIGACoreImpl()
{
	
}

void CIGACoreImpl::SetExportPath(LPCTSTR lpszExportPath)
{
	if (lpszExportPath == NULL)
		return;
	_tcsncpy(m_chExportPath, lpszExportPath, MAX_PATH);
	m_chExportPath[MAX_PATH - 1] = '\0';
}

void CIGACoreImpl::LoadConfigDocument(LPCTSTR lpszFileName)
{
	std::string name;
	std::string exports;
	std::string scans;
	std::string export_path;

	TCHAR chDebugString[128];

	TiXmlDocument doc(lpszFileName);
	if (!doc.LoadFile()) return;
	
	TiXmlHandle hDoc(&doc);
	TiXmlElement* pElem;
	TiXmlHandle hRoot(0);

	// block: name
	{
		pElem = hDoc.FirstChildElement().Element();
		if (!pElem)
			return;

		name = pElem->Attribute("name");
		exports = pElem->Attribute("exports");
		scans = pElem->Attribute("scans");
		export_path = pElem->Attribute("export_path");

		std::transform(exports.begin(), exports.end(), exports.begin(), std::toupper);
		if (exports == "TRUE")
			m_bEnableExport = TRUE;
		else if (exports == "FALSE")
			m_bEnableExport = FALSE;

		std::transform(scans.begin(), scans.end(), scans.begin(), std::toupper);
		if (scans == "TRUE")
			m_bEnableScan = TRUE;
		else if (scans == "FALSE")
			m_bEnableScan = FALSE;

		SetExportPath(export_path.c_str());

		// save this for later
		hRoot = TiXmlHandle(pElem);
	}

	TiXmlElement* pAdsTextureNode = hRoot.FirstChild( "adstexture" ).FirstChild().Element();
	for (pAdsTextureNode; pAdsTextureNode != NULL; pAdsTextureNode = pAdsTextureNode->NextSiblingElement("iga2d_ads"))
	{
		CIDA2D_ADS ads;

		const char* pAdsName = pAdsTextureNode->Attribute("name");
		if (pAdsName != NULL)
			ads.name = pAdsName;

		const char* pAdsType = pAdsTextureNode->Attribute("type");
		if (pAdsType != NULL)
			ads.type = pAdsType;

		pAdsTextureNode->QueryIntAttribute("level", &ads.level); // If this fails, original value is left as-is

		const char* pmatch_file = pAdsTextureNode->Attribute("match_file");
		if (pmatch_file != NULL)
			ads.match_file = pmatch_file;

		pAdsTextureNode->QueryIntAttribute("adslevel", &ads.adslevel);

		const char* padsfile = pAdsTextureNode->Attribute("adsfile");
		if (padsfile != NULL)
			ads.adsfile = padsfile;

		// ������Ͷ���б�
		g_listIGA2DAds.push_back(ads);

		// <iga2d_ads name="tex001" type="DDS" level="0" match_file="" adslevel="0" adsfile="" />
		_stprintf(chDebugString, _T("iga2d_ads(%s, %s, %d, %s, %d, %s)\r\n"), ads.name.c_str(), ads.type.c_str(), ads.level, ads.match_file.c_str(), ads.adslevel, ads.adsfile.c_str());
		OutputDebugString(chDebugString);
	}

	m_bRegistered = TRUE;
	// ��
//	m_bRegistered = DSValidate(".\\IGACore.ini");

	CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
	if( p == g_listIGA2DAds.end() )
	{
		OutputDebugString("���������ļ���ͼƬ���ǿյ�");
	}
	else
	{
		OutputDebugString("ͼƬ������Ԫ��");
	}
}

void CIGACoreImpl::LoadAdsResource()
{
	Load2DAdsResource();
//	LoadVideoAdsResource();
}

void CIGACoreImpl::Load2DAdsResource()
{
	OutputDebugString("����2D���ͼƬ....");
	HRESULT hr;
	LPDIRECT3DTEXTURE8 pMatchTexture;
	LPDIRECT3DTEXTURE8 pAdsTexture;
	
	OutputDebugString("��ʼ��2D���ͼƬ��list....");
	CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
	OutputDebugString("��ʼ�������....");
	
	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *tmp;
	tmp = strrchr(TmpPath,'\\');
	if(!tmp)
		return ;
	*(tmp+1) = 0;
    OutputDebugString("��õ�ǰ����·��....");
    OutputDebugString(TmpPath);
	while (p != g_listIGA2DAds.end())
	{
		pMatchTexture = NULL;
		pAdsTexture = NULL;

		CIDA2D_ADS& ads = *p;

		//hr = D3DXCreateTextureFromFileEx(m_pIDirect3DDevice8, ads.match_file.c_str(), 
		//		D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT,
		//		0, D3DFMT_UNKNOWN, D3DPOOL_MANAGED, 
		//		D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, 
		//		&pMatchTexture);
		//OutputDebugString("����ƥ��ͼƬ....");

		char szLogFile[MAX_PATH];
		sprintf(szLogFile,"%s%s",TmpPath,ads.match_file.c_str());

		OutputDebugString(szLogFile);

		hr = D3DXCreateTextureFromFileA(m_pIDirect3DDevice8, szLogFile, &pMatchTexture);
		if (FAILED(hr)) {
			p++;
			OutputDebugString("ƥ��ͼƬ����ʧ�ܣ�");
			CHAR tex[256];
			sprintf( tex, "������룺%08x", hr);
			OutputDebugString(tex);
			continue;
		}
	
		OutputDebugString("�����滻ͼƬ....");
		char szLogFile1[MAX_PATH];

		//����ƥ��ͼƬ����Ϣ
		DWORD LevelCount = pMatchTexture->GetLevelCount();
		sprintf(szLogFile1,"LevelCount: %d", LevelCount );
		OutputDebugString( szLogFile1 );

		int w = 0;
		int h = 0;
		D3DFORMAT fmt;
		D3DPOOL pool;
		DWORD Usage = 0;
        if( LevelCount > 0 )
		{
			D3DSURFACE_DESC desc;
			hr = pMatchTexture->GetLevelDesc( 0, &desc );
			if( SUCCEEDED(hr) )
			{
				w = desc.Width;
				h = desc.Height;
				fmt = desc.Format;
				pool = desc.Pool;
				Usage = desc.Usage;
			}
		}

		sprintf(szLogFile1,"%s%s",TmpPath,ads.adsfile.c_str());	
		OutputDebugString( szLogFile1 );
		//hr = D3DXCreateTextureFromFileA(m_pIDirect3DDevice8, szLogFile1, &pAdsTexture);
		
		hr = D3DXCreateTextureFromFileEx(m_pIDirect3DDevice8, szLogFile1, 
											w, h, LevelCount,
											Usage, fmt, pool, 
											D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, 
											&pAdsTexture);

		if (FAILED(hr)) 
		{
			p++;
			OutputDebugString("�滻ͼƬ����ʧ�ܣ�");
			continue;
		}
		if( pMatchTexture == NULL )
		{
			OutputDebugString("pMatchTexture == NULL ");
		}

		if( pAdsTexture == NULL )
		{
			OutputDebugString("pAdsTexture == NULL ");
		}
		ads.match_tex = pMatchTexture;
		ads.ads_tex = pAdsTexture;

		p++;
	}
	OutputDebugString("���ͼƬ����....");
}

void CIGACoreImpl::LoadVideoAdsResource()
{
}

void CIGACoreImpl::ReleaseAdsResource()
{
	TCHAR chDebugString[128];

	_stprintf(chDebugString, _T("ReleaseAdsResource\r\n"));
	OutputDebugString(chDebugString);

	CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
	while (p != g_listIGA2DAds.end())
	{
		CIDA2D_ADS& ads = *p;

		if (ads.match_tex)
			ads.match_tex->Release();
		if (ads.ads_tex)
			ads.ads_tex->Release();

		p++;
	}
}

void CIGACoreImpl::PrintTextureDesc(IDirect3DTexture8* pTexture)
{
	HRESULT hr;
	D3DSURFACE_DESC desc;
	TCHAR chDebugString[128];

	if (pTexture == NULL) 
		return;

	DWORD dwLevelCount;

	pTexture->AddRef();

	dwLevelCount = pTexture->GetLevelCount();

	_stprintf(chDebugString, _T("PrintTextureDesc(0x%p, Level Count: %d)\r\n"), pTexture, dwLevelCount);
	OutputDebugString(chDebugString);

	hr = pTexture->GetLevelDesc(0, &desc);
	if (SUCCEEDED(hr))
	{
		_stprintf(chDebugString, _T("\tWidth: %d\r\n\tHeight: %d\r\n\tType: %d\r\n\tFormat: 0x%08X\r\n\tPool: 0x%08X\r\n"), 
			desc.Width, 
			desc.Height, 
			desc.Type,
			desc.Format,
			desc.Pool
			);
		OutputDebugString(chDebugString);
	}
	pTexture->Release();
}

void CIGACoreImpl::OnDirect3DCreate(IDirect3D8 *pD3D)
{
	
}

void CIGACoreImpl::OnInitD3DDevice(IDirect3DDevice8* pDevice)
{
	m_pIDirect3DDevice8 = pDevice;
//	TCHAR chDebugString[128];
//	_stprintf(chDebugString, _T("OnInitD3DDevice(0x%p)\r\n"), pDevice);
//	OutputDebugString(chDebugString);

	char  TmpPath[MAX_PATH];
	GetModuleFileName(NULL,TmpPath,MAX_PATH);
	char *p;
	p = strrchr(TmpPath,'\\');
	if(!p)
		return ;
	*(p+1) = 0;
    
	char szLogFile[MAX_PATH];
	sprintf(szLogFile,"%sIGAConfig.xml",TmpPath);

	// ���������ļ�
	LoadConfigDocument( szLogFile );
	//OutputDebugString(szLogFile);

	OutputDebugString("�����������ļ�����ʼ���ع��ͼƬ");
	// ���ع����Դ
	LoadAdsResource();
	OutputDebugString("��������ͼƬ");
}

void CIGACoreImpl::OnDestroyD3DDevice(IDirect3DDevice8* pDevice)
{
	ReleaseAdsResource();
}

void CIGACoreImpl::OnCreateTexture(UINT Width,UINT Height,UINT Levels,DWORD Usage,D3DFORMAT Format,D3DPOOL Pool,IDirect3DTexture8* pTexture)
{

}

void CIGACoreImpl::OnReleaseTexture(IDirect3DTexture8* pTexture)
{
	if (m_bEnableExport) {
		// ɾ��Ԫ�شӵ����б�
		g_mapDXTextureExport.erase(pTexture);
	}

	if (m_bEnableScan) {
		// ɾ��Ԫ�ش�ɨ���б�
		g_mapDXTextureScan.erase(pTexture);
	}
}


extern bool	g_bOutputTexture;
IDirect3DTexture8* CIGACoreImpl::OnSetTexture(DWORD Stage,IDirect3DTexture8* pTexture)
{
	int len;
	static DWORD sCounter;
	TCHAR chDebugString[128];

	HRESULT hr;
	INT nLevelCount;
	TCHAR chFileName[MAX_PATH];
	TCHAR chExportFile[MAX_PATH];
	
	IDirect3DTexture8* tex = pTexture;
	IDirect3DTexture8* res = tex;

	sCounter++;
	//OutputDebugString("��ʼ׼������ͼƬ....");
	if (/*m_bEnableExport*/g_bOutputTexture && m_bRegistered)
	{
		// ����δ�����б�
		if (g_mapDXTextureExport.find(tex) == g_mapDXTextureExport.end()) {
			g_mapDXTextureExport.insert(std::pair<IDirect3DTexture8*, BOOL>(tex, FALSE));
		}

		// �ж��Ƿ��Ѿ�����
		//OutputDebugString("�ж��Ƿ��Ѿ�����....");
		if (!g_mapDXTextureExport[tex])
		{
			//OutputDebugString("��ʼ����ͼƬ....");
			nLevelCount = pTexture->GetLevelCount();
			_tcsncpy(chExportFile, m_chExportPath, MAX_PATH);
			len = (int)_tcslen(chExportFile);
			if ((len > 0) && (len < MAX_PATH) && chExportFile[len - 1] != '\\') {
				_tcscat(chExportFile, _T("\\"));
			}

			_stprintf(chFileName, _T("0x%p_%08X_%d_mipmap%02d.dds"), pTexture, GetCurrentThreadId(), sCounter, nLevelCount);
			//_stprintf(chFileName, _T(" %d.dds"), num );
			_tcscat(chExportFile, chFileName);_stprintf(chFileName, _T("0x%p_%08X_%d_mipmap%02d.dds"), pTexture, GetCurrentThreadId(), sCounter, nLevelCount);
			chExportFile[MAX_PATH - 1] = '\0';
			
			//OutputDebugString("����ͼƬ....");
			hr = D3DXSaveTextureToFile(
					chExportFile,
					D3DXIFF_DDS,
					pTexture,
					NULL);
			
			if (SUCCEEDED(hr))
			{
				g_mapDXTextureExport[tex] = TRUE;
				//OutputDebugString("D3DXSaveTextureToFile����ͼƬ�ɹ���");
				//num++;
			}
			else
			{
				g_mapDXTextureExport[tex] = FALSE;
				//OutputDebugString("D3DXSaveTextureToFile����ͼƬʧ�ܣ�");
			}
		}
	}

	//OutputDebugString("��ʼ׼���滻ͼƬ....");
	BOOL bReplaced;
	if (m_bEnableScan && m_bRegistered) // ͼ��ɨ�迪��
	{
		// ����δɨ���б�
		if (g_mapDXTextureScan.find(tex) == g_mapDXTextureScan.end()) {
			g_mapDXTextureScan.insert(std::pair<IDirect3DTexture8*, BOOL>(tex, FALSE));
		}

		// �ж��Ƿ��Ѿ�ɨ���
		//OutputDebugString("�ж��Ƿ��Ѿ�ɨ���....");
		if (!g_mapDXTextureScan[tex])
		{
			// 2D ���Ͷ�Ų���
			//OutputDebugString("��ʼѰ��Ҫ�滻��ͼƬ....");
			CIGA2DAdsList::iterator p = g_listIGA2DAds.begin();
			while (p != g_listIGA2DAds.end())
			{
				bReplaced = FALSE;
				CIDA2D_ADS& ads = *p;
				
				//.............
				//if ( ads.match_tex == NULL)
				//{
				//	OutputDebugString("ads.match_tex == NULL");
				//}
				//.................
				if (ads.match_tex != NULL)
				{
					//OutputDebugString("����Ҫ�滻��ͼƬ....");
					if (ads.level == -1 && ads.adslevel == -1)
					{
						// �����滻
						//OutputDebugString("������ȫ�滻....");
						//OutputDebugString("��ʼƥ�����....");
						if (CompareTexture(0, tex, 0, ads.match_tex)) // �Ƚ�level 0��������ͼ
						{
							//OutputDebugString("�滻ͼƬ���в�....");
							if (ReplaceTextureAllLevel(m_pIDirect3DDevice8, tex, ads.ads_tex)) 
							{
							}
							bReplaced = TRUE;
						}
					}
					else
					{
						// �����滻
						//OutputDebugString("���е����滻....");
						//OutputDebugString("��ʼƥ�����....");
						if (CompareTexture(0, tex, 0, ads.match_tex)) // �Ƚ�level 0��������ͼ
						{
							//OutputDebugString("�滻ͼƬ��һ��....");
							if (ReplaceTexture(m_pIDirect3DDevice8, ads.level, tex, ads.adslevel, ads.ads_tex)) 
							{
							}
							bReplaced = TRUE;
						}
					}
				}
				// �滻�ɹ����Ƴ���档
				if (bReplaced) 
				{
					//OutputDebugString("ͼƬ�滻�ɹ���");
					break;
				}
				p++;
			}
			g_mapDXTextureScan[tex] = TRUE;
		}
	}
	//sprintf(text, "%d �� ", num-1);
	//OutputDebugString(text);
	//OutputDebugString("-----------------------------");
	return res;
}

void CIGACoreImpl::OnGetSurfaceLevel(IDirect3DTexture8* pTexture, UINT Level, IDirect3DSurface8* pSurfaceLevel)
{
	
}

void CIGACoreImpl::OnBeginScene()
{
	
}

void CIGACoreImpl::OnEndScene()
{
	
}

void CIGACoreImpl::OnSurfaceLockRect(IDirect3DSurface8* pSurface, D3DLOCKED_RECT* pLockedRect,CONST RECT* pRect,DWORD Flags)
{
	
}

void CIGACoreImpl::OnSurfaceUnlockRect(IDirect3DSurface8* pSurface)
{

}