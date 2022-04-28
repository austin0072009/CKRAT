// Setting.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Setting.h"
#include "MainFrm.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
extern CMainFrame	*g_pFrame;
extern CIOCPServer	*m_iocpServer;
// CSetting dialog

BOOL dawed_1,dawed_2,dawed_3,dawed_4,dawed_5,dawed_6,dawed_7,dawed_8,dawed_9,dawed_10,dawed_11,dawed_12,dawed_13,dawed_14,dawed_15,dawed_16;

CSetting::CSetting(CWnd* pParent /*=NULL*/)
	: CDialog(CSetting::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSetting)
	m_listen_port = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenPort", 81);
	m_max_connections = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnection", 8000);
	m_connect_auto =((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "MaxConnectionAuto", 1);
	m_heartbeat_time = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "HeartBeatTime", 0);
	m_bisDisableSound = FALSE;
	m_bIsDisablePopTips = FALSE;
	m_SkinShe = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "SkinShe", TRUE);
	m_line = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "gridlines", TRUE);
	dawed_1 = m_dawed_1 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width01",TRUE);
	dawed_2 = m_dawed_2 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width02",TRUE);
	dawed_3 = m_dawed_3 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width03",TRUE);
	dawed_4 = m_dawed_4 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width04",FALSE);
	dawed_5 = m_dawed_5 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width05",TRUE);
	dawed_6 = m_dawed_6 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width06",TRUE);
	dawed_7 = m_dawed_7 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width07",TRUE);
	dawed_8 = m_dawed_8 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width08",TRUE);
	dawed_9 = m_dawed_9 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width09",FALSE);
	dawed_10 = m_dawed_10 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width10",TRUE);
	dawed_11 = m_dawed_11 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width11",TRUE);
	dawed_12 = m_dawed_12 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width12",FALSE);
	dawed_13 = m_dawed_13 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width13",FALSE);
	dawed_14 = m_dawed_14 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width14",TRUE);
	dawed_15 = m_dawed_15 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width15",TRUE);
	dawed_16 = m_dawed_16 = ((CClientApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "Width16",FALSE);
	//}}AFX_DATA_INIT
}


void CSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetting)
	DDX_Text(pDX, IDC_LISTEN_PORT, m_listen_port);
	DDX_Text(pDX, IDC_CONNECT_MAX, m_max_connections);
	DDX_Check(pDX, IDC_CONNECT_AUTO, m_connect_auto);
	DDX_Text(pDX, IDC_HEARTBEAT_TIME, m_heartbeat_time);
	DDX_Check(pDX, IDC_DISABLE_SOUND, m_bisDisableSound);
	DDX_Check(pDX, IDC_DISABLE_POPTIPS, m_bIsDisablePopTips);
	DDX_Check(pDX, IDC_CHECK_SKIN, m_SkinShe);
	DDX_Control(pDX, IDC_COMBO_COLOUR, m_ListColour);
	DDX_Control(pDX, IDC_COMBO_OPERATE, m_operateCmb);
	DDX_Check(pDX, IDC_LINE, m_line);
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_DAWED_1, m_dawed_1);
	DDX_Check(pDX, IDC_DAWED_2, m_dawed_2);
	DDX_Check(pDX, IDC_DAWED_3, m_dawed_3);
	DDX_Check(pDX, IDC_DAWED_4, m_dawed_4);
	DDX_Check(pDX, IDC_DAWED_5, m_dawed_5);
	DDX_Check(pDX, IDC_DAWED_6, m_dawed_6);
	DDX_Check(pDX, IDC_DAWED_7, m_dawed_7);
	DDX_Check(pDX, IDC_DAWED_8, m_dawed_8);
	DDX_Check(pDX, IDC_DAWED_9, m_dawed_9);
	DDX_Check(pDX, IDC_DAWED_10, m_dawed_10);
	DDX_Check(pDX, IDC_DAWED_11, m_dawed_11);
	DDX_Check(pDX, IDC_DAWED_12, m_dawed_12);
	DDX_Check(pDX, IDC_DAWED_13, m_dawed_13);
	DDX_Check(pDX, IDC_DAWED_14, m_dawed_14);
	DDX_Check(pDX, IDC_DAWED_15, m_dawed_15);
	DDX_Check(pDX, IDC_DAWED_16, m_dawed_16);
}


BEGIN_MESSAGE_MAP(CSetting, CDialog)
	//{{AFX_MSG_MAP(CSetting)
	ON_BN_CLICKED(IDC_APPLY, OnApply)
	ON_BN_CLICKED(IDC_CONNECT_AUTO, OnConnectAuto)
	ON_BN_CLICKED(IDC_DISABLE_SOUND, OnDisableSound)
	ON_BN_CLICKED(IDC_DISABLE_POPTIPS, OnDisablePoptips)
	ON_BN_CLICKED(IDC_CHECK_SKIN, OnCheckSkin)
	ON_BN_CLICKED(IDC_LINE, OnLine)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetting message handlers

void CSetting::rebootmeA()//重启本程序
{
	PROCESS_INFORMATION   info;
	STARTUPINFO startup;
	char szPath[128];
	char *szCmdLine;
	
	GetModuleFileName(AfxGetApp()->m_hInstance,   szPath,   sizeof(szPath));
	szCmdLine   =   GetCommandLine();
	GetStartupInfo(&startup);
	BOOL   bSucc   =   CreateProcess(szPath,   szCmdLine,   NULL,   NULL,
		FALSE,   NORMAL_PRIORITY_CLASS,   NULL,   NULL, &startup,   &info);
	
	if(TRUE   &&   bSucc)
	{
		CWnd *pWnd   =   AfxGetMainWnd();
		
		if(pWnd   !=   NULL)
		{
			pWnd->PostMessage(WM_CLOSE,   0,   0);
		}   
		else
			ExitProcess(-1);
	}   
	else   
		ExitProcess(-1);
	
	NOTIFYICONDATA tnd;
	tnd.cbSize=sizeof(NOTIFYICONDATA);
	tnd.hWnd=AfxGetMainWnd()->m_hWnd;
	tnd.uID=IDR_MAINFRAME;//保证删除的是我们的图标
	Shell_NotifyIcon(NIM_DELETE,&tnd);
}

void CSetting::OnApply() 
{
	// TODO: Add your control notification handler code here
	UINT prev_heatbeattime = m_heartbeat_time;
	int prev_port = m_listen_port;
	int prev_max_connections = m_max_connections;
	UpdateData(TRUE);
	
	operate=m_operateCmb.GetCurSel();//双击
	Colour=m_ListColour.GetCurSel(); //颜色

	if (prev_max_connections != m_max_connections)
	{
		
		if (m_connect_auto)
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, 8000);
		else
			InterlockedExchange((LPLONG)&m_iocpServer->m_nMaxConnections, m_max_connections);
	}
	
	if (prev_port != m_listen_port)
		g_pFrame->StartIocp(m_listen_port, m_iocpServer->m_nMaxConnections);

	if(prev_heatbeattime!= m_heartbeat_time)
	{

		((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "HeartBeatTime", m_heartbeat_time);			


	}

	PlaySound(MAKEINTRESOURCE(IDR_WAVE3),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);

	((CClientApp *)AfxGetApp())->m_IniFile.SetDWORD("Settings","operate",operate);
	((CClientApp *)AfxGetApp())->m_IniFile.SetDWORD("Settings","ListColor",Colour);           
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "ListenPort", m_listen_port);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnection", m_max_connections);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width01", m_dawed_1);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width02", m_dawed_2);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width03", m_dawed_3);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width04", m_dawed_4);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width05", m_dawed_5);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width06", m_dawed_6);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width07", m_dawed_7);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width08", m_dawed_8);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width09", m_dawed_9);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width10", m_dawed_10);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width11", m_dawed_11);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width12", m_dawed_12);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width13", m_dawed_13);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width14", m_dawed_14);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width15", m_dawed_15);
	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Width16", m_dawed_16);

	BOOL dawed_DATA = NULL;
	if((dawed_1 != m_dawed_1)||(dawed_2 != m_dawed_2)||(dawed_3 != m_dawed_3)||(dawed_4 != m_dawed_4)||(dawed_5 != m_dawed_5)||
		(dawed_6 != m_dawed_6)||(dawed_7 != m_dawed_7)||(dawed_8 != m_dawed_8)||(dawed_9 != m_dawed_9)||(dawed_10 != m_dawed_10)||
		(dawed_11 != m_dawed_11)||(dawed_12 != m_dawed_12)||(dawed_13 != m_dawed_13)||(dawed_14 != m_dawed_14)||(dawed_15 != m_dawed_15)
		||(dawed_16 != m_dawed_16)
		)
	{
		dawed_DATA = TRUE;
	}


	if (((unsigned int)prev_port != m_listen_port)||(prev_heatbeattime!= m_heartbeat_time)||(dawed_DATA == TRUE))  //比较端口及密码是否改变
	{
		if(MessageBox("系统参数已经更改，需重启软件生效..."," 温馨提示！",MB_ICONQUESTION|MB_YESNO)==IDYES)
		{

			rebootmeA();
			ExitProcess(0);

		}
	}

	
}

void CSetting::OnConnectAuto() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(!m_connect_auto);
	m_connect_auto = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "MaxConnectionAuto", m_connect_auto);
}

BOOL CSetting::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	if (m_connect_auto==TRUE)
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(FALSE);
	else
		GetDlgItem(IDC_CONNECT_MAX)->EnableWindow(TRUE);
	
	InitOperate();
	ListColour();
// 	SetIcon(m_hIcon, TRUE);			// Set big icon
// 	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CSetting::OnDisableSound() 
{
	// TODO: Add your control notification handler code here
// 	UpdateData(true);
// 	if ( ((CButton *)GetDlgItem(IDC_DISABLE_SOUND))->GetCheck() == TRUE )
// 	{
// 		SetDlgItemText(IDC_SHOWTIPS, "已关闭提示音");
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_SHOWTIPS, "已开启提示音");
// 	}
// 	m_bisDisableSound=((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "Sound", m_bisDisableSound);
}

void CSetting::OnDisablePoptips() 
{
	// TODO: Add your control notification handler code here
	UpdateData();
// 	((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "PopTips", m_bIsDisablePopTips);
// 	((CClientApp *)AfxGetApp())->m_bIsDisablePopTips = m_bIsDisablePopTips;
// 	if ( ((CButton *)GetDlgItem(IDC_DISABLE_POPTIPS))->GetCheck() == TRUE )
// 	{
// 		SetDlgItemText(IDC_SHOWTIPS, "已关闭上线气泡显示");
// 	}
// 	else
// 	{
// 		SetDlgItemText(IDC_SHOWTIPS, "已开启上线气泡显示");
// 	}
}



// 初始化双击时执行的操作
void CSetting::InitOperate(void)
{
	m_operateCmb.AddString("文件管理");
	m_operateCmb.SetItemData(0,0);

	m_operateCmb.AddString("桌面协助");
	m_operateCmb.SetItemData(0,1);

	m_operateCmb.AddString("键盘记录");
	m_operateCmb.SetItemData(0,2);

	m_operateCmb.AddString("视频查看");
	m_operateCmb.SetItemData(0,3);

	m_operateCmb.AddString("语音监听");
	m_operateCmb.SetItemData(0,4);

	m_operateCmb.AddString("远程终端");
	m_operateCmb.SetItemData(0,5);

	m_operateCmb.AddString("系统信息");
	m_operateCmb.SetItemData(0,6);

	m_operateCmb.AddString("系统管理");
	m_operateCmb.SetItemData(0,7);

	m_operateCmb.AddString("服务管理");
	m_operateCmb.SetItemData(0,8);

	m_operateCmb.AddString("注册表");
	m_operateCmb.SetItemData(0,9);

	m_operateCmb.AddString("远程定位");
	m_operateCmb.SetItemData(0,10);

	m_operateCmb.AddString("远程聊天");
	m_operateCmb.SetItemData(0,11);

	m_operateCmb.AddString("好友信息");
	m_operateCmb.SetItemData(0,12);

	operate=((CClientApp *)AfxGetApp())->m_IniFile.GetDWORD("Settings","operate");
	if (operate<=0||operate>=13)
    {
		operate=0;
	}
	m_operateCmb.SetCurSel(operate);
}

// 列表颜色
void CSetting::ListColour(void)
{
	m_ListColour.AddString("黑色");
	m_ListColour.SetItemData(0,0);
	
	m_ListColour.AddString("蓝色");
	m_ListColour.SetItemData(0,1);
	
	m_ListColour.AddString("黄色");
	m_ListColour.SetItemData(0,2);
	
	m_ListColour.AddString("红色");
	m_ListColour.SetItemData(0,3);
	
	m_ListColour.AddString("绿色");
	m_ListColour.SetItemData(0,4);
	
	m_ListColour.AddString("紫色");
	m_ListColour.SetItemData(0,5);
	
	Colour=((CClientApp *)AfxGetApp())->m_IniFile.GetDWORD("Settings","ListColor");
	if (Colour<=0||Colour>=6)
    {
		Colour=0;
	}
	m_ListColour.SetCurSel(Colour);
}

void CSetting::OnCheckSkin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	if ( ((CButton *)GetDlgItem(IDC_CHECK_SKIN))->GetCheck() == TRUE )
	{
		HGLOBAL hRes;
		HRSRC hResInfo;
		HINSTANCE hinst = AfxGetInstanceHandle();
		hResInfo = FindResource( hinst, MAKEINTRESOURCE(IDR_SKIN), "SKIN" );
		if (hResInfo != NULL)
		{
			hRes = LoadResource( hinst, hResInfo);
			if (hRes != NULL)
			{
				SkinH_AttachRes( (LPBYTE)hRes, SizeofResource(hinst,hResInfo), NULL, NULL, NULL, NULL );
				SkinH_SetAero(FALSE);
				FreeResource(hRes);
			}
		}
		
		SetDlgItemText(IDC_CHECK_SKIN, "关闭软件皮肤");
	}
	else
	{
		SkinH_Detach();//关闭皮肤效果
		SetDlgItemText(IDC_CHECK_SKIN, "启用软件皮肤");
		
	}
	m_SkinShe = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "SkinShe", m_SkinShe);
}

void CSetting::OnLine() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	m_line = ((CClientApp *)AfxGetApp())->m_IniFile.SetInt("Settings", "gridlines", m_line);
	if(MessageBox("系统参数已经更改，需重启软件生效..."," 温馨提示！",MB_ICONQUESTION|MB_YESNO)==IDYES)
	{
		AfxGetMainWnd()->SendMessage(WM_CLOSE);
	}
}
