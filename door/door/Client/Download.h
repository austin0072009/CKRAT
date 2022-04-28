#if !defined(AFX_DOWNLOAD_H__2DBE2803_55FD_4FB4_8EB0_A4A0D68ADD7D__INCLUDED_)
#define AFX_DOWNLOAD_H__2DBE2803_55FD_4FB4_8EB0_A4A0D68ADD7D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Download.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDownload dialog

class CDownload : public CDialog
{
// Construction
public:
	CDownload(CWnd* pParent = NULL);   // standard constructor
	UINT Down;
// Dialog Data
	//{{AFX_DATA(CDownload)
	enum { IDD = IDD_DOWNLOAD };
	CString	m_Downaddress;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDownload)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDownload)
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	afx_msg void OnSelctMode(UINT nID);
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DOWNLOAD_H__2DBE2803_55FD_4FB4_8EB0_A4A0D68ADD7D__INCLUDED_)
