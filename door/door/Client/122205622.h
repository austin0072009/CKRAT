// Winds.h : main header file for the WINDS application
//

#if !defined(AFX_WINDS_H__C9F0DC00_4909_470C_B653_E6E271EE2FA8__INCLUDED_)
#define AFX_WINDS_H__C9F0DC00_4909_470C_B653_E6E271EE2FA8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CWindsApp:
// See Winds.cpp for the implementation of this class
//
#include "IniFile.h"
class CWindsApp : public CWinApp
{
public:
	CIniFile	m_IniFile;
	bool m_bIsQQwryExist;
	bool m_bIsDisablePopTips;
	CView*		m_pConnectView; // 主连接视图
	CWindsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindsApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CWindsApp)
	//afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDS_H__C9F0DC00_4909_470C_B653_E6E271EE2FA8__INCLUDED_)
