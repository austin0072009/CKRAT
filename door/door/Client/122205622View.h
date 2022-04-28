// WindsView.h : interface of the CWindsView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDSVIEW_H__16B6391C_8762_4C38_8813_3E0A36652850__INCLUDED_)
#define AFX_WINDSVIEW_H__16B6391C_8762_4C38_8813_3E0A36652850__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "MySuperGrid.h"
#include "122205622Doc.h"

class CWindsView : public CView
{
protected: // create from serialization only
	CWindsView();
	DECLARE_DYNCREATE(CWindsView)

// Attributes
public:
	CXTPTabControl m_wndTabControl;
	CWindsDoc* GetDocument();
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindsView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	void UpdateDocTitle();
	virtual ~CWindsView();
//	CMySuperGrid m_List;
	CIOCPServer* m_iocpServer;
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWindsView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
private:
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle);
};

#ifndef _DEBUG  // debug version in WindsView.cpp
inline CWindsDoc* CWindsView::GetDocument()
   { return (CWindsDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDSVIEW_H__16B6391C_8762_4C38_8813_3E0A36652850__INCLUDED_)
