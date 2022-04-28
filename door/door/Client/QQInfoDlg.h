#if !defined(AFX_QQINFODLG_H__DEA04B54_3EF5_40F7_81DB_9C5CF780E723__INCLUDED_)
#define AFX_QQINFODLG_H__DEA04B54_3EF5_40F7_81DB_9C5CF780E723__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// QQInfoDlg.h : header file
//
#include "cJSON.h"
/////////////////////////////////////////////////////////////////////////////
// CQQInfoDlg dialog

class CQQInfoDlg : public CDialog
{
// Construction
public:
	void OnReceiveComplete();
	CQQInfoDlg(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CQQInfoDlg)
	enum { IDD = IDD_QQINFO };
	CTreeCtrl	m_treegroup;
	CTreeCtrl	m_treefriend;
	CListBox	m_listuser;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQQInfoDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CQQInfoDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnBtnUser();
	afx_msg void OnBtnFriend();
	afx_msg void OnBtnGroup();
	afx_msg void OnDblclkListUser();
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	void AddTreeFriendByNumber(CString strQQNumber);
	void AddTreeGroupByNumber(CString strQQNumber);
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	cJSON *m_pQQLocalJson;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QQINFODLG_H__DEA04B54_3EF5_40F7_81DB_9C5CF780E723__INCLUDED_)
