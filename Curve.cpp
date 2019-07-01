// Curve.cpp: implementation of the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Curve.h"
#include "Global.h"

#define PT_OFFSET 4

BOOL gbNoMeaning;
CString strZHKJ="ZHKJ";

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_SERIAL(CCurve,CObject,1)
CCurve::CCurve()
{
	m_bIsOutline = FALSE;
	m_bIsPolygon = FALSE;
    m_bIsSelected = FALSE;
	m_nCurveType = CURVE_TYPE_PEN;
	m_dxxx = 0.0;
	m_pointList = new CArray<CPoint, CPoint>;
}

CCurve::~CCurve()
{
	m_pointList->RemoveAll();
	delete m_pointList;
}

void CCurve::operator=(CCurve &a)
{
	this->m_bIsOutline = a.m_bIsOutline;
	this->m_bIsPolygon = a.m_bIsPolygon;
	this->m_bIsSelected = a.m_bIsSelected;
	this->m_nCurveType = a.m_nCurveType;
///	this->m_dSlotWidth = a.m_dSlotWidth;
	this->m_nXmax = a.m_nXmax;
	this->m_nXmin = a.m_nXmin;
	this->m_nYmax = a.m_nYmax;
	this->m_nYmin = a.m_nYmin;
	this->m_pointList->RemoveAll();
	for (int i=0; i<a.m_pointList->GetSize(); i++)
	{
		CPoint pt = a.m_pointList->GetAt(i);
		pt.x = pt.x;
		pt.y = pt.y;
		this->m_pointList->Add(pt);
	}
}

void CCurve::CheckIsPieceOutLine()
{
	m_bIsOutline = FALSE;
	if (FALSE == m_bIsPolygon) //不是封闭曲线
	{
		return;
	}
//----------------------------滤除小片--------------------------------
	if( ( (m_nXmax-m_nXmin) < 120 ) || ( (m_nYmax-m_nYmin)<120) )
	{
		return;
	}

	if( ( (m_nXmax-m_nXmin) < 1200 ) && ( (m_nYmax-m_nYmin)<1200) )
	{
		return;
	}
	if (m_nCurveType <= 1)
	{
		return;
	}
	m_bIsOutline = TRUE;
}

void CCurve::GetCurveMaxMin()
{
	int i;
	CPoint pt;

	pt = m_pointList->GetAt(0);
	m_nXmin = m_nXmax = pt.x;
	m_nYmin = m_nYmax = pt.y;

	if (m_pointList->GetSize() > 1)
	{
		for (i=1; i<m_pointList->GetSize(); i++)
		{
			pt = m_pointList->GetAt(i);
			m_nXmin = (m_nXmin < pt.x) ? m_nXmin:pt.x;
			m_nXmax = (m_nXmax > pt.x) ? m_nXmax:pt.x;
			m_nYmin = (m_nYmin < pt.y) ? m_nYmin:pt.y;
			m_nYmax = (m_nYmax > pt.y) ? m_nYmax:pt.y;
		}
	}
	m_nXSize = m_nXmax - m_nXmin;
	m_nYSize = m_nYmax - m_nYmin;
}

void CCurve::OnRemoveDupPoints()
{
	int i,nSize;
	CPoint pt[3];
	double deltaX1,deltaY1,deltaX2,deltaY2;
	double dA,dB;

	nSize = m_pointList->GetSize();
	if (nSize <= 1)
	{
		return;
	}
	if (nSize == 2)
	{
		if (m_pointList->GetAt(0) == m_pointList->GetAt(1))
		{
			m_pointList->RemoveAt(1);
		}
		return;
	}

	for (i=1;i<nSize-1;i++)
	{
		pt[0]=m_pointList->GetAt(i-1);
		pt[1]=m_pointList->GetAt(i);
		pt[2]=m_pointList->GetAt(i+1);
			
		deltaY1 = pt[2].y - pt[1].y;
		deltaX1 = pt[2].x - pt[1].x;
			
		deltaY2 = pt[1].y - pt[0].y;
		deltaX2 = pt[1].x - pt[0].x;
		
		dA = atan2(deltaY1 ,deltaX1);
		dB = atan2(deltaY2 ,deltaX2);
		if( dA == dB )
		{
			m_pointList->RemoveAt(i);

			i--;
			nSize--;
		}
	}

	nSize = m_pointList->GetSize();
	if (nSize == 2)
	{
		if (m_pointList->GetAt(0) == m_pointList->GetAt(1))
		{
			m_pointList->RemoveAt(1);
		}
		return;
	}
	return;
}

void CCurve::OnAngleAdjust(double dAngle,int iMaxX)
{
	int i;
	CPoint pt;
	for (i=0; i<m_pointList->GetSize(); i++)
	{
		pt = m_pointList->GetAt(i);
		if (dAngle >=0 )
		{
			pt.y = pt.y + ftoi(pt.x * tan(dAngle));
		}
		else
		{
			pt.y = pt.y + ftoi( (iMaxX-pt.x) * fabs(tan(dAngle)) );
		}
		m_pointList->RemoveAt(i);
		m_pointList->InsertAt(i,pt);
	}
}

void CCurve::OnCheckCurveIsPolygon() 
{
	m_bIsPolygon = FALSE;

	int nSize = m_pointList->GetSize();
	if (nSize>2)
	{
		CPoint ptS,ptE;
		ptS = m_pointList->GetAt(0);
		ptE = m_pointList->GetAt(nSize-1);
		if ( ptS == ptE )
		{
			m_bIsPolygon = TRUE;
			return;
		}
		int absdx;
		int absdy;
		absdx = abs(ptE.x-ptS.x);
		absdy = abs(ptE.y-ptS.y);
		if( ( (absdx < 100) && (absdy < 100) )&&(m_nCurveType == CURVE_TYPE_MM) ) //人为增加一个点强制闭合
		{
			m_pointList->Add(ptS);
			m_bIsPolygon = TRUE;
			return;
		}
	}
}	

//获取曲线的类型，（封闭，非封闭）
void CCurve::GetCurveStyle() 
{
	m_bIsPolygon = FALSE;
	int nSize = m_pointList->GetSize();
	if (nSize>2)
	{
		CPoint ptS,ptE;
		ptS = m_pointList->GetAt(0);
		ptE = m_pointList->GetAt(nSize-1);
		if ( ptS == ptE )
		{
			m_bIsPolygon = TRUE;
		}
	}
}

void CCurve::OnReverse() //曲线反向
{
    CArray <CPoint, CPoint> m_PLHpglTemp;
	int nSize = m_pointList->GetSize();
	int i;
	for (i=nSize-1;i>=0;i--)
	{
		m_PLHpglTemp.Add(m_pointList->GetAt(i));
	}
	m_pointList->RemoveAll();
	
	for (i=0;i<nSize;i++)
	{
		m_pointList->Add(m_PLHpglTemp.GetAt(i));
	}
	m_PLHpglTemp.RemoveAll();
}

void CCurve::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_bIsOutline<<strZHKJ<<gbNoMeaning<<m_bIsPolygon<<m_nCurveType<<m_nXmax<<m_nYmax<<m_nXmin<<m_nYmin<<m_dxxx;
		m_pointList->Serialize(ar);
	}
	else
	{
		ar>>m_bIsOutline>>strZHKJ>>gbNoMeaning>>m_bIsPolygon>>m_nCurveType>>m_nXmax>>m_nYmax>>m_nXmin>>m_nYmin>>m_dxxx;
		m_pointList->Serialize(ar);
	}
}

void CCurve::OnMoveCurve(int deltaX, int deltaY)
{
	int i;
	CPoint pt;
	
	for (i=0; i<m_pointList->GetSize(); i++)
	{
		pt = m_pointList->GetAt(i);
		pt.x = pt.x + deltaX;
		pt.y = pt.y + deltaY;
		m_pointList->RemoveAt(i);
		m_pointList->InsertAt(i,pt);
	}
	m_nXmin += deltaX; 
	m_nXmax += deltaX; 
	m_nYmin += deltaY; 
	m_nYmax += deltaY; 
}

void CCurve::RotateCurve(int CenterX, int CenterY, double dAngle)
{
	int i;
	CPoint pt;
	double r;
	double dAngle0,dAngle1;
	for (i=0; i<m_pointList->GetSize(); i++)
	{
		pt = m_pointList->GetAt(i);
		dAngle0 = atan2((double)(pt.y-CenterY),(double)(pt.x-CenterX));
		dAngle1 = dAngle0-dAngle;
		r = sqrt( (double)( (CenterX-pt.x)*(CenterX-pt.x)+(CenterY-pt.y)*(CenterY-pt.y)) );
		pt.x = (int)(CenterX+r*cos(dAngle1));
		pt.y = (int)(CenterY+r*sin(dAngle1));
		m_pointList->RemoveAt(i);
		m_pointList->InsertAt(i,pt);
	}
	
}

void CCurve::CurveMirror(int MaxX)
{
	int i;
	CPoint pt;
	for (i=0; i<m_pointList->GetSize(); i++)
	{
		pt = m_pointList->GetAt(i);
		pt.x = MaxX*2-pt.x;
		m_pointList->RemoveAt(i);
		m_pointList->InsertAt(i,pt);
	}
}

#define TRI_H 15 //三角形高
#define TRI_HALF_D 6  //三角形底边长的一半
void CCurve::OnDrawCurve(CDC *pDrawDC,int nDrawType)
{
	CPoint ptScr;
	int i;
	int nPointSum = m_pointList->GetSize(); 
	CRect rect;
	CPen penNew,*pPenOld;
	CBrush brushNew,*pBrushOld;
	double dScale = gSet.getDispScale();
	if (nDrawType != 0) //裁片处于选中状态 
	{
		penNew.CreatePen(LT_SOLID,0,gColorSet.getClrPieceSelected());
		pPenOld = pDrawDC->SelectObject(&penNew);

		brushNew.CreateSolidBrush(RGB(255,255,255));
		pBrushOld = pDrawDC->SelectObject(&brushNew);
		if (m_nCurveType != CURVE_TYPE_DRILL)
		{
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
			pDrawDC->MoveTo(ptScr);
			for (i=1;i<nPointSum;i++)
			{
				ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(i).x,m_pointList->GetAt(i).y));
				pDrawDC->LineTo(ptScr);
			}
		}
		else
		{
			CRect rect;
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
			
			rect.left = ptScr.x-4;
			rect.right = ptScr.x+4;
			rect.top = ptScr.y+4;
			rect.bottom = ptScr.y-4;
			pBrushOld = pDrawDC->SelectObject(&brushNew);
			pDrawDC->Ellipse(rect);
		}
		pDrawDC->SelectObject(pPenOld);
		pDrawDC->SelectObject(pBrushOld);
		brushNew.DeleteObject();
		penNew.DeleteObject();
		return;
	}

	if (FALSE == m_bIsSelected)
	{
		switch(m_nCurveType)
		{
			case CURVE_TYPE_PEN:
			penNew.CreatePen(LT_SOLID,0,gColorSet.getClrPen());
			pPenOld = pDrawDC->SelectObject(&penNew);
			break;

			case CURVE_TYPE_MM:
			penNew.CreatePen(LT_SOLID,0,gColorSet.getClrMM());
			pPenOld = pDrawDC->SelectObject(&penNew);
			break;

			case CURVE_TYPE_DRILL:
			penNew.CreatePen(LT_SOLID,0,gColorSet.getClrMDrill());
			pPenOld = pDrawDC->SelectObject(&penNew);
			break;
		}
		brushNew.CreateSolidBrush(gColorSet.getClrMDrill());
		pBrushOld = pDrawDC->SelectObject(&brushNew);

		if (m_nCurveType != CURVE_TYPE_DRILL)
		{
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
			pDrawDC->MoveTo(ptScr);
			for (i=1;i<nPointSum;i++)
			{
				ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(i).x,m_pointList->GetAt(i).y));
				pDrawDC->LineTo(ptScr);
			}
		}
		else
		{
			CRect rect;
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
			
			rect.left = ptScr.x-4;
			rect.right = ptScr.x+4;
			rect.top = ptScr.y+4;
			rect.bottom = ptScr.y-4;
			pBrushOld = pDrawDC->SelectObject(&brushNew);
			pDrawDC->Ellipse(rect);
		}

		pDrawDC->SelectObject(pPenOld);
		pDrawDC->SelectObject(pBrushOld);
		brushNew.DeleteObject();
		penNew.DeleteObject();
		return;
	}

	
	if (m_nCurveType != CURVE_TYPE_DRILL)
	{
		penNew.CreatePen(LT_SOLID,0,gColorSet.getClrPieceSelected());
		pPenOld = pDrawDC->SelectObject(&penNew);
		brushNew.CreateSolidBrush(RGB(0,0,0));
		pBrushOld = pDrawDC->SelectObject(&brushNew);

		ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
		pDrawDC->MoveTo(ptScr);
		for (i=1;i<nPointSum;i++)
		{
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(i).x,m_pointList->GetAt(i).y));
			pDrawDC->LineTo(ptScr);
		}
		for (i=1;i<nPointSum;i++) //画小方框
		{
			ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(i).x,m_pointList->GetAt(i).y));
			rect = CRect(ptScr, ptScr );
			rect.InflateRect(2, 2);
			pDrawDC->Rectangle(rect);
		}

		pDrawDC->SelectObject(pBrushOld);
		brushNew.DeleteObject();
		brushNew.CreateSolidBrush(RGB(255,255,255));
		pBrushOld = pDrawDC->SelectObject(&brushNew);

		double thta1;
		CPoint pt0,pt1;
		pt0=m_pointList->GetAt(0);
		pt1=m_pointList->GetAt(1);
		thta1 = atan2( (double)(pt0.y - pt1.y),	(double)(pt1.x - pt0.x) );
		CPoint ptDraw;
		ptDraw = m_pointList->GetAt(0);
		ptDraw = ptHpglToScr(ptDraw);
		CPoint p[3] = 
		{
			CPoint((int)(ptDraw.x+TRI_H*cos(thta1)),(int)(ptDraw.y+TRI_H*sin(thta1))),
				CPoint((int)(ptDraw.x+TRI_HALF_D*cos(PI/2+thta1)),(int)(ptDraw.y+TRI_HALF_D*sin(PI/2+thta1))),
				CPoint((int)(ptDraw.x+TRI_HALF_D*cos(thta1-PI/2)),(int)(ptDraw.y+TRI_HALF_D*sin(thta1-PI/2))),
		};
		pDrawDC->Polygon(p,3);
		pDrawDC->SelectObject(pPenOld);
		pDrawDC->SelectObject(pBrushOld);
		brushNew.DeleteObject();
		penNew.DeleteObject();
		return;
	}
	else
	{
		penNew.CreatePen(LT_SOLID,0,gColorSet.getClrPieceSelected());
		pPenOld = pDrawDC->SelectObject(&penNew);
		brushNew.CreateSolidBrush(RGB(255,255,255));
		pBrushOld = pDrawDC->SelectObject(&brushNew);

		CRect rect;
		ptScr = ptHpglToScr(CPoint(m_pointList->GetAt(0).x,m_pointList->GetAt(0).y));
		rect.left = ptScr.x-4;
		rect.right = ptScr.x+4;
		rect.top = ptScr.y+4;
		rect.bottom = ptScr.y-4;
		pBrushOld = pDrawDC->SelectObject(&brushNew);
		pDrawDC->Ellipse(rect);

		pDrawDC->SelectObject(pPenOld);
		pDrawDC->SelectObject(pBrushOld);
		brushNew.DeleteObject();
		penNew.DeleteObject();
		return;
	}
	return;
}

BOOL CCurve::CurveInRect(CRect rect)
{
	CPoint ptLB(m_nXmin,m_nYmin);
	CPoint ptRT(m_nXmax,m_nYmax);
	if (! rect.PtInRect(ptHpglToScr(ptLB))) //左下角
	{
		return FALSE;
	}

	if (!rect.PtInRect(ptHpglToScr(ptRT))) //右上角
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CCurve::PointOnCurve(CPoint point)
{
	CPoint point4[4];
	CPoint ptLastHpgl,ptCurHpgl,ptLastScr,ptCurScr;
	double dAngle;
	int i;
	CRgn rgn;
	double dScale = gSet.getDispScale();

	ptLastHpgl = m_pointList->GetAt(0);
	ptLastScr = ptHpglToScr(ptLastHpgl);

	for(i = 1; i < m_pointList->GetSize(); i++)
	{
		ptCurHpgl = m_pointList->GetAt(i);
		ptCurScr = ptHpglToScr(ptCurHpgl);
		
		dAngle = atan2( (double)(ptCurScr.y-ptLastScr.y), (double)(ptCurScr.x-ptLastScr.x) );
		point4[0].x = (int)(ptLastScr.x - PT_OFFSET*cos(dAngle) - PT_OFFSET*sin(dAngle));
		point4[0].y = (int)(ptLastScr.y - PT_OFFSET*sin(dAngle) + PT_OFFSET*cos(dAngle));
		
		point4[1].x = (int)(ptLastScr.x - PT_OFFSET*cos(dAngle) + PT_OFFSET*sin(dAngle));
		point4[1].y = (int)(ptLastScr.y- PT_OFFSET*sin(dAngle) - PT_OFFSET*cos(dAngle));
		
		point4[2].x = (int)(ptCurScr.x + PT_OFFSET*cos(dAngle) + PT_OFFSET*sin(dAngle));
		point4[2].y = (int)(ptCurScr.y + PT_OFFSET*sin(dAngle) - PT_OFFSET*cos(dAngle));
		
		point4[3].x = (int)(ptCurScr.x + PT_OFFSET*cos(dAngle) - PT_OFFSET*sin(dAngle));
		point4[3].y = (int)(ptCurScr.y + PT_OFFSET*sin(dAngle) + PT_OFFSET*cos(dAngle));
		
		rgn.CreatePolygonRgn(point4, 4, ALTERNATE);
        if(rgn.PtInRegion(point))
            return true;
        rgn.DeleteObject();
		ptLastScr = ptCurScr;
    }
    return false;
}

CPoint CCurve::GetCrossPoint(CPoint ptOrg[3], double d, BOOL bLeft)
{
	CPoint ptCross;
	ST_LINE line1,line2;
	ST_DPOINT dPtCross;
	double dAngle1,dAngle2;
	dAngle1 = atan2( (double)(ptOrg[1].y-ptOrg[0].y) , (double)(ptOrg[1].x-ptOrg[0].x) );
	dAngle2 = atan2( (double)(ptOrg[2].y-ptOrg[1].y) , (double)(ptOrg[2].x-ptOrg[1].x) );

	if (bLeft) //左侧
	{
		line1.Start.x = ptOrg[0].x + d*sin(dAngle1);
		line1.Start.y = ptOrg[0].y - d*cos(dAngle1);
		line1.End.x = ptOrg[1].x + d*sin(dAngle1);
		line1.End.y = ptOrg[1].y - d*cos(dAngle1);
		
		line2.Start.x = ptOrg[1].x + d*sin(dAngle2);
		line2.Start.y = ptOrg[1].y - d*cos(dAngle2);
		line2.End.x = ptOrg[2].x + d*sin(dAngle2);
		line2.End.y = ptOrg[2].y - d*cos(dAngle2);
	}
	else //右侧
	{
		line1.Start.x = ptOrg[0].x - d*sin(dAngle1);
		line1.Start.y = ptOrg[0].y + d*cos(dAngle1);
		line1.End.x = ptOrg[1].x - d*sin(dAngle1);
		line1.End.y = ptOrg[1].y + d*cos(dAngle1);
		
		line2.Start.x = ptOrg[1].x - d*sin(dAngle2);
		line2.Start.y = ptOrg[1].y + d*cos(dAngle2);
		line2.End.x = ptOrg[2].x - d*sin(dAngle2);
		line2.End.y = ptOrg[2].y + d*cos(dAngle2);
	}

	dPtCross = GetLineIntersection(line1,line2);
	ptCross.x = (int)dPtCross.x;
	ptCross.y = (int)dPtCross.y;
	return ptCross;
}

inline double isLeft( CPoint *P0, CPoint *P1, CPoint *P2 )
{
	double d;
	d = ( (P1->x - P0->x) * 0.025 * (P2->y - P0->y) - (P2->x - P0->x) * 0.025 * (P1->y - P0->y) );
	return d;
}

BOOL CCurve::PtInPolygon(CPoint pt)
{
	int i,nRgnPointSize;
	int wn=0;
	CPoint ptS,ptE;
	nRgnPointSize = m_pointList->GetSize();
	for (i=0;i<nRgnPointSize-1;i++)
	{
		ptS = m_pointList->GetAt(i);
		ptE = m_pointList->GetAt(i+1);

		if (ptS.y <= pt.y)
		{
			if (ptE.y > pt.y)
				if (isLeft( &ptS, &ptE, &pt) > 0.0)
					++wn;
		}
		else
		{
			if (ptE.y <= pt.y)
				if (isLeft( &ptS, &ptE, &pt) < 0.0)
					--wn;
		}
	}
	if (wn==0)
		return FALSE;
	
	return TRUE;
}

void CCurve::OnExpand(int MmBefore, int MmAfter)
{
	CPoint ptS,ptE,ptNew;
	int nCurveSize;
	double dd;

	nCurveSize = m_pointList->GetSize();
	if (nCurveSize<2)
	{
		return;
	}

	ptS = m_pointList->GetAt(0);
	ptE = m_pointList->GetAt(1);
	double dAngle = atan2( (double)(ptE.y-ptS.y),(double)(ptE.x-ptS.x));
	if (MmBefore > 0 )
	{
		ptNew.x = (int)(ptS.x-MmBefore*40.0*cos(dAngle));
		ptNew.y = (int)(ptS.y-MmBefore*40.0*sin(dAngle));

		m_pointList->RemoveAt(0);
		m_pointList->InsertAt(0,ptNew);
	}
	else if (MmBefore < 0)
	{
		dd = getdistance(ptS,ptE);
		if (fabs(MmBefore*40.0) >= dd)
		{
			m_pointList->RemoveAt(0);
		}
		else
		{
			ptNew.x = (int)(ptS.x-MmBefore*40.0*cos(dAngle));
			ptNew.y = (int)(ptS.y-MmBefore*40.0*sin(dAngle));
			m_pointList->RemoveAt(0);
			m_pointList->InsertAt(0,ptNew);
		}
	}
//-------------------------------------------------------------

	nCurveSize = m_pointList->GetSize();
	ptS = m_pointList->GetAt(nCurveSize-2);
	ptE = m_pointList->GetAt(nCurveSize-1);
	dAngle = atan2( (double)(ptE.y-ptS.y),(double)(ptE.x-ptS.x));
	if (MmAfter > 0)
	{
		ptNew.x = (int)(ptE.x+MmAfter*40.0*cos(dAngle));
		ptNew.y = (int)(ptE.y+MmAfter*40.0*sin(dAngle));
		m_pointList->RemoveAt(nCurveSize-1);
		m_pointList->Add(ptNew);
	}
	else if (MmAfter < 0)
	{
		dd = getdistance(ptS,ptE);
		if (fabs(MmAfter*40.0) >= dd)
		{
			m_pointList->RemoveAt(nCurveSize-1);
		}
		else
		{
			ptNew.x = (int)(ptE.x+MmAfter*40.0*cos(dAngle));
			ptNew.y = (int)(ptE.y+MmAfter*40.0*sin(dAngle));
			m_pointList->RemoveAt(nCurveSize-1);
			m_pointList->Add(ptNew);
		}
	}
}

void CCurve::PolyBigger_1(double d) //曲线外扩
{
	int i;
	CPoint linePt[3];
	CPoint ptInterSection0,ptInterSection;
	bool bLeft;

	if (FALSE == m_bIsPolygon)
	{
		return;
	}
	int nPtSum = m_pointList->GetSize();

	linePt[0] = m_pointList->GetAt(nPtSum-2);
	linePt[1] = m_pointList->GetAt(0);
	linePt[2] = m_pointList->GetAt(1);
	
	ptInterSection0 = GetCrossPoint(linePt,d*40.0,TRUE); //获取交点(左侧偏移d)
	if (!PtInPolygon(ptInterSection0)) //交点不在原图中
	{
		bLeft = FALSE;
	}
	else
	{
		ptInterSection0 = GetCrossPoint(linePt,d*40.0,FALSE); //获取交点(左侧偏移d)
		bLeft = TRUE;
	}

	m_pointList->Add(ptInterSection0);
	for(i = 1; i < nPtSum-1; i++)
	{
		linePt[0] = m_pointList->GetAt(i-1);
		linePt[1] = m_pointList->GetAt(i);
		linePt[2] = m_pointList->GetAt(i+1);

		ptInterSection = GetCrossPoint(linePt,d*40.0,!bLeft); //获取交点(左侧偏移d)
		m_pointList->Add(ptInterSection);
	}
	m_pointList->Add(ptInterSection0);
	for(i=0;i<nPtSum;i++)
	{
		m_pointList->RemoveAt(0);
	}
}

void CCurve::PolyInflate(double d)
{
	double dd = d;
	while (dd > 0.8)
	{
		CurveRemoveShortSec();
		PolyBigger_1(0.5);
		dd = dd - 0.5;
		CurveSmoothMethod1(); //删除交叉线段
		OnPointMove(-3);
		CurveSmoothMethod1(); //删除交叉线段
		OnPointMove(3);
		NewCurveSmoothAccordOldCurve(NULL);
		GetCurveMaxMin();
	}

	CurveRemoveShortSec();
	PolyBigger_1(dd);
	CurveSmoothMethod1(); 
	GetCurveMaxMin();
}

void CCurve::PolyShrink(double d)
{
	int i;
	CPoint linePt[3];
	CPoint ptInterSection0,ptInterSection;
	bool bLeft;
	CArray <CPoint,CPoint> *m_pointListNew = new CArray <CPoint,CPoint>;

	if (FALSE == m_bIsPolygon)
	{
		return;
	}
	if( ( (m_nXmax-m_nXmin) < 4*d*40.0) || ( (m_nYmax-m_nYmin) < 4*d*40.0) )
	{
		AfxMessageBox("曲线太小无法收缩");
		return;
	}

	int nPtSum = m_pointList->GetSize();

	linePt[0] = m_pointList->GetAt(nPtSum-2);
	linePt[1] = m_pointList->GetAt(0);
	linePt[2] = m_pointList->GetAt(1);
	
	ptInterSection0 = GetCrossPoint(linePt,d*40.0,TRUE); //获取交点(左侧偏移d)
	if (PtInPolygon(ptInterSection0)) //在原图中
	{
		bLeft = FALSE;
	}
	else
	{
		ptInterSection0 = GetCrossPoint(linePt,d*40.0,FALSE); //获取交点(左侧偏移d)
		bLeft = TRUE;
	}
	m_pointListNew->Add(ptInterSection0);
	for(i = 1; i < nPtSum-1; i++)
	{
		linePt[0] = m_pointList->GetAt(i-1);
		linePt[1] = m_pointList->GetAt(i);
		linePt[2] = m_pointList->GetAt(i+1);
		
		ptInterSection = GetCrossPoint(linePt,d*40.0,!bLeft); //获取交点(左侧偏移d)
		m_pointListNew->Add(ptInterSection);
	}
	m_pointListNew->Add(ptInterSection0);
//------------------TODO---------------------------------------------
//YHZ	原始曲线和新曲线进行角度比较，如果角度不一样，则移除响应的的点 暂时不需要实现
//------------------TODO---------------------------------------------
	while(m_pointList->GetSize() > 0) //移除原来的点
	{
		m_pointList->RemoveAt(0);
	}
	for(i=0;i<m_pointListNew->GetSize();i++)
	{
		m_pointList->Add(m_pointListNew->GetAt(i));
	}
	while(m_pointListNew->GetSize() > 0) //移除原来的点
	{
		m_pointListNew->RemoveAt(0);
	}
	delete m_pointListNew;
}

void CCurve::CurveInflate(double d) //d实际槽宽
{
	CArray <CPoint,CPoint> *m_pointList1 = new CArray<CPoint, CPoint>;
	CArray <CPoint,CPoint> *m_pointList2 = new CArray<CPoint, CPoint>;
	
	CPoint PP1[2];
	CPoint PP2[2];
	double dAngle;
	int i;
	CPoint ptLast,ptCur;
//	double dDistOrg;

	int nPLSIZE = m_pointList->GetSize();
	m_pointList1->RemoveAll();
	m_pointList2->RemoveAll();

	ptLast = m_pointList->GetAt(0);
	for(i = 1; i < nPLSIZE; i++)
	{
		ptCur = m_pointList->GetAt(i);

		dAngle = atan2( (double)(ptCur.y-ptLast.y), (double)(ptCur.x-ptLast.x) );
//		dDistOrg = ;

		PP1[0].x = (int)(ptLast.x - d*sin(dAngle)); //左边
		PP1[0].y = (int)(ptLast.y + d*cos(dAngle));
		PP1[1].x = (int)(ptCur.x  - d*sin(dAngle));
		PP1[1].y = (int)(ptCur.y  + d*cos(dAngle));
		
		PP2[0].x = (int)(ptLast.x + d*sin(dAngle)); //右边
		PP2[0].y = (int)(ptLast.y - d*cos(dAngle));
		PP2[1].x = (int)(ptCur.x + d*sin(dAngle));
		PP2[1].y = (int)(ptCur.y  - d*cos(dAngle));

		ptLast = ptCur;
		m_pointList1->Add(PP1[0]);
		m_pointList1->Add(PP1[1]);

		m_pointList2->Add(PP2[0]);
		m_pointList2->Add(PP2[1]);
    }

	m_pointList->RemoveAll();
	for (i=0;i<m_pointList1->GetSize();i++)
	{
		m_pointList->Add(m_pointList1->GetAt(i));
	}

	for (i=m_pointList2->GetSize()-1; i >= 0; i--)
	{
		m_pointList->Add(m_pointList2->GetAt(i));
	}
	m_pointList->Add(m_pointList1->GetAt(0)); //形成闭合曲线

	m_pointList1->RemoveAll();
	m_pointList2->RemoveAll();
	delete m_pointList1;
	delete m_pointList2;
    return;
}

bool between(double a, double X0, double X1)
{
	double temp1 = a - X0;
	double temp2 = a - X1;
	if ((temp1<1e-8 && temp2>-1e-8) || (temp2<1e-6 && temp1>-1e-8))
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool detectIntersect(ST_DPOINT p1, ST_DPOINT p2, ST_DPOINT p3, ST_DPOINT p4)
{
	double line_x, line_y; //交点  
	if ((fabs(p1.x - p2.x)<1e-6) && (fabs(p3.x - p4.x)<1e-6))
	{
		return false;
	}
	else if ((fabs(p1.x - p2.x)<1e-6)) //如果直线段p1p2垂直与y轴  
	{
		if (between(p1.x, p3.x, p4.x))
		{
			double k = (p4.y - p3.y) / (p4.x - p3.x);
			line_x = p1.x;
			line_y = k*(line_x - p3.x) + p3.y;

			if (between(line_y, p1.y, p2.y))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else if ((fabs(p3.x - p4.x)<1e-6)) //如果直线段p3p4垂直与y轴  
	{
		if (between(p3.x, p1.x, p2.x))
		{
			double k = (p2.y - p1.y) / (p2.x - p1.x);
			line_x = p3.x;
			line_y = k*(line_x - p2.x) + p2.y;

			if (between(line_y, p3.y, p4.y))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		double k1 = (p2.y - p1.y) / (p2.x - p1.x);
		double k2 = (p4.y - p3.y) / (p4.x - p3.x);

		if (fabs(k1 - k2)<1e-6)
		{
			return false;
		}
		else
		{
			line_x = ((p3.y - p1.y) - (k2*p3.x - k1*p1.x)) / (k1 - k2);
			line_y = k1*(line_x - p1.x) + p1.y;
		}

		if (between(line_x, p1.x, p2.x) && between(line_x, p3.x, p4.x))
		{
			return true;
		}
		else
		{
			return false;
		}
	}
}

void CCurve::CurveSmoothMethod1() //删除交叉线段
{
	if(m_pointList->GetSize()<4)
	{
		return;
	}
	CPoint pt[4];
	CPoint ptNew;
	ST_LINE line1,line2;
	ST_DPOINT dPtCross;

	int i;
	int nPtSize = m_pointList->GetSize();

	for(i=0; i<nPtSize-3; i++)
	{
		pt[0] = m_pointList->GetAt(i);
		pt[1] = m_pointList->GetAt(i+1);

		pt[2] = m_pointList->GetAt(i+2);
		pt[3] = m_pointList->GetAt(i+3);

		if (  (pt[1].y - pt[0].y)*(pt[3].x - pt[2].x) == (pt[3].y - pt[2].y)*(pt[1].x - pt[0].x) ) //两线平行
		{
			continue;
		}

		line1.Start.x = pt[0].x;
		line1.Start.y = pt[0].y;
		line1.End.x = pt[1].x;
		line1.End.y = pt[1].y;

		line2.Start.x = pt[2].x;
		line2.Start.y = pt[2].y;
		line2.End.x = pt[3].x;
		line2.End.y = pt[3].y;

		dPtCross = GetLineIntersection(line1, line2);
		if ( detectIntersect(line1.Start, line1.End, line2.Start, line2.End) )
		{
			ptNew.x = (int)dPtCross.x;
			ptNew.y = (int)dPtCross.y;
			m_pointList->RemoveAt(i + 1);
			m_pointList->RemoveAt(i + 1);
			m_pointList->InsertAt(i + 1, ptNew);
			nPtSize--;
//			i--;
		}

	}
}

#define  SMOOTH_DIST2 (10.0)
void CCurve::CurveRemoveShortSec() //曲线平滑处理--删除过短的线段 
{
	CPoint pt[2];
	int i;
	int nPtSize = m_pointList->GetSize();
	for (i = 0; i<nPtSize - 2; i++)
	{
		pt[0] = m_pointList->GetAt(i);
		pt[1] = m_pointList->GetAt(i + 1);
		if (getdistance(pt[0], pt[1]) < SMOOTH_DIST2)
		{
			m_pointList->RemoveAt(i + 1);
			nPtSize--;
			i--;
		}
	}
}

void CCurve::NewCurveSmoothAccordOldCurve(CCurve* pOldCurve)
{

}


//#define START_MX_MM 7
//#define START_PENX_MM -30
//#define START_MY_MM 50
//#define START_PENY_MM 115

void CCurve::OnToPlotData()
{
	int i,nSizeCurve;
	ST_CNC_DATA_ALL *pCncData;
	nSizeCurve = m_pointList->GetSize();
	if (nSizeCurve == 0)
	{
		return;
	}
	for (i=0;i<nSizeCurve;i++)
	{
		pCncData = new ST_CNC_DATA_ALL;
		pCncData->m_iX = (int)( ( START_PENX_MM
			+m_pointList->GetAt(i).x*250.0/gSet.get10000X()
			+gSet.getMPDistX()*0.1)*gSet.getPPMMX() );

		pCncData->m_iY = (int)( ( START_PENY_MM
			+m_pointList->GetAt(i).y*250.0/gSet.get10000Y()
			+gSet.getMPDistY()*0.1)*gSet.getPPMMY() );
		if (0 == i)
		{
			pCncData->m_nCmdType = TYPE_MOVE;
		}
		else
		{
			pCncData->m_nCmdType = TYPE_PLOT;
		}
		g_ptrCncList.Add(pCncData);
	}
}

void CCurve::OnToMMData()
{
	int i,nSizeCurve;
	ST_CNC_DATA_ALL *pCncData;
	
	nSizeCurve = m_pointList->GetSize();
	if (nSizeCurve == 0)
	{
		return;
	}
	for (i=0;i<nSizeCurve;i++)
	{
		pCncData = new ST_CNC_DATA_ALL;
		pCncData->m_iX = (int)( (START_MX_MM
			+m_pointList->GetAt(i).x*250.0/gSet.get10000X())*gSet.getPPMMX() );
		pCncData->m_iY = (int)( (START_MY_MM
			+m_pointList->GetAt(i).y*250.0/gSet.get10000Y())*gSet.getPPMMY() );
		if (0 == i)
		{
			pCncData->m_nCmdType = TYPE_MOVE;
		}
		else
		{
			pCncData->m_nCmdType = TYPE_MM;
		}
		g_ptrCncList.Add(pCncData);
	}
}

void CCurve::OnToDrillData()
{
	ST_CNC_DATA_ALL *pCncData;
	
	if (m_pointList->GetSize() != 1)
	{
		return;
	}

	pCncData = new ST_CNC_DATA_ALL;
	pCncData->m_iX = (int)( ( START_MX_MM
		+m_pointList->GetAt(0).x*250.0/gSet.get10000X())*gSet.getPPMMX() );
	pCncData->m_iY = (int)( ( START_MY_MM
		+m_pointList->GetAt(0).y*250.0/gSet.get10000Y())*gSet.getPPMMY() );
	pCncData->m_nCmdType = TYPE_MOVE;
	g_ptrCncList.Add(pCncData);

	pCncData = new ST_CNC_DATA_ALL;
	pCncData->m_iX = (int)( ( START_MX_MM
		+m_pointList->GetAt(0).x*250.0/gSet.get10000X())*gSet.getPPMMX() );
	pCncData->m_iY = (int)( ( START_MY_MM
		+m_pointList->GetAt(0).y*250.0/gSet.get10000Y())*gSet.getPPMMY() );
	pCncData->m_nCmdType = TYPE_MM;
	g_ptrCncList.Add(pCncData);
}


void CCurve::OnSmoothRpFile()
{
	CPoint pt0, pt1;
	CPoint ptend;
	int i,j;
	int nPlSize;
	pt0 = m_pointList->GetAt(0);
	pt1 = m_pointList->GetAt(1);
	if (getdistance(pt0, pt1) < 5.0)
	{
		m_pointList->RemoveAt(1);
	}
	if (m_pointList->GetSize() > 4)
	{
		pt0 = m_pointList->GetAt(0);
		nPlSize = m_pointList->GetSize();
		for (i = 1; i <= 3; i++)
		{
			ptend = m_pointList->GetAt(nPlSize -1 - i);
			if (getdistance(pt0, ptend) < 5.0)
			{
				for (j = 0; j < (i + 1); j++)
				{
					m_pointList->RemoveAt(m_pointList->GetSize() - 1);
				}
				m_pointList->Add(pt0);
				break;
			}
		}
	}
}


void CCurve::OnSetCurveStart(CPoint ptMouse)
{
	CPoint pt;
	CPoint ptScr;
	int i,j;
	int nCurveSize;
	nCurveSize = m_pointList->GetSize();
	for (i=1;i<nCurveSize;i++)
	{
		pt = m_pointList->GetAt(i);
		ptScr = ptHpglToScr(pt);
		if (getdistance(ptMouse,ptScr) < 6.0)
		{
			for (j=0;j<i;j++)
			{
				m_pointList->Add(m_pointList->GetAt(j+1));
			}
			for (j=0;j<i;j++)
			{
				m_pointList->RemoveAt(0);
			}
			return;
		}
	}
}

void CCurve::OnPointMove(int n)
{
	int absn;
	int nPtSum;
	int i;
	if (n < 0)
		absn = -n;
	else
		absn = n;
	nPtSum = m_pointList->GetSize();
	if (absn > nPtSum)
		return;
	if (m_bIsPolygon == FALSE)
		return;
	
	if (n > 0)
	{
		for (i = 0; i < n; i++)
		{
			m_pointList->Add(m_pointList->GetAt(i+1));
		}
		for (i = 0; i < n; i++)
		{
			m_pointList->RemoveAt(0);
		}
		return;
	}
	if (n < 0)
	{
		for (i = 0; i < n; i++)
		{
			m_pointList->RemoveAt(m_pointList->GetSize()-1);
			m_pointList->InsertAt(0, m_pointList->GetAt(m_pointList->GetSize() - 1));
		}
	}
	return;
}
