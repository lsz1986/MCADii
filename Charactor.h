// Charactor.h: interface for the CCharactor class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARACTOR_H__EEB048E6_4476_4C3E_BA79_F2F84F7A13F9__INCLUDED_)
#define AFX_CHARACTOR_H__EEB048E6_4476_4C3E_BA79_F2F84F7A13F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CCharactor  
{
public:
	WORD m_wCharCode;
	WORD m_wBytes;
	WORD m_wWidth;
	DWORD m_dwPosition;

public:
	CCharactor();
	virtual ~CCharactor();

};

#endif // !defined(AFX_CHARACTOR_H__EEB048E6_4476_4C3E_BA79_F2F84F7A13F9__INCLUDED_)
