// ChangeGroup.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "ChangeGroup.h"
#include "ClientView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CClientView* g_pTabView;
/////////////////////////////////////////////////////////////////////////////
// CChangeGroup dialog


CChangeGroup::CChangeGroup(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeGroup::IDD, pParent)
{
	//{{AFX_DATA_INIT(CChangeGroup)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CChangeGroup::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeGroup)
	DDX_Control(pDX, IDC_COMBO_GROUP, m_combo_group);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeGroup, CDialog)
	//{{AFX_MSG_MAP(CChangeGroup)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeGroup message handlers

BOOL CChangeGroup::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	/*CString strGroupName, strTemp;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	for ( int i = 0; i < nTabs; i++ )
	{
		strTemp = g_pTabView->m_wndTabControl.GetItem(i)->GetCaption();
		int n = strTemp.ReverseFind('(');
		if ( n > 0 )
		{
			strGroupName = strTemp.Left(n);
		}
		else
		{
			strGroupName = strTemp;
		}
		m_combo_group.AddString(strGroupName);
	}
	m_combo_group.SetCurSel(0);*/
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CChangeGroup::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
			return true;
		if (pMsg->wParam == VK_RETURN)
		{
			OnOK();
			return TRUE;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CChangeGroup::OnOK() 
{
	// TODO: Add extra validation here
	/*((CEdit*)GetDlgItem(IDC_COMBO_GROUP))->GetWindowText(strGroup);
	if (strGroup == "" )
	{
		AfxMessageBox("输入不能为空!");
		return;
	}
	CString strTemp, Temp;
	strTemp = g_pTabView->m_wndTabControl.GetSelectedItem()->GetCaption();
	
	int n = strTemp.ReverseFind('(');
	if ( n > 0 )
	{
		Temp = strTemp.Left(n);
	}
	else
	{
		Temp = strTemp;
	}
	if ( strGroup == Temp )
	{
		AfxMessageBox("分组没有改变!");
		return;
	}
	CDialog::OnOK();*/
}
