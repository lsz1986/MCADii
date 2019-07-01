#if !defined(AFX_PIECELISTVIEW_H__448C4306_220C_41C7_A1D1_2D41733B32EF__INCLUDED_)
#define AFX_PIECELISTVIEW_H__448C4306_220C_41C7_A1D1_2D41733B32EF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "MCADView.h"
#endif // _MSC_VER > 1000
// PieceListView.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPieceListView view

class CPieceListView : public CScrollView
{
protected:
	CPieceListView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CPieceListView)

// Attributes
public:

// Operations
public:
	void UpdateDisplay();
	void OnDrawPieceList(CMCADDoc* pDoc,CDC* pDC);
	CMCADView* m_pMainView;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPieceListView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CPieceListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CPieceListView)
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnListClear();
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PIECELISTVIEW_H__448C4306_220C_41C7_A1D1_2D41733B32EF__INCLUDED_)
