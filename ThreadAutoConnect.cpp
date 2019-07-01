#include "StdAfx.h"
#include "Global.h"

CWinThread* gThreadAutoConnect;

UINT ThreadAutoConnect(LPVOID pParam) //自动连接机器并开始服务
{
	static int kk=0;
	while(1)
	{
		if (gSet.getAppExitFlag()) //程序关闭前先关闭线程
		{
			gThreadAutoConnect = NULL;
			return 0;
		}
		if(gSet.getQueryEnable()) //允许查询
		{
			if (!g_bParaRead)
			{
				ReadMacPara(); //定时检测，未读取则读取一次参数
			}
			if (gUSB.OnGetMacState() != 0 )
			{
				kk++;
				if (kk > 4)
				{
					kk = 0;
					g_bParaRead = FALSE;
				}
			}
			else
			{
				kk = 0;
			}
			g_pMainfrm->SendMessage(USER_DISP_STAT,0,0);
		}
		Sleep(400);
	}		
}

