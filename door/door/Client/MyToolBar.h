
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

	CBitmap m_bitmap1;  
    CBitmap m_bitmap2;  
    CBitmap m_bitmap3;  
	CBitmap m_bitmap4;  
    CBitmap m_bitmap5;  
    CBitmap m_bitmap6;  
	

protected:
	DECLARE_MESSAGE_MAP()
};


