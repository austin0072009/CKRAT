#pragma once


// CVideoWall �Ի���

class CVideoWall : public CDialog
{
	DECLARE_DYNAMIC(CVideoWall)

public:
	CVideoWall(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CVideoWall();

	CStatic* pStatic0,pStatic1,pStatic2,pStatic3,pStatic4,pStatic5,
		pStatic6,pStatic7,pStatic8;

	int m_nWallCount;
	static bool m_nWallFlag[9];

// �Ի�������
	enum { IDD = IDD_VIDEOWALL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
};


