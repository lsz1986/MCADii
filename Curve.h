// Curve.h: interface for the CCurve class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_)
#define AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CURVE_TYPE_PEN		1 //笔
#define CURVE_TYPE_MM		2 //铣
#define CURVE_TYPE_DRILL	3 //钻孔

class CCurve : public CObject  
{
public:
	CCurve();
	DECLARE_SERIAL(CCurve)
	virtual ~CCurve();

public:
	CArray <CPoint,CPoint> *m_pointList;

public:
    BOOL m_bIsSelected; //被选中了
	BOOL m_bIsPolygon; //是封闭曲线
	BOOL m_bIsOutline; //是原来裁片的外框

	int m_nCurveType;
	int m_nPenNumInPlt; //从plt文件中读取的笔号
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
	void CurveRemoveShortSec(); //删除过短的线段
	void NewCurveSmoothAccordOldCurve(CCurve* pOldCurve); //删除过短的线段

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

	void OnReverse(); //改变方向
	virtual void Serialize(CArchive& ar);
	void OnSetCurveStart(CPoint ptMouse);
	void OnPointMove(int n);
};

#endif // !defined(AFX_CURVE_H__CB6506A9_3D34_429C_B30F_83B9CC32A154__INCLUDED_)
