#if !defined(AFX_LOGIN_H__7E130239_8193_4761_8556_B81F6251EC0E__INCLUDED_)
#define AFX_LOGIN_H__7E130239_8193_4761_8556_B81F6251EC0E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Login.h : header file
//
#include "PictureEx.h"
/////////////////////////////////////////////////////////////////////////////
// CLogin dialog

class CLogin : public CDialog
{
// Construction
public:
	CLogin(CWnd* pParent = NULL);   // standard constructor
	DWORD dLogin;
	CIniFile	m_IniFile;
	CPictureEx	m_Picture;
//	CString InternetGetInfo(CString szUrl);
// Dialog Data
	//{{AFX_DATA(CLogin)
	enum { IDD = IDD_LOGIN };
	CString	m_UserName;
	CString	m_UserPass;
	CString	m_HostName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLogin)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CLogin)
	afx_msg void OnLogin();
	afx_msg void OnBtnHide();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LOGIN_H__7E130239_8193_4761_8556_B81F6251EC0E__INCLUDED_)
