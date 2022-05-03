
#include "resource.h"

#pragma once


// MyToolBar

class MyToolBar : public CDialogBar
{
	DECLARE_DYNAMIC(MyToolBar)

public:
	MyToolBar();
	virtual ~MyToolBar();

	enum { IDD = IDD_DIALOGBAR };
	CSize LoadMyBitmap(UINT nID);




	


// Implementation
protected:
	CToolBar			m_wndToolBar;	
	CImageList			m_ToolBarList;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnPaint();
	

protected:
	DECLARE_MESSAGE_MAP()
};


