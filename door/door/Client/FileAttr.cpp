// FileAttr.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "FileAttr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFileAttr dialog


CFileAttr::CFileAttr(CWnd* pParent /*=NULL*/ )
	: CDialog(CFileAttr::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFileAttr)
	m_bReadOnly = FALSE;
	m_bSystem = FALSE;
	m_bHidden = FALSE;
	m_bArchive = FALSE;
	m_strCTime = _T("");
	m_strMTime = _T("");
	m_strATime = _T("");
	m_strSize = _T("");
	m_FilePath = _T("");
	m_FileName = _T("");
	//}}AFX_DATA_INIT
//	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICOFILE));
}


void CFileAttr::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFileAttr)
	DDX_Check(pDX, IDC_CHECK_READONLY, m_bReadOnly);
	DDX_Check(pDX, IDC_CHECK_SYSTEM, m_bSystem);
	DDX_Check(pDX, IDC_CHECK_HIDDEN, m_bHidden);
	DDX_Check(pDX, IDC_CHECK_ARCHIVE, m_bArchive);
	DDX_Text(pDX, IDC_STATIC_CTIME, m_strCTime);
	DDX_Text(pDX, IDC_STATIC_MTIME, m_strMTime);
	DDX_Text(pDX, IDC_STATIC_ATIME, m_strATime);
	DDX_Text(pDX, IDC_STATIC_SIZE, m_strSize);
	DDX_Text(pDX, IDC_STATIC_PATH, m_FilePath);
	DDX_Text(pDX, IDC_STATIC_FILENAME, m_FileName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFileAttr, CDialog)
	//{{AFX_MSG_MAP(CFileAttr)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFileAttr message handlers

void CFileAttr::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnClose();
}

BOOL CFileAttr::OnInitDialog() 
{
	CDialog::OnInitDialog();
	SetIcon(m_hIcon,TRUE);
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
