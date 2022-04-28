#if !defined(AFX_CHANGEGROUP_H__985B747B_A75E_4F88_B55B_2B8827217B3C__INCLUDED_)
#define AFX_CHANGEGROUP_H__985B747B_A75E_4F88_B55B_2B8827217B3C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ChangeGroup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CChangeGroup dialog

class CChangeGroup : public CDialog
{
// Construction
public:
	CChangeGroup(CWnd* pParent = NULL);   // standard constructor
	CString strGroup;
	
// Dialog Data
	//{{AFX_DATA(CChangeGroup)
	enum { IDD = IDD_CHANGE_GROUP };
	CComboBox	m_combo_group;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CChangeGroup)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CChangeGroup)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHANGEGROUP_H__985B747B_A75E_4F88_B55B_2B8827217B3C__INCLUDED_)
