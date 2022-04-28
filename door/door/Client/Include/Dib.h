// Dib.h: interface for the CDib class.
//////////////////////////////////////////////////////////////////////
//
//��;��IconSnap������õ���
//���ܣ�DIB�豸�޹�λͼ����������
//���ߣ��쾰��
//���ڣ�2001��9��
//
//////////////////////////////////////////////////////////////////////
#if !defined(AFX_DIB_H__1D89F734_EABB_4AFC_A3CD_DDEBA1E58AF3__INCLUDED_)
#define AFX_DIB_H__1D89F734_EABB_4AFC_A3CD_DDEBA1E58AF3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// How wide, in bytes, would this many bits be, DWORD aligned?
#define WIDTHBYTES(bits)      ((((bits) + 31)>>5)<<2)

class CDib  
{
public:
	CDib();
	virtual ~CDib();

public:
    LPSTR FindDIBBits (LPSTR lpbi);
	WORD DIBNumColors (LPSTR lpbi);
	WORD PaletteSize (LPSTR lpbi);
	DWORD BytesPerLine( LPBITMAPINFOHEADER lpBMIH );
	LPBYTE ConvertDIBFormat( LPBITMAPINFO lpSrcDIB, UINT nWidth, UINT nHeight, UINT nColors, BOOL bStretch );
	void SetMonoDIBPixel( LPBYTE pANDBits, DWORD dwWidth, DWORD dwHeight, DWORD x, DWORD y, BOOL bWhite );
	LPBYTE ReadBMPFile( LPCTSTR szFileName );
	BOOL WriteBMPFile( LPCTSTR szFileName, LPBYTE lpDIB );

private:
	BOOL CopyColorTable( LPBITMAPINFO lpTarget, LPBITMAPINFO lpSource );

};

#endif // !defined(AFX_DIB_H__1D89F734_EABB_4AFC_A3CD_DDEBA1E58AF3__INCLUDED_)
