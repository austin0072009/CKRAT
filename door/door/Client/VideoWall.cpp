// VideoWall.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "VideoWall.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CVideoWall 对话框

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


// CVideoWall 消息处理程序

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
	// TODO: 在此添加专用代码和/或调用基类
	//#TODO 关闭屏幕墙，释放所有内存，还原计数

	//非模态对话框必须要重写这个来销毁，不然会造成内存泄漏
	

			delete this;
	CDialog::PostNcDestroy();
}

void CVideoWall::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//AfxMessageBox("Close VideoWall");
	//for(int i =0 ; i< 9;i++)
	//{
	//	CStatic* pStatic;
	//	pStatic = (CStatic*)this->GetDlgItem(staticList[i]);
	//}
	//DestroyWindow();

	CDialog::OnClose();
}
