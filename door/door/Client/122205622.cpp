// Winds.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "122205622.h"

#include "MainFrm.h"
#include "122205622Doc.h"
#include "122205622View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWindsApp1111

BEGIN_MESSAGE_MAP(CWindsApp, CWinApp)
	//{{AFX_MSG_MAP(CWindsApp)
	//ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWindsApp construction

CWindsApp::CWindsApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
	typedef BOOL (WINAPI * pfn_FileIconInit) (BOOL fFullInit);
	pfn_FileIconInit FileIconInit = (pfn_FileIconInit) GetProcAddress(LoadLibrary("shell32.dll"), (LPCSTR)660);
	FileIconInit(TRUE);
	
	HANDLE	hFile = CreateFile("QQwry.dat", 0, 0, NULL, OPEN_EXISTING, 0, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		m_bIsQQwryExist = true;
	}
	else
	{
		m_bIsQQwryExist = false;
	}
	CloseHandle(hFile);
	
	m_bIsDisablePopTips = m_IniFile.GetInt("Settings", "PopTips", false);
	m_pConnectView = NULL;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWindsApp object

CWindsApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWindsApp initialization

BOOL CWindsApp::InitInstance()
{
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
		RUNTIME_CLASS(CWindsDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CWindsView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
/*	// 启动IOCP服务器
	int	nPort = 8086;// m_IniFile.GetInt("Settings", "ListenPort");
	int	nMaxConnection = 10000; // m_IniFile.GetInt("Settings", "MaxConnection");*/
	// 启动IOCP服务器
	int	nPort = m_IniFile.GetInt("Settings", "ListenPort", 8086);
	int	nMaxConnection = m_IniFile.GetInt("Settings", "MaxConnection", 10000);

	//((CMainFrame*) m_pMainWnd)->Activate(nPort, nMaxConnection);
	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CWindsApp message handlers

