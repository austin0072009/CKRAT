#pragma once


// CVideoWall �Ի���

class CVideoWall : public CDialog
{
	DECLARE_DYNAMIC(CVideoWall)

public:
	CVideoWall(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoWall();

// �Ի�������
	enum { IDD = IDD_VIDEOWALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};


