#include "StdAfx.h"
#include "Global.h"

CWinThread* gThreadAutoConnect;

UINT ThreadAutoConnect(LPVOID pParam) //�Զ����ӻ�������ʼ����
{
	static int kk=0;
	while(1)
	{
		if (gSet.getAppExitFlag()) //����ر�ǰ�ȹر��߳�
		{
			gThreadAutoConnect = NULL;
			return 0;
		}
		if(gSet.getQueryEnable()) //�����ѯ
		{
			if (!g_bParaRead)
			{
				ReadMacPara(); //��ʱ��⣬δ��ȡ���ȡһ�β���
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

