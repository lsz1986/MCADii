#include "StdAfx.h"
#include "Settings.h"
#include "Global.h"

CSettings gSet; //全局变量
CMyReg gReg;
CColorSettings gColorSet;
COutLineProp gOutLineProp;
CTextProp gTextProp;

bool g_bParaRead = FALSE;

ST_MAC_PARA gPara;
UNION_MAC_PARA gTempPara;
ST_SYS_STATE gSysState;
UNION_SYS_STATE gTempSysState;
CMyUSB gUSB;
CPtrArray g_ptrCncList;  //加工指令列表

long IntRound(double x)
{
    double d;
    long lResult;
	
    if (x < 0)
        d = x - 0.5;
    else
        d = x + 0.5;
    lResult = (long)d;
    return lResult;
}

BOOL IsAlpha(char ch)
{
    if(ch >= 'a' && ch <= 'z') return TRUE;
    if(ch >= 'A' && ch <= 'Z') return TRUE;
    return FALSE;
}

BOOL IsDigit(char ch)
{
    if(ch >= '0' && ch <= '9') return TRUE;
    if(ch == '.') return TRUE;
    if(ch == '-' || ch == '+') return TRUE;
    return FALSE;
}

int ftoi(double f)
{
    int k;
    if(f > 5e+75 || f < -5e+75) return 1;
    if(f >= 0)
    {
        f += 0.5;
        k = (int)f;
    }
    else
    {
        f -= 0.5;
        k = (int)f;
        if(k == f) k++; // ftoi(0.5)=1,ftoi(-0.5)=0
    }
    return k;
}

//0 点在线上
//>0 点在线段的左侧
//<0 点在线段的右侧
int PtCrossLine(CPoint pt,CPoint ptStart,CPoint ptEnd)
{
	return (pt.y-ptStart.y)*(ptEnd.x-ptStart.x) - (pt.x-ptStart.x)*(ptEnd.y-ptStart.y);
}

void delayus(int num)
{
	LARGE_INTEGER litmp;
	LONGLONG QPart1,QPart2;
	double dfMinus, dfFreq, dfTim;
	QueryPerformanceFrequency(&litmp);
	dfFreq = (double)litmp.QuadPart;// 获得计数器的时钟频率
	QueryPerformanceCounter(&litmp);
	QPart1 = litmp.QuadPart;// 获得初始值
	do
	{
		QueryPerformanceCounter(&litmp);
		QPart2 = litmp.QuadPart;//获得中止值
		dfMinus = (double)(QPart2-QPart1);
		dfTim = dfMinus / dfFreq;// 获得对应的时间值，单位为秒
	}while(dfTim < 0.000001*num);
}

//9位整数 最大 23 27 5E 62
const int YHZIntArray[16] = {
	214072011,/*0*/
	180416531,/*1*/
	188092317,/*2*/
	122988619,/*3*/
	142797956,/*4*/
	366543210,/*5*/
	305219805,/*6*/
	111022060,/*7*/
	151032792,/*8*/
	187654328,/*9*/
	169227610,/*10*/
	168688886,/*11*/
	121314872,/*12*/
	151327678,/*13*/
	141526598,/*14*/
	589782626/*15*/
};

CPoint ptHpglToScr(CPoint ptHpgl)
{
	CPoint ptScr;
	ptScr.x = (int)((double)(ptHpgl.x*gSet.getDispScale()+DISP_START_X - gSet.getDispStartX() ) );
	ptScr.y = (int)((double)(gSet.getScrYPixel()-ptHpgl.y*gSet.getDispScale()-DISP_START_Y - gSet.getDispStartY() ) );
	return ptScr;
}

CPoint ptScrToHpgl(CPoint ptScr)
{
	CPoint ptHpgl;
	ptHpgl.x = (int)( (double)(ptScr.x + gSet.getDispStartX() - DISP_START_X)/gSet.getDispScale() );
	ptHpgl.y = (int)( (double)(gSet.getScrYPixel()-ptScr.y-gSet.getDispStartY()-DISP_START_Y)/gSet.getDispScale() );
	return ptHpgl;
}

ST_DPOINT GetLineIntersection(ST_LINE RefLine1, ST_LINE RefLine2)
{
	ST_DPOINT Intersection;
	double K1,K2,Delta_X1,Delta_Y1,Delta_X2,Delta_Y2;
	Delta_X1=RefLine1.End.x-RefLine1.Start.x;
	Delta_Y1=RefLine1.End.y-RefLine1.Start.y;
	Delta_X2=RefLine2.End.x-RefLine2.Start.x;
	Delta_Y2=RefLine2.End.y-RefLine2.Start.y;
	
	if (Delta_X1==0) 
   	{
		if (Delta_X2==0)
		{
			Intersection = RefLine1.End;
			return Intersection;
		}
		else
		{
			K2=(double)Delta_Y2/((double)(Delta_X2));
			Intersection.x=RefLine1.Start.x;
			Intersection.y=K2*(Intersection.x- RefLine2.Start.x)+ RefLine2.Start.y ;
			return Intersection;
		}
   	}
	else if (Delta_X2==0)
   	{
   	    K1=(double)Delta_Y1/((double)(Delta_X1));
		Intersection.x = RefLine2.Start.x;
    	Intersection.y = K1*(Intersection.x- RefLine1.Start.x)+ RefLine1.Start.y ;
		return Intersection;
   	}
	else
   	{
   	    K1=(double)Delta_Y1/((double)(Delta_X1));
		K2=(double)Delta_Y2/((double)(Delta_X2));
		if(K1==K2)
		{
			Intersection = RefLine1.End;
			return Intersection;
		}
		else
		{
			Intersection.x = (RefLine2.Start.y-RefLine1.Start.y-(K2*RefLine2.Start.x-K1*RefLine1.Start.x))/(K1-K2);
			Intersection.y = K1*(Intersection.x-RefLine1.Start.x)+RefLine1.Start.y;
			return Intersection;
		}
   	}
}

double getdistance(CPoint pt1,CPoint pt2)
{
	double dx,dy;
	dx = fabs( (double)(pt1.x-pt2.x));
	dy = fabs( (double)(pt1.y-pt2.y));
	return sqrt(dx*dx+dy*dy);
}

int ReadMacPara()
{
	g_bParaRead = FALSE;
	if(0 == gUSB.OnParaRead()) //参数读取成功
	{
		g_bParaRead = TRUE;
		gPara = gTempPara.stBuff;

		gSet.setPPMMX(gPara.m_dPPMMX);
		gSet.setPPMMY(gPara.m_dPPMMY);
		gSet.setPPMMZ(gPara.m_dPPMMZ);

		gSet.setMacSizeX(gPara.m_sXSizeMm);
		gSet.setMacSizeY(gPara.m_sYSizeMm);

		gSet.set10000X(gPara.m_s10000X);
		gSet.set10000Y(gPara.m_s10000Y);

		gSet.setMPDistX(gPara.m_cMPDistX);
		gSet.setMPDistY(gPara.m_cMPDistY);

		gSet.setKPDistX(gPara.m_cKPDistX);
		gSet.setKPDistY(gPara.m_cKPDistY);

		gSet.setSpdPlot(gPara.m_cSpdPlot);
		gSet.setSpdMilling(gPara.m_cSpdMilling);
		gSet.setSpdCut(gPara.m_cSpdCut);
		gSet.setSpdMove(gPara.m_cSpdMove);

		gSet.setPwmPen(gPara.m_cPwmPen);
		gSet.setPwmCut(gPara.m_cPwmCut);

		gSet.setZPosMove(gPara.m_sZPosMove);
		gSet.setZPosWork(gPara.m_sZPosWork);
		gSet.setAngleAdjust(gPara.m_cAngleAdjust);
		gSet.setVmEnable(gPara.m_cVMEnable);
		gSet.setMMotorOnDelay(gPara.m_cMMOnDelay);
		return 0;
	}
	return -1;
}

void WriteMacPara()
{
	if (FALSE == g_bParaRead)
	{
		return;
	}
	gPara.m_dPPMMX = gSet.getPPMMX();
	gPara.m_dPPMMY = gSet.getPPMMY();
	gPara.m_dPPMMZ = gSet.getPPMMZ();

	gPara.m_sXSizeMm = gSet.getMacSizeX();
	gPara.m_sYSizeMm = gSet.getMacSizeY();

	gPara.m_s10000X = gSet.get10000X();
	gPara.m_s10000Y = gSet.get10000Y();

	gPara.m_cMPDistX = gSet.getMPDistX();
	gPara.m_cMPDistY = gSet.getMPDistY();
	gPara.m_cKPDistX = gSet.getKPDistX();
	gPara.m_cKPDistY = gSet.getKPDistY();

	gPara.m_cSpdPlot = gSet.getSpdPlot();
	gPara.m_cSpdMilling = gSet.getSpdMilling();
	gPara.m_cSpdCut = gSet.getSpdCut();
	gPara.m_cSpdMove = gSet.getSpdMove();

	gPara.m_cPwmPen = gSet.getPwmPen();
	gPara.m_cPwmCut =  gSet.getPwmCut();

	gPara.m_sZPosMove = gSet.getZPosMove();
	gPara.m_sZPosWork = gSet.getZPosWork();

	gPara.m_cAngleAdjust = gSet.getAngleAdjust();
	gPara.m_cVMEnable = gSet.getVmEnable();
	gPara.m_cMMOnDelay = gSet.getMMotorOnDelay();

	gTempPara.stBuff = gPara;
	if(0 != gUSB.OnParaWrite() )
	{
		AfxMessageBox("保存参数失败!");
	}
}