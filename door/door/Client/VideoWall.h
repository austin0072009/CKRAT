#pragma once


// CVideoWall 对话框

class CVideoWall : public CDialog
{
	DECLARE_DYNAMIC(CVideoWall)

public:
	CVideoWall(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CVideoWall();

// 对话框数据
	enum { IDD = IDD_VIDEOWALL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
};


