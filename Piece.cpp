// Piece.cpp: implementation of the CPiece class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MCAD.h"
#include "Piece.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_SERIAL(CPiece, CObject, 1 )
CPiece::CPiece()
{
	m_bRefreshDisplay = FALSE;
	m_nPieceSelStat = 0;
	m_nCutSequence = 0;
}

CPiece::~CPiece()
{
	CCurve* pCurve;
	while (m_CurveList.GetSize() > 0)
	{
		pCurve = m_CurveList.GetAt(0);
		delete pCurve;
		m_CurveList.RemoveAt(0);
	}
}

void CPiece::Serialize(CArchive& ar)
{
	if(ar.IsStoring())
	{
		ar<<m_nXmax<<m_nYmax<<m_nXmin<<m_nYmin;
	}
	else
	{
		ar>>m_nXmax>>m_nYmax>>m_nXmin>>m_nYmin;
	}
	m_CurveList.Serialize(ar);
}

void CPiece::DrawPieceInListView(int piecenum,CDC *pDrawDC)
{
	pDrawDC->Rectangle(
		2,
		(int)(PIECE_LIST_RECTSIZE*piecenum+2),
		(int)(PIECE_LIST_RECTSIZE-2),
		(int)((PIECE_LIST_RECTSIZE*piecenum)+PIECE_LIST_RECTSIZE-2));
	double d;
	if( (m_nXmax-m_nXmin) >= (m_nYmax-m_nYmin) )
	{
		d = (PIECE_LIST_RECTSIZE-10.0)/(m_nXmax-m_nXmin);
	}
	else
	{
		d = (PIECE_LIST_RECTSIZE-10.0)/(m_nYmax-m_nYmin);
	}
	CCurve* pCurve;
	pCurve = m_CurveList.GetAt(0); //第一条曲线是外框
	
	int i;
	int curX,curY;

	curX = pCurve->m_pointList->GetAt(0).x-m_nXmin;
	curY = pCurve->m_pointList->GetAt(0).y-m_nYmin;

	curX = (int)(5.0+curX * d);
	curY = (int)(PIECE_LIST_RECTSIZE*piecenum+PIECE_LIST_RECTSIZE-5.0-curY * d);
	pDrawDC->MoveTo(curX,curY);
	int nPointSum = pCurve->m_pointList->GetSize(); 
	for (i=1;i<nPointSum;i++)
	{
		curX = pCurve->m_pointList->GetAt(i).x-m_nXmin;
		curY = pCurve->m_pointList->GetAt(i).y-m_nYmin;

		curX = (int)(5.0+curX * d);
		curY = (int)(PIECE_LIST_RECTSIZE*piecenum+PIECE_LIST_RECTSIZE-5.0-curY * d);
		pDrawDC->LineTo(curX,curY);
	}
	CString str;
	str.Format("%d",(piecenum+1));
	pDrawDC->TextOut(
		(int)(PIECE_LIST_RECTSIZE/2)-4,
		(int)(PIECE_LIST_RECTSIZE*piecenum+PIECE_LIST_RECTSIZE/2)-8,
		str);
}

void CPiece::DrawPieceInMCADView(CDC *pDrawDC)
{
	int i;
	CCurve* pCurve;
	switch(m_nPieceSelStat)
	{
		case 0: //未选中
		pDrawDC->SetROP2(R2_COPYPEN);
		for(i=0;i<m_CurveList.GetSize();i++)
		{
			pCurve = m_CurveList.GetAt(i);
			pCurve->OnDrawCurve(pDrawDC,0);//使用本来颜色绘制
		}
		break;

		case 1:
		pDrawDC->SetROP2(R2_COPYPEN);
		for(i=0;i<m_CurveList.GetSize();i++)
		{
			pCurve = m_CurveList.GetAt(i);
			pCurve->OnDrawCurve(pDrawDC,1); //使用选中颜色绘制
		}
		break;
	
		case 2: //绘制外框
		pDrawDC->SetROP2(R2_XORPEN);
		OnGetPieceOutLine();
		for (i=0;i<m_CurveList.GetSize();i++)
		{
			pCurve = m_CurveList.GetAt(i);
			if (pCurve->m_bIsOutline)
			{
				pCurve->OnDrawCurve(pDrawDC,1); //使用选中颜色绘制
				pDrawDC->SetROP2(R2_COPYPEN);
				return;
			}
		}
		pCurve = m_CurveList.GetAt(0);
		pCurve->OnDrawCurve(pDrawDC,1); //使用选中颜色绘制
		pDrawDC->SetROP2(R2_COPYPEN);
		break;
	}

}

BOOL CPiece::CurveInPiece(CCurve *pCurve)
{
	CPoint ptCenter;
	CPoint ptTemp0,ptTemp1;
	int i;
	if (pCurve->m_pointList->GetSize() <= 1)
	{
		ptTemp0 = pCurve->m_pointList->GetAt(0);
		if (PtHpglInPiece(ptTemp0))
		{
			return TRUE;
		}
	}
	else
	{
		for (i=0;i<(pCurve->m_pointList->GetSize()-1);i++)
		{
			ptTemp0 = pCurve->m_pointList->GetAt(i);
			ptTemp1 = pCurve->m_pointList->GetAt(i+1);
			
			ptCenter.x = (ptTemp0.x+ptTemp1.x)/2;
			ptCenter.y = (ptTemp0.y+ptTemp1.y)/2;
			if (PtHpglInPiece(ptTemp0))
			{
				return TRUE;
			}
			if (PtHpglInPiece(ptCenter))
			{
				return TRUE;
			}
		}
		ptTemp0 = pCurve->m_pointList->GetAt(pCurve->m_pointList->GetSize()-1);
		if (PtHpglInPiece(ptTemp0))
		{
			return TRUE;
		}
	}
	return FALSE;
}


inline double isLeft( CPoint *P0, CPoint *P1, CPoint *P2 )
{
	double d;
	d = ( (P1->x - P0->x) * 0.025 * (P2->y - P0->y) - (P2->x - P0->x) * 0.025 * (P1->y - P0->y) );
	return d;
}

BOOL CPiece::PtHpglInPiece(CPoint pt)
{
	CCurve *pOutlineCurve; //外框
	int wn=0;
	CPoint ptS,ptE;
	pOutlineCurve = m_CurveList.GetAt(0);
	return pOutlineCurve->PtInPolygon(pt);
}

BOOL CPiece::PtScrInPiece(CPoint pt,double dScale)
{
	CCurve *pOutlineCurve; //外框
	CCurve *pCurve;
	int i,nRgnPointSize;
	int wn=0;
	CPoint ptS,ptE;
	CPoint ScrPtS,ScrPtE;
	pOutlineCurve = NULL;
	OnGetPieceOutLine();
	for (i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		if (pCurve->m_bIsOutline)
		{
			pOutlineCurve = pCurve;
		}
	}
	if (NULL == pOutlineCurve)
	{
		pOutlineCurve = m_CurveList.GetAt(0);
	}
	nRgnPointSize = pOutlineCurve->m_pointList->GetSize();

	for (i=0;i<nRgnPointSize-1;i++)
	{
		ptS = pOutlineCurve->m_pointList->GetAt(i);
		ptE = pOutlineCurve->m_pointList->GetAt(i+1);
		ScrPtS = ptHpglToScr(ptS);
		ScrPtE = ptHpglToScr(ptE);

		if (ScrPtS.y <= pt.y)
		{
			if (ScrPtE.y > pt.y)
				if (isLeft( &ScrPtS, &ScrPtE, &pt) > 0.0)
					++wn;
		}
		else
		{
			if (ScrPtE.y <= pt.y)
				if (isLeft( &ScrPtS, &ScrPtE, &pt) < 0.0)
					--wn;
		}
	}
	if (wn==0)
		return FALSE;
	
	return TRUE;
}

void CPiece::OnMove(int deltaX, int deltaY)
{
	CCurve* pCurve;
	int CurveSum = m_CurveList.GetSize();
	int i;
	for (i=0;i<CurveSum;i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->OnMoveCurve(deltaX,deltaY);
	}
	m_nXmax = m_nXmax + deltaX;
	m_nXmin = m_nXmin + deltaX;
	m_nYmax = m_nYmax + deltaY;
	m_nYmin = m_nYmin + deltaY;
}

void CPiece::ClearCurveSelState()
{
	CCurve* pCurve;
	int CurveSum = m_CurveList.GetSize();
	int i;
	for (i=0;i<CurveSum;i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->m_bIsSelected = FALSE;
	}
}

void CPiece::PieceRotate(double dAngle)
{
	int CenterX,CenterY;
	CenterX = (m_nXmax+m_nXmin)/2;
	CenterY = (m_nYmax+m_nYmin)/2;
	CCurve* pCurve;
	for (int i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->RotateCurve(CenterX,CenterY,dAngle);
		pCurve->GetCurveMaxMin();
	}
	pCurve = m_CurveList.GetAt(0);
	m_nXmax = pCurve->m_nXmax;
	m_nXmin = pCurve->m_nXmin;
	m_nYmax = pCurve->m_nYmax;
	m_nYmin = pCurve->m_nYmin;
}

void CPiece::PieceMirror()
{
	CCurve* pCurve;
	for (int i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->CurveMirror(m_nXmax);
	}
}

void CPiece::operator=(CPiece &a)
{
	CCurve* pCurve;
	this->m_nPieceSelStat = a.m_nPieceSelStat;
	this->m_nXmax = a.m_nXmax;
	this->m_nXmin = a.m_nXmin;
	this->m_nYmax = a.m_nYmax;
	this->m_nYmin = a.m_nYmin;

	for (int i=0;i<a.m_CurveList.GetSize();i++)
	{
		pCurve = a.m_CurveList.GetAt(i);
		CCurve* pNewCurve = new CCurve;
		*pNewCurve = *pCurve;
		this->m_CurveList.Add(pNewCurve);
	}
}

int CPiece::AddOutLineType1()
{
	double dOldXmax,dOldXmin,dOldYmax,dOldYmin;
	double dNewXmax,dNewXmin,dNewYmax,dNewYmin;
	double dOldXSize,dOldYSize;
	double dNewXSize,dNewYSize;
	int iCutNum;

	dOldXSize = m_nXmax-m_nXmin;
	dOldYSize = m_nYmax-m_nYmin;

	dOldXmax = m_nXmax;
	dOldXmin = m_nXmin;
	dOldYmax = m_nYmax;
	dOldYmin = m_nYmin;

	dNewXmax = m_nXmax+gOutLineProp.getMmRight()*40.0;
	dNewXmin = m_nXmin-gOutLineProp.getMmLeft()*40.0;
	dNewYmax = m_nYmax+gOutLineProp.getMmTop()*40.0;
	dNewYmin = m_nYmin-gOutLineProp.getMmBot()*40.0;

	dNewXSize = dNewXmax - dNewXmin;
	dNewYSize = dNewYmax - dNewYmin;
	
	if ( (dNewXSize < 2.0*gOutLineProp.getMmOutlineRadius()*40.0) 
		|| (dNewYSize < 2.0*gOutLineProp.getMmOutlineRadius()*40.0) )
	{
		AfxMessageBox("圆角半径设置过大,创建外框失败");
		return -1;
	}
	m_nXmax = (int)dNewXmax;
	m_nXmin = (int)dNewXmin;
	m_nYmax = (int)dNewYmax;
	m_nYmin = (int)dNewYmin;
	
	CCurve* pCurve = new CCurve;
	pCurve->m_bIsOutline = TRUE;
	pCurve->m_bIsPolygon = TRUE;
	pCurve->m_bIsSelected = FALSE;
	pCurve->m_nCurveType = CURVE_TYPE_MM;
//	pCurve->m_dSlotWidth = 0;
	pCurve->m_dxxx = 0.0;
	pCurve->m_nXmax = (int)dNewXmax;
	pCurve->m_nXmin = (int)dNewXmin;
	pCurve->m_nYmax = (int)dNewYmax;
	pCurve->m_nYmin = (int)dNewYmin;
	
	CPoint pt;
	int arcSecSum; //圆弧分段数
	double dCenterX,dCenterY;

	arcSecSum = (int)(gOutLineProp.getMmOutlineRadius()*PI*1.0); //1mm每段
	if (arcSecSum < 1)
	{
		arcSecSum = 1;
	}
	if (arcSecSum > 100)
	{
		arcSecSum = 100;
	}

	switch(gOutLineProp.getCutPos())
	{
		case 0:
		break;

		case 1:
		gOutLineProp.setRoundLT(0);
		gOutLineProp.setRoundLB(0);
		break;

		case 2:
		gOutLineProp.setRoundLB(0);
		gOutLineProp.setRoundRB(0);
		break;

		case 3:
		gOutLineProp.setRoundRB(0);
		gOutLineProp.setRoundRT(0);
		break;

		case 4:
		gOutLineProp.setRoundLT(0);
		gOutLineProp.setRoundRT(0);
		break;
	}

	if ( gOutLineProp.getCutPos() == 1 ) //左边加剪口
	{
		int X0 = (int)(dNewXmin);
		int X1 = (int)(dNewXmin+gOutLineProp.getCutDepth()*40.0);
		int iCenterY = (int)(dOldYmin+dOldYSize/2.0); //中间点位置坐标
		int nOneSideSum = (int)(dNewYSize / (2.0* gOutLineProp.getCutDist()*40.0 ));
		int iStartY = (int)(iCenterY+nOneSideSum*gOutLineProp.getCutDist()*40.0);
		
		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmax);
		pCurve->m_pointList->Add(pt); //左上
		
		for (iCutNum = 0; iCutNum<(nOneSideSum*2+1); iCutNum++ )
		{
			pt.x = X0;
			pt.y = (int)(iStartY-gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X1;
			pt.y = (int)(iStartY-gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X1;
			pt.y = (int)(iStartY-gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X0;
			pt.y = (int)(iStartY-gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
		}
	}
	else if (gOutLineProp.getRoundLT()) //左上倒圆角
	{
		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmax-gOutLineProp.getMmOutlineRadius()*40.0);
		pCurve->m_pointList->Add(pt);
	}
	else
	{
		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmax);
		pCurve->m_pointList->Add(pt); //左上
	}

	if ( gOutLineProp.getCutPos() == 2 ) //下边加剪口
	{
		int Y0 = (int)(dNewYmin);
		int Y1 = (int)(dNewYmin+gOutLineProp.getCutDepth()*40.0);
		int iCenterX = (int)(dOldXmin+dOldXSize/2.0); //中间点位置坐标
		int nOneSideSum = (int)(dNewXSize / (2.0* gOutLineProp.getCutDist()*40.0 ));
		int iStartX = (int)(iCenterX-nOneSideSum*gOutLineProp.getCutDist()*40.0);

		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmin);
		pCurve->m_pointList->Add(pt); //左上
		
		for (iCutNum = 0; iCutNum<(nOneSideSum*2+1); iCutNum++ )
		{
			pt.x = (int)(iStartX+gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pt.y = Y0;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX+gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pt.y = Y1;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX+gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pt.y = Y1;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX+gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pt.y = Y0;
			pCurve->m_pointList->Add(pt);
		}	
	}
	else if (gOutLineProp.getRoundLB()) //左下倒圆角
	{
		dCenterX = dNewXmin+gOutLineProp.getMmOutlineRadius()*40.0;
		dCenterY = dNewYmin+gOutLineProp.getMmOutlineRadius()*40.0;
		for (int i=0;i<=arcSecSum;i++)
		{
			pt.x = (int)( dCenterX-gOutLineProp.getMmOutlineRadius()*40.0*cos(i*PI/arcSecSum/2.0) );
			pt.y = (int)( dCenterY-gOutLineProp.getMmOutlineRadius()*40.0*sin(i*PI/arcSecSum/2.0) );
			pCurve->m_pointList->Add(pt); //左下圆弧
		}
	}
	else
	{
		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmin);
		pCurve->m_pointList->Add(pt); //左下
	}
	
	if ( gOutLineProp.getCutPos() == 3 ) //右边加剪口
	{
		int X0 = (int)(dNewXmax);
		int X1 = (int)(dNewXmax-gOutLineProp.getCutDepth()*40.0);
		int iCenterY = (int)(dOldYmin+dOldYSize/2.0); //中间点位置坐标
		int nOneSideSum = (int)(dNewYSize / (2.0* gOutLineProp.getCutDist()*40.0 ));
		int iStartY = (int)(iCenterY-nOneSideSum*gOutLineProp.getCutDist()*40.0);
		
		pt.x = (int)(dNewXmax);
		pt.y = (int)(dNewYmin);
		pCurve->m_pointList->Add(pt); //右下
		
		for (iCutNum = 0; iCutNum<(nOneSideSum*2+1); iCutNum++ )
		{
			pt.x = X0;
			pt.y = (int)(iStartY+gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X1;
			pt.y = (int)(iStartY+gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X1;
			pt.y = (int)(iStartY+gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
			
			pt.x = X0;
			pt.y = (int)(iStartY+gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pCurve->m_pointList->Add(pt);
		}
	}
	else if (gOutLineProp.getRoundRB()) //右下倒圆角
	{
		dCenterX = dNewXmax-gOutLineProp.getMmOutlineRadius()*40.0;
		dCenterY = dNewYmin+gOutLineProp.getMmOutlineRadius()*40.0;
		for (int i=0;i<=arcSecSum;i++)
		{
			pt.x = (int)( dCenterX+gOutLineProp.getMmOutlineRadius()*40.0*sin(i*PI/arcSecSum/2.0) );
			pt.y = (int)( dCenterY-gOutLineProp.getMmOutlineRadius()*40.0*cos(i*PI/arcSecSum/2.0) );
			pCurve->m_pointList->Add(pt); //右下圆弧
		}
	}
	else
	{
		pt.x = (int)(dNewXmax);
		pt.y = (int)(dNewYmin);
		pCurve->m_pointList->Add(pt); //右上
	}
	
	if ( gOutLineProp.getCutPos() == 4 ) //上边加剪口
	{
		int Y0 = (int)(dNewYmax);
		int Y1 = (int)(dNewYmax-gOutLineProp.getCutDepth()*40.0);
		int iCenterX = (int)(dOldXmin+dOldXSize/2.0); //中间点位置坐标
		int nOneSideSum = (int)(dNewXSize / (2.0* gOutLineProp.getCutDist()*40.0 ));
		int iStartX = (int)(iCenterX+nOneSideSum*gOutLineProp.getCutDist()*40.0);
		
		pt.x = (int)(dNewXmax);
		pt.y = (int)(dNewYmax);
		pCurve->m_pointList->Add(pt); //左上
		
		for (iCutNum = 0; iCutNum<(nOneSideSum*2+1); iCutNum++ )
		{
			pt.x = (int)(iStartX-gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pt.y = Y0;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX-gOutLineProp.getCutDist()*iCutNum*40.0+gOutLineProp.getCutWidth()*20.0);
			pt.y = Y1;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX-gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pt.y = Y1;
			pCurve->m_pointList->Add(pt);
			
			pt.x = (int)(iStartX-gOutLineProp.getCutDist()*iCutNum*40.0-gOutLineProp.getCutWidth()*20.0);
			pt.y = Y0;
			pCurve->m_pointList->Add(pt);
		}	
	}
	else if (gOutLineProp.getRoundRT()) //右上倒圆角
	{
		dCenterX = dNewXmax-gOutLineProp.getMmOutlineRadius()*40.0;
		dCenterY = dNewYmax-gOutLineProp.getMmOutlineRadius()*40.0;
		for (int i=0;i<=arcSecSum;i++)
		{
			pt.x = (int)( dCenterX+gOutLineProp.getMmOutlineRadius()*40.0*cos(i*PI/arcSecSum/2.0) );
			pt.y = (int)( dCenterY+gOutLineProp.getMmOutlineRadius()*40.0*sin(i*PI/arcSecSum/2.0) );
			pCurve->m_pointList->Add(pt); //右上圆弧
		}
	}
	else
	{
		pt.x = (int)(dNewXmax);
		pt.y = (int)(dNewYmax);
		pCurve->m_pointList->Add(pt); //右上
	}
	
	if (gOutLineProp.getRoundLT())
	{
		dCenterX = dNewXmin+gOutLineProp.getMmOutlineRadius()*40.0;
		dCenterY = dNewYmax-gOutLineProp.getMmOutlineRadius()*40.0;
		for (int i=0;i<=arcSecSum;i++)
		{
			pt.x = (int)( dCenterX-gOutLineProp.getMmOutlineRadius()*40.0*sin(i*PI/arcSecSum/2.0) );
			pt.y = (int)( dCenterY+gOutLineProp.getMmOutlineRadius()*40.0*cos(i*PI/arcSecSum/2.0) );
			pCurve->m_pointList->Add(pt); //右上圆弧
		}
	}
	else
	{
		pt.x = (int)(dNewXmin);
		pt.y = (int)(dNewYmax);
		pCurve->m_pointList->Add(pt); //左上
	}
	m_CurveList.InsertAt(0,pCurve);

	if (gOutLineProp.getClockWise()) //设置了顺时针
	{
		pCurve->OnReverse();
	}
	return 0;
}

void CPiece::OnPieceAddDrill(CPoint ptHpgl)
{
	CCurve *pCurve = new CCurve;

	m_CurveList.Add(pCurve);
}

void CPiece::ReCalcPieceSize()
{
	int i;
	CCurve* pCurve;
	for (i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->GetCurveMaxMin();
	}

	pCurve = m_CurveList.GetAt(0);

	m_nXmin = pCurve->m_nXmin;
	m_nXmax = pCurve->m_nXmax;
	m_nYmin = pCurve->m_nYmin;
	m_nYmax = pCurve->m_nYmax;

	for (i=1;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
//		if (pCurve->m_nCurveType != CURVE_TYPE_DRILL)
		{
			m_nXmin = (m_nXmin < pCurve->m_nXmin )? m_nXmin : pCurve->m_nXmin;
			m_nXmax = (m_nXmax > pCurve->m_nXmax )? m_nXmax : pCurve->m_nXmax;

			m_nYmin = (m_nYmin < pCurve->m_nYmin )? m_nYmin : pCurve->m_nYmin;
			m_nYmax = (m_nYmax > pCurve->m_nYmax )? m_nYmax : pCurve->m_nYmax;
		}
	}
}

void CPiece::OnCheckIfBelowZero()
{
	int i;
	CCurve* pCurve;
	if (m_nXmin < 0)
	{
		for (i=0;i<m_CurveList.GetSize();i++)
		{
			pCurve = m_CurveList.GetAt(i);
			pCurve->OnMoveCurve(-m_nXmin,0);
		}
		m_nXmin = 0;
	}
	if (m_nYmin<0)
	{
		for (i=0;i<m_CurveList.GetSize();i++)
		{
			pCurve = m_CurveList.GetAt(i);
			pCurve->OnMoveCurve(0,-m_nYmin);
		}
		m_nYmin = 0;
	}
}

int CPiece::AddOutLineType2(double d)
{
	CCurve* pCurveNew = new CCurve;
	CCurve* pCurve;
	int i;
	for (i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = (CCurve*)(m_CurveList.GetAt(i));
		if (pCurve->m_bIsOutline)
		{
			pCurve->OnSmoothRpFile(); //20161206

			*pCurveNew = *pCurve;
			pCurveNew->PolyInflate(d);
			m_CurveList.InsertAt(0,pCurveNew);
			return 0;
		}
	}
	return 0;
}

void CPiece::OnGetPieceOutLine()
{
	int i;
	CCurve* pCurve;
	for (i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		pCurve->GetCurveMaxMin();
		pCurve->m_bIsOutline = FALSE;
		pCurve->GetCurveStyle(); //是否闭合曲线，置m_bIsPolygon标志
	}

	ReCalcPieceSize();

	for (i=0;i<m_CurveList.GetSize();i++)
	{
		pCurve = m_CurveList.GetAt(i);
		if ( (TRUE == pCurve->m_bIsPolygon)
			&& (CURVE_TYPE_MM == pCurve->m_nCurveType)
			&& (m_nXmax == pCurve->m_nXmax) 
			&& (m_nXmin == pCurve->m_nXmin)
			&& (m_nYmax == pCurve->m_nYmax)
			&& (m_nYmin == pCurve->m_nYmin)
			)
		{
			pCurve->m_bIsOutline = TRUE;
			return;
		}
	}

}

