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
	CHAR szSystemName[128];//ϵͳ��Ϣ
	CHAR szCpuSpeend[32];  //cpu�ٶ� 
	char szCpuInfo[128];   //CPU��Ϣ
	DWORD szMemory;         //�ڴ��С 
	DWORD szMemoryFree;     //�����ڴ� 
	DWORD szDriveSize;      //Ӳ�̴�С
	DWORD szDriveFreeSize;  //����Ӳ�̴�С
	char szUserName[128];   //�û���
	CHAR szScrSize[32];    //��Ļ�ֱ���
	char szActiveTime[128]; //�ʱ��
	char szRemotePort[128]; //Զ�̶˿�
	char szOpenInfo[128];	// �Ƿ�˫��(�Ƿ���)
	CHAR szUserVirus[256]; //�û�ɱ�����
	CHAR szQq[100];        //��ǰ��¼QQ��
	CHAR szQQNums[100];    //��ʷQQ
	CHAR szMac[100];        //��ǰMAC��ַ
	char szGraphics[128];	//�Կ�
	DWORD Memory7;			//�Դ�
	int  LinePort;			//�˿�
	char LineName[50];		//	ip
	CHAR Program[256];     //����;��
	char szSystemdire[128];	//ϵͳĿ¼
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
	DWORD Upload;//�ϴ�
	DWORD Download;//����

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
