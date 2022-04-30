// StaticView.cpp : ʵ���ļ�
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
	{"����ͳ��",	    150	}
	//{"�¼���¼",	    650	}
};

CString StaticTitle[] = {"�ϼ�:0   ̨"};
CString StaticOld[] = {"Vista      :0   ̨ ","Win   NT:0   ̨","Win2000:0   ̨", "Win2003:0   ̨","Win2008:0   ̨", "Win2012:0   ̨", "Win2016:0   ̨" };
CString StaticNew[] = {"WinXP:0   ̨","Win  7:0   ̨","Win  8:0   ̨", "Win10:0   ̨" };
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


// CStaticView ���

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


// CStaticView ��Ϣ�������


void CStaticView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: �ڴ����ר�ô����/����û���
	 // this code only works for a report-mode list view
   ASSERT(GetStyle() & LVS_REPORT);

   CListCtrl& listCtrl = GetListCtrl();
   //listCtrl.SetTextBkColor(0x000000FF);
   //listCtrl.SetExtendedStyle(LVS_EX_GRIDLINES);
   // Insert a column. This override is the most convenient.
   //listCtrl.InsertItem(0,"1111",1);
   listCtrl.InsertColumn(0, _T("���������豸��Ϣͳ��"), LVCFMT_LEFT);

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


// �ο�ʵ��
//   void CMFCApplication7Dlg::OnBnClickedButtonAdd()
// 3 {
// 4     int nCount = m_list.GetItemCount();//��ȡ�б������������
// 5     CString strText;
// 6     strText.Format(TEXT("��%d�У���1��"), nCount + 1);//Ҫ���������
// 7     m_list.InsertItem(nCount, strText,0);            //������
// 8     strText.Format(TEXT("��%d�У���2��"), nCount + 1);//Ҫ���������
// 9     m_list.SetItemText(nCount, 1, strText);
//10 }

   //#TODO ��Ҫдһ����������������

	
	

  
   
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
	// TODO: �ڴ����ר�ô����/����û���
	CDocument* pDoc = GetDocument();
}


BOOL CStaticView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���
	cs.style |=  LVS_REPORT;
	cs.style   &=~WS_BORDER;
	return CListView::PreCreateWindow(cs);
}
