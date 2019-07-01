// MyReg.h: interface for the CMyReg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYREG_H__DBABDB7B_3B64_4559_A4B5_3BBEA8E106D7__INCLUDED_)
#define AFX_MYREG_H__DBABDB7B_3B64_4559_A4B5_3BBEA8E106D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CMyReg  
{
public:
	unsigned int m_nV2McuCode;
	unsigned int m_nV2EndTime;
	int m_nRegState;
public:
	int OnCheckRegCode(CString strRegCode);
	int OnReadMbV2RegCode(CString str);
	CMyReg();
	virtual ~CMyReg();

};

#endif // !defined(AFX_MYREG_H__DBABDB7B_3B64_4559_A4B5_3BBEA8E106D7__INCLUDED_)
