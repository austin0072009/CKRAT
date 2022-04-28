// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__EA814F44_A7A1_4CFC_8377_1909AE535992__INCLUDED_)
#define AFX_STDAFX_H__EA814F44_A7A1_4CFC_8377_1909AE535992__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions
#include <afxdisp.h>        // MFC Automation classes
#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT

#include "XTToolkitPro.h"

#include "include\IOCPServer.h"
#include "macros.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi.lib")

#pragma comment(lib,"vfw32.lib")//屏幕控制所需的lib文件

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "SkinH.h"
#pragma comment(lib, "SkinH.lib")

enum 
{
		WM_ADDTOLIST = WM_USER + 102,		// 添加到列表视图中
		WM_REMOVEFROMLIST,				// 从列表视图中删除
		WM_ADDFINDGROUP,				// 上线时查找分组
		WM_OPENMANAGERDIALOG,			//  打开一个文件管理窗口
		WM_OPENSCREENSPYDIALOG,			//  打开一个屏幕监控窗口
		WM_OPENKEYBOARDDIALOG,			//  打开一个键盘记录窗口
		WM_OPENWEBCAMDIALOG,			//  打开一个视频查看窗口
		WM_OPENAUDIODIALOG,				//  打开一个语音监听窗口
		WM_OPENSHELLDIALOG,				//  打开一个远程终端窗口
		WM_OPENSYSTEMINFODIALOG,		//  打开一个系统信息窗口
		WM_OPENPSLISTDIALOG,			//  打开一个进程管理窗口
		WM_OPENSERVICEDIALOG,			//  打开一个服务管理窗口
		WM_OPENREGEDITDIALOG,			//  打开一个注册表管理窗口
		WM_OPENTEXTCHATDIALOG,			//  打开一个远程聊天窗口
		WM_OPENQQINFODIALOG,			//  打开一个好友信息窗口
		WM_OPENPROXYMAPDIALOG,			//  打开一个代理映射窗口
		//////////////////////////////////////////////////////////////////////////
		FILEMANAGER_DLG = 1,
		SCREENSPY_DLG,
		KEYBOARD_DLG,
		WEBCAM_DLG,
		AUDIO_DLG,
		SHELL_DLG,
		SYSTEMINFO_DLG,
		SYSTEM_DLG,
		SERVICE_DLG,
		REGEDIT_DLG,
		CHAT_DLG,
		QQINFO_DLG,
		PROXYMAP_DLG
};

typedef struct 
{
	DWORD	dwSizeHigh;
	DWORD	dwSizeLow;
}FILESIZE;

extern CHAR Notice[MAX_PATH];

#define MAKEINT64(low, high) ((unsigned __int64)(((DWORD)(low)) | ((unsigned __int64)((DWORD)(high))) << 32))

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__EA814F44_A7A1_4CFC_8377_1909AE535992__INCLUDED_)
