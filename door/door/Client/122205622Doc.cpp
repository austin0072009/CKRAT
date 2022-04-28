// WindsDoc.cpp : implementation of the CWindsDoc class
//

#include "stdafx.h"
#include "122205622.h"

#include "122205622Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindsDoc

IMPLEMENT_DYNCREATE(CWindsDoc, CDocument)

BEGIN_MESSAGE_MAP(CWindsDoc, CDocument)
	//{{AFX_MSG_MAP(CWindsDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindsDoc construction/destruction

CWindsDoc::CWindsDoc()
{
	// TODO: add one-time construction code here

}

CWindsDoc::~CWindsDoc()
{
}

BOOL CWindsDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CWindsDoc serialization

void CWindsDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CWindsDoc diagnostics

#ifdef _DEBUG
void CWindsDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CWindsDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWindsDoc commands
