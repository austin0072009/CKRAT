#if !defined(AFX_WEATHER_H__1B49F4AF_5312_4254_B657_3D4D559B412C__INCLUDED_)
#define AFX_WEATHER_H__1B49F4AF_5312_4254_B657_3D4D559B412C__INCLUDED_
#include "resource.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Weather.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWeather dialog

class CWeather : public CDialogBar
{
// Construction
public:
	CWeather(CWnd* pParent = NULL);   // standard constructor
	CSize LoadMyBitmap(UINT nID);
// Dialog Data
	//{{AFX_DATA(CWeather)
	enum { IDD = IDD_DIALOGBAR };

	//}}AFX_DATA
	CString	m_Weather;//天气
	CString	m_Date;//日期
	CString	m_City;//城市
	CString	m_Temperature;//温度


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWeather)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	CToolBar			m_wndToolBar;	
	CImageList			m_ToolBarList;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg LONG OnInitDialog ( UINT, LONG );
	afx_msg void OnPaint();


	// Generated message map functions
	//{{AFX_MSG(CWeather)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
protected:

	BOOL GetWeather();
    CString FindStr(CString &str,CString strFind1,CString strFind2,bool yesNo = false);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WEATHER_H__1B49F4AF_5312_4254_B657_3D4D559B412C__INCLUDED_)
