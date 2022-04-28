// ShllCodeDec.cpp : Defines the entry point for the application.
//
#include "Stdafx.h"
#include "MyFileTabLe.h"//新算法的头文件
HANDLE hDllFile;
void*  MemDll;
int    SizeDll;
DWORD  BytesRead;




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

void DecryptData(unsigned char *szRec, unsigned long nLen, unsigned long key)//加密
{
	unsigned long i;
	unsigned char p;
	
	p = (unsigned char ) key % 490 + 841 + 425 + 421 + 813 + 899 + 141 + 712 + 451 + 543 + 448 + 145 + 136 + 772 + 332 + 445 + 614 + 281 + 725;
	 
	for(i = 0; i < nLen; i++)
	{
		*szRec -= p;
		*szRec++ ^= p;
	}
}



bool SaveFile(char *FileName)
{
	bool Result = false;
	HANDLE hFile; 
	DWORD  dwBytesWritten;
	hFile = CreateFile(FileName,GENERIC_WRITE,FILE_SHARE_READ,NULL,CREATE_ALWAYS,NULL,NULL);
	if (hFile == INVALID_HANDLE_VALUE) Result =false;
	if(WriteFile(hFile, MemDll, SizeDll, &dwBytesWritten, NULL)) Result =true;
	CloseHandle(hFile); 
	return Result;
}

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	hDllFile =CreateFile("1.dll",GENERIC_READ,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_NORMAL,0);//这里第一个参数调用上面那个GetModuleFileName

	SizeDll  =GetFileSize(hDllFile,0);
	MemDll   =VirtualAlloc(0,SizeDll,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
	ReadFile(hDllFile,MemDll,SizeDll,&BytesRead,0);
	memcpy(MemDll,"00",2);
	CloseHandle(hDllFile);
    QQVS360((unsigned char *)MemDll,SizeDll);//双重加密  
	DecryptData((unsigned char *)MemDll,SizeDll,2019);
	SaveFile("2.dll");

	return 0;
}



