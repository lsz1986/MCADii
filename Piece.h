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
	int m_nPieceSelStat; //ҳ���в�Ƭ����״̬
						//0 ��ҳ���У�δѡ��
						//1 ��ҳ���У���ѡ��
						//2 ��ҳ���У���������ƶ�
	int m_nCutSequence; //�ü�˳��
	BOOL m_bRefreshDisplay; //��Ҫˢ����ʾ
public:
//	void SmoothOutline();
	void OnGetPieceOutLine(); //Ѱ�Ҳ�Ƭ�����
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
