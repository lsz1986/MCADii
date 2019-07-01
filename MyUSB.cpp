// MyUSB.cpp: implementation of the CMyUSB class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MyUSB.h"
#include "Global.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

#define MY_VID 0xA728 //³§ÉÌID
#define MY_PID 0x0501 //²úÆ·ID Ä£°å£¬ÇÐ¸îÒ»Ìå»ú

#define MY_CONFIG 1
#define MY_INTF 0

#define CMD_HEAD "ZHM2"

#define EP1_IN 0x81
#define EP2_OUT 0x02
#define EP3_OUT 0x03

usb_dev_handle *open_dev(void);

usb_dev_handle *open_dev(void)
{
	struct usb_bus *bus;
	struct usb_device *dev;
	
	for (bus = usb_get_busses(); bus; bus = bus->next)
	{
		for (dev = bus->devices; dev; dev = dev->next)
		{
			if (dev->descriptor.idVendor == MY_VID
				&& dev->descriptor.idProduct == MY_PID)
			{
				return usb_open(dev);
			}
		}
	}
	return NULL;
}

CMyUSB::CMyUSB()
{
	usb_init(); /* initialize the library */
	usb_find_busses(); /* find all busses */
	m_nDeviceFound = usb_find_devices(); /* find all connected devices */
}

CMyUSB::~CMyUSB()
{
	Close();
}

void CMyUSB::Close()
{
	if (m_pUSBdev != 0)
	{
		usb_release_interface(m_pUSBdev,0);
		usb_close(m_pUSBdev);
		m_pUSBdev = 0;
	}
}

int CMyUSB::Open()
{
	if (m_pUSBdev == 0)
	{
		if (m_nDeviceFound != 1)
		{
			m_nDeviceFound = usb_find_devices();
//			return -1;
		}
		if (!(m_pUSBdev = open_dev()))
		{
			return -1;
		}
		if (usb_claim_interface(m_pUSBdev, 0) < 0)
		{
			usb_close(m_pUSBdev);
			return -1;
		}
	}
	return 0;
}

int CMyUSB::Read(char *pbBuf, ULONG len)
{
	int ret;
	if (Open() != 0)
	{
		return -1;
	}

	ret = usb_bulk_read(m_pUSBdev, EP1_IN, pbBuf, len, 1000);
	Close(); //Ã¿¶ÁÈ¡Ò»´Î¾Í¹Ø±Õ??

	return ret;
}

int CMyUSB::WriteCmd(char *pbBuf, ULONG len)
{
	int ret;

	if (Open() != 0)
	{
		return -1;
	}

	ret = usb_bulk_write(m_pUSBdev, EP2_OUT, pbBuf, len, 50);

	Close(); //Ã¿·¢ËÍÒ»¸öÃüÁî¾Í¹Ø±Õ??
	return ret;
}

int CMyUSB::WriteBulk(char *pbBuf, ULONG len)
{
	int ret=0;

	if (Open() != 0)
	{
		ret = -1;
	}

	ret = usb_bulk_write(m_pUSBdev, EP3_OUT, pbBuf, len, 1000);
	Close(); //Ã¿·¢ËÍÒ»°üÊý¾Ý¾Í¹Ø±Õ

	return ret;
}

int CMyUSB::OnParaRead()
{
//ÃüÁî¸ñÊ½: ³¤¶È(1B) + ÃüÁîÍ·ZHKJ(4B) + CMD_READ_PARA 
//·µ»ØÖµ: -1 ÃüÁîÊ§°Ü,0 ÃüÁî³É¹¦

	char buff[64];
	int ret=0;
	int i;
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_PARA_READ;
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	if( Read(buff,64) != MAC_PARA_SIZE)
	{
		return -1;
	}
	for(i=0;i<MAC_PARA_SIZE;i++)
	{
		gTempPara.charBuff[i] = buff[i];
	}
	return 0;
}

int CMyUSB::OnParaWrite()
{
//ÃüÁî¸ñÊ½: ³¤¶È(1B) + ÃüÁîÍ·ZHKJ(4B) + CMD_READ_WITE 
//·µ»ØÖµ: -1 ÃüÁîÊ§°Ü,0 ÃüÁî³É¹¦
	char buff[64];
	int ret=0;
	int i;
	buff[0] = MAC_PARA_SIZE+6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_PARA_WRITE;

	for (i=0;i<MAC_PARA_SIZE;i++)
	{
		buff[i+6] = gTempPara.charBuff[i];
	}
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; 
	}
	return OnCmdReturn();
}

int CMyUSB::OnGetMacState()
{
	//ÃüÁî¸ñÊ½: ³¤¶È(1B) + ÃüÁîÍ·ZHKJ(4B) + CMD_READ_WITE 
	//·µ»ØÖµ: -1 ÃüÁîÊ§°Ü,0 ÃüÁî³É¹¦
	char buff[64];
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_GET_STATE;
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}

	return OnCmdReturn();
}



int CMyUSB::OnCmdReturn()
{
	char buff[64];
	int i;
	int revlen = Read(buff,64);
	if( revlen != MAC_STATE_SIZE)
	{
		return -1;
	}
	for(i=0;i<MAC_STATE_SIZE;i++)
	{
		gTempSysState.charBuff[i] = buff[i];
	}
	gSysState = gTempSysState.stBuff;
	return gSysState.lastCmdState;
}

int CMyUSB::OnNewJob(int cncMaxX,int cncMaxY,int CncDataSum)
{
	char buff[64];
	UNION_U832 un832_temp;
	int i;

	buff[0] = 18;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_NEW_JOB;

	un832_temp.u32buff = cncMaxX;
	for (i=0;i<4;i++)
	{
		buff[6+i] = un832_temp.u8buff[i];
	}

	un832_temp.u32buff = cncMaxY;
	for (i=0;i<4;i++)
	{
		buff[10+i] = un832_temp.u8buff[i];
	}

	un832_temp.u32buff = CncDataSum;
	for (i=0;i<4;i++)
	{
		buff[14+i] = un832_temp.u8buff[i];
	}

	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	return OnCmdReturn();
}

int CMyUSB::OnStartCnc(BOOL bWorkStartAtPause)
{
	char buff[64];	
	buff[0] = 7;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_START_JOB;
	buff[6] = bWorkStartAtPause;

	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	return OnCmdReturn();
}

int CMyUSB::OnStartTest(u8 TestType,u16 TestPara)
{
	char buff[64];
	UNION_U816 u816temp;
	u816temp.u16buff = TestPara;
	buff[0] = 9;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_START_TEST;
	buff[6] = TestType;
	buff[7] = u816temp.u8buff[0];
	buff[8] = u816temp.u8buff[1];
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}

	return OnCmdReturn();
}

int CMyUSB::OnGetChipID()
{
	UNION_U832 temp;
	char buff[64];
	
	buff[0] = 7;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_GET_CHIPID;
	buff[6] = 0x60; //¿É±äµØÖ· ·ÀÖ¹ÆÆ½â 
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	if( Read(buff,64) != 4)
	{
		return -1;
	}
	for (int i=0;i<4;i++)
	{
		temp.u8buff[i] = buff[i];
	}

	return temp.u32buff;
}

int CMyUSB::OnEraseMcuFlash()
{
	char buff[64];
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_ERASE;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	return OnCmdReturn();
}

int CMyUSB::OnSetLastTime(u32 Time)
{
	char buff[64];
	UNION_U832 u832temp;
	u832temp.u32buff = Time;
	buff[0] = 10;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_SET_LAST_TIME;

	buff[6] = u832temp.u8buff[0];
	buff[7] = u832temp.u8buff[1];
	buff[8] = u832temp.u8buff[2];
	buff[9] = u832temp.u8buff[3];
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	return OnCmdReturn();
}

int CMyUSB::OnGetLastTime()
{
	UNION_U832 temp;
	char buff[64];
	
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_GET_LAST_TIME;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	if( Read(buff,64) != 4)
	{
		return -1;
	}
	for (int i=0;i<4;i++)
	{
		temp.u8buff[i] = buff[i];
	}
	return temp.u32buff;

}

int CMyUSB::OnWorkPause()
{
	char buff[64];
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_WORK_PAUSE;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	
	return OnCmdReturn();
}

int CMyUSB::OnWorkResume()
{
	char buff[64];
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_WORK_RESUME;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	
	return OnCmdReturn();
}

int CMyUSB::OnWorkCancel()
{
	char buff[64];
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_WORK_CANCEL;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	
	return OnCmdReturn();
}

int CMyUSB::OnSetDeblockCode(u32 debCode)
{
	int rev;
//	EnterCriticalSection(&m_cs);
	rev = OnSetDeblockCode_0(debCode);
//	LeaveCriticalSection(&m_cs);
	return rev;
}

int CMyUSB::OnSetDeblockCode_0(u32 debCode)
{
	char buff[64];
	UNION_U832 u832temp;
	u832temp.u32buff = debCode;
	buff[0] = 10;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_SET_DEB_CODE;
	
	buff[6] = u832temp.u8buff[0];
	buff[7] = u832temp.u8buff[1];
	buff[8] = u832temp.u8buff[2];
	buff[9] = u832temp.u8buff[3];
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	if( Read(buff,64) != 1)
	{
		return -2;
	}
	return buff[0];
}


int CMyUSB::OnGetDeblockCode()
{
	int rev;
//	EnterCriticalSection(&m_cs);
	rev = OnGetDeblockCode_0();
//	LeaveCriticalSection(&m_cs);
	return rev;
}

int CMyUSB::OnGetDeblockCode_0()
{
	UNION_U832 temp;
	char buff[64];
	
	buff[0] = 6;
	strncpy(&buff[1],CMD_HEAD,4);
	buff[5] = CMD_GET_DEB_CODE;
	
	if ( ((int)buff[0] ) != (WriteCmd(&buff[0],buff[0]) ) )
	{
		return -1; //ÃüÁî·¢ËÍÊ§°Ü
	}
	if( Read(buff,64) != 4)
	{
		return -1;
	}
	for (int i=0;i<4;i++)
	{
		temp.u8buff[i] = buff[i];
	}
	return temp.u32buff;
}
