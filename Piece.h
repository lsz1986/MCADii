// Piece.h: interface for the CPiece class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PIECE_H__F0B46E0E_6165_4CAE_8716_621645D592EA__INCLUDED_)
#define AFX_PIECE_H__F0B46E0E_6165_4CAE_8716_621645D592EA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Curve.h"

class CPiece : public CObject  
{
public:
	CPiece();
	DECLARE_SERIAL(CPiece)
	virtual ~CPiece();
	virtual void Serialize(CArchive& ar);
public:
	int m_nPieceSelStat; //页面中裁片的新状态
						//0 在页面中，未选中
						//1 在页面中，已选中
						//2 在页面中，跟随鼠标移动
	int m_nCutSequence; //裁剪顺序
	BOOL m_bRefreshDisplay; //需要刷新显示
public:
//	void SmoothOutline();
	void OnGetPieceOutLine(); //寻找裁片的外框
	void OnCheckIfBelowZero();
	void ReCalcPieceSize();
	void OnPieceAddDrill(CPoint ptHpgl);
	void operator=(CPiece &a);
	int AddOutLineType1();
	int AddOutLineType2(double d);
	void PieceMirror();
	void ClearCurveSelState();
	void PieceRotate(double dAngle);
	void OnMove(int deltaX,int deltaY);
	BOOL PtHpglInPiece(CPoint ptHpgl);
	BOOL PtScrInPiece(CPoint ptScr,double dScale);
	BOOL CurveInPiece(CCurve *pCurve);

	void DrawPieceInListView(int piecenum,CDC *pDrawDC);
	void DrawPieceInMCADView(CDC* pDC);

	CTypedPtrArray <CObArray, CCurve *> m_CurveList;
	int m_nXmin;
	int m_nXmax;
	int m_nYmin;
	int m_nYmax;
};

#endif // !defined(AFX_PIECE_H__F0B46E0E_6165_4CAE_8716_621645D592EA__INCLUDED_)
