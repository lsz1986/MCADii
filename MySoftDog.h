// MySoftDog.h: interface for the CMySoftDog class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYSOFTDOG_H__D2BA674F_DDB9_48F2_A303_8B95427E7124__INCLUDED_)
#define AFX_MYSOFTDOG_H__D2BA674F_DDB9_48F2_A303_8B95427E7124__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "GlobalDef.h"

extern "C" {
#include "hidsdi.h"
#include "setupapi.h"
#include <dbt.h>
}


class CMySoftDog  
{
public:
	BOOL m_SoftDogDetected;
	HANDLE m_hSoftDogDevHandle;
	HANDLE m_hSoftDogRead;
	HANDLE m_hSoftDogWrite;
	OVERLAPPED m_HIDOverlapped;
	PSP_DEVICE_INTERFACE_DETAIL_DATA m_detailData;
	HIDP_CAPS m_Capabilities;
	u8 m_cHidRdBuf[256];

public:
	void ReadHidData();
	void SendCmdToSoftDog(int nCmd1,int nCmd2);
	void CloseHandles();
	void PrepareForOverlappedTransfer();
	void GetDeviceCapabilities();
	void FindTheHID();
	CMySoftDog();
	virtual ~CMySoftDog();

};

#endif // !defined(AFX_MYSOFTDOG_H__D2BA674F_DDB9_48F2_A303_8B95427E7124__INCLUDED_)
