// Login.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"
#include "afxdialogex.h"


// CLogin �Ի���

IMPLEMENT_DYNAMIC(CLogin, CDialog)

CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
	, m_strUser(_T(""))
	, m_strPassword(_T(""))
{

}

CLogin::~CLogin()
{
}

void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strUser);
	DDX_Text(pDX, IDC_EDIT2, m_strPassword);
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
END_MESSAGE_MAP()


// CLogin ��Ϣ�������
