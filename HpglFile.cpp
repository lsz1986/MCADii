// HpglFile.cpp: implementation of the CHpglFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MCAD.h"
#include "HpglFile.h"
#include "Global.h"
#include "Curve.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CHpglFile::CHpglFile()
{
	RemoveBuffer();
}

CHpglFile::~CHpglFile()
{
	RemoveBuffer();
}

CPoint CHpglFile::CPToLP(CPoint ptCP, int nHeight, CPoint ptPointLU, double dSin, double dCos, double dScale)
{
    double dx, dy;
    CPoint ptPoint1;
    dx = (double)ptCP.x;
    dy = (double)ptCP.y - (double)nHeight;
    ptPoint1.x = ptPointLU.x + IntRound((dx * dCos - dy * dSin) / dScale);
    ptPoint1.y = ptPointLU.y + IntRound((dx * dSin + dy * dCos) / dScale);
    return ptPoint1;
}

void CHpglFile::WriteTextData(BOOL bFirst, long x, long y)
{
    CPlotCmd *p;
    if(bFirst)
    {
        p = new CPlotCmd;
		m_pCmdList_ORG.Add(p);
        p->m_nType = PLOT_CMD_PU;
        p->m_anData.Add(x);
        p->m_anData.Add(y);

		p = new CPlotCmd;
		m_pCmdList_ORG.Add(p);
        p->m_nType = PLOT_CMD_PD;
    }
    else
    {
		p = (CPlotCmd *)m_pCmdList_ORG.GetAt(m_pCmdList_ORG.GetSize()-1);
        p->m_anData.Add(x);
        p->m_anData.Add(y);
    }
}

void CHpglFile::LineTo(long x, long y)
{
    CPoint ptCurrentPoint;
    CPoint ptPoint1;
    CPoint ptPoint2;
    CRect rect1;
	
    ptCurrentPoint.x = x;
    ptCurrentPoint.y = y;
	
    ptPoint1 = m_ptCurrentPosition;
    ptPoint2 = ptCurrentPoint;
    if ((m_ptCurrentPosition == m_ptPrevPosition) && (0 < m_ptCurrentPosition.x) && (0 < m_ptCurrentPosition.y))
    {
    }
    else
    {
        WriteTextData(true, ptPoint1.x, ptPoint1.y);
    }
    WriteTextData(false, ptPoint2.x, ptPoint2.y);
    m_ptPrevPosition = ptCurrentPoint;
    m_ptCurrentPosition = ptCurrentPoint;
}

void CHpglFile::MoveTo(long x, long y)
{
    m_ptCurrentPosition.x = x;
    m_ptCurrentPosition.y = y;
	
    m_ptPrevPosition = m_ptCurrentPosition;
    m_ptPrevPosition.x = m_ptPrevPosition.x + 1; //为在LineTo中优化用
}

void CHpglFile::Text2LineInit(void)
{
    CString strPL;
    CFile fFile1;
    int nCountChar;
    CCharactor Chinese_char1;
    CCharactor English_char1;
    BYTE byte[10];
    int nCharCount_C, nCharCount_E;
    CString strNetworkPath, string1;
    CFileFind filefind;
	
    m_ptCurrentPosition = NULL;
    fFile1.Open((LPCTSTR)gSet.m_strFonEn, CFile::modeRead | CFile::typeBinary);
    fFile1.SeekToBegin();
    fFile1.Read(byte, 10);
    m_nInternalLeading_E = (int)((WORD)byte[0] | ((WORD)byte[1] << 8));
    m_nHeight_E = (int)((WORD)byte[2] | ((WORD)byte[3] << 8));
    m_nDescent_E = (int)((WORD)byte[4] | ((WORD)byte[5] << 8));
    nCharCount_E = (int)((DWORD)byte[6] | ((DWORD)byte[7] << 8) | ((DWORD)byte[9] << 16) | ((DWORD)byte[9] << 24));
    m_aEnglish_char.RemoveAll();
    m_aEnglish_char.SetSize(nCharCount_E);
    fFile1.Seek(10, CFile::begin);
    nCountChar = nCharCount_E;
    while (nCountChar > 0)
    {
        fFile1.Read(byte, 10);
		
        English_char1.m_wCharCode = (WORD)byte[0] | ((WORD)byte[1] << 8);
        English_char1.m_wBytes = (WORD)byte[2] | ((WORD)byte[3] << 8);
        English_char1.m_wWidth = (WORD)byte[4] | ((WORD)byte[5] << 8);
        English_char1.m_dwPosition = (DWORD)byte[6] | ((DWORD)byte[7] << 8) | ((DWORD)byte[9] << 16) | ((DWORD)byte[9] << 24);
		
        m_aEnglish_char.SetAt(nCharCount_E - nCountChar, English_char1);
		
        nCountChar--;
    }
    fFile1.Close();

    fFile1.Open((LPCTSTR)gSet.m_strFonHz, CFile::modeRead | CFile::typeBinary);
    fFile1.SeekToBegin();
    fFile1.Read(byte, 8);
    m_nWidth_C = (int)((WORD)byte[0] | ((WORD)byte[1] << 8));
    m_nHeight_C = (int)((WORD)byte[2] | ((WORD)byte[3] << 8));
    nCharCount_C = (int)((DWORD)byte[4] | ((DWORD)byte[5] << 8) | ((DWORD)byte[6] << 16) | ((DWORD)byte[7] << 24));
	
    m_aChinese_char.RemoveAll();
    m_aChinese_char.SetSize(nCharCount_C);
    fFile1.Seek(8, CFile::begin);
    nCountChar = nCharCount_C;
    while (nCountChar > 0)
    {
        fFile1.Read(byte, 8);
		
        Chinese_char1.m_wCharCode = (WORD)byte[0] | ((WORD)byte[1] << 8);
        Chinese_char1.m_wBytes = (WORD)byte[2] | ((WORD)byte[3] << 8);
        Chinese_char1.m_dwPosition = (DWORD)byte[4] | ((DWORD)byte[5] << 8) | ((DWORD)byte[6] << 16) | ((DWORD)byte[7] << 24);
		
        m_aChinese_char.SetAt(nCharCount_C - nCountChar, Chinese_char1);
		
        nCountChar--;
    }
    fFile1.Close();
}

//输入参数:
//  ptPointLU  显示字符串的左上角坐标(即TextOut(...)的x,y值)
//  pbyData 字符的描述数据
//  wBytes 字符的描述数据个数
//  nLeft,nDown 字符的左下角坐标(即以英汉字库的公共单位及坐标水平绘制字符串时,字符的的左下角坐标)
//  nFontScale 字库的长度比例
void CHpglFile::PlotChar(CPoint ptPointLT, 
							  BYTE *pbyData, 
							  WORD wBytes, 
							  int nLeft, 
							  int nDown, 
							  int nFontScale,
							  double dTextAngle)
{
    BYTE byByteX, byByteY;
    CPoint ptCurrentPos, ptCenter, ptPointS, ptPointE;
    CPoint ptPoint1;
    WORD wIndex1;
    WORD wDirection, wLength;
    BOOL bPenUp, bVerTextCommand;

    double dCos, dSin;
    double dLToDScale;
    int nHeight;

    if (wBytes == 0)
        return;

    nHeight = (m_nInternalLeading_E + m_nHeight_E) * m_nHeight_C;
    dLToDScale = (double)nHeight / 	(m_dTextHeight * 400); //实际毫米数

    dCos = cos(dTextAngle / 180.0 * PI); 
    dSin = sin(dTextAngle / 180.0 * PI);

    ptCurrentPos = CPoint(nLeft, nDown);
    ptPoint1 = CPToLP(ptCurrentPos, nHeight, ptPointLT, dSin, dCos, dLToDScale);
    MoveTo(ptPoint1.x, ptPoint1.y);
    bVerTextCommand = false;
    bPenUp = false;
    wIndex1 = 0;
    while (wIndex1 < wBytes)
    {
        switch (pbyData[wIndex1])
        {
	        case 0x0:
            wIndex1++;
            break;

	        case 0x1:
            wIndex1++;
            bPenUp = false;
            break;

	        case 0x2:
            wIndex1++;
            bPenUp = true;
            break;

		    case 0x3:
            wIndex1++;
            wIndex1++;
            bVerTextCommand = false;
            break;

	        case 0x4:
            wIndex1++;
            wIndex1++;
            bVerTextCommand = false;
            break;

	        case 0x5:
            break;

		    case 0x6:
            break;

	        case 0x7:
            break;

		    case 0x8:
            if (bVerTextCommand)
            {
                wIndex1 = wIndex1 + 3;
            }
            else
            {
                wIndex1++;
                ptCurrentPos.x = ptCurrentPos.x + (char)pbyData[wIndex1] * nFontScale;
                wIndex1++;
                ptCurrentPos.y = ptCurrentPos.y + (char)pbyData[wIndex1] * nFontScale;
                ptPoint1 = CPToLP(ptCurrentPos, nHeight, ptPointLT, dSin, dCos, dLToDScale);
                if (bPenUp)
                    MoveTo(ptPoint1.x, ptPoint1.y);
                else
                    LineTo(ptPoint1.x, ptPoint1.y);
                wIndex1++;
            }
            bVerTextCommand = false;
            break;

			case 0x9:
            if (bVerTextCommand)
            {
                wIndex1++;
                byByteX = pbyData[wIndex1];
                wIndex1++;
                byByteY = pbyData[wIndex1];
                while ((byByteX != 0) || (byByteY != 0))
                {
                    wIndex1++;
                    byByteX = pbyData[wIndex1];
                    wIndex1++;
                    byByteY = pbyData[wIndex1];
                }
                wIndex1++;
            }
            else
            {
                wIndex1++;
                byByteX = pbyData[wIndex1];
                wIndex1++;
                byByteY = pbyData[wIndex1];
                while ((byByteX != 0) || (byByteY != 0))
                {
                    ptCurrentPos.x = ptCurrentPos.x + (char)byByteX * nFontScale;
                    ptCurrentPos.y = ptCurrentPos.y + (char)byByteY * nFontScale;
                    ptPoint1 = CPToLP(ptCurrentPos, nHeight, ptPointLT, dSin, dCos, dLToDScale);
                    if (bPenUp)
                        MoveTo(ptPoint1.x, ptPoint1.y);
                    else
                        LineTo(ptPoint1.x, ptPoint1.y);
                    wIndex1++;
                    byByteX = pbyData[wIndex1];
                    wIndex1++;
                    byByteY = pbyData[wIndex1];
                }
                wIndex1++;
            }
            bVerTextCommand = false;
            break;

	        case 0xA:
	        case 0xB:
	        case 0xC:
            break;

	        case 0xD:
            break;

	        case 0xE:
            bVerTextCommand = true;
            wIndex1++;
            break;

	        default:
            if (bVerTextCommand)
            {
                wIndex1++;
            }
            else
            {
                wDirection = (WORD)(pbyData[wIndex1] % 16);
                wLength = (WORD)(pbyData[wIndex1] / 16) * (WORD)nFontScale;
                ptCurrentPos = GetNextCoodinate(wDirection, wLength, ptCurrentPos);

                ptPoint1 = CPToLP(ptCurrentPos, nHeight, ptPointLT, dSin, dCos, dLToDScale);
                if (bPenUp)
                    MoveTo(ptPoint1.x, ptPoint1.y);
                else
                    LineTo(ptPoint1.x, ptPoint1.y);
                wIndex1++;
            }
            bVerTextCommand = false;
            break;
        }
    }
}

CPoint CHpglFile::GetNextCoodinate(WORD wDirection, WORD wLength, CPoint ptPoint)
{
    CPoint ptPoint1;
	
    switch (wDirection)
    {
    case 0:
        ptPoint1.x = ptPoint.x + (int)wLength;
        ptPoint1.y = ptPoint.y;
        break;
    case 1:
        ptPoint1.x = ptPoint.x + (int)wLength;
        ptPoint1.y = ptPoint.y + (int)wLength / 2;
        break;
    case 2:
        ptPoint1.x = ptPoint.x + (int)wLength;
        ptPoint1.y = ptPoint.y + (int)wLength;
        break;
    case 3:
        ptPoint1.x = ptPoint.x + (int)wLength / 2;
        ptPoint1.y = ptPoint.y + (int)wLength;
        break;
    case 4:
        ptPoint1.x = ptPoint.x;
        ptPoint1.y = ptPoint.y + (int)wLength;
        break;
    case 5:
        ptPoint1.x = ptPoint.x - (int)wLength / 2;
        ptPoint1.y = ptPoint.y + (int)wLength;
        break;
    case 6:
        ptPoint1.x = ptPoint.x - (int)wLength;
        ptPoint1.y = ptPoint.y + (int)wLength;
        break;
    case 7:
        ptPoint1.x = ptPoint.x - (int)wLength;
        ptPoint1.y = ptPoint.y + (int)wLength / 2;
        break;
    case 8:
        ptPoint1.x = ptPoint.x - (int)wLength;
        ptPoint1.y = ptPoint.y;
        break;
    case 9:
        ptPoint1.x = ptPoint.x - (int)wLength;
        ptPoint1.y = ptPoint.y - (int)wLength / 2;
        break;
    case 10:
        ptPoint1.x = ptPoint.x - (int)wLength;
        ptPoint1.y = ptPoint.y - (int)wLength;
        break;
    case 11:
        ptPoint1.x = ptPoint.x - (int)wLength / 2;
        ptPoint1.y = ptPoint.y - (int)wLength;
        break;
    case 12:
        ptPoint1.x = ptPoint.x;
        ptPoint1.y = ptPoint.y - (int)wLength;
        break;
    case 13:
        ptPoint1.x = ptPoint.x + (int)wLength / 2;
        ptPoint1.y = ptPoint.y - (int)wLength;
        break;
    case 14:
        ptPoint1.x = ptPoint.x + (int)wLength;
        ptPoint1.y = ptPoint.y - (int)wLength;
        break;
    case 15:
        ptPoint1.x = ptPoint.x + (int)wLength;
        ptPoint1.y = ptPoint.y - (int)wLength / 2;
        break;
    }
	
    return ptPoint1;
}

void CHpglFile::Instruct2PlotCmd(CInstruct *pInstruct)
{
	int nSize;
	int i;
	CString str;
	CPlotCmd *p;

	CPoint dp;
	POSITION pos = NULL;
	double d1;
	double d2;

    if(strcmp(pInstruct->m_cName, "PD") == 0) //落笔
    {
		nSize = pInstruct->m_adPara.GetSize();
		if(nSize >= 2)
		{
			p = new CPlotCmd;
			m_pCmdList_ORG.Add(p);
			p->m_nType = PLOT_CMD_PU;
			p->m_anData.Add(m_dpCurrent.x);
			p->m_anData.Add(m_dpCurrent.y);

			p = new CPlotCmd;
			m_pCmdList_ORG.Add(p);
			p->m_nType = PLOT_CMD_PD;

			for(i = 0; i <nSize; i+=2)
			{
				dp.x = ftoi( pInstruct->m_adPara.GetAt(i) );
				dp.y = ftoi( pInstruct->m_adPara.GetAt(i+1) );
				p->m_anData.Add(dp.x);
				p->m_anData.Add(dp.y);
			}
			m_dpCurrent = dp;
		}
		m_bPenDown = TRUE;
    }
    else if(strcmp(pInstruct->m_cName, "PU") == 0) //提笔
    {
        nSize = pInstruct->m_adPara.GetSize();
        if(nSize >= 2)
        {
            dp.x = ftoi(pInstruct->m_adPara.GetAt(nSize-2) );
            dp.y = ftoi(pInstruct->m_adPara.GetAt(nSize-1) );
            m_dpCurrent = dp;
        }
        m_bPenDown = FALSE;
    }
	else if(strcmp(pInstruct->m_cName, "PA") == 0) //绝对绘图
	{
        nSize = pInstruct->m_adPara.GetSize();
        if(nSize >= 2)
        {
            if(m_bPenDown) 
            {
                p = new CPlotCmd;
				m_pCmdList_ORG.Add(p);
                p->m_nType = PLOT_CMD_PU;
                p->m_anData.Add(m_dpCurrent.x);
                p->m_anData.Add(m_dpCurrent.y);

                p = new CPlotCmd;
				m_pCmdList_ORG.Add(p);
                p->m_nType = PLOT_CMD_PD;

                for(i = 0; i < nSize; i+=2)
                {
                    dp.x = ftoi(pInstruct->m_adPara.GetAt(i) );
                    dp.y = ftoi(pInstruct->m_adPara.GetAt(i+1) );
                    p->m_anData.Add(dp.x);
                    p->m_anData.Add(dp.y);
                }
                m_dpCurrent = dp;
            }
            else
            {
                m_dpCurrent.x = ftoi(pInstruct->m_adPara.GetAt(nSize-2) );
                m_dpCurrent.y = ftoi(pInstruct->m_adPara.GetAt(nSize-1) );
            }
        }
    }
    else if((strcmp(pInstruct->m_cName, "PG") == 0) || (strcmp(pInstruct->m_cName, "FR") == 0)) //推进页面
    {
		p = new CPlotCmd;
		p->m_nType = PLOT_CMD_PG;
		m_pCmdList_ORG.Add(p);
    }
    else if(strcmp(pInstruct->m_cName, "LT") == 0) //线型
    {
        nSize = pInstruct->m_adPara.GetSize();
        p = new CPlotCmd;
		m_pCmdList_ORG.Add(p);
        p->m_nType = PLOT_CMD_LT;

		nSize = pInstruct->m_adPara.GetSize();
		if ( nSize == 0)
		{
			p->m_anData.Add(LT_SOLID);
		}
		else
		{
			switch (ftoi(pInstruct->m_adPara.GetAt(nSize-1)))
			{
				case 0:
				p->m_anData.Add(LT_SOLID);
				break;

				default:
				p->m_anData.Add(LT_DASH);
				break;
			}
		}
	}
    else if(strcmp(pInstruct->m_cName, "SP") == 0) //选笔
    {
        p = new CPlotCmd;
		m_pCmdList_ORG.Add(p);
        p->m_nType = PLOT_CMD_SP;

		nSize = pInstruct->m_adPara.GetSize(); 
        if( nSize == 0)
		{
			p->m_anData.Add(0);
		}
		else
		{
			p->m_anData.Add( ftoi(pInstruct->m_adPara.GetAt(0)) );
	    }
    }
    else if(strcmp(pInstruct->m_cName, "LO") == 0) //字符相对原点位置
    {
        nSize = pInstruct->m_adPara.GetSize();
        if(nSize >= 1)
        {
            int n = ftoi(pInstruct->m_adPara.GetAt(0));
            if(n < 1 || n > 19 || n == 10)
                n = 1;
            m_nTextPosition = n;
        }
        else m_nTextPosition = 1;
    }
    else if(strcmp(pInstruct->m_cName, "SI") == 0) //字符大小
    {
        nSize = pInstruct->m_adPara.GetSize();

        if(nSize >= 2)
        {
            m_dTextHeight = pInstruct->m_adPara.GetAt(1);
            m_dTextWidth = pInstruct->m_adPara.GetAt(0);
        }
        else
        {
            m_dTextHeight = 0.375;
            m_dTextWidth = 0.285;
        }
    }
    else if(strcmp(pInstruct->m_cName, "DI") == 0) //字符方向
    {
        nSize = pInstruct->m_adPara.GetSize();
        if(nSize >= 2)
        {
            d1 = pInstruct->m_adPara.GetAt(0);
            d2 = pInstruct->m_adPara.GetAt(1);

            if((d1 == 0) && (d2 != 0))
                m_dTextAngle = 90;
            else if((d1 != 0) && (d2 == 0))
                m_dTextAngle = 0;
            else if((d1 != 0) && (d2 != 0))
                m_dTextAngle = atan2(d2, d1) * 180.0 /PI;
        }
        else
            m_dTextAngle = 0;
    }
    else if(strcmp(pInstruct->m_cName, "DR") == 0) //字符方向
    {
        nSize = pInstruct->m_adPara.GetSize();
        if((nSize == 0) || ((nSize == 2) && ((pInstruct->m_adPara.GetAt(0) == 0) || (pInstruct->m_adPara.GetAt(1) == 0))))
        {
            if(nSize == 0)
                m_dTextAngle = 0;
            else if(nSize == 2)
            {
                d1 = pInstruct->m_adPara.GetAt(0);
                d2 = pInstruct->m_adPara.GetAt(1);
                if((d1 == 0) && (d2 != 0))
                    m_dTextAngle = 90;
                else if((d1 != 0) && (d2 == 0))
                    m_dTextAngle = 0;
            }
        }
    }
    else if(strcmp(pInstruct->m_cName, "LB") == 0) //字符
    {
        nSize = m_strText.GetLength();
        int j = 0;
		char str1[513];
        CPoint dp0;
		CDPoint dp1;
        dp = m_dpCurrent;

        while(nSize > 0)
        {
            if(nSize > 512)
                nSize = 512;
            for(i = 0; i < nSize; i++)
            {
                if(m_strText[j] == 0x0d) //回车
                {
                    str1[i] = '\0';
                    if(i != 0)
                    {
						dp1.x = dp.x;
						dp1.y = dp.y;
						CalcTextStartPoint(&dp1,strlen(&str1[0]));
						dp0.x = ftoi(dp1.x);
						dp0.y = ftoi(dp1.y);
                        Text2Line(dp0, (LPCTSTR)str1, i);
                    }
                    j++;
                    dp.x = m_dpCurrent.x;
                    break;
				}
                else if(m_strText[j] == 0x0a) //换行
                {
                    str1[i] = '\0';
                    if(i != 0)
                    {
						dp1.x = dp.x;
						dp1.y = dp.y;
						CalcTextStartPoint(&dp1,strlen(&str1[0]));
						dp0.x = ftoi(dp1.x);
						dp0.y = ftoi(dp1.y);
						Text2Line(dp0, (LPCTSTR)str1, i);
                    }
                    j++;
                    dp.y += ftoi(m_dTextHeight * 400);
                    break;
                }
                else
				{
                    str1[i] = m_strText[j];
				}
                j++;
            }
            if((i == nSize) && (i != 0))
            {
				str1[i] = '\0';
				dp1.x = dp.x;
				dp1.y = dp.y;
				CalcTextStartPoint(&dp1,strlen(&str1[0]));
				dp0.x = ftoi(dp1.x);
				dp0.y = ftoi(dp1.y);                
				Text2Line(dp0, (LPCTSTR)str1, i);
            }
            else
                i++;
            nSize -= i;
        }
    }
    else if(strcmp(pInstruct->m_cName, "DF") == 0) //缺省
    {
        m_nLineType = LT_SOLID;

        m_cTerminator = 3; //标志终结符
        m_cTerminatorMode = 1;

        m_dTextHeight = 0.375; //cm
        m_dTextWidth = 0.285; //cm
        m_dTextAngle = 0;
        m_nTextPosition = 1;

		if (m_pCmdList_ORG.GetSize() != 0)
		{
			p=new CPlotCmd;
			m_pCmdList_ORG.Add(p);
			p->m_nType = PLOT_CMD_LT;
		}
    }
    else if(strcmp(pInstruct->m_cName, "IN") == 0) //初始化
    {
        m_dpCurrent = CPoint(0, 0);
        m_bPenDown = FALSE;

        m_nLineType = LT_SOLID;

        m_cTerminator = 3; //标志终结符
        m_cTerminatorMode = 1;
        m_dTextHeight = 0.375; //cm
        m_dTextWidth = 0.285; //cm
        m_dTextAngle = 0;
        m_nTextPosition = 1;

		if (m_pCmdList_ORG.GetSize() != 0)
		{
			p=new CPlotCmd;
			m_pCmdList_ORG.Add(p);
			p->m_nType = PLOT_CMD_LT;
		}
    }
}


int CHpglFile::GetParameterType(char cName[])
{
	int nType = -1;
	if(strcmp(cName, "DF") == 0) nType = NONE_TYPE; //缺省
	else if(strcmp(cName, "IN") == 0) nType = DIGIT_TYPE; //初始化
	else if(strcmp(cName, "PG") == 0) nType = DIGIT_TYPE; //推进页面
	else if(strcmp(cName, "FR") == 0) nType = DIGIT_TYPE; //推进页面
	else if(strcmp(cName, "PA") == 0) nType = DIGIT_TYPE; //绝对绘图
	else if(strcmp(cName, "PD") == 0) nType = DIGIT_TYPE; //落笔
	else if(strcmp(cName, "PU") == 0) nType = DIGIT_TYPE; //提笔
	else if(strcmp(cName, "SP") == 0) nType = DIGIT_TYPE; //选笔
	else if(strcmp(cName, "DI") == 0) nType = DIGIT_TYPE; //绝对方向------------
	else if(strcmp(cName, "DR") == 0) nType = DIGIT_TYPE; //相对方向
	else if(strcmp(cName, "SI") == 0) nType = DIGIT_TYPE; //绝对字符尺寸
	else if(strcmp(cName, "LO") == 0) nType = DIGIT_TYPE; //标志原点172
	else if(strcmp(cName, "DT") == 0) nType = DT_TYPE; //定义标志终结符
    else if(strcmp(cName, "LB") == 0)
    {
        nType = STRING_TYPE; //标志171
        m_strText.Empty();
    }
    else if(strcmp(cName, "LT") == 0) nType = DIGIT_TYPE; //线型119
    if(nType == -1)
    {
        nType = DIGIT_TYPE;
    }
    return nType;
}

void CHpglFile::InitHPGL()
{
    m_dpCurrent = CPoint(0, 0);
    m_bPenDown = FALSE;
    m_nLineType = LT_SOLID;

    m_cTerminator = 3; //标志终结符
    m_cTerminatorMode = 1;
    m_dTextHeight = 0.375; //cm
    m_dTextWidth = 0.285; //cm
    m_dTextAngle = 0;
    m_nTextPosition = 1;

    RemoveBuffer();
}

void CHpglFile::Text2Line(CPoint dp, LPCTSTR lpszString, int nCount)
{
    CFile fileEnglish, fileChinese;
    BYTE *pbyData;
    int nLeft;
    BYTE byByte1, byByte2;
    WORD wCharCode;
    int nIndex1, nIndex2, nIndex3, nIndex4;
    BOOL bEnglishChar;

    fileEnglish.Open((LPCTSTR)gSet.m_strFonEn, CFile::modeRead | CFile::typeBinary);
    fileChinese.Open((LPCTSTR)gSet.m_strFonHz, CFile::modeRead | CFile::typeBinary);

    nLeft = 0;
    nIndex1 = 0;
    while (nIndex1 < nCount) //nIndex1 ,nCount是字符串中字符的个数
    {
        pbyData = NULL;
        byByte1 = (BYTE)lpszString[nIndex1];
        if ((byByte1 >= 128) && ((nIndex1 + 1) < nCount)) //是汉字且字符串未完
        {
            bEnglishChar = false;
            nIndex1++;
            byByte2 = (BYTE)lpszString[nIndex1];
            wCharCode = (((WORD)byByte1) << 8) | (WORD)byByte2;
            nIndex2 = 0;
            nIndex3 = m_aChinese_char.GetSize() - 1;
            if (m_aChinese_char[nIndex2].m_wCharCode == wCharCode)
                nIndex4 = nIndex2;
            else if (m_aChinese_char[nIndex3].m_wCharCode == wCharCode)
                nIndex4 = nIndex3;
            else
                nIndex4 = (nIndex2 + nIndex3) / 2;
            while (((nIndex3 - nIndex2) > 1) && (m_aChinese_char[nIndex4].m_wCharCode != wCharCode))
            {
                if (m_aChinese_char[nIndex4].m_wCharCode > wCharCode)
                {
                    nIndex3 = nIndex4;
                    if (nIndex2 > nIndex3)
                        break;
                    nIndex4 = (nIndex2 + nIndex3) / 2;
                }
                else
                {
                    nIndex2 = nIndex4;
                    if (nIndex2 > nIndex3)
                        break;
                    nIndex4 = (nIndex2 + nIndex3) / 2;
                }
            }
            if ((m_aChinese_char[nIndex4].m_wCharCode == wCharCode) && (m_aChinese_char[nIndex4].m_wBytes > 0))
            {
                pbyData = new BYTE[m_aChinese_char[nIndex4].m_wBytes + 1];
                fileChinese.Seek(m_aChinese_char[nIndex4].m_dwPosition, CFile::begin);
                fileChinese.Read(pbyData, m_aChinese_char[nIndex4].m_wBytes);
            }
        }
        else
        {
            bEnglishChar = true;
            wCharCode = (WORD)byByte1;
            nIndex2 = 0;
            nIndex3 = m_aEnglish_char.GetSize() - 1;
            if (m_aEnglish_char[nIndex2].m_wCharCode == wCharCode)
                nIndex4 = nIndex2;
            else if (m_aEnglish_char[nIndex3].m_wCharCode == wCharCode)
                nIndex4 = nIndex3;
            else
                nIndex4 = (nIndex2 + nIndex3) / 2;
            while (((nIndex3 - nIndex2) > 1) && (m_aEnglish_char[nIndex4].m_wCharCode != wCharCode))
            {
                if (m_aEnglish_char[nIndex4].m_wCharCode > wCharCode)
                {
                    nIndex3 = nIndex4;
                    if (nIndex2 > nIndex3)
                        break;
                    nIndex4 = (nIndex2 + nIndex3) / 2;
                }
                else
                {
                    nIndex2 = nIndex4;
                    if (nIndex2 > nIndex3)
                        break;
                    nIndex4 = (nIndex2 + nIndex3) / 2;
                }
            }
            if ((m_aEnglish_char[nIndex4].m_wCharCode == wCharCode) && (m_aEnglish_char[nIndex4].m_wBytes > 0))
            {
                pbyData = new BYTE[m_aEnglish_char[nIndex4].m_wBytes + 1];
                fileEnglish.Seek(m_aEnglish_char[nIndex4].m_dwPosition, CFile::begin);
                fileEnglish.Read(pbyData, m_aEnglish_char[nIndex4].m_wBytes);
            }
        }
        if (pbyData != NULL)
        {
            if (bEnglishChar)
                PlotChar(dp, pbyData, m_aEnglish_char[nIndex4].m_wBytes, nLeft, 0, m_nHeight_C,m_dTextAngle);
            else
                PlotChar(dp, pbyData, m_aChinese_char[nIndex4].m_wBytes, nLeft, 0, m_nInternalLeading_E + m_nHeight_E,m_dTextAngle);
            delete pbyData;
            if (bEnglishChar)
                nLeft = nLeft + m_aEnglish_char[nIndex4].m_wWidth * m_nHeight_C;
            else
                nLeft = nLeft + m_nWidth_C * (m_nInternalLeading_E + m_nHeight_E);
        }
        else
        {
            if (bEnglishChar)
                nLeft = nLeft + m_aEnglish_char[0].m_wWidth * m_nHeight_C;
            else
                nLeft = nLeft + m_nWidth_C * (m_nInternalLeading_E + m_nHeight_E);
        }
        nIndex1++;
    }
    fileEnglish.Close();
    fileChinese.Close();
}


void CHpglFile::RemoveBuffer()
{
	int nsize = m_pCmdList_ORG.GetSize();
	int i;
	for (i=0; i<nsize; i++)
	{
		CPlotCmd *p = (CPlotCmd *)m_pCmdList_ORG.GetAt(i);
		p->m_anData.RemoveAll();
		delete p;
	}
	m_pCmdList_ORG.RemoveAll();
}

void CHpglFile::CalcTextStartPoint(CDPoint* dp, int nLen)
{

	double dTextAngle = m_dTextAngle / 180.0 * PI;
	double dLength;

	if(nLen == 0)
		return;

	switch(m_nTextPosition)
	{
		case 1:
        dp->x = dp->x - (m_dTextHeight * 400) * sin(dTextAngle);
        dp->y = dp->y + (m_dTextHeight * 400) * cos(dTextAngle);
        break;

		case 2:
        dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
        dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
        break;

		case 4:
		dp->x = dp->x - (m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;

		case 5:
		dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 6:
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 7:
		dp->x = dp->x - (m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 8:
		dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;

		case 9:
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 11:
		dTextAngle = atan2(0.25 * m_dTextHeight * 400, 0.25 * m_dTextWidth * 400) + m_dTextAngle;
		dTextAngle = dTextAngle / 180.0 * PI;
		dLength = sqrt((0.25 * m_dTextHeight * 400) * (0.25 * m_dTextHeight * 400) + (0.25 * m_dTextWidth * 400) * (0.25 * m_dTextWidth * 400));
		dp->x = dp->x + dLength * cos(dTextAngle);
		dp->y = dp->y + dLength * sin(dTextAngle);
		dTextAngle = m_dTextAngle / 180.0 * PI;
		dp->x = dp->x - (m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (m_dTextHeight * 400) * cos(dTextAngle);
		break;
		
		case 12:
		dp->x = dp->x + (0.25 * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y + (0.25 * m_dTextWidth * 400) * sin(dTextAngle);
		dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
		break;

		case 13:
		dTextAngle = atan2(0.25 * m_dTextHeight * 400, 0.25 * m_dTextWidth * 400) - m_dTextAngle;
		dTextAngle = dTextAngle / 180.0 * PI;
		dLength = sqrt((0.25 * m_dTextHeight * 400) * (0.25 * m_dTextHeight * 400) + (0.25 * m_dTextWidth * 400) * (0.25 * m_dTextWidth * 400));
		dp->x = dp->x + dLength * cos(dTextAngle);
		dp->y = dp->y - dLength * sin(dTextAngle);
		dTextAngle = m_dTextAngle / 180.0 * PI;
		break;
		
		case 14:
		dp->x = dp->x - (1.25 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (1.25 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 15:
		dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 16:
		dp->x = dp->x + (0.25 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y - (0.25 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (0.5 * nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.5 * nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;

		case 17:
		dTextAngle = atan2(0.25 * m_dTextHeight * 400, 0.25 * m_dTextWidth * 400) - m_dTextAngle;
		dTextAngle = dTextAngle / 180.0 * PI;
		dLength = sqrt((0.25 * m_dTextHeight * 400) * (0.25 * m_dTextHeight * 400) + (0.25 * m_dTextWidth * 400) * (0.25 * m_dTextWidth * 400));
		dp->x = dp->x - dLength * cos(dTextAngle);
		dp->y = dp->y + dLength * sin(dTextAngle);
		dTextAngle = m_dTextAngle / 180.0 * PI;
		dp->x = dp->x - (m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 18:
		dp->x = dp->x - (0.25 * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (0.25 * m_dTextWidth * 400) * sin(dTextAngle);
		dp->x = dp->x - (0.5 * m_dTextHeight * 400) * sin(dTextAngle);
		dp->y = dp->y + (0.5 * m_dTextHeight * 400) * cos(dTextAngle);
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
		
		case 19:
		dTextAngle = atan2(0.25 * m_dTextHeight * 400, 0.25 * m_dTextWidth * 400) + m_dTextAngle;
		dTextAngle = dTextAngle / 180.0 * PI;
		dLength = sqrt((0.25 * m_dTextHeight * 400) * (0.25 * m_dTextHeight * 400) + (0.25 * m_dTextWidth * 400) * (0.25 * m_dTextWidth * 400));
		dp->x = dp->x - dLength * cos(dTextAngle);
		dp->y = dp->y - dLength * sin(dTextAngle);
		dTextAngle = m_dTextAngle / 180.0 * PI;
		dp->x = dp->x - (nLen * m_dTextWidth * 400) * cos(dTextAngle);
		dp->y = dp->y - (nLen * m_dTextWidth * 400) * sin(dTextAngle);
		break;
	}
}

int CHpglFile::OpenPltFile()
{
	/*
	返回值	1：文件打开失败
			2：文件大小太小或者太大
			3：
	*/
	CFile file;
	if(!file.Open(m_strPathAndName, CFile::modeRead | CFile::shareExclusive))
		return 1;

	m_strName = file.GetFileName(); //用于显示

	int nLength = (int)(file.GetLength());
	if ((nLength<10) || (nLength>8000000))
	{
		return -2;
	}

	BOOL bError = FALSE;
	CInstruct Instruct;

	int nProcess;
	int nMnemonicCount;

	int nParameterType = NONE_TYPE;
	int nEscCount=0;
	char cFileBuf[1001];
	int i, j, k, m;
	unsigned char ch;

	InitHPGL();
	Text2LineInit();
	Instruct.m_adPara.RemoveAll();
	Instruct.m_cName[2] = 0;
	nProcess = PROCESS_MNEMONIC;
	nMnemonicCount = 0;
	m = j = 0;

    for(i = 0; i < nLength;)
    {
        if (m < j)
        {
            k = 0;
            while(k < (j - m))
            {
                cFileBuf[k] = cFileBuf[m+k];
                k++;
            }
            if ((i + 1000 - k) < nLength)
                j = 1000 - k;
            else
                j = nLength - i;
        }
        else
        {
            if ((i + 1000) < nLength)
                j = 1000;
            else
                j = nLength - i;
            k = 0;
        }
        file.Read(&cFileBuf[k], j);
        i += j;
        j += k;
        k = 0;
        m = j;
        while (k < j)
        {
            if (nProcess == PROCESS_FIND)
            {
                //寻找下一个';'
                while (k < j)
                {
                    ch = cFileBuf[k++];
                    if(ch == ';')
                    {
                        nProcess = PROCESS_MNEMONIC;
                        break;
                    }
                }
            }
            else if (nProcess == PROCESS_MNEMONIC)//助记符
            {
                ch = cFileBuf[k++];
                if (nMnemonicCount == 0)
                {
                    if(IsAlpha(ch))
                        Instruct.m_cName[nMnemonicCount++] = toupper(ch);
                    else if(ch == 27)
                    {
                        nProcess = PROCESS_PARAMETER;
                        nParameterType = ESC_TYPE;
                        nEscCount = 4;
                    }
                }
                else
                {
                    if(IsAlpha(ch))
                    {
                        Instruct.m_cName[nMnemonicCount++] = toupper(ch);
                        nProcess = PROCESS_PARAMETER;
                        nParameterType = GetParameterType(Instruct.m_cName);
                    }
                    else
                        nMnemonicCount = 0;
                }
            }
            else
            {
                switch(nParameterType)//参数
                {
                case ESC_TYPE:
                    if ((k + nEscCount) <= j)
                    {
                        k += nEscCount;
                        nProcess = PROCESS_MNEMONIC;
                    }
                    else
                    {
                        nEscCount -= (j - k);
                        k = j;
                    }
                    break;

                case NONE_TYPE:
                    ch = cFileBuf[k++];
                    if(ch == ';')
                    {
                        Instruct2PlotCmd(&Instruct);
                        nProcess = PROCESS_MNEMONIC;
                        nMnemonicCount = 0;
                    }
                    else if(IsAlpha(ch))
                    {
                        Instruct2PlotCmd(&Instruct);
                        nProcess = PROCESS_MNEMONIC;
                        nMnemonicCount = 0;
                        Instruct.m_cName[nMnemonicCount++] = toupper(ch);
                    }
                    else if(IsDigit(ch))
                        bError = TRUE;
                    break;

                case DIGIT_TYPE:
                    m = k;
                    while (k < j)
                    {
                        ch = cFileBuf[k++];
                        if (!IsDigit(ch))
                        {
                            if (ch == '.')
                                continue;
                            if (m != (k - 1))
                                Instruct.m_adPara.Add(atof(&cFileBuf[m]));
                            if (ch == ';')
                            {
                                Instruct2PlotCmd(&Instruct);
                                Instruct.m_adPara.RemoveAll();
                                nProcess = PROCESS_MNEMONIC;
                                nMnemonicCount = 0;
                                m = j;
                                break;
                            }
                            else if (IsAlpha(ch))
                            {
                                Instruct2PlotCmd(&Instruct);
                                Instruct.m_adPara.RemoveAll();
                                nMnemonicCount = 0;
                                nProcess = PROCESS_MNEMONIC;
                                Instruct.m_cName[nMnemonicCount++] = toupper(ch);
                                m = j;
                                break;
                            }
                            else if(ch == 27)
                            {
                                Instruct2PlotCmd(&Instruct);
                                Instruct.m_adPara.RemoveAll();
                                nMnemonicCount = 0;
                                nProcess = PROCESS_PARAMETER;
                                nParameterType = ESC_TYPE;
                                nEscCount = 4;
                                m = j;
                                break;
                            }
                            else
                                m = k;
                        }
                    }
                    break;

                case STRING_TYPE:
                    ch = cFileBuf[k++];
                    if(ch == m_cTerminator)
                    {
                        //add by yhz,如果是连续的LB指令，则将其合并
                        if (('L' == cFileBuf[k]) && ('B' == cFileBuf[k+1]))
                        {
                            ch = cFileBuf[k+2];
                            k += 2;
                        }
                        //add by yhz,如果是连续的LB指令，则将其合并
                        else
                        {
                            if(m_cTerminatorMode == 0)
                            {
                                CString str = "1";
                                str.SetAt(0, ch);
                                m_strText += str;
                            }
                            Instruct2PlotCmd(&Instruct);
                            Instruct.m_adPara.RemoveAll();
                            nProcess = PROCESS_MNEMONIC;
                            nMnemonicCount = 0;
                        }
                    }
                    else
                    {
                        CString str = "1";
                        str.SetAt(0, ch);
                        m_strText += str;
                    }
                    break;

                case DT_TYPE:
                    ch = cFileBuf[k++];
                    if(m_cTerminatorMode == 0x81)
                    {
                        if(ch == ',')
                            m_cTerminatorMode = 0x82;
                        else
                        {
                            m_cTerminatorMode = 1;
                            nProcess = PROCESS_MNEMONIC;
                            nMnemonicCount = 0;
                            k--;
                        }
                    }
                    else if(m_cTerminatorMode == 0x82)
                    {
                        if(ch == 0 || ch == 1)
                            m_cTerminatorMode = ch;
                        else
                        {
                            m_cTerminatorMode = 1;
                            nProcess = PROCESS_MNEMONIC;
                            nMnemonicCount = 0;
                            k--;
                        }
                    }
                    else
                    {
                        if(ch == ';')
                        {
                            m_cTerminator = 3;
                            m_cTerminatorMode = 1;
                            nProcess = PROCESS_MNEMONIC;
                            nMnemonicCount = 0;
                        }
                        else if(ch != 0 && ch != 5 && ch != 27)
                        {
                            m_cTerminator = ch;
                            m_cTerminatorMode = 0x81;
                        }
                        else
                        {
                            nProcess = PROCESS_MNEMONIC;
                            nMnemonicCount = 0;
                        }
                    }
                    break;

                default:
                    break;
                }
            }
            if(bError)
                break;
        }
        if(bError)
            break;
    }
    if(!bError)
        Instruct2PlotCmd(&Instruct);
    Instruct.m_adPara.RemoveAll();
    Instruct.m_cName[2] = 0;
    file.Close();
    if(bError)
        return -3; //文件格式错误
    return 0;
}
