#pragma once


// CStaticView 视图

class CStaticView : public CListView
{
	DECLARE_DYNCREATE(CStaticView)

protected:
	CStaticView();           // 动态创建所使用的受保护的构造函数
	virtual ~CStaticView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	virtual void OnDraw(CDC* /*pDC*/);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	CListCtrl* m_pStaticList;
	CImageList I_StaticList;
};


