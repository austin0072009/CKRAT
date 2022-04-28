// Openweb.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Openweb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// COpenweb dialog


COpenweb::COpenweb(CWnd* pParent /*=NULL*/)
	: CDialog(COpenweb::IDD, pParent)
{
	//{{AFX_DATA_INIT(COpenweb)
	m_Webaddress = _T("");
	//}}AFX_DATA_INIT
}


void COpenweb::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COpenweb)
	DDX_Text(pDX, IDC_WEBADDRESS, m_Webaddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COpenweb, CDialog)
	//{{AFX_MSG_MAP(COpenweb)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_DISPLAY,IDC_HIDE,OnOpenWEB)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COpenweb message handlers
void COpenweb::OnOpenWEB(UINT nID)
{
	switch(nID)
	{
	case IDC_DISPLAY://显示打开
		Webtype = 0;
		break;
	case IDC_HIDE://隐藏打开
		Webtype = 1;
		break;
	default:
		Webtype = 0;
		break;
	}
}

BOOL COpenweb::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(TRUE);
	Webtype = 1;
	((CButton *)GetDlgItem(IDC_DISPLAY))->SetCheck(TRUE);//默认选中下载打开

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
