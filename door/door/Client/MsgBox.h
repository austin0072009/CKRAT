#if !defined(AFX_MSGBOX_H__1B14AD79_662A_4000_89F1_A1A2A61E5E7C__INCLUDED_)
#define AFX_MSGBOX_H__1B14AD79_662A_4000_89F1_A1A2A61E5E7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MsgBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgBox dialog

class CMsgBox : public CDialog
{
// Construction
public:
	CMsgBox(CWnd* pParent = NULL);   // standard constructor
	UINT m_msgButton;
	UINT m_msgIcon;
// Dialog Data
	//{{AFX_DATA(CMsgBox)
	enum { IDD = IDD_MSGBOX };

	CStatic	m_Icons1;
	CStatic	m_Icons2;
	CStatic	m_Icons3;
	CStatic	m_Icons4;
	CString	m_Title;
	CString	m_Text;

	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgBox)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CMsgBox)
	virtual BOOL OnInitDialog();
//	afx_msg void OnSend();
	afx_msg void OnShow();
	afx_msg void OnRadio12();
	afx_msg void OnRadio11();
	afx_msg void OnRadio10();
	afx_msg void OnRadio7();
	afx_msg void OnRadio13();
	afx_msg void OnRadio5();
	afx_msg void OnRadio8();
	afx_msg void OnRadio1();
	afx_msg void OnRadio2();
	afx_msg void OnRadio3();
	afx_msg void OnRadio4();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGBOX_H__1B14AD79_662A_4000_89F1_A1A2A61E5E7C__INCLUDED_)
