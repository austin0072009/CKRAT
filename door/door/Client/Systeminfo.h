#if !defined(AFX_SYSTEMINFO_H__DB07043E_21BA_4CE7_95C6_215B875E04C8__INCLUDED_)
#define AFX_SYSTEMINFO_H__DB07043E_21BA_4CE7_95C6_215B875E04C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Systeminfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSysteminfo dialog

struct tagSystemInfo
{
	CHAR szSystemName[128];//系统信息
	CHAR szCpuSpeend[32];  //cpu速度 
	char szCpuInfo[128];   //CPU信息
	DWORD szMemory;         //内存大小 
	DWORD szMemoryFree;     //可用内存 
	DWORD szDriveSize;      //硬盘大小
	DWORD szDriveFreeSize;  //可用硬盘大小
	char szUserName[128];   //用户名
	CHAR szScrSize[32];    //屏幕分辨率
	char szActiveTime[128]; //活动时间
	char szRemotePort[128]; //远程端口
	char szOpenInfo[128];	// 是否双开(是否开启)
	CHAR szUserVirus[256]; //用户杀毒软件
	CHAR szQq[100];        //当前登录QQ号
	CHAR szQQNums[100];    //历史QQ
	CHAR szMac[100];        //当前MAC地址
	char szGraphics[128];	//显卡
	DWORD Memory7;			//显存
	int  LinePort;			//端口
	char LineName[50];		//	ip
	CHAR Program[256];     //程序途径
	char szSystemdire[128];	//系统目录
};

struct tagSysstatus
{
	int		Mem;
	int		cpu;
	DWORD	Bandout;
	DWORD	Bandout_B;	
};

class CSysteminfo : public CDialog
{
// Construction
public:
	CSysteminfo(CWnd* pParent = NULL, CIOCPServer* pIOCPServer = NULL, ClientContext *pContext = NULL);   // standard constructor
	void OnReceiveComplete();
// Dialog Data
	//{{AFX_DATA(CSysteminfo)
	enum { IDD = IDD_SYSTEMINFO };
	CProgressCtrl	m_MemPross;
	CProgressCtrl	m_CPUPross;
	CListCtrl	m_List;
	CString	m_CpuUsed;
	CString	m_Memory;
	CString	m_Download;
	CString	m_Upload;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysteminfo)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	ClientContext* m_pContext;
	CIOCPServer* m_iocpServer;
	CString IPAddress;
	CImageList     m_ImgList;
	
	DWORD dwMem;
	DWORD dwCpu;
	DWORD Upload;//上传
	DWORD Download;//下载

	// Generated message map functions
	//{{AFX_MSG(CSysteminfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	BOOL m_bOnClose;
	void DeleteList();
	void AdjustList();

	void ShowSysInfoList();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSTEMINFO_H__DB07043E_21BA_4CE7_95C6_215B875E04C8__INCLUDED_)
