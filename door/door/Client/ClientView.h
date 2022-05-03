// ClientView.h : interface of the CClientView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_CLIENTVIEW_H__7C4A27DF_BBAF_4BB0_9FB9_F53C9933BD9D__INCLUDED_)
#define AFX_CLIENTVIEW_H__7C4A27DF_BBAF_4BB0_9FB9_F53C9933BD9D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ClientDoc.h"

class CClientView : public CView
{
protected: // create from serialization only
	CClientView();
	DECLARE_DYNCREATE(CClientView)

// Attributes
public:
	CClientDoc* GetDocument();
	void UpdateDocTitle();
	BOOL AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle);
	BOOL AddGroup( LPCTSTR lpszTitle );
	BOOL UpDateNumber();

// Operations
public:
	//CXTPTabControl m_wndTabControl;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CClientView)
	public:

	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CClientView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	CXTPTabManagerItem* pRightItem;

// Generated message map functions
protected:
	//{{AFX_MSG(CClientView)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMenuitemGroupAdd();
	afx_msg void OnMenuitemGroupDel();
	afx_msg void OnMenuitemChange();
	//}}AFX_MSG
	afx_msg void OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnAddFindGroup(WPARAM, LPARAM);

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ClientView.cpp
inline CClientDoc* CClientView::GetDocument()
   { return (CClientDoc*)m_pDocument; }
#endif
 
/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CLIENTVIEW_H__7C4A27DF_BBAF_4BB0_9FB9_F53C9933BD9D__INCLUDED_)
