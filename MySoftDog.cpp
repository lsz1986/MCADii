// MySoftDog.cpp: implementation of the CMySoftDog class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "mcad.h"
#include "MySoftDog.h"

CMySoftDog gMySoftDog;

#define USBHID_PID 0x0550
#define USBHID_VID 0xA728

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HANDLE g_hEventObject;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySoftDog::CMySoftDog()
{

}

CMySoftDog::~CMySoftDog()
{

}

void CMySoftDog::FindTheHID()
{
	HIDD_ATTRIBUTES Attributes;
	SP_DEVICE_INTERFACE_DATA devInfoData;
	int MemberIndex = 0;
	LONG Result;
	CString UsageDescription;
	ULONG Required;
	ULONG Length;
	HANDLE hDevInfo;
	GUID HidGuid;

	m_detailData = NULL;
	m_hSoftDogDevHandle = NULL;
	HidD_GetHidGuid(&HidGuid);	
	
	hDevInfo=SetupDiGetClassDevs(&HidGuid,NULL,NULL,DIGCF_PRESENT|DIGCF_INTERFACEDEVICE);
	devInfoData.cbSize = sizeof(devInfoData);
	
	MemberIndex = 0;
	do
	{
		Result=SetupDiEnumDeviceInterfaces(hDevInfo,0,&HidGuid,MemberIndex,&devInfoData);
		if (Result == 0)
		{
			break;
		}
		Result = SetupDiGetDeviceInterfaceDetail(hDevInfo,&devInfoData,NULL,0,&Length,NULL);
		m_detailData = (PSP_DEVICE_INTERFACE_DETAIL_DATA)malloc(Length);
		m_detailData -> cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);
		Result = SetupDiGetDeviceInterfaceDetail(hDevInfo,&devInfoData,m_detailData,Length,&Required,NULL);
		m_hSoftDogDevHandle = CreateFile(m_detailData->DevicePath,0,FILE_SHARE_READ|FILE_SHARE_WRITE,(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,0,NULL);
		Attributes.Size = sizeof(Attributes);
		Result = HidD_GetAttributes(m_hSoftDogDevHandle,&Attributes);
		
		//Is it the desired device?
		m_SoftDogDetected = FALSE;
		if( (Attributes.VendorID == USBHID_VID )&&(Attributes.ProductID == USBHID_PID) )
		{
			m_SoftDogDetected = TRUE;
			GetDeviceCapabilities();
			m_hSoftDogWrite=CreateFile(m_detailData->DevicePath,GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE, 
				(LPSECURITY_ATTRIBUTES)NULL,OPEN_EXISTING,0,NULL);
			PrepareForOverlappedTransfer();
		}
		else
		{
			CloseHandle(m_hSoftDogDevHandle);
		}
		free(m_detailData);
		MemberIndex = MemberIndex + 1;
	}
	while(m_SoftDogDetected == FALSE);
	
	SetupDiDestroyDeviceInfoList(hDevInfo);
	return;
}

void CMySoftDog::GetDeviceCapabilities()
{
	PHIDP_PREPARSED_DATA	PreparsedData;
	HidD_GetPreparsedData (m_hSoftDogDevHandle, &PreparsedData);
	HidP_GetCaps(PreparsedData, &m_Capabilities);
	HidD_FreePreparsedData(PreparsedData);
}

void CMySoftDog::PrepareForOverlappedTransfer()
{
	m_hSoftDogRead = CreateFile (m_detailData->DevicePath, GENERIC_READ, 
		FILE_SHARE_READ|FILE_SHARE_WRITE, (LPSECURITY_ATTRIBUTES)NULL, 
		OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
	
	if (g_hEventObject == 0)
	{
		g_hEventObject = CreateEvent (NULL, TRUE, TRUE, "");
		m_HIDOverlapped.hEvent = g_hEventObject;
		m_HIDOverlapped.Offset = 0;
		m_HIDOverlapped.OffsetHigh = 0;
	}
}

void CMySoftDog::CloseHandles()
{
	if (m_hSoftDogDevHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSoftDogDevHandle);
	}
	
	if (m_hSoftDogRead != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSoftDogRead);
	}
	
	if (m_hSoftDogWrite != INVALID_HANDLE_VALUE)
	{
		CloseHandle(m_hSoftDogWrite);
	}
}

void CMySoftDog::SendCmdToSoftDog(int nCmd1,int nCmd2)
{
	char WriteBuf[256];
	int i;
	UNION_U832 unionU832Temp;
	WriteBuf[0] = 0;
	WriteBuf[1] = 0;
	WriteBuf[2] = 'Z';
	WriteBuf[3] = 'H';
	WriteBuf[4] = 'K';
	WriteBuf[5] = 'J';
	unionU832Temp.u32buff = nCmd1;
	for (i=0;i<4;i++)
	{
		WriteBuf[i+6] = unionU832Temp.u8buff[i];
	}
	WriteBuf[10] = 'M';
	WriteBuf[11] = 'C';
	unionU832Temp.u32buff = nCmd2;
	for (i=0;i<4;i++)
	{
		WriteBuf[i+12] = unionU832Temp.u8buff[i];
	}
	for (i=16;i<256;i++)
	{
		WriteBuf[i] = 0;
	}
	
	if (m_SoftDogDetected==FALSE)
	{
		FindTheHID();
	}
	
	if (m_SoftDogDetected != TRUE)
		return;
	
	DWORD BytesWritten = 0;
	ULONG Result = 0;
	
	if (m_hSoftDogWrite != INVALID_HANDLE_VALUE)
	{
		Result = WriteFile(m_hSoftDogWrite,
			WriteBuf,
			m_Capabilities.OutputReportByteLength,
			&BytesWritten,NULL);
	}
	if (!Result)
	{
		CloseHandles();
		m_SoftDogDetected = FALSE;
	}
}

void CMySoftDog::ReadHidData()
{
	u8 ReadBuf[256];
	int i;
	DWORD NumberOfBytesRead;
	DWORD	Result;
	memset(ReadBuf,0,256);
	
	if (m_hSoftDogRead != INVALID_HANDLE_VALUE)
	{
		Result = ReadFile(m_hSoftDogRead,
			ReadBuf,
			m_Capabilities.InputReportByteLength,
			&NumberOfBytesRead,
			(LPOVERLAPPED) &m_HIDOverlapped); 
	}
	
	Result = WaitForSingleObject(g_hEventObject,6000);
	switch (Result)
	{
	case WAIT_OBJECT_0:
		break;
		
	case WAIT_TIMEOUT:
		Result = CancelIo(m_hSoftDogRead);
		CloseHandles();
		m_SoftDogDetected = FALSE;
		break;
		
	default:
		CloseHandles();
		m_SoftDogDetected = FALSE;
		break;
	}
	ResetEvent(g_hEventObject);
	
	//------------ADD-----------
	CloseHandles();
	m_SoftDogDetected = FALSE;
	//-------------------------
	memset(m_cHidRdBuf,0,256);

	for (i=0; i < m_Capabilities.InputReportByteLength; i++)
	{
		m_cHidRdBuf[i] = ReadBuf[i];
	}
}


