#if !defined(AFX_FILEATTR_H__30881131_7A1C_433A_8D65_8D1A9C9ECB05__INCLUDED_)
#define AFX_FILEATTR_H__30881131_7A1C_433A_8D65_8D1A9C9ECB05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FileAttr.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFileAttr dialog

class CFileAttr : public CDialog
{
// Construction
public:
	CFileAttr(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CFileAttr)
	enum { IDD = IDD_FILEATTR };
	BOOL	m_bReadOnly;
	BOOL	m_bSystem;
	BOOL	m_bHidden;
	BOOL	m_bArchive;
	CString	m_strCTime;
	CString	m_strMTime;
	CString	m_strATime;
	CString	m_strSize;
	CString	m_FilePath;
	CString	m_FileName;
	//}}AFX_DATA

	HICON m_hIcon;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFileAttr)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	
	// Generated message map functions
	//{{AFX_MSG(CFileAttr)
	afx_msg void OnClose();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FILEATTR_H__30881131_7A1C_433A_8D65_8D1A9C9ECB05__INCLUDED_)
