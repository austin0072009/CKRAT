// Download.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Download.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CDownload dialog


CDownload::CDownload(CWnd* pParent /*=NULL*/)
	: CDialog(CDownload::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDownload)
	m_Downaddress = _T("");
	//}}AFX_DATA_INIT
}


void CDownload::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDownload)
	DDX_Text(pDX, IDC_DOWNADDRESS, m_Downaddress);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDownload, CDialog)
	//{{AFX_MSG_MAP(CDownload)
	//}}AFX_MSG_MAP
	ON_COMMAND_RANGE(IDC_DOWNOPEN,IDC_DOWNUPDATE,OnSelctMode)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDownload message handlers

void CDownload::OnSelctMode(UINT nID)
{
	switch(nID)
	{
	case IDC_DOWNOPEN://下载打开
		Down = 0;
		break;
	case IDC_DOWNUPDATE://下载更新
		Down = 1;
		break;
	default:
		Down = 0;
		break;
	}
}

BOOL CDownload::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateData(TRUE);
	Down = 0;
	((CButton *)GetDlgItem(IDC_DOWNOPEN))->SetCheck(TRUE);//默认选中下载打开

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
