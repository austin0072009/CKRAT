// VideoWall.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "VideoWall.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CVideoWall �Ի���

IMPLEMENT_DYNAMIC(CVideoWall, CDialog)


	bool m_nWallFlag[9] = {0,0,0,0,0,0,0,0,0};
CVideoWall::CVideoWall(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoWall::IDD, pParent)
{
	m_nWallCount = 0;
}

CVideoWall::~CVideoWall()
{
}

void CVideoWall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoWall, CDialog)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CVideoWall ��Ϣ�������

static UINT staticList[] =
{
	IDC_STATIC0,
	IDC_STATIC1,
	IDC_STATIC2,
	IDC_STATIC3,
	IDC_STATIC4,
	IDC_STATIC5,
	IDC_STATIC6,
	IDC_STATIC7,
	IDC_STATIC8
};
void CVideoWall::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���
	//#TODO �ر���Ļǽ���ͷ������ڴ棬��ԭ����

	//��ģ̬�Ի������Ҫ��д��������٣���Ȼ������ڴ�й©
	

			delete this;
	CDialog::PostNcDestroy();
}

void CVideoWall::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	//AfxMessageBox("Close VideoWall");
	//for(int i =0 ; i< 9;i++)
	//{
	//	CStatic* pStatic;
	//	pStatic = (CStatic*)this->GetDlgItem(staticList[i]);
	//}
	//DestroyWindow();

	CDialog::OnClose();
}
