// LogView.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "LogView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CLogView* g_pLogView;

typedef struct
{
	char	*title;
	int		nWidth;
}COLUMNSTRUCT;

COLUMNSTRUCT g_Log_Data[] = 
{
	{"����ʱ��",	    150	},
	{"�¼���¼",	    650	}
};

int g_Log_Width = 0;
int	g_Log_Count = (sizeof(g_Log_Data) / 8);
BOOL gLogUpdate = FALSE;
/////////////////////////////////////////////////////////////////////////////
// CLogView

IMPLEMENT_DYNCREATE(CLogView, CListView)

CLogView::CLogView()
{
	g_pLogView = this;
}

CLogView::~CLogView()
{
}


BEGIN_MESSAGE_MAP(CLogView, CListView)
	//{{AFX_MSG_MAP(CLogView)
	ON_WM_SIZE()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDM_EVENT_DELETE, OnEventDelete)
	ON_COMMAND(IDM_ALL_DELETE, OnAllDelete)
	ON_COMMAND(IDM_EVENT_SAVE, OnEventSave)
	ON_COMMAND(IDM_EVENT_COPY, OnEventCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogView drawing

void CLogView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CLogView diagnostics

#ifdef _DEBUG
void CLogView::AssertValid() const
{
	CListView::AssertValid();
}

void CLogView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLogView message handlers

void CLogView::OnInitialUpdate() 
{
	CListView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class

	m_pLogList = &GetListCtrl();
	m_pLogList->SetRedraw(FALSE);
	
	I_LogList.Create(16, 16, ILC_COLOR32|ILC_MASK,10, 0);
	HICON hIcon = NULL;//����Ϊ����3��ͼ����Դ
	//����0
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SHENGMING), IMAGE_ICON, 16, 16, 0);
	I_LogList.Add(hIcon);
	DestroyIcon(hIcon); 
	//����1
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_SHANGXIAN), IMAGE_ICON, 16, 16, 0);
	I_LogList.Add(hIcon);
	DestroyIcon(hIcon); 
	//����2
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_XIAXIAN), IMAGE_ICON, 16, 16, 0);
	I_LogList.Add(hIcon);
	DestroyIcon(hIcon); 
	//�ظ�����3
	hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_CHONGFUSHANGXIAN), IMAGE_ICON, 16, 16, 0);
	I_LogList.Add(hIcon);
	DestroyIcon(hIcon); 
/*
		//�����ɹ�4
		hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_JIANTINGCHENGGONG), IMAGE_ICON, 16, 16, 0);
		I_LogList.Add(hIcon);
		DestroyIcon(hIcon); 
		//����ʧ��5
		hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_JIANTINGSHIBAI), IMAGE_ICON, 16, 16, 0);
		I_LogList.Add(hIcon);
		DestroyIcon(hIcon); */
	

	ListView_SetImageList(m_pLogList->m_hWnd, I_LogList, LVSIL_SMALL);


	m_pLogList->SetExtendedStyle(LVS_EX_FULLROWSELECT |LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES);

	for (int i = 0; i < g_Log_Count; i++)
	{	
		m_pLogList->InsertColumn(i, g_Log_Data[i].title);
		g_Log_Width += g_Log_Data[i].nWidth; // �ܿ��
	}

	gLogUpdate = TRUE;

	HWND hWndHeader = m_pLogList->GetDlgItem(0)->GetSafeHwnd();

	m_pLogList->SetRedraw(TRUE);

	//��Ҫ����OnSize����
	CRect rect ;
	this->GetWindowRect( rect );
	ScreenToClient( rect ) ;
	this->OnSize(SIZE_RESTORED,rect.Width(),rect.Height());
}

BOOL CLogView::PreCreateWindow(CREATESTRUCT& cs) 
{
	// TODO: Add your specialized code here and/or call the base class
	cs.style |=  LVS_REPORT;
	return CListView::PreCreateWindow(cs);
}

void CLogView::OnSize(UINT nType, int cx, int cy) 
{
	CListView::OnSize(nType, cx, cy);
/*
	if(gLogUpdate)
	{
		m_pLogList->LockWindowUpdate();
		double dcx=(double)(cx-5)/g_Log_Width; 
		if (m_pLogList != NULL)
		{
			for(int i=0;i<g_Log_Count;i++){           //����ÿһ����
				double dd=g_Log_Data[i].nWidth;          //�õ���ǰ�еĿ��
				dd = dd*dcx;
				m_pLogList->SetColumnWidth(i,(int)dd);       //���õ�ǰ�Ŀ��
			}	
		}
		m_pLogList->UnlockWindowUpdate();
	}
	*/

	if(gLogUpdate)
	{
		double dcx=cx-5;     //�Ի�����ܿ��  g_Column_cx
		if (m_pLogList != NULL)
		{
			for(int i=0;i<g_Log_Count;i++){                   //����ÿһ����
				double dd=g_Log_Data[i].nWidth;               //�õ���ǰ�еĿ��
				dd/=g_Log_Width;                              //��һ����ǰ���ռ�ܳ��ȵļ���֮��
				dd*=dcx;                                         //��ԭ���ĳ��ȳ�����ռ�ļ���֮���õ���ǰ�Ŀ��
				m_pLogList->SetColumnWidth(i,(int)dd);          //���õ�ǰ�Ŀ��
			}

		}
	}

}

void CLogView::g_ColumnWidth()
{
	
}

void CLogView::InsertLogItem(LPCTSTR Text,int Mode, int Flag)
{

// 	font.CreatePointFont(90, _T("����"));//��������
// 	m_pLogList->SetFont(&font,TRUE);//����

	char m_Text[512] = {0};
	CTime time = CTime::GetCurrentTime();		//����CTime���� 

	CString strTime = time.Format(" %Y-%m-%d %H:%M:%S");
	m_pLogList->InsertItem(0, strTime, Flag);//int InsertItem( int nItem, LPCTSTR lpszItem, int nImage );
	m_pLogList->SetItemText(0, 1, Text);  //BOOL SetItemText( int nItem, int nSubItem, LPTSTR lpszText );
//	m_pLogList->SetTextColor(RGB(0,0,0)); // ��ɫ��ʾ

}

void CLogView::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
	CMenu	popup;
	popup.LoadMenu(IDR_LOG);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
//	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
	CXTPCommandBars::TrackPopupMenu(pM,TPM_RIGHTBUTTON,p.x,p.y,this);

}

void CLogView::OnEventDelete() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_pLogList->GetFirstSelectedItemPosition();
	if  (pos  ==  NULL)
	{
		::MessageBox(NULL,"����ѡ��Ҫɾ�����¼���¼ ...","��ʾ",MB_ICONINFORMATION);
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_pLogList->GetNextSelectedItem(pos);
			m_pLogList->DeleteItem(nItem);
			pos = m_pLogList->GetFirstSelectedItemPosition();
		}
	}
}

void CLogView::OnAllDelete() 
{
	// TODO: Add your command handler code here
	m_pLogList->DeleteAllItems();
}

void CLogView::OnEventSave() 
{
	// TODO: Add your command handler code here
	POSITION pos = m_pLogList->GetFirstSelectedItemPosition();
	if  (pos  ==  NULL)
	{
		::MessageBox(NULL,"����ѡ��Ҫ������¼���¼ ...","��ʾ",MB_ICONINFORMATION);
		return;
	}
	else
	{
		CTime time = CTime::GetCurrentTime(); ///����CTime���� 
		CString strTime = time.Format("%Y-%m-%d %H-%M-%S");
		
		CFileDialog dlg(FALSE, _T("log"), strTime, OFN_OVERWRITEPROMPT, _T("*.log|*.log|*.txt|*.log"), NULL);
		if (dlg.DoModal() != IDOK)
			return;
		CFile file;
		char strip[50] = {0};
		if (file.Open(dlg.GetPathName(), CFile::modeCreate | CFile::modeWrite))
		{
			while (pos)
			{
				int nItem = m_pLogList->GetNextSelectedItem(pos);
				CString strTitle;
				strTitle.Format(_T("ʱ��:%s �¼�:%s"), m_pLogList->GetItemText(nItem, 0),m_pLogList->GetItemText(nItem, 1));
				char *strip = strTitle.GetBuffer(strTitle.GetLength() + 1);	//CStringת��Ϊchar*
				strTitle.ReleaseBuffer();
				strcat(strip, "\r\n");
				file.Write(strip, strlen(strip));
				memset(strip, 0, 50);
			}
			file.Close();
			strTime = time.Format("[%Y-%m-%d %H:%M:%S]") + " ��־�����ɹ� ...";
			::MessageBox(0,strTime,"��ʾ",MB_ICONINFORMATION);
	  }
	}
}

void CLogView::OnEventCopy() 
{
	// TODO: Add your command handler code here
	int nItem;
    CString strText(_T(""));
    POSITION pos = m_pLogList->GetFirstSelectedItemPosition();
	if  (pos  ==  NULL)
	{
		::MessageBox(NULL,"����ѡ��Ҫ���Ƶ��¼���¼ ...","��ʾ",MB_ICONINFORMATION);
		return;
	}
	else
	{
		//��ȡ����ѡ����Ŀ�����ݡ�
		while (pos)
		{
			nItem = m_pLogList->GetNextSelectedItem(pos);
			strText += m_pLogList->GetItemText(nItem, 0) + " ";
			strText += m_pLogList->GetItemText(nItem, 1) + _T("\r\n");
		}
		//�����ݱ��浽�����塣
		if (!strText.IsEmpty())
		{
			if (OpenClipboard())
			{
				EmptyClipboard();
				HGLOBAL hClipboardData = GlobalAlloc(GHND | GMEM_SHARE, (strText.GetLength() + 1) * sizeof(TCHAR));
				if (hClipboardData)
				{
					TCHAR* pszData = (TCHAR *)GlobalLock(hClipboardData);
					_tcscpy(pszData, strText);
					GlobalUnlock(hClipboardData);
					SetClipboardData(CF_TEXT, hClipboardData);
				}
				CloseClipboard();
			}
		}
	}
}

