// WindsDoc.h : interface of the CWindsDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_WINDSDOC_H__23C7D517_249B_4984_A9C8_A3F71D4495D5__INCLUDED_)
#define AFX_WINDSDOC_H__23C7D517_249B_4984_A9C8_A3F71D4495D5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CWindsDoc : public CDocument
{
protected: // create from serialization only
	CWindsDoc();
	DECLARE_DYNCREATE(CWindsDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWindsDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWindsDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CWindsDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WINDSDOC_H__23C7D517_249B_4984_A9C8_A3F71D4495D5__INCLUDED_)
