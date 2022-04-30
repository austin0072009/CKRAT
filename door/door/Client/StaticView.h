#pragma once


// CStaticView ��ͼ

class CStaticView : public CListView
{
	DECLARE_DYNCREATE(CStaticView)

protected:
	CStaticView();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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


