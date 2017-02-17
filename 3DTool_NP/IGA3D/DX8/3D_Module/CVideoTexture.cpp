#include "stdafx.h"

CVideoTexture::CVideoTexture() : 
	CBaseVideoRenderer( __uuidof(CLSID_VideoTexture),NAME("Video Texture"), NULL, NULL)
{
	m_pMediaControl  = NULL;
	m_pMediaPosition = NULL;
	m_pMediaEvent    = NULL;
	m_pGraphBuilder  = NULL;
	m_pTexture       = NULL;
}

CVideoTexture::~CVideoTexture()
{
	//Stop the video
	if (m_pMediaControl)
	{
		m_pMediaControl->Stop();
		m_pMediaControl.Release();
	}

	if (m_pMediaEvent)
		m_pMediaEvent.Release();

	if (m_pMediaPosition)
		m_pMediaPosition.Release();


	//Let the graph builder clean everything up
	if (m_pGraphBuilder)
		m_pGraphBuilder.Release();

	//Clean up the texture
	if (m_pTexture)
	{
		m_pTexture->Release();
		m_pTexture = NULL;
	}
}
HRESULT CVideoTexture::CreateFromAVIFile(LPDIRECT3DDEVICE8 pDevice, TCHAR *pFileName)
{
	//Make sure the device is set
	//�����豸ָ��
	m_pDevice = pDevice;

	//Reformat the filename parameter so that it will be accepted by AddSourceFilter
	//��·���ַ�ת��Ϊ���ַ�
	WCHAR wFileName[MAX_PATH];
	MultiByteToWideChar(CP_ACP, 0, pFileName, -1, wFileName, MAX_PATH);

	//These are some working objects used to build the filter graph
	//����Դ�����������롢����Ͽڣ����ţ�
	CComPtr<IPin>           pPinIn;
	CComPtr<IBaseFilter>    pSourceFilter;
	CComPtr<IPin>           pPinOut;

	//Add a reference so that the graph builder does not delete this object.
	//That causes badness because the object is destroyed too early
	//ΪCOM����������ü���
	AddRef();

	//initialize the COM library
	//��ʼ��COMϵͳ
	CoInitialize(NULL) ;
			
	//OutputDebugString("DirectShow Start");
	// Create the filter graph
	//�ȴ����˲���ͼ���˲���������
	if (SUCCEEDED(CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER, IID_IFilterGraph, (void **)&m_pGraphBuilder)))
	{
		//OutputDebugString("Step 1");
		//Add this filter to the bag of tricks
		//���˲������������һ���˲������������һ���˲�����
		if (SUCCEEDED(m_pGraphBuilder->AddFilter(this, L"TEXTURERENDERER")))
		{
			//OutputDebugString("Step 2");

			//Add a source filter for the AVI file
			//ΪAVI�ļ����һ��Դ�˲���������ת��AVI�ļ����ݣ����ҽ�ת��������ݷ��͵���һ���˲���
			//���˲������������Դ�˲�����ͨ���ļ�����������ӣ�����DDRAWϵͳ��Ѱ���ܹ�ƥ�䵱ǰ�ļ����͵��˲���������
			if (SUCCEEDED(m_pGraphBuilder->AddSourceFilter (wFileName, L"SOURCE", &pSourceFilter)))
			{
				//OutputDebugString("Step 3");

				//Get the input pin for this filter
				//�����˲������������ţ����Ǳ�����������ڣ�Դ��������������ݣ�AVI���ݣ�������˿�
				if (SUCCEEDED(FindPin(L"In", &pPinIn)))
				{
					//OutputDebugString("Step 4");

					//Get the output pin of the source filter
					//����Դ�˲�����������ţ�Դ�˲���������AVI���ݴ��������
					if (SUCCEEDED(pSourceFilter->FindPin(L"Output", &pPinOut)))
					{
						//OutputDebugString("Step 4");


						//Let the graph builder figure out how to connect them
						//���˲��������ࣩ����ں�Դ�˲������������������������һ���˲���ϵͳ������˲�����������
						if (SUCCEEDED(m_pGraphBuilder->Connect(pPinOut, pPinIn)))
						{
							//OutputDebugString("Step 5");

							//If the graph was successful, retreive all the interfaces
							//��ȡ����˲���ϵͳ�Ŀ��ƽӿ�
							m_pGraphBuilder.QueryInterface(&m_pMediaControl);	//����AVI��Ƶ��������ֹͣ�ȹ�����
							m_pGraphBuilder.QueryInterface(&m_pMediaEvent);		//ý���¼��ӿ�
							m_pGraphBuilder.QueryInterface(&m_pMediaPosition);	//����AVI��Ƶ�Ĳ��Ŷ�λ

							//This is for audio. Some files may not have audio and the following
							//code may not work for all media types. Check the graph builder tool
							//����һ���ֽ��������ڷ�����Ƶ���ݣ���Ƶ���ݴ�pAudioPin�����������
							//CComPtr<IBaseFilter>    pSplitterFilter;
							//CComPtr<IPin>           pAudioPin;

							////Find the a/v splitter in the graph
							////���˲�����������Ѱ����Ƶ�ֽ���
							//if (SUCCEEDED(m_pGraphBuilder->FindFilterByName(L"AVI Splitter", &pSplitterFilter)))

							////if (SUCCEEDED(m_pGraphBuilder->FindFilterByName(L"Windows Media Video Decoder", &pSplitterFilter)))
							//{
							//	OutputDebugString("Step 6");

							//	//Find the pin for the first audio stream and render it. If it's not there,
							//	//it's OK, just move on...
							//	//��������Ƶ�ֽ�����Ѱ������ڣ���ɺ�ֱ�Ӱ���Ƶ�����͵��������в���
							//	if (SUCCEEDED(pSplitterFilter->FindPin(L"Stream 01", &pAudioPin))){
							//		OutputDebugString("Step 6-1");
							//		m_pGraphBuilder->Render(pAudioPin);
							//	}
							//}

							//If everything was successful, start retrieving frames
							//��ʼ��ȡ��Ƶ����
							if (SUCCEEDED(m_pMediaControl->Run())) 
							{
								//OutputDebugString("All OK");
								return S_OK;
							}
						}
					}
				}
			}
		}
	}

	//If you're here, something was bad
	return E_FAIL;
}

void CVideoTexture::CheckForLoop()
{
	long lEventCode;
	long lParam1;
	long lParam2;

	//If the video is complete, loop back to the start
	m_pMediaEvent->GetEvent(&lEventCode, &lParam1, &lParam2, 0);
	if (EC_COMPLETE == lEventCode)
		m_pMediaPosition->put_CurrentPosition(0);
}

HRESULT CVideoTexture::CheckMediaType(const CMediaType *pmt)
{
	//Only accept video stuff
	if(*pmt->FormatType() != FORMAT_VideoInfo) {
		//OutputDebugString("CheckMediaType E_INVALIDARG");
		return E_INVALIDARG;
	}

	//Only accept RGB24
	if(IsEqualGUID(*pmt->Type(),    MEDIATYPE_Video)  &&
		IsEqualGUID(*pmt->Subtype(), MEDIASUBTYPE_RGB24))
	{
		//OutputDebugString("CheckMediaType S_OK");
		return S_OK;
	}
	//OutputDebugString("CheckMediaType E_FAIL");
	return E_FAIL;
}

HRESULT CVideoTexture::SetMediaType(const CMediaType *pmt)
{
	//The video format is in the form of a bitmap info header.
	//Use the bitmap info to set the width and height of the texture
	VIDEOINFO *pVideoFormat = (VIDEOINFO *)pmt->Format();
	m_VideoWidth  = pVideoFormat->bmiHeader.biWidth;
	m_VideoHeight = abs(pVideoFormat->bmiHeader.biHeight);

	//The pitch is based on an RGB24 bitmap
	m_VideoPitch = (m_VideoWidth * 3 + 3) & ~(3);

	// Create the texture that maps to this media type
	if(FAILED(D3DXCreateTexture(m_pDevice, m_VideoWidth, m_VideoHeight, 1, 0, 
		D3DFMT_A8R8G8B8, D3DPOOL_MANAGED, &m_pTexture)))
		return E_FAIL;
	
	//Make sure that we actually got the format we asked for
	D3DSURFACE_DESC SurfaceDescription;
	m_pTexture->GetLevelDesc(0, &SurfaceDescription);
	if (SurfaceDescription.Format != D3DFMT_A8R8G8B8) 
		return VFW_E_TYPE_NOT_ACCEPTED;

	m_TextureWidth  = SurfaceDescription.Width; 
	m_TextureHeight = SurfaceDescription.Height;

	return S_OK;
}

HRESULT CVideoTexture::DoRenderSample(IMediaSample *pMediaSample)
{
	BYTE  *pVideoBuffer;
	BYTE  *pTextureBuffer;
	long  TexturePitch;

	//The the image buffer for the video frames
	pMediaSample->GetPointer(&pVideoBuffer);

	//Lock the Texture buffer for writing
	D3DLOCKED_RECT LockedRect;
	if (FAILED(m_pTexture->LockRect(0, &LockedRect, 0, 0)))
		return E_FAIL;

	// Get the texture buffer & pitch
	pTextureBuffer = static_cast<byte *>(LockedRect.pBits);
	TexturePitch = LockedRect.Pitch;

	//If we've gotten this far, we are dealing with 24bit frames.
	//Walk through row by row
	
	pTextureBuffer = pTextureBuffer + TexturePitch*m_VideoHeight;
	byte	tmp = 0x10;
	for(long y = 0; y < m_VideoHeight; y++) 
	{
		BYTE *pVideoRowStart = pVideoBuffer;
		BYTE *pTextureRowStart = pTextureBuffer;  

		//walk through each pixel of the row
		for (int x = 0; x < m_VideoWidth; x++) 
		{
			pTextureBuffer[0] = pVideoBuffer[0];
			pTextureBuffer[1] = pVideoBuffer[1];
			pTextureBuffer[2] = pVideoBuffer[2];
			pTextureBuffer[3] = 0xff;
			if( pTextureBuffer[0] + tmp < 254 )
			{
				pTextureBuffer[0] += tmp;
			}
			if( pTextureBuffer[1] + tmp < 254 )
			{
				pTextureBuffer[1] += tmp;
			}			
			if( pTextureBuffer[2] + tmp < 254 )
			{
				pTextureBuffer[2] += tmp;
			}
			pTextureBuffer += 4;
			pVideoBuffer   += 3;
		}
		//Increment to the next row
		pVideoBuffer   = pVideoRowStart   + m_VideoPitch;
		pTextureBuffer = pTextureRowStart - TexturePitch;
	}

	//unlock the texture
	if (FAILED(m_pTexture->UnlockRect(0)))
		return E_FAIL;
	return S_OK;
}
