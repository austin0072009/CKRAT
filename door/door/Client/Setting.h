	#if !defined(AFX_SETTING_H__45406952_F845_428E_9FCA_7854224F7A45__INCLUDED_)
#define AFX_SETTING_H__45406952_F845_428E_9FCA_7854224F7A45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Setting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSetting dialog

class CSetting : public CDialog
{
// Construction
public:
	CSetting(CWnd* pParent = NULL);   // standard constructor
	void rebootmeA();
	DWORD operate;        //Ë«»÷         
	DWORD Colour;		  //ÑÕÉ«      
	CComboBox   m_operateCmb;
	CComboBox	m_ListColour;
	void InitOperate(void);
	void ListColour(void);
// Dialog Data
	//{{AFX_DATA(CSetting)
	enum { IDD = IDD_SETTING };
	UINT	m_listen_port;
	UINT	m_max_connections;
	BOOL	m_connect_auto;
	UINT	m_heartbeat_time;
	BOOL	m_bisDisableSound;
	BOOL	m_bIsDisablePopTips;
	BOOL	m_SkinShe;
	BOOL	m_line;
	//}}AFX_DATA


// Overrides
public:
	BOOL m_dawed_1;
	BOOL m_dawed_2;
	BOOL m_dawed_3;
	BOOL m_dawed_4;
	BOOL m_dawed_5;
	BOOL m_dawed_6;
	BOOL m_dawed_7;
	BOOL m_dawed_8;
	BOOL m_dawed_9;
	BOOL m_dawed_10;
	BOOL m_dawed_11;
	BOOL m_dawed_12;
	BOOL m_dawed_13;
	BOOL m_dawed_14;
	BOOL m_dawed_15;
	BOOL m_dawed_16;
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSetting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CSetting)
	virtual BOOL OnInitDialog();
	afx_msg void OnApply();
	afx_msg void OnConnectAuto();
	afx_msg void OnDisableSound();
	afx_msg void OnDisablePoptips();
	afx_msg void OnCheckSkin();
	afx_msg void OnLine();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
// 	HICON m_hIcon;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SETTING_H__45406952_F845_428E_9FCA_7854224F7A45__INCLUDED_)
