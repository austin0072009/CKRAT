// Audio.h: interface for the CAudio class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AUDIO_H__4DD286A3_85E9_4492_A1A5_C3B2D860BD1A__INCLUDED_)
#define AFX_AUDIO_H__4DD286A3_85E9_4492_A1A5_C3B2D860BD1A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")

/*
SIZE_AUDIO_FRAME  �� 160 ����
SIZE_AUDIO_PACKED �� ��Ӧ����� Ϊ SIZE_AUDIO_FRAME / 160 * 10
*/

#define SIZE_AUDIO_FRAME 16000
#define SIZE_AUDIO_PACKED 1500	// ��Ƶ���Ĵ�С
class CAudio
{
public:
	CAudio();
	virtual ~CAudio();
	int m_nBufferLength;

	LPBYTE m_lpInAudioData[2]; // ��������������
	LPBYTE m_lpOutAudioData[2];

	HWAVEIN m_hWaveIn;
	int m_nWaveInIndex;
	int m_nWaveOutIndex;

	HANDLE	m_hEventWaveIn; // MM_WIM_DATA����
	HANDLE	m_hStartRecord; // getRecordBuffer ���غ󴥷�

	int m_SelectedDevice;
    void SetDeviceType(WAVEFORMATEX* pwfe);
	// ���� �����豸�� ö��������· ��������ʹ�õ���·�±�
	int EnumerateInputLines( char * szPname,  char * str );
	bool ActiveMixerInputChannel(char * szMixerDevice, int nMixerLineIndex);

	LPBYTE getRecordBuffer(LPDWORD lpdwBytes);
	bool playBuffer(LPBYTE lpWaveBuffer, DWORD dwBytes);
private:
	HANDLE	m_hThreadCallBack;

	LPWAVEHDR m_lpInAudioHdr[2];
	LPWAVEHDR m_lpOutAudioHdr[2];

	HWAVEOUT m_hWaveOut;
	
	bool	m_bIsWaveInUsed;
	bool	m_bIsWaveOutUsed;

	bool InitializeWaveIn();
	bool InitializeWaveOut();

	static DWORD WINAPI waveInCallBack(LPVOID lparam);

public:
    // ��ѹ
	int DeCompress(char *pBuf, char *pDstBuf, int nLen);
	// ѹ��
	int Compress(char *pBuf,char* pDstBuf, int nLen);
};

#endif // !defined(AFX_AUDIO_H__4DD286A3_85E9_4492_A1A5_C3B2D860BD1A__INCLUDED_)
