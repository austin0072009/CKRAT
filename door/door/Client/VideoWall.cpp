// VideoWall.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "VideoWall.h"
#include "afxdialogex.h"


// CVideoWall �Ի���

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


// CVideoWall ��Ϣ�������


void CVideoWall::PostNcDestroy()
{
	// TODO: �ڴ����ר�ô����/����û���

	//��ģ̬�Ի������Ҫ��д��������٣���Ȼ������ڴ�й©
	CDialog::PostNcDestroy();
	delete this;
}