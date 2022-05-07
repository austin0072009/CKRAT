#pragma once


// CVideoWall 对话框

class CVideoWall : public CDialog
{
	DECLARE_DYNAMIC(CVideoWall)

public:
	CVideoWall(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoWall();

	CStatic* pStatic0,pStatic1,pStatic2,pStatic3,pStatic4,pStatic5,
		pStatic6,pStatic7,pStatic8;

	int m_nWallPoint;
	bool m_nWallFlag[9];

// 对话框数据
	enum { IDD = IDD_VIDEOWALL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	afx_msg void OnClose();
};


