#if !defined(AFX_SORTLISTCTRL_H__C1111AF4_B95F_4E2D_9D87_26D3EF5A34E6__INCLUDED_)
#define AFX_SORTLISTCTRL_H__C1111AF4_B95F_4E2D_9D87_26D3EF5A34E6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// SortListCtrl.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSortListCtrl window

class CSortListCtrl : public CListCtrl
{
// Construction
public:
	CSortListCtrl();
	BOOL m_fAsc;//�Ƿ�˳������
	int m_nSortedCol;//��ǰ�������

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSortListCtrl)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSortListCtrl();

	// Generated message map functions
protected:
	//{{AFX_MSG(CSortListCtrl)
	//�ֹ�������Ϣӳ�䣬��������ť�����µ���Ϣ
	afx_msg void OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	//�ֹ�������Ϣӳ�䣬��������ť�����µ���Ϣ

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SORTLISTCTRL_H__C1111AF4_B95F_4E2D_9D87_26D3EF5A34E6__INCLUDED_)
