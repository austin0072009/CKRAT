// WindsView.cpp : implementation of the CWindsView class
//

#include "stdafx.h"
#include "122205622.h"

#include "PcView.h"
#include "122205622Doc.h"
#include "122205622View.h"





#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CWindsView *g_pTabView;
#define IDC_TABCONTROL 100
/////////////////////////////////////////////////////////////////////////////
// CWindsView

IMPLEMENT_DYNCREATE(CWindsView, CView)

BEGIN_MESSAGE_MAP(CWindsView, CView)
	//{{AFX_MSG_MAP(CWindsView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindsView construction/destruction

CWindsView::CWindsView()
{
	// TODO: add construction code here
	((CWindsApp *)AfxGetApp())->m_pConnectView = this;
	g_pTabView = this;
}

CWindsView::~CWindsView()
{
}

BOOL CWindsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CWindsView drawing

void CWindsView::OnDraw(CDC* pDC)
{
	CWindsDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CWindsView diagnostics

#ifdef _DEBUG
void CWindsView::AssertValid() const
{
	CView::AssertValid();
}

void CWindsView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CWindsDoc* CWindsView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CWindsDoc)));
	return (CWindsDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CWindsView message handlers
BOOL CWindsView::AddView(CRuntimeClass* pViewClass, LPCTSTR lpszTitle)
{
	CCreateContext contextT;
	contextT.m_pCurrentDoc     = GetDocument();
	contextT.m_pNewViewClass   = pViewClass;
	contextT.m_pNewDocTemplate = GetDocument()->GetDocTemplate();
	
	CWnd* pWnd;
	TRY
	{
		pWnd = (CWnd*)pViewClass->CreateObject();
		if (pWnd == NULL)
		{
			AfxThrowMemoryException();
		}
	}
	CATCH_ALL(e)
	{
		TRACE0( "Out of memory creating a view.\n" );
		// Note: DELETE_EXCEPTION(e) not required
		return FALSE;
	}
	END_CATCH_ALL
		
		DWORD dwStyle = AFX_WS_DEFAULT_VIEW;
	dwStyle &= ~WS_BORDER;
	
	int nTab = m_wndTabControl.GetItemCount();
	
	// Create with the right size (wrong position)
	CRect rect(0, 0, 0, 0);
	if (!pWnd->Create(NULL, NULL, dwStyle,
		rect, &m_wndTabControl, (AFX_IDW_PANE_FIRST + nTab), &contextT))
	{
		TRACE0( "Warning: couldn't create client tab for view.\n" );
		// pWnd will be cleaned up by PostNcDestroy
		return NULL;
	}
	m_wndTabControl.InsertItem(nTab, lpszTitle, pWnd->GetSafeHwnd());
	pWnd->SetOwner(this);
// 	if (nTab != 0)
// 	{
// 		CPcView* pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(m_wndTabControl.GetItem(nTab)->GetHandle()));
// 		pView->NewInitialUpdate();
// 		pView->m_iocpServer = g_pConnectView->m_iocpServer;
// 	}
	return TRUE;
	
}

int CWindsView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
//	m_List.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);	
//	m_List.InitializeGrid();
	m_wndTabControl.Create(WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, IDC_TABCONTROL);
	
	int nPos = ((CWindsApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "GroupStyle", 3);
	XTPTabAppearanceStyle TabGroupStyle = (XTPTabAppearanceStyle)nPos;
	m_wndTabControl.GetPaintManager()->SetAppearance(TabGroupStyle);
	
	int nPos1 = ((CWindsApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "GroupPos", 2);
	XTPTabPosition TabPos = (XTPTabPosition)nPos1;
	m_wndTabControl.SetPosition(TabPos);
	
	m_wndTabControl.GetPaintManager()->m_bHotTracking = TRUE;
	m_wndTabControl.GetPaintManager()->DisableLunaColors(FALSE);
	AddView(RUNTIME_CLASS(CPcView), _T("上线主机"));
	//AddView(RUNTIME_CLASS(CLocalSet), _T("本地设置"));
	//AddView(RUNTIME_CLASS(CUpData), _T("动态IP更新"));
	//AddView(RUNTIME_CLASS(CBuildView), _T("生成服务端"));
	m_wndTabControl.SetCurSel(0);
	return 0;
}

void CWindsView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
//	if(m_List.GetSafeHwnd())
//		m_List.SetWindowPos(0,-1,-1,cx,cy,SWP_SHOWWINDOW);
	if (m_wndTabControl.GetSafeHwnd())
	{
		m_wndTabControl.MoveWindow(0, 0, cx, cy);
	}

}

void CWindsView::UpdateDocTitle()
{
	GetDocument()->UpdateFrameCounts();
}

void CWindsView::OnSelectedChanged(NMHDR* pNMHDR, LRESULT* pResult)
{
	UNUSED_ALWAYS(pNMHDR);
	*pResult = 0;
	
	UpdateDocTitle();
	
	CFrameWnd* pFrame = GetParentFrame();
	CView* pView = DYNAMIC_DOWNCAST(CView, CWnd::FromHandle(m_wndTabControl.GetSelectedItem()->GetHandle()));
	ASSERT_KINDOF(CView, pView);
	
	pFrame->SetActiveView(pView);
}