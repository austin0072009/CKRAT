#if !defined(AFX_OPENWEB_H__A768A33C_5C12_4A8F_B16D_9C1181B1EC4A__INCLUDED_)
#define AFX_OPENWEB_H__A768A33C_5C12_4A8F_B16D_9C1181B1EC4A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Openweb.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// COpenweb dialog

class COpenweb : public CDialog
{
// Construction
public:
	COpenweb(CWnd* pParent = NULL);   // standard constructor
	UINT Webtype;;
// Dialog Data
	//{{AFX_DATA(COpenweb)
	enum { IDD = IDD_OPENWEB };
	CString	m_Webaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(COpenweb)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(COpenweb)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnOpenWEB(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_OPENWEB_H__A768A33C_5C12_4A8F_B16D_9C1181B1EC4A__INCLUDED_)
