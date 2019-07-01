// MyUSB.h: interface for the CMyUSB class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYUSB_H__A22D3F96_2E73_4786_AB10_0918E5CE35C0__INCLUDED_)
#define AFX_MYUSB_H__A22D3F96_2E73_4786_AB10_0918E5CE35C0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "lusb0_usb.h"
#include "GlobalDef.h"
#include "afxmt.h"

class CMyUSB  
{
public:
	int m_nDeviceFound;

public:
	int OnSetDeblockCode(u32 debCode);
	int OnSetDeblockCode_0(u32 debCode);
	int OnGetDeblockCode_0();
	int OnGetDeblockCode();
	
	int OnGetLastTime();
	int OnSetLastTime(u32 Time);
	int OnEraseMcuFlash();

	int OnGetChipID();

	int OnWorkPause();
	int OnWorkResume();
	int OnWorkCancel();

	int OnStartTest(u8 TestType,u16 TestPara);
	int OnCmdReturn();
	int OnGetMacState();
	int OnParaWrite();
	int OnParaRead();

	int OnNewJob(int cncMaxX,int cncMaxY,int CncDataSum); //进入传输切割数据状态
	int OnStartCnc(BOOL bWorkStartAtPause); //进入传输切割数据状态
	void Close();
	int Open();
	int Read(char *pbBuf, ULONG len);
	int WriteCmd(char *pbBuf, ULONG len);
	int WriteBulk(char *pbBuf, ULONG len);
	usb_dev_handle *m_pUSBdev;
	CMyUSB();
	virtual ~CMyUSB();
};

#endif // !defined(AFX_MYUSB_H__A22D3F96_2E73_4786_AB10_0918E5CE35C0__INCLUDED_)
