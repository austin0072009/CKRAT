#if !defined(AFX_WEBCAMDLG_H__C03CAC35_55A8_4998_B6BD_73D20EFC108A__INCLUDED_)
#define AFX_WEBCAMDLG_H__C03CAC35_55A8_4998_B6BD_73D20EFC108A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WebCamDlg.h : header file
//
#include <list>
#include <vector>
#include <Vfw.h>
#include "BmpToAvi.h"
//#include "XvidDec.h"
/////////////////////////////////////////////////////////////////////////////
// CWebCamDlg dialog
class ResolutionInfo
{
public:
	int m_DeviceIndex;
	int m_iWidth;
	int m_iHeight;
};
class CWebCamDlg : public CDialog
{
// Construction
public:
	CWebCamDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CWebCamDlg)
	enum { IDD = IDD_WEBCAM };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA
	void OnReceive();
	void OnReceiveComplete();

	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	HICON m_hIcon;
	CString m_IPAddress;
	UINT m_nCount; // �ڼ�֡
	char* m_deviceList;
	int m_nWebcamSelected; // ѡ����Ƶ�豸��id
	int m_nDeviceNums; // �豸����
	int m_iResNum;
	std::vector<ResolutionInfo> m_resInfo;// ��֪��ɶ��˼
	LPBITMAPINFO m_lpbmi;
	MINMAXINFO m_MMI;

	HDRAWDIB	m_hDD;
	HDC			m_hDC;
	LPVOID m_lpScreenDIB;

	CString m_aviFile; // ¼���ļ���������ļ�����Ϊ�վ�д��
	CBmpToAvi	m_aviStream;

	int	m_nOldWidth; // OnSizeʱ�ж��Ǹ߻�����仯��������

//	CXvidDec *	m_dec ; 

	BOOL m_bOnClose;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWebCamDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	void DrawDIB();

	void ResetScreen();
	void InitMMI();
	LRESULT OnGetMiniMaxInfo(WPARAM, LPARAM);
	bool SendResetScreen(int nWidth, int nHeight); // ��������ı���Ƶ��С

	bool SaveSnapshot();
	void SaveAvi();

	// Generated message map functions
	//{{AFX_MSG(CWebCamDlg)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEBCAMDLG_H__C03CAC35_55A8_4998_B6BD_73D20EFC108A__INCLUDED_)
