#if !defined(AFX_PCVIEW_H__F0FEB613_C6C2_4AB1_AAA5_A4D9675DA86E__INCLUDED_)
#define AFX_PCVIEW_H__F0FEB613_C6C2_4AB1_AAA5_A4D9675DA86E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PcView.h : header file
//
#include "MySuperGrid.h"
/////////////////////////////////////////////////////////////////////////////
// CPcView view

class CPcView : public CView
{
protected:
	CPcView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPcView)

	// Attributes
public:
	CBitmap my_Bit;
	// Operations
public:

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPcView)
protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	static void SetClipboardText(CString &Data);
	//}}AFX_VIRTUAL

	// Implementation
public:
	CMySuperGrid m_List;
	virtual ~CPcView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CPcView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnFilemanager();
	afx_msg void OnScreenspy();
	afx_msg void OnKeyboard();
	afx_msg void OnRemoteshell();
	afx_msg void OnSystem();
	afx_msg void OnWebcam();
	afx_msg void OnAudioListen();
	afx_msg void OnRegedit();
	afx_msg void OnServices();
	afx_msg void OnBuild();
	afx_msg void OnRemark();
	afx_msg void OnChangeGroup();
	afx_msg void OnRemove();
	afx_msg void OnLogoff();
	afx_msg void OnReboot();
	afx_msg void OnShutdown();
	afx_msg void OnDownexec();
	afx_msg void OnUpdateServer();
	afx_msg void OnCleanevent();
	afx_msg void OnOpenUrlHide();
	afx_msg void OnOpenUrlShow();
	afx_msg void OnSelectAll();
	afx_msg void OnUnselectAll();
	afx_msg void OnLanProxy();
	afx_msg void OnStartHide();
	afx_msg void OnStartNormal();
	afx_msg void OnNoStart();
	afx_msg void OnDisconnect();
	afx_msg void OnCreateGroup();
	afx_msg void OnDeleteGroup();
	afx_msg void OnSelgroup();
	afx_msg void OnFindProcess();
	afx_msg void OnFindWindow();
	afx_msg void OnShowmsg();
	afx_msg void OnShell();
	afx_msg void OnSysteminfo();
	afx_msg void OnQqinfo();
	afx_msg void OnStart();
	afx_msg void OnProxyMap();
	afx_msg void OnProxy();
	afx_msg void OnDownload();
	afx_msg void OnOpenweb();

	afx_msg void OnCopyWanip();
	afx_msg void OnCopyAll();

	afx_msg void OnUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PCVIEW_H__F0FEB613_C6C2_4AB1_AAA5_A4D9675DA86E__INCLUDED_)
