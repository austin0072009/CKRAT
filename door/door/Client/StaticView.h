#pragma once


// CStaticView 视图

class CStaticView : public CListView
{
	DECLARE_DYNCREATE(CStaticView)

protected:
	CStaticView();           // 动态创建所使用的受保护的构造函数
	virtual ~CStaticView();

public:
	void SetLogItem(LPCTSTR Text,int pos1, int pos2);


#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	afx_msg void OnSize(UINT nType, int cx, int cy);

	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	CListCtrl* m_pStaticList;
	CImageList I_StaticList;
};


