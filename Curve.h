// Curve.h: interface for the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_)
#define AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CURVE_TYPE_PEN		1 //��
#define CURVE_TYPE_MM		2 //ϳ
#define CURVE_TYPE_DRILL	3 //���

class CCurve : public CObject  
{
public:
	CCurve();
	DECLARE_SERIAL(CCurve)
	virtual ~CCurve();

public:
	CArray <CPoint,CPoint> *m_pointList;

public:
    BOOL m_bIsSelected; //��ѡ����
	BOOL m_bIsPolygon; //�Ƿ������
	BOOL m_bIsOutline; //��ԭ����Ƭ�����

	int m_nCurveType;
	int m_nPenNumInPlt; //��plt�ļ��ж�ȡ�ıʺ�
	int m_nXmin;
	int m_nXmax;
	int m_nYmin;
	int m_nYmax;

	int m_nXSize;
	int m_nYSize;

//	double m_dSlotWidth;
	double m_dxxx;

public:
	void OnToDrillData();
	void OnToMMData();
	void OnToPlotData();

	void OnSmoothRpFile();
	void CurveSmoothMethod1();
	void CurveRemoveShortSec(); //ɾ�����̵��߶�
	void NewCurveSmoothAccordOldCurve(CCurve* pOldCurve); //ɾ�����̵��߶�

	void CurveInflate(double d);
	void OnExpand(int MmBefore,int MmAfter);
	void operator=(CCurve &a);

	BOOL PtInPolygon(CPoint pt);
	CPoint GetCrossPoint(CPoint pt[3] ,double d,BOOL bLeft);
	BOOL PointOnCurve(CPoint point);
	BOOL CurveInRect(CRect rect);
	void OnDrawCurve(CDC* pDrawDC,int nDrawType);
	void CurveMirror(int CenterY);
	void RotateCurve(int CenterX,int CenterY,double dAngle);
	void OnAngleAdjust(double dAngle,int iMaxX);
	void OnCheckCurveIsPolygon();
	void GetCurveStyle();
	void OnMoveCurve(int deltaX,int deltaY);
	void GetCurveMaxMin();
	void CheckIsPieceOutLine();
	void OnRemoveDupPoints();

	void PolyBigger_1(double d);
	void PolyInflate(double d);
	void PolyShrink(double d);

	void OnReverse(); //�ı䷽��
	virtual void Serialize(CArchive& ar);
	void OnSetCurveStart(CPoint ptMouse);
	void OnPointMove(int n);
};

#endif // !defined(AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_)
