// CaptureVideo.h: interface for the CCaptureVideo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CAPTUREVIDEO_H__D9B9F1BB_9049_48A7_B185_0CB960BA76E3__INCLUDED_)
#define AFX_CAPTUREVIDEO_H__D9B9F1BB_9049_48A7_B185_0CB960BA76E3__INCLUDED_
#include <list>
#include <DShow.h>

#include <qedit.h>
#include "XvidEnc.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// 0 ���� 1 ���� 2 ���� 
enum
{
	CMD_CAN_COPY,
	CMD_CAN_SEND
};

#define SAFE_RELEASE( x ) \
	if ( NULL != x ) \
{ \
	x->Release( ); \
	x = NULL; \
}

class SmartString
{
public:
	SmartString():str(NULL) {}
	
	SmartString(char* pStr):str(NULL)
	{
		if (pStr)
		{
			int size = int(strlen(pStr));
			str = new char[size+1];
			strcpy(str, pStr);
		}
	}
	
	SmartString(SmartString& sStr)
	{
		SetString(sStr.GetBuffer());
	}
	
	~SmartString()
	{
		if (str)
			delete[] str;
	}
	
	SmartString& operator =(char* pStr)
	{
		SetString(pStr);
		return *this;
	}
	
	SmartString& operator =(SmartString& sStr)
	{
		SetString(sStr.GetBuffer());
		return *this;
	}
	
	char* GetBuffer() {return str;}
	
protected:
	void SetString(char *pStr)
	{
		if (str)
			delete[] str;
		
		if (!pStr)
		{
			str = NULL;
		}
		else
		{
			int size = int(strlen(pStr));
			str = new char[size + 1];
			strcpy(str, pStr);
		}
	}
	
	char* str;
};

class ResolutionInfo
{
public:
	ResolutionInfo():next(NULL){}
	~ResolutionInfo()
	{
		if (next)
			delete next;
	}
	SmartString Resolution; //�ֱ���
	ResolutionInfo* next;
};

class DeviceInfo
{
public:
	DeviceInfo():next(NULL), deviceId(-1),resInfo(NULL)	{} 
	~DeviceInfo()
	{
		if (resInfo)
			delete resInfo;
		if (next)
			delete next;
	}
	
	SmartString friendlyName; //�豸��
	int			deviceId;	//��Ƶ�豸��������
	ResolutionInfo* resInfo;
	DeviceInfo* next;
};

class CSampleGrabberCB : public ISampleGrabberCB 
{
public:
	long				   lWidth ; 
	long				   lHeight ; 

	BITMAPINFO	*lpbmi;     //λͼ�ļ�ͷ 
	BYTE*   m_lpFullBits;   //ͼ������
	DWORD   dwSize;         //���ݴ�С
	BOOL    bStact;         //����״̬ 0���Ը���  1���Է���

public:
	CSampleGrabberCB()
	{ 
		lWidth = 0 ; 
		lHeight = 0 ; 

		lpbmi = NULL;
		m_lpFullBits = NULL;
		bStact = CMD_CAN_COPY;

		dwSize = 0;
	} 
	~CSampleGrabberCB()
	{
		if (m_lpFullBits)
		{
			delete[] m_lpFullBits;
		}
		if (lpbmi)
		{
			delete[] lpbmi;
		}
	}

	LPBITMAPINFO ConstructBI(int biBitCount, int biWidth, int biHeight)
	{
		
		int	color_num = biBitCount <= 8 ? 1 << biBitCount : 0;
		
		int nBISize = sizeof(BITMAPINFOHEADER) + (color_num * sizeof(RGBQUAD));
		lpbmi = (BITMAPINFO *) new BYTE[nBISize];
		
		BITMAPINFOHEADER	*lpbmih = &(lpbmi->bmiHeader);
		lpbmih->biSize = sizeof(BITMAPINFOHEADER);
		lpbmih->biWidth = biWidth;
		lpbmih->biHeight = biHeight;
		lpbmih->biPlanes = 1;
		lpbmih->biBitCount = biBitCount;
		lpbmih->biCompression = BI_RGB;
		lpbmih->biXPelsPerMeter = 0;
		lpbmih->biYPelsPerMeter = 0;
		lpbmih->biClrUsed = 0;
		lpbmih->biClrImportant = 0;
		lpbmih->biSizeImage = (((lpbmih->biWidth * lpbmih->biBitCount + 31) & ~31) >> 3) * lpbmih->biHeight;

		/////////////////////��ʼ����Ա����///////////////////////////////////////////////
		dwSize=lpbmih->biSizeImage;
		m_lpFullBits=new BYTE[dwSize+10];
		ZeroMemory(m_lpFullBits,dwSize+10);
		if (biBitCount >= 16)
			return lpbmi;
		
		return lpbmi;	
	}

	LPBITMAPINFO GetBmpInfo()	  
	{
		lpbmi=ConstructBI(24, lWidth, lHeight);
		return lpbmi;	  
	}
	BYTE* GetNextScreen(DWORD &nSize)       //�õ���һ֡���� 
	{
		nSize=dwSize;
		return (BYTE*)m_lpFullBits;      //���Ǻܼ� ���Ƿ���һ����������ָ�룬�����ָ������ô��ȡ����Ƶ���ݵ��أ�
	}

	STDMETHODIMP_(ULONG) AddRef() { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	STDMETHODIMP QueryInterface(REFIID riid, void ** ppv) 
	{
		if( riid == IID_ISampleGrabberCB || riid == IID_IUnknown )
		{ 
			*ppv = (void *) static_cast<ISampleGrabberCB*> ( this );
			return NOERROR;
		} 
		return E_NOINTERFACE;
	}
	
	STDMETHODIMP SampleCB( double SampleTime, IMediaSample * pSample ) 
	{
		return 0;
	}
	//�ص����� ������õ� bmp ������
	STDMETHODIMP BufferCB( double dblSampleTime, BYTE * pBuffer, long lBufferSize )
	{
		if (!pBuffer)
			return E_POINTER;
		
		if (bStact == CMD_CAN_COPY)         //δ��ʼ�� ���͵�ͬ�����һ��
		{
			ZeroMemory(m_lpFullBits,dwSize+10);

			memcpy(m_lpFullBits,pBuffer,lBufferSize);     //������õ���Ƶ�����ݣ���Ϊʲô�ܵõ���Ƶ�������أ���

			InterlockedExchange((LPLONG)&bStact,CMD_CAN_SEND);      //ԭ���������Է���  
		}
		return 0;
	}
};

class CCaptureVideo 
{
	friend class CSampleGrabberCB;
public:
	CCaptureVideo();
	virtual ~CCaptureVideo();

	CSampleGrabberCB mCB;
	
	int EnumDevices(DeviceInfo *head);
	LPBITMAPINFO GetBmpInfo();

    HRESULT Start(int iDeviceID,HWND hWnd = NULL, int width = 320, int height = 240);
	HRESULT Stop(); 
    HRESULT InitCaptureGraphBuilder();
    bool BindFilter(int deviceId, IBaseFilter **pFilter);
	void FreeMediaType(AM_MEDIA_TYPE& mt);

	LPBYTE GetDIB(DWORD& nSize);
	void SendEnd();                //���ͽ���

    ICaptureGraphBuilder2*	m_pCapture;
	IGraphBuilder *			m_pGB;
    IMediaControl*			m_pMC;
    IBaseFilter*            m_pBF;
	IBaseFilter*        	m_pNullRenderer;
    ISampleGrabber*			m_pGrabber;
	
	int m_nWidth ; // �ֱ��� �� X ��
	int m_nHeight;

	CXvidEnc *m_pEnc;
	BOOL m_bIsStarted;
};

#endif // !defined(AFX_CAPTUREVIDEO_H__D9B9F1BB_9049_48A7_B185_0CB960BA76E3__INCLUDED_)
