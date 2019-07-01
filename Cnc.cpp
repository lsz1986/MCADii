#include "StdAfx.h"
#include "Global.h"
UNION_CNC_DATA_SEND unionCncData[50000];
int gnCncXMax,gnCncYMax;

void ReleaseCncDataBuffer()
{
	int i;
	ST_CNC_DATA_ALL* pCutData;
	for (i=0;i<g_ptrCncList.GetSize();i++)
	{
		pCutData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		delete pCutData;
	}
	g_ptrCncList.RemoveAll();
}

void GetLongAxialSteps()
{
	int i;
	ST_CNC_DATA_ALL *pCncData;
	int nSize;
	nSize =g_ptrCncList.GetSize();
	if (nSize == 0)
	{
		return;
	}
	for (i=0;i<nSize;i++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		//获得长轴脉冲数
		pCncData->m_nLongAxialStep = 
			(abs(pCncData->m_iDeltaX) > abs(pCncData->m_iDeltaY))?
			abs(pCncData->m_iDeltaX):abs(pCncData->m_iDeltaY);
	}
}

void CalculateAngles()
{
	int i;
	ST_CNC_DATA_ALL *pCncData;
	int iLastX,iLastY;
	double dLastAngle;
	int nSize;
	nSize = g_ptrCncList.GetSize();
	if (nSize <= 0)
	{
		return;
	}
	
	pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(0);
	pCncData->m_iDeltaX = 0;
	pCncData->m_iDeltaY = 0;
	pCncData->m_dAngle = 0;
	pCncData->m_dDeltaAngle = 0;
	
	iLastX = pCncData->m_iX;
	iLastY = pCncData->m_iY;
	dLastAngle = pCncData->m_dAngle;
	
	for (i=1;i<nSize;i++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		pCncData->m_iDeltaX = pCncData->m_iX - iLastX;
		pCncData->m_iDeltaY = pCncData->m_iY - iLastY;
		pCncData->m_dAngle = atan2((double)(pCncData->m_iDeltaY), (double)(pCncData->m_iDeltaX) );
		pCncData->m_dDeltaAngle = pCncData->m_dAngle - dLastAngle;
		
		if (pCncData->m_dDeltaAngle>PI)
		{
			pCncData->m_dDeltaAngle = pCncData->m_dDeltaAngle - 2.0*PI;
		}
		else if (pCncData->m_dDeltaAngle <= -PI)
		{
			pCncData->m_dDeltaAngle = pCncData->m_dDeltaAngle + 2.0*PI;
		}
		
		iLastX = pCncData->m_iX;
		iLastY = pCncData->m_iY;
		dLastAngle = pCncData->m_dAngle;
	}
}

void DumpDataToFile()
{
	int i;
	ST_CNC_DATA_ALL *pCncData;
	CFile fileData;
	CString strTemp;
	CString strFileName;
	int nSize;
	strFileName.Format("d:\\MCAD.csv");
	fileData.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
	nSize = g_ptrCncList.GetSize();
	strTemp.Format("目标X,目标Y,DeltaX,DeltaY,加速次数,角度,角度变化,曲率,降速比,MaxSpd,EndSpd,类型\n");
	
	fileData.Write(strTemp.GetBuffer(0),strTemp.GetLength());
	for (i=0;i<nSize;i++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		
		strTemp.Format("%d,%d, %d,%d,%d, %f,%f,%f,%d, %d,%d,%c\n",
			pCncData->m_iX,
			pCncData->m_iY,
			
			pCncData->m_iDeltaX,
			pCncData->m_iDeltaY,
			pCncData->m_nLongAxialStep,
			
			pCncData->m_dAngle*180.0/PI,
			pCncData->m_dDeltaAngle*180.0/PI,
			pCncData->m_dCurvature,
			(u16)( pCncData->m_dDccPerCent * 4096.0 ),
			pCncData->m_nMaxSpeed,
			pCncData->m_nEndSpeed,
			pCncData->m_nCmdType);
		fileData.Write(strTemp.GetBuffer(0),strTemp.GetLength());
	}
	fileData.Close();
	AfxMessageBox(strFileName+" Saved");
}

void CalcCurvature()
{
	int i;
	ST_CNC_DATA_ALL *pCncData;
	double dX,dY,dR;

	int nSize;
	nSize =g_ptrCncList.GetSize();
	if (nSize == 0)
	{
		return;
	}
	pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(0);
	pCncData->m_dCurvature = 0;

	for (i=1;i<nSize;i++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		dX = pCncData->m_iDeltaX;
		dY = pCncData->m_iDeltaY;
		dR = sqrt(dX*dX+dY*dY);
		if (dR == 0)
		{
			pCncData->m_dCurvature = 0;
		}
		else
		{
			pCncData->m_dCurvature = fabs(pCncData->m_dDeltaAngle)/( dR / gSet.getPPMMY() );
			pCncData->m_dCurvature = pCncData->m_dCurvature*180.0;
		}
	}
}

void CalcSdPercent(void)
{
	int i,nSize;
	ST_CNC_DATA_ALL *pCncData;
	double dDeltaX,dDeltaY;
	double dR;
	nSize = g_ptrCncList.GetSize();
	if (nSize == 0)
	{
		return;
	}
	for (i=0;i<nSize;i++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		dDeltaX = fabs((double)(pCncData->m_iDeltaX));
		dDeltaY = fabs((double)(pCncData->m_iDeltaY));
		dR = sqrt(dDeltaX*dDeltaX+dDeltaY*dDeltaY);
		if ( dDeltaX > dDeltaY )
		{
			if (dDeltaX == 0.0)
			{
				pCncData->m_dDccPerCent = 1.0;
			}
			else
			{
				pCncData->m_dDccPerCent = dR/dDeltaX;
			}
		}
		else
		{
			if (dDeltaY == 0.0)
			{
				pCncData->m_dDccPerCent = 1.0;
			}
			else
			{
				pCncData->m_dDccPerCent = dR/dDeltaY;
			}
		}
	}
}

void CalcCncSpeed(void)
{
	int i,nSize;
	int nSpdLimit;
	ST_CNC_DATA_ALL *pCncData;
	ST_CNC_DATA_ALL *pCncDataNext;
	nSize = g_ptrCncList.GetSize();
	if (nSize == 0)
	{
		return;
	}

	pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(nSize-1);
	pCncData->m_nEndSpeed = 0;
	pCncData->m_nMaxSpeed = pCncData->m_nLongAxialStep;
	if ( TYPE_MM == pCncData->m_nCmdType )
	{
		nSpdLimit = (int)(CNC_MAX_SPD*(gSet.getSpdMilling()+3)*0.005-20);
	}
	else
	{
		nSpdLimit = CNC_MAX_SPD;
	}
	if (pCncData->m_nMaxSpeed > nSpdLimit)
	{
		pCncData->m_nMaxSpeed = nSpdLimit;
	}

	for (i=nSize-2;i>=0;i--) 
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		if (TYPE_MM == pCncData->m_nCmdType)
		{
			nSpdLimit = (int)(CNC_MAX_SPD*(gSet.getSpdMilling()+3)*0.005-20);
		}
		else
		{
			nSpdLimit = CNC_MAX_SPD;
		}

		pCncDataNext = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i+1);
		if (pCncDataNext->m_nCmdType == TYPE_MOVE) //下一条指令是移动
		{
			pCncData->m_nEndSpeed = 0;
			pCncData->m_nMaxSpeed = pCncData->m_nLongAxialStep;
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
			continue;
		}

		if (fabs(pCncDataNext->m_dDeltaAngle*180.0/PI) > SD_LV0_ANGLE)
		{
			pCncData->m_nEndSpeed = 0;
			pCncData->m_nMaxSpeed = pCncData->m_nLongAxialStep;
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
			continue;
		}
		else if (fabs(pCncDataNext->m_dDeltaAngle*180.0/PI) > SD_LV1_ANGLE)
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R002)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R002;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
			continue;
		}
		else if( pCncData->m_nLongAxialStep > (gSet.getCurveLen()*90) )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
			continue;
		}

		if (pCncDataNext->m_dCurvature > CURVATURE_R002 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R002)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R002;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R002)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R002;
			}	
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if (pCncDataNext->m_dCurvature > CURVATURE_R004 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R004)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R004;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R004)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R004;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if (pCncDataNext->m_dCurvature > CURVATURE_R005 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R005)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R005;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R005)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R005;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R010 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R010)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R010;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R010)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R010;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R020 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R020)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R020;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R020)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R020;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R050 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R050)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R050;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R050)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R050;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R100 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R100)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R100;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R100)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R100;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R150 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R150)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R150;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R150)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R150;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R200 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R200)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R200;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R200)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R200;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else if(pCncDataNext->m_dCurvature > CURVATURE_R300 )
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > CURVE_SPD_R300)
			{
				pCncData->m_nEndSpeed = CURVE_SPD_R300;
			}
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > CURVE_SPD_R300)
			{
				pCncData->m_nMaxSpeed = CURVE_SPD_R300;
			}
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
		else
		{
			pCncData->m_nEndSpeed = pCncDataNext->m_nMaxSpeed;
			if (pCncData->m_nEndSpeed > nSpdLimit)
			{
				pCncData->m_nEndSpeed = nSpdLimit;
			}
			pCncData->m_nMaxSpeed = (pCncData->m_nEndSpeed+pCncData->m_nLongAxialStep);
			if (pCncData->m_nMaxSpeed > nSpdLimit)
			{
				pCncData->m_nMaxSpeed = nSpdLimit;
			}
		}
	}
}

void GetCncMaxPulse()
{
	int nSize;
	ST_CNC_DATA_ALL *pCncData;
	int nIndex;
	nSize = g_ptrCncList.GetSize();

	pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(0);
	gnCncXMax = pCncData->m_iX;
	gnCncYMax = pCncData->m_iY;

	for (nIndex=1; nIndex<nSize; nIndex++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(nIndex);
		gnCncXMax = (gnCncXMax > pCncData->m_iX)? gnCncXMax : pCncData->m_iX;
		gnCncYMax = (gnCncYMax > pCncData->m_iY)? gnCncYMax : pCncData->m_iY;
	}
	return;
}

int SendCncData()
{
	int nSize;
	int rev;
	int nIndex;
	ST_CNC_DATA_ALL *pCncData;
	nSize = g_ptrCncList.GetSize();
	if (nSize >= 0x15555) //0xfffff / 0x0d = 0x15555 切割数据大小超过1M
	{
		return ERROR_BYTESIZE; //DOCUT_ERROR_BYTESIZE
	}
	rev = gUSB.OnNewJob(gnCncXMax,gnCncYMax,nSize);
	if (rev != ERROR_NONE)
	{
		return rev; // DOCUT_ERROR_XYSIZE
	}
	
	for (nIndex=0; nIndex<nSize; nIndex++)
	{
		pCncData = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(nIndex);
		unionCncData[nIndex].stBuff.m_iX = pCncData->m_iX;
		unionCncData[nIndex].stBuff.m_iY = pCncData->m_iY;
		unionCncData[nIndex].stBuff.m_nMaxSpeed = pCncData->m_nMaxSpeed;
		unionCncData[nIndex].stBuff.m_nEndSpeed = pCncData->m_nEndSpeed;
		unionCncData[nIndex].stBuff.m_nCmdType = pCncData->m_nCmdType;
		unionCncData[nIndex].stBuff.m_nDccPerCent = (u16)( pCncData->m_dDccPerCent * 4096.0 );
	}
	
	if (gUSB.WriteBulk((char*)(&unionCncData[0].u8Buff[0]),CNC_DATA_SIZE*nSize) != CNC_DATA_SIZE*nSize)
	{
		return ERROR_DATA;
	}
	
	if( 0 != gUSB.OnStartCnc(FALSE) )
	{
		return ERROR_NOTREADY;
	}
	return 0;
}



