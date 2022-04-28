#if !defined(AFX_TOOLS_H__C9E9ECE4_A167_423D_8C59_0438780A6068__INCLUDED_)
#define AFX_TOOLS_H__C9E9ECE4_A167_423D_8C59_0438780A6068__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Tools.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTools dialog
#include "Icons.h"
class CTools : public CDialog
{
// Construction
public:
	CTools(CWnd* pParent = NULL);   // standard constructor
	LPICONRESOURCE lpIR;  // ICON×ÊÔ´Ö¸Õë
	EXEDLLICONINFO EDII;
	CString szOpenFileName,szOpenFileExtName,szSaveFileName,szSaveFileExtName;
// Dialog Data
	//{{AFX_DATA(CTools)
	enum { IDD = IDD_DIALOG_TOOL };
	CSliderCtrl	m_Sllider;
	CListBox	m_List;
	//}}AFX_DATA


// Overrides
	virtual BOOL ChangeExeIcon(LPSTR lpIconFile, LPSTR lpExeName);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTools)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CTools)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnChoiceico();
	afx_msg void OnChoiceexe();
	afx_msg void OnModifyicon();
	afx_msg void OnExtractIcon();
	afx_msg void OnPreservationIcon();
	afx_msg void OnSelchangeListIcon();
	afx_msg void OnChoiceExefile();
	afx_msg void OnOutofmemoryFileSize(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnIncrease();
	afx_msg void GetRandString(LPSTR szStr,DWORD len);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TOOLS_H__C9E9ECE4_A167_423D_8C59_0438780A6068__INCLUDED_)
