// Client.h : main header file for the CLIENT application
//

#if !defined(AFX_CLIENT_H__0E0D0AD0_4C7F_44CF_82F2_C9EF6B4D5524__INCLUDED_)
#define AFX_CLIENT_H__0E0D0AD0_4C7F_44CF_82F2_C9EF6B4D5524__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols
#include "IniFile.h"
/////////////////////////////////////////////////////////////////////////////
// CClientApp:
// See Client.cpp for the implementation of this class
//

class CClientApp : public CWinApp
{
public:
	CClientApp();
	CIniFile	m_IniFile;
	bool m_bIsQQwryExist;
	CHAR AppPath[MAX_PATH];
	HMODULE m_Hmodule;
	DWORD GetRand();
	CHAR QQwryPath[MAX_PATH];

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CClientApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENT_H__0E0D0AD0_4C7F_44CF_82F2_C9EF6B4D5524__INCLUDED_)
