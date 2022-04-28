// Login.cpp : implementation file
//

#include "stdafx.h"
#include "Client.h"
#include "Login.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern char* MyDecode(char *str);
/////////////////////////////////////////////////////////////////////////////
// CLogin dialog


CLogin::CLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLogin)
	m_UserName = _T("");
	m_UserPass = _T("");
	
	//}}AFX_DATA_INIT
}


void CLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLogin)
	DDX_Text(pDX, IDC_USERNAME, m_UserName);
	DDX_Text(pDX, IDC_USERPASS, m_UserPass);
	DDX_Text(pDX, IDC_HOSTNAME, m_HostName);
	DDX_Control(pDX, IDC_GIF_LOGIN, m_Picture);
	if (m_Picture.Load(MAKEINTRESOURCE(IDR_GIF),_T("GIF")))   //��ʾGIFͼƬ
		m_Picture.Draw();
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLogin, CDialog)
	//{{AFX_MSG_MAP(CLogin)
	ON_BN_CLICKED(IDC_LOGIN, OnLogin)
	ON_BN_CLICKED(IDC_BTN_HIDE, OnBtnHide)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLogin message handlers



void CLogin::OnLogin() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
	DeleteFile("C:\\WINDOWS\\WIN32.cc");

	if ( m_UserName.GetLength() == 0 )
	{
		MessageBox("�������𻵣�","��ʾ��", MB_OK | MB_ICONASTERISK);
		return;
	}
	if ( m_UserPass.GetLength() == 0 )
	{
		MessageBox("�������𻵣�","��ʾ��", MB_OK | MB_ICONASTERISK);
		return;
	}


	char UserName[50];
	char UserPass[50];
	GetDlgItemText(IDC_USERNAME,UserName,sizeof(UserName));
	GetDlgItemText(IDC_USERPASS,UserPass,sizeof(UserPass));

	char TmpPath[MAX_PATH];
	GetTempPath( sizeof(TmpPath), TmpPath );
	lstrcatA( TmpPath, "\\WindsTemp.exe" );
	DeleteFile(TmpPath);
	
	//////////////////////////////////////////////////////////////////////////////////////////////////
	DWORD recvsize = 0;
	SOCKET sockInt;
	struct sockaddr_in serverAddr;
	//struct hostent *hp;
	WORD sockVersion;
    WSADATA wsaData;
    sockVersion = MAKEWORD(2,2);
    WSAStartup(sockVersion, &wsaData);
	//����SOCK
	sockInt = socket(AF_INET, SOCK_STREAM, 0);
	if(sockInt == INVALID_SOCKET)
	{
		SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
		AfxMessageBox("socket error!\n");
		WSACleanup();
		return;
	}
	
	//��ȡ������IP�Ͷ˿�
	serverAddr.sin_family = AF_INET;    
	char tgtIP[30] = {0};
	struct hostent *hp = NULL;

    if ((hp = gethostbyname(MyDecode((char *)m_HostName.GetBuffer(0)))) != NULL)
  	{
		in_addr in;
		memcpy(&in, hp->h_addr, hp->h_length);
		lstrcpy(tgtIP,inet_ntoa(in));
		m_IniFile.SetString( "HostName", "HostName",(char *)m_HostName.GetBuffer(0));
	}
	
	serverAddr.sin_addr.s_addr = inet_addr(tgtIP);
	serverAddr.sin_port = htons(1110);
	
	//���ӷ���
	if(connect(sockInt, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
	{
		SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
		WSACleanup();
		return;
	}
	char USERIMFOR[256] = {0}, buff[256] = {0};
	wsprintf( USERIMFOR, "Login:%s@%s", m_UserName, m_UserPass );
	if( send(sockInt, USERIMFOR, sizeof(USERIMFOR), 0) == SOCKET_ERROR )
	{
		SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
		WSACleanup();
		return;
	}
	Sleep(50);
	int Ret = recv( sockInt, buff, sizeof(buff), NULL );
	if ( Ret == 0 || Ret == SOCKET_ERROR )
	{
		SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
		closesocket(sockInt);
		WSACleanup();
		return;
	}
	 Sleep(1000);
	SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
	if (strstr( buff, "Pass" ) != NULL)//ͨ����֤
	{
		closesocket(sockInt);
		WSACleanup();
		UpdateData(FALSE);
		SetDlgItemText(IDC_STATIC_TIPS,"�������𻵣�");
		strcpy(Notice, UserName);
		dLogin = GetTickCount();
		SetDlgItemText(IDC_STATIC_TIPS,buff+6);

	//	Sleep(1000);
	//	SetDlgItemText(IDC_LOGIN,"�� ¼��3��");
	//	Sleep(1000);
	//	SetDlgItemText(IDC_LOGIN,"�� ¼��2��");
	//	Sleep(1000);
	//	SetDlgItemText(IDC_LOGIN,"�� ¼��1��");
		Sleep(1000);
		SetDlgItemText(IDC_LOGIN,"�� ¼��0��");
		OnCancel();

	}
}

 void CLogin::OnBtnHide() 
{
	// TODO: Add your control notification handler code here
	static BOOL bShow = TRUE;
	RECT rct;
	GetWindowRect(&rct);
	bShow = !bShow;
	
	if	(bShow)
		rct.bottom = rct.bottom - 60;
	else
		rct.bottom = rct.bottom + 60;
	
	MoveWindow(&rct, TRUE);
	
	rct.top = rct.bottom - 60;
	InvalidateRect(NULL);
	UpdateWindow();
}

/*
#include "Wininet.h"
#include "Mmsystem.h"
#include "Windows.h"

#include "afxsock.h" 
#include <afxinet.h> 

CString CLogin::InternetGetInfo(CString szUrl)
{
    DWORD dwFlags;
	InternetGetConnectedState(&dwFlags, 0);
	CString reValue,strContent;
	reValue=_T("Error");
	CHAR strAgent[64];
	memset(strAgent,0,sizeof(strAgent));
	sprintf(strAgent, "Agent%ld", timeGetTime());
	HINTERNET hOpen;
	if(!(dwFlags & INTERNET_CONNECTION_PROXY))
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG_WITH_NO_AUTOPROXY, NULL, NULL, 0);
	else
		hOpen = InternetOpenA(strAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if(!hOpen)
	{
		return reValue;
	}
	
	DWORD dwSize;
	CHAR   szHead[] = "Accept: * / *\r\n\r\n";
	VOID* szTemp[16384];
	HINTERNET  hConnect;
	
	if ( !(hConnect = InternetOpenUrlA ( hOpen, szUrl, szHead,
		lstrlenA (szHead), INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0)))
	{
		InternetCloseHandle(hOpen);
		return reValue;
	}
	
	DWORD dwByteToRead = 0;
	DWORD dwSizeOfRq = 4;
	DWORD dwBytes = 0;
	
	CString strInfoTemp;
	
    if (!HttpQueryInfo(hConnect, HTTP_QUERY_CONTENT_LENGTH | HTTP_QUERY_FLAG_NUMBER, 
		(LPVOID)&dwByteToRead, &dwSizeOfRq, NULL))
	{
		dwByteToRead = 0;
	}
	
	
	memset(szTemp,0,sizeof(szTemp));
	do
	{
		if (!InternetReadFile (hConnect, szTemp, 16384,  &dwSize))
		{
			return reValue;
		}
		if (dwSize==0)
		{
			break;
		}
		else
		{
			strInfoTemp.Format("%s",szTemp);
			strContent+=strInfoTemp;
		}
		
	}while (TRUE);
	
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hOpen);
	
	return strContent;
}*/


BOOL CLogin::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

/*
		//����°汾,��������°汾,������
		CString NewVersion,strUpdateUrl,OldVersion,strUrl,strInfo;
		//������İ汾��
		OldVersion=_T("1.0.0.1");
		
		strUrl="http://zf67ab.150a.cn/Ver.asp";
		
		strInfo=InternetGetInfo(strUrl);
		
		if(strInfo!=_T("Error"))
		{
			//�õ��汾�ż�������ַ
			int iPos1,iPos2,strLen;
			
			strLen=strInfo.GetLength();
			iPos1=strInfo.Find("#");
			iPos2=strInfo.Find("#",iPos1+1);
			NewVersion=strInfo.Mid(iPos1+1,iPos2-iPos1-1);//�汾��
			
			iPos1=iPos2;
			iPos2=strInfo.Find("#",iPos1+1);
			strUpdateUrl=strInfo.Mid(iPos1+1,iPos2-iPos1-1);//������ַ
			//�Ƚ��¾ɰ汾
			if(OldVersion!=NewVersion)
			{
				if(MessageBox("��ǰ�汾���ڣ����������°汾��"," ��ܰ��ʾ��",MB_ICONQUESTION|MB_YESNO)==IDYES)
				{
					ShellExecute(NULL,"open",strUpdateUrl,NULL,NULL,SW_SHOW);
				}
				AfxGetMainWnd()->SendMessage(WM_CLOSE);
			}
		}
		else
		{
			AfxMessageBox(_T("���������������"));
			AfxGetMainWnd()->SendMessage(WM_CLOSE);
		}
		
		
		SetDlgItemText(IDC_STATIC_VERSION,NewVersion);*/
	

	RECT rct;
	GetWindowRect(&rct);
	rct.bottom = rct.bottom - 60;
	MoveWindow(&rct, TRUE);

	SetDlgItemText(IDC_HOSTNAME,((CClientApp *)AfxGetApp())->m_IniFile.GetString("HostName", "HostName", ""));


	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
