// PieceListView.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "PieceListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPieceListView

IMPLEMENT_DYNCREATE(CPieceListView, CScrollView)

CPieceListView::CPieceListView()
{
}

CPieceListView::~CPieceListView()
{
}


BEGIN_MESSAGE_MAP(CPieceListView, CScrollView)
	//{{AFX_MSG_MAP(CPieceListView)
	ON_WM_RBUTTONDOWN()
	ON_COMMAND(IDM_LIST_CLEAR, OnListClear)
	ON_WM_LBUTTONUP()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPieceListView drawing

void CPieceListView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: calculate the total size of this view
	sizeTotal.cx = 82;
	sizeTotal.cy = 2400;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPieceListView::OnDraw(CDC* pDC)
{
	CMCADDoc* pDoc = (CMCADDoc*)GetDocument();
	// TODO: add draw code here
	OnDrawPieceList(pDoc,pDC);
}

/////////////////////////////////////////////////////////////////////////////
// CPieceListView diagnostics

#ifdef _DEBUG
void CPieceListView::AssertValid() const
{
	CScrollView::AssertValid();
}

void CPieceListView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CPieceListView message handlers

void CPieceListView::OnDrawPieceList(CMCADDoc *pDoc,CDC *pDrawDC)
{
	int i;
	int nListSize = pDoc->m_PiecesInList.GetSize();
	for (i=0;i<nListSize;i++)
	{
		CPiece* pPiece;
		pPiece = pDoc->m_PiecesInList.GetAt(i);
		pPiece->DrawPieceInListView(i,pDrawDC);
	}
}

void CPieceListView::UpdateDisplay()
{
	Invalidate();
}

void CPieceListView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	CMenu menu;
	menu.LoadMenu(IDR_MENU_LIST);
	CPoint pt;
	GetCursorPos(&pt);
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);

	CScrollView::OnRButtonDown(nFlags, point);
}

void CPieceListView::OnListClear() 
{
	CMCADDoc* pDoc;
	pDoc = (CMCADDoc*)GetDocument();
	pDoc->BackUp(); //清空列表前
	while (pDoc->m_PiecesInList.GetSize() > 0)
	{
		CPiece* pPiece = pDoc->m_PiecesInList.GetAt(0);
		delete pPiece;
		pDoc->m_PiecesInList.RemoveAt(0);
	}
	Invalidate();
}

void CPieceListView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	int nSelPieceNum;
	int nPieceSum;
	CMCADDoc* pDoc;
	CClientDC dc(this);
	OnPrepareDC(&dc);
	dc.DPtoLP(&point);

	pDoc = (CMCADDoc*)GetDocument();
	pDoc->m_nCurTool = TOOL_SEL_PIECE;
	nPieceSum = pDoc->m_PiecesInList.GetSize();
	
	nSelPieceNum = point.y/(int)PIECE_LIST_RECTSIZE;
	if (nSelPieceNum < nPieceSum)
	{
		pDoc->BackUp(); //列表视图单击之前

		CMCADDoc* pDoc;
		pDoc = (CMCADDoc*)GetDocument();

		CPiece* pPiece;
		pPiece = pDoc->m_PiecesInList.GetAt(nSelPieceNum);
		pDoc->m_PiecesInMain[pDoc->m_nCurPage].Add(pPiece);
		
		pDoc->m_PiecesInList.RemoveAt(nSelPieceNum);
		Invalidate();
		m_pMainView->OnRedraw();
	}
	CScrollView::OnLButtonUp(nFlags, point);
}

BOOL CPieceListView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	// 设置所要求背景色的刷子
	CBrush backBrush(RGB(0, 0, 0));
	// 保存旧刷子
	CBrush* pOldBrush = pDC->SelectObject(&backBrush);
	CRect rect;
	pDC->GetClipBox(&rect);     // 擦除所需的区域
	pDC->PatBlt(rect.left, rect.top, rect.Width(), rect.Height(), PATCOPY);
	pDC->SelectObject(pOldBrush);
	return TRUE;
//	return CScrollView::OnEraseBkgnd(pDC);
}
