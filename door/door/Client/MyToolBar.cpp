// MyToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "MyToolBar.h"


// MyToolBar

IMPLEMENT_DYNAMIC(MyToolBar, CDialogBar)

MyToolBar::MyToolBar()
{

}

MyToolBar::~MyToolBar()
{
}


BEGIN_MESSAGE_MAP(MyToolBar, CDialogBar)

	ON_MESSAGE(WM_INITDIALOG, OnInitDialog)
END_MESSAGE_MAP()



// common function
void TransParentDC(CRect rect,CDC * pDC)
{
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CBitmap m_Bitmap;
	m_Bitmap.CreateCompatibleBitmap
		(pDC,rect.Width(),rect.Height());
	CBitmap *pOldBitmap = 
		m_MemDC.SelectObject(&m_Bitmap);
	m_MemDC.FillSolidRect(0,0,rect.Width(),
		rect.Height(),GetSysColor(COLOR_MENU));
	
	COLORREF cor =  pDC->GetPixel(0,0);
	for(int y = 0; y < rect.Height();y++)
	{
		for(int x = 0; x < rect.Width(); x++)
		{
			COLORREF ch = pDC->GetPixel(x,y);
			if(ch != cor)
				m_MemDC.SetPixelV(x,y,ch);
		}
	}
	pDC->BitBlt(0,0,rect.Width(),
		rect.Height(),&m_MemDC,0,0,SRCCOPY);
	m_MemDC.SelectObject(pOldBitmap);
	m_Bitmap.DeleteObject();
}

int URLEncode(LPCTSTR pszUrl, LPTSTR pszEncode, int nEncodeLen)
{
	if (pszUrl == NULL)
		return 0;
	
	if (pszEncode == NULL || nEncodeLen == 0)
		return 0;
	
	//定义变量  
	int nLength = 0;
	WCHAR* pWString = NULL;
	TCHAR* pString = NULL;
	
	//先将字符串由多字节转换成UTF-8编码  
	nLength = MultiByteToWideChar(CP_ACP, 0, pszUrl, -1, NULL, 0);
	
	//分配Unicode空间  
	pWString = new WCHAR[nLength];
	
	//先转换成Unicode  
	MultiByteToWideChar(CP_ACP, 0, pszUrl, -1, pWString, nLength);
	
	//分配UTF-8空间  
	nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, NULL, 0, NULL, NULL);
	pString = new TCHAR[nLength];
	
	//Unicode转到UTF-8  
	nLength = WideCharToMultiByte(CP_UTF8, 0, pWString, -1, pString, nLength, NULL, NULL);
	
	static char hex[] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F' };
	memset(pszEncode, 0, nEncodeLen / sizeof(TCHAR));
	
	for (int i = 0; i < nLength - 1; i++)
	{
		unsigned char c = pString[i];
		if (c > 0x20 && c < 0x7f)    // 数字或字母  
		{
			*pszEncode++ = c;
		}
		else if (c == 0x20)        // 包含空格  
		{
			*pszEncode++ = '+';
		}
		else                        // 进行编码  
		{
			*pszEncode++ = '%';
			*pszEncode++ = hex[c / 16];
			*pszEncode++ = hex[c % 16];
		}
	}
	
	//删除内存  
	delete pWString;
	delete pString;
	
	return nLength;
}
// MyToolBar 消息处理程序
LONG MyToolBar::OnInitDialog(UINT wParam, LONG lParam)
{
	
	BOOL bRet = HandleInitDialog(wParam, lParam);
	if(!UpdateData(FALSE))
	{
		TRACE0(“Warning, Unalbe to init update.\n”);
	}
	
/*
		if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP //CBRS_LEFT
			| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!m_wndToolBar.LoadToolBar(IDR_TOOLBAR_MAIN))
		{
			TRACE0("Failed to create toolbar\n");
			return -1;      // fail to create
		}
		
		m_ToolBarList.Create(60,32,ILC_COLOR24|ILC_MASK,10,10);
		CSize m_Size = 
		LoadMyBitmap(IDB_BITMAP_XTSZ);
		LoadMyBitmap(IDB_BITMAP_SYGN);
		LoadMyBitmap(IDB_BITMAP_CJKF);
		LoadMyBitmap(IDB_BITMAP_GYCX);
		LoadMyBitmap(IDB_BITMAP_TCCX);
		
		
	   	m_wndToolBar.SetButtonText(0,_T("系统设置"));
		m_wndToolBar.SetButtonText(1,_T("实用功能"));
		m_wndToolBar.SetButtonText(2,_T("创建客户"));
		m_wndToolBar.SetButtonText(3,_T("关于程序"));
		m_wndToolBar.SetButtonText(4,_T("退出程序"));
		
		
		m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
		m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
		m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(60,33 + m_Size.cy + 5));
		m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(58,32));*/
	
	//GetWeather();
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP //CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_TOOLBAR1))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}
	//下面是允许工具栏可以托动
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY); //允许工具栏在工具栏区域中移动
	EnableDocking(CBRS_ALIGN_ANY);  //此框架任何地方允许可拖动，如果已经为别的ToolBar写过可以不用再写
	
   	m_ToolBarList.Create(60,32,ILC_COLOR24|ILC_MASK,0,0);
	CSize m_Size = 
	LoadMyBitmap(IDB_BITMAP_XTSZ);
	LoadMyBitmap(IDB_BITMAP_SYGN);
	LoadMyBitmap(IDB_BITMAP_CJKF);
	LoadMyBitmap(IDB_BITMAP_GYCX);
	LoadMyBitmap(IDB_BITMAP_TCCX);
	
	
    m_wndToolBar.SetButtonText(0,_T("系统设置"));
	m_wndToolBar.SetButtonText(1,_T("实用功能"));
	m_wndToolBar.SetButtonText(2,_T("创建客户"));
	m_wndToolBar.SetButtonText(3,_T("关于程序"));
	m_wndToolBar.SetButtonText(4,_T("退出程序"));
	   
	   
	m_wndToolBar.GetToolBarCtrl().SetImageList(&m_ToolBarList);
	m_wndToolBar.GetToolBarCtrl().SetDisabledImageList(&m_ToolBarList);
	m_wndToolBar.GetToolBarCtrl().SetButtonSize(CSize(60,32 + m_Size.cy + 20));
	m_wndToolBar.GetToolBarCtrl().SetBitmapSize(CSize(60,32));
	   
 	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //显示状态栏

	return bRet;   
}


CSize MyToolBar::LoadMyBitmap(UINT nID)
{
	CDC * pDC = GetDC();
	CDC m_MemDC;
	m_MemDC.CreateCompatibleDC(pDC);
	CSize m_Size = pDC->GetTextExtent("刷新");
	ReleaseDC(pDC);
	CRect rect(0,0,60,32);
	CBitmap *pBitmap,*pOldBitmap;
	pBitmap = new CBitmap;
	pBitmap->LoadBitmap(nID);
	pOldBitmap = m_MemDC.SelectObject(pBitmap);
	TransParentDC(rect,&m_MemDC);
	m_MemDC.SelectObject(pOldBitmap);
	m_ToolBarList.Add(pBitmap,GetSysColor(COLOR_MENU));
	pBitmap->DeleteObject();
	delete pBitmap;
	return m_Size;
}







