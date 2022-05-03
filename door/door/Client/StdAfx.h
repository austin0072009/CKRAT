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

#pragma comment(lib,"vfw32.lib")//��Ļ���������lib�ļ�

#include <mmsystem.h>
#pragma comment(lib,"winmm.lib")
#include "SkinH.h"
#include <afxcview.h>
#pragma comment(lib, "SkinH.lib")

enum 
{
		WM_ADDTOLIST = WM_USER + 102,		// ��ӵ��б���ͼ��
		WM_REMOVEFROMLIST,				// ���б���ͼ��ɾ��
		WM_ADDFINDGROUP,				// ����ʱ���ҷ���
		WM_OPENMANAGERDIALOG,			//  ��һ���ļ�������
		WM_OPENSCREENSPYDIALOG,			//  ��һ����Ļ��ش���
		WM_OPENKEYBOARDDIALOG,			//  ��һ�����̼�¼����
		WM_OPENWEBCAMDIALOG,			//  ��һ����Ƶ�鿴����
		WM_OPENAUDIODIALOG,				//  ��һ��������������
		WM_OPENSHELLDIALOG,				//  ��һ��Զ���ն˴���
		WM_OPENSYSTEMINFODIALOG,		//  ��һ��ϵͳ��Ϣ����
		WM_OPENPSLISTDIALOG,			//  ��һ�����̹�����
		WM_OPENSERVICEDIALOG,			//  ��һ�����������
		WM_OPENREGEDITDIALOG,			//  ��һ��ע��������
		WM_OPENTEXTCHATDIALOG,			//  ��һ��Զ�����촰��
		WM_OPENQQINFODIALOG,			//  ��һ��������Ϣ����
		WM_OPENPROXYMAPDIALOG,			//  ��һ������ӳ�䴰��
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
