// MCADView.h : interface of the CMCADView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCADVIEW_H__7374404A_B1E9_466C_AC32_110E5143215E__INCLUDED_)
#define AFX_MCADVIEW_H__7374404A_B1E9_466C_AC32_110E5143215E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "MCADDoc.h"
#endif // _MSC_VER > 1000


class CMCADView : public CView
{
protected: // create from serialization only
	CMCADView();
	DECLARE_DYNCREATE(CMCADView)

// Attributes
public:
	CMCADDoc* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCADView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL m_bLBDown;
	BOOL m_bRBDown;

	int m_nDrawLineStep; //添加线段进行到哪一个步骤

	CPoint m_ptLBDown; //鼠标左键按下时的坐标(矩形起点)
	CPoint m_ptRBDown; //鼠标右键按下时的坐标
	CPoint m_ptMouseMove;

	CPoint m_ptAddLineFirstP; //画线第一个点
	CPoint m_ptHpglPMLastP;

public:
	int m_nNextCutSequence;
	void OnMMoveAddRect(CPoint point);
	void OnMMoveSelCurve(CPoint point);
	void OnMMoveSelPiece(CPoint point);
	void OnMMoveDrawLineCurve(CPoint point);
	void OnMMoveRbDown(CPoint point);
	void OnCalcDispProportion();
	void OnLBtnDownSetCutSeq(CPoint point);
	void OnLBtnDownAddLine(CPoint point);
	void OnLbtnDownAddText(CPoint point);
	void OnLBtnDownAddCurve(CPoint point);
	void OnLBtnDownAddRect(CPoint point);
	void OnLBtnDownSelCurve(CPoint point);
	void OnLBtnDownSelPiece(CPoint point);
	void OnLbtnDownAddDrill(CPoint point);

	void OnLBtnUpSelCurve(CPoint point);
	void OnLBtnUpDrawRect(CPoint point);

	void OnBreakCurveByAngle();
	CPoint FindNearestHpglpt(CPoint point);

	void OnRedraw();
	CPiece* GetMovingPiece();
	CPiece* GetSelPiece();
	CPiece* GetPieceClicked(CPoint); //寻找被点击的裁片

	void UpdatePieceDisplay(CDC *pDrawDC); //哪个裁片状态发送改变就重绘

	void MCADViewZoomAll();
	void MCADViewZoomIn(CPoint ptMouse);
	void MCADViewZoomOut(CPoint ptMouse);
	void OnDrawPage(CDC* pDrawDC );
	virtual ~CMCADView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMCADView)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnPieceMakeACopy();
	afx_msg void OnPieceMirror();
	afx_msg void OnPieceRemove();
	afx_msg void OnPieceRot90ccw();
	afx_msg void OnClear();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnSelToList();
	afx_msg void OnOutlineType1();
	afx_msg void OnOutlineType2();
	afx_msg void OnPieceRot1ccw();
	afx_msg void OnPieceRot1cw();
	afx_msg void OnCurveToSlot();
	afx_msg void OnCurveToMm();
	afx_msg void OnCurveToPen();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnCurveRemove();
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnCurveReverse();
	afx_msg void OnViewPolyInflate();
	afx_msg void OnPolyShrink();
	afx_msg void OnCurveConnect();
	afx_msg void OnCurveBreak();
	afx_msg void OnCurveLonger();
	afx_msg void OnForceToPoly();
	afx_msg void OnUpdateCurveBreak(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurveConnect(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurveExpand(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurveRemove(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurveReverse(CCmdUI* pCmdUI);
	afx_msg void OnUpdateCurveToSlot(CCmdUI* pCmdUI);
	afx_msg void OnUpdateForceToPoly(CCmdUI* pCmdUI);
	afx_msg void OnUpdateViewPolyInflate(CCmdUI* pCmdUI);
	afx_msg void OnUpdatePolyShrink(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToMm(CCmdUI* pCmdUI);
	afx_msg void OnUpdateToPen(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddoutline1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateAddoutline2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMakeacopy(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMirror(CCmdUI* pCmdUI);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnUpdateAddDrill(CCmdUI* pCmdUI);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPieceRot90cw();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnAllTolist();
	afx_msg void OnFindOutline();
	afx_msg void OnLineToDot();
	afx_msg void OnCopyMirror();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnAddText(void);
	void OnDemoRunDrawLine(int iDrawType);
	afx_msg void OnSetCurveStart();
};

#ifndef _DEBUG  // debug version in MCADView.cpp
inline CMCADDoc* CMCADView::GetDocument()
   { return (CMCADDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCADVIEW_H__7374404A_B1E9_466C_AC32_110E5143215E__INCLUDED_)
