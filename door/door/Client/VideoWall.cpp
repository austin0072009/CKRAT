// VideoWall.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "VideoWall.h"
#include "afxdialogex.h"


// CVideoWall 对话框

IMPLEMENT_DYNAMIC(CVideoWall, CDialog)

CVideoWall::CVideoWall(CWnd* pParent /*=NULL*/)
	: CDialog(CVideoWall::IDD, pParent)
{

}

CVideoWall::~CVideoWall()
{
}

void CVideoWall::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CVideoWall, CDialog)
END_MESSAGE_MAP()


// CVideoWall 消息处理程序


void CVideoWall::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	//非模态对话框必须要重写这个来销毁，不然会造成内存泄漏
	CDialog::PostNcDestroy();
	delete this;
}