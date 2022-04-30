// StaticView.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "StaticView.h"


typedef struct
{
	char	*title;
	int		nWidth;
}COLUMNSTRUCT;


COLUMNSTRUCT g_Static_Data[] = 
{
	{"主机统计",	    150	}
	//{"事件记录",	    650	}
};

CString StaticTitle[] = {"合计:0   台"};
CString StaticOld[] = {"Vista      :0   台 ","Win   NT:0   台","Win2000:0   台", "Win2003:0   台","Win2008:0   台", "Win2012:0   台", "Win2016:0   台" };
CString StaticNew[] = {"WinXP:0   台","Win  7:0   台","Win  8:0   台", "Win10:0   台" };
int g_Static_Width = 0;
int	g_Static_Count = (sizeof(g_Static_Data) / 8);
BOOL gStaticUpdate = FALSE;


CStaticView * g_pStaticView;





// CStaticView

IMPLEMENT_DYNCREATE(CStaticView, CListView)

CStaticView::CStaticView()
{
	g_pStaticView = this;

}

CStaticView::~CStaticView()
{
}

BEGIN_MESSAGE_MAP(CStaticView, CListView)
END_MESSAGE_MAP()


// CStaticView 诊断

#ifdef _DEBUG
void CStaticView::AssertValid() const
{
	CListView::AssertValid();
}

#ifndef _WIN32_WCE
void CStaticView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif
#endif //_DEBUG


// CStaticView 消息处理程序


void CStaticView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: 在此添加专用代码和/或调用基类
	 // this code only works for a report-mode list view
   ASSERT(GetStyle() & LVS_REPORT);

   CListCtrl& listCtrl = GetListCtrl();
   //listCtrl.SetTextBkColor(0x000000FF);
   //listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
   // Insert a column. This override is the most convenient.
   //listCtrl.InsertItem(0,"1111",1);
   listCtrl.InsertColumn(0, _T("在线主机设备信息统计"), LVCFMT_LEFT);

   // The other InsertColumn() override requires an initialized
   // LVCOLUMN structure.
   LVCOLUMN col;
   col.mask = LVCF_FMT | LVCF_TEXT;
   col.pszText = _T("");
   col.fmt = LVCFMT_LEFT;
   listCtrl.InsertColumn(1, &col);

   //// Set reasonable widths for our columns
   listCtrl.SetColumnWidth(0, LVSCW_AUTOSIZE_USEHEADER);
   listCtrl.SetColumnWidth(1, LVSCW_AUTOSIZE_USEHEADER);


// 参考实现
//   void CMFCApplication7Dlg::OnBnClickedButtonAdd()
// 3 {
// 4     int nCount = m_list.GetItemCount();//获取列表中数据项个数
// 5     CString strText;
// 6     strText.Format(TEXT("第%d行，第1列"), nCount + 1);//要插入的数据
// 7     m_list.InsertItem(nCount, strText,0);            //插入行
// 8     strText.Format(TEXT("第%d行，第2列"), nCount + 1);//要插入的数据
// 9     m_list.SetItemText(nCount, 1, strText);
//10 }

   //#TODO 还要写一个函数来更新数据

	
	

  
   
	for(int i = 0 ; i < 6 ; i++)
	listCtrl.InsertItem(i,StaticOld[i],0);
	
	for(int i = 0 ; i < 4 ; i++)
	listCtrl.SetItemText(i,1,StaticNew[i]);

	//Insert Blank
	listCtrl.InsertItem(10,"",0);
	listCtrl.InsertItem(11,StaticTitle[0],0);
}


void CStaticView::OnDraw(CDC* /*pDC*/)
{
	// TODO: 在此添加专用代码和/或调用基类
	CDocument* pDoc = GetDocument();
}


BOOL CStaticView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此添加专用代码和/或调用基类
	cs.style |=  LVS_REPORT;
	cs.style   &=~WS_BORDER;
	return CListView::PreCreateWindow(cs);
}
