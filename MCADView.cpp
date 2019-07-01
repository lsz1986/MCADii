// MCADView.cpp : implementation of the CMCADView class
//

#include "stdafx.h"
#include "MCAD.h"
#include "MainFrm.h"
#include "MCADDoc.h"
#include "MCADView.h"
#include "DlgGetInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCADView

IMPLEMENT_DYNCREATE(CMCADView, CView)

BEGIN_MESSAGE_MAP(CMCADView, CView)
	//{{AFX_MSG_MAP(CMCADView)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_COMMAND(IDMP_MAKEACOPY, OnPieceMakeACopy)
	ON_COMMAND(IDMP_MIRROR, OnPieceMirror)
	ON_COMMAND(IDMP_REMOVE, OnPieceRemove)
	ON_COMMAND(IDMP_90CCW, OnPieceRot90ccw)
	ON_COMMAND(IDMP_REMOVE_ALL, OnClear)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(IDMP_SEL_TOLIST, OnSelToList)
	ON_COMMAND(IDMP_ADDOUTLINE1, OnOutlineType1)
	ON_COMMAND(IDMP_ADDOUTLINE2, OnOutlineType2)
	ON_COMMAND(IDMP_01CCW, OnPieceRot1ccw)
	ON_COMMAND(IDMP_01CW, OnPieceRot1cw)
	ON_COMMAND(IDML_CURVE_TO_SLOT, OnCurveToSlot)
	ON_COMMAND(IDML_TO_MM, OnCurveToMm)
	ON_COMMAND(IDML_TO_PEN, OnCurveToPen)
	ON_WM_LBUTTONUP()
	ON_COMMAND(IDML_CURVE_REMOVE, OnCurveRemove)
	ON_WM_KEYUP()
	ON_COMMAND(IDML_CURVE_REVERSE, OnCurveReverse)
	ON_COMMAND(IDML_POLY_INFLATE, OnViewPolyInflate)
	ON_COMMAND(IDML_POLY_SHRINK, OnPolyShrink)
	ON_COMMAND(IDML_CURVE_MERGE, OnCurveConnect)
	ON_COMMAND(IDML_BK_BY_ANGLE, OnCurveBreak)
	ON_COMMAND(IDML_CURVE_EXPAND, OnCurveLonger)
	ON_COMMAND(IDML_FORCE_TO_POLY, OnForceToPoly)
	ON_UPDATE_COMMAND_UI(IDML_BK_BY_ANGLE, OnUpdateCurveBreak)
	ON_UPDATE_COMMAND_UI(IDML_CURVE_MERGE, OnUpdateCurveConnect)
	ON_UPDATE_COMMAND_UI(IDML_CURVE_EXPAND, OnUpdateCurveExpand)
	ON_UPDATE_COMMAND_UI(IDML_CURVE_REMOVE, OnUpdateCurveRemove)
	ON_UPDATE_COMMAND_UI(IDML_CURVE_REVERSE, OnUpdateCurveReverse)
	ON_UPDATE_COMMAND_UI(IDML_CURVE_TO_SLOT, OnUpdateCurveToSlot)
	ON_UPDATE_COMMAND_UI(IDML_FORCE_TO_POLY, OnUpdateForceToPoly)
	ON_UPDATE_COMMAND_UI(IDML_POLY_INFLATE, OnUpdateViewPolyInflate)
	ON_UPDATE_COMMAND_UI(IDML_POLY_SHRINK, OnUpdatePolyShrink)
	ON_UPDATE_COMMAND_UI(IDML_TO_MM, OnUpdateToMm)
	ON_UPDATE_COMMAND_UI(IDML_TO_PEN, OnUpdateToPen)
	ON_UPDATE_COMMAND_UI(IDMP_ADDOUTLINE1, OnUpdateAddoutline1)
	ON_UPDATE_COMMAND_UI(IDMP_ADDOUTLINE2, OnUpdateAddoutline2)
	ON_UPDATE_COMMAND_UI(IDMP_MAKEACOPY, OnUpdateMakeacopy)
	ON_UPDATE_COMMAND_UI(IDMP_MIRROR, OnUpdateMirror)
	ON_WM_RBUTTONDOWN()
	ON_UPDATE_COMMAND_UI(IDTBS_ADRILL, OnUpdateAddDrill)
	ON_WM_RBUTTONUP()
	ON_COMMAND(IDMP_90CW, OnPieceRot90cw)
	ON_WM_SIZE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(IDMP_ALL_TOLIST, OnAllTolist)
	ON_COMMAND(IDML_FIND_OUTLINE, OnFindOutline)
	ON_COMMAND(IDML_LINE_TO_DOT, OnLineToDot)
	ON_COMMAND(IDMP_COPY_MIRROR, OnCopyMirror)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(IDML_SET_CURVE_START, &CMCADView::OnSetCurveStart)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCADView construction/destruction

CMCADView::CMCADView()
{
	m_bLBDown = FALSE;
	m_bRBDown = FALSE;
	m_nDrawLineStep = 0;
}

CMCADView::~CMCADView()
{
}

BOOL CMCADView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CMCADView drawing

void CMCADView::OnDraw(CDC* pDC)
{
	OnRedraw();
}

/////////////////////////////////////////////////////////////////////////////
// CMCADView printing

BOOL CMCADView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMCADView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMCADView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

/////////////////////////////////////////////////////////////////////////////
// CMCADView diagnostics

#ifdef _DEBUG
void CMCADView::AssertValid() const
{
	CView::AssertValid();
}

void CMCADView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMCADDoc* CMCADView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMCADDoc)));
	return (CMCADDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCADView message handlers
void CMCADView::OnDrawPage(CDC *pDrawDC)
{
	CRect rect;
	CBrush brushNew,*pBrushOld;
	brushNew.CreateSolidBrush(RGB(0,0,0));
	pBrushOld = pDrawDC->SelectObject(&brushNew);
	GetWindowRect(rect);
	pDrawDC->Rectangle(0,0,rect.Width(),rect.Height());
	brushNew.DeleteObject();

 	int i;
 	int nGridDist;
 	CPen penNew,*pPenOld; //
 	CString strItem;
 	strItem.Format("第%d页(%dmm*%dmm)", (GetDocument()->m_nCurPage+1), gSet.getDispSizeX(), gSet.getDispSizeY());
 
 	pDrawDC->SetBkMode(TRANSPARENT);
 	pDrawDC->SetTextColor(RGB(0,255,0));

 	CPoint ptText;
 	ptText = ptHpglToScr(CPoint(0,(int)(gSet.getDispSizeY()*40.0) ));
 	pDrawDC->TextOut(ptText.x, ptText.y-18, strItem);
 
 	penNew.CreatePen(LT_SOLID,0,RGB(0,0xff,0)); //绿色外框
 	pPenOld = pDrawDC->SelectObject(&penNew);
 
	CPoint pt1,pt2;
	pt1 = ptHpglToScr(CPoint(0,0));
	pt2 = ptHpglToScr(CPoint((int)(gSet.getDispSizeX()*40.0 ),(int)(gSet.getDispSizeY()*40.0 )));
	
	pDrawDC->Rectangle((pt1.x-4),(pt1.y+4),(pt2.x+4),(pt2.y-4));
	pDrawDC->SetTextColor(RGB(255,255,255));

	CPoint ptCursor;
	if (gSet.getDispScale() < 0.005)
	{
		nGridDist = 200;
	}
	else if (gSet.getDispScale() < 0.01)
	{
		nGridDist = 100;
	}
	else if (gSet.getDispScale() < 0.02)
	{
		nGridDist = 50;
	}
	else if (gSet.getDispScale() < 0.05)
	{
		nGridDist = 20;
	}
	else if (gSet.getDispScale() < 0.1)
	{
		nGridDist = 10;
	}
	else
	{
		nGridDist = 5;
	}
	int XLineNum = gSet.getDispSizeX()/nGridDist;
	int YLineNum = gSet.getDispSizeY()/nGridDist;

	for (i=0; i <= YLineNum; i++)
	{
		ptCursor = ptHpglToScr(CPoint(0,i*40*nGridDist));
		strItem.Format("%03d",i*nGridDist);
		pDrawDC->TextOut(ptCursor.x-36, ptCursor.y-8, strItem);
	}

	for (i=0;i <= (XLineNum/2);i++)
	{
		ptCursor = ptHpglToScr(CPoint(i*80*nGridDist,0));
		strItem.Format("%03d",i*2*nGridDist);
		pDrawDC->TextOut(ptCursor.x-12, ptCursor.y+6, strItem);
	}
	pDrawDC->SelectObject(pPenOld);
	penNew.DeleteObject();

 	penNew.CreatePen(LT_SOLID,0,RGB(20,60,60)); //灰色网格
 	pPenOld = pDrawDC->SelectObject(&penNew);
 
	for (i=1;i<YLineNum;i++)
	{
		ptCursor = ptHpglToScr(CPoint(0,i*40*nGridDist));
		pDrawDC->MoveTo(ptCursor.x, ptCursor.y);
		ptCursor = ptHpglToScr(CPoint(gSet.getDispSizeX()*40,i*40*nGridDist));
		pDrawDC->LineTo(ptCursor.x, ptCursor.y);
	}
	for (i=1;i<XLineNum;i++)
	{
		ptCursor = ptHpglToScr(CPoint(i*40*nGridDist,0));
		pDrawDC->MoveTo(ptCursor.x, ptCursor.y);
		ptCursor = ptHpglToScr(CPoint(i*40*nGridDist,gSet.getDispSizeY()*40));
		pDrawDC->LineTo(ptCursor.x, ptCursor.y);
	}

 	pDrawDC->SelectObject(pPenOld);
 	penNew.DeleteObject();
 
 	CMCADDoc* pDoc = GetDocument();
 	int nCurveSum;
 	CPiece* pPiece;
	CFont font;
	font.CreatePointFont(160,_T("Arial")); //参数含义1，字体大小，2字体类型，3字体颜色
	CFont* pOldFont = pDrawDC->SelectObject(&font);
	pDrawDC->SetTextColor(RGB(255,255,50));
 
 	nCurveSum = pDoc->m_PiecesInMain[pDoc->m_nCurPage].GetSize();
 	for(i=0;i<nCurveSum;i++)
 	{
 		pPiece=pDoc->m_PiecesInMain[pDoc->m_nCurPage].GetAt(i);
		pPiece->DrawPieceInMCADView(pDrawDC);

		if ( TOOL_SET_CUT_SEQ == GetDocument()->m_nCurTool)
		{
			CPoint ptCenter;
			ptCenter.x = (pPiece->m_nXmax+pPiece->m_nXmin)/2;
			ptCenter.y = (pPiece->m_nYmax+pPiece->m_nYmin)/2;
			ptCenter = ptHpglToScr(ptCenter);
			
			CString str;
			str.Format("%02d",(pPiece->m_nCutSequence+1));
			pDrawDC->TextOut(ptCenter.x,ptCenter.y,str);
		}
	}
	pDrawDC->SelectObject(pOldFont);
	font.DeleteObject();

	int FreeCurveSum = pDoc->m_CurveInMain[pDoc->m_nCurPage].GetSize();
	CCurve* pCurve;
	for (i=0;i<FreeCurveSum;i++)
	{
		pCurve =  pDoc->m_CurveInMain[pDoc->m_nCurPage].GetAt(i);
		pCurve->OnDrawCurve(pDrawDC,0);
	}
}

void CMCADView::MCADViewZoomAll() 
{
	OnCalcDispProportion();
	OnRedraw();
}

void CMCADView::MCADViewZoomIn(CPoint ptMouse)
{
	CPoint ptMouseNew;
	CPoint ptHpgl;
	int deltaX,deltaY;

// 	CClientDC dc(this);
// 	dc.DPtoLP(&ptMouse);

	ptHpgl = ptScrToHpgl(ptMouse);
	gSet.setDispScale(gSet.getDispScale()*1.25);
	ptMouseNew = ptHpglToScr(ptHpgl);

//	dc.DPtoLP(ptMouse);
//	dc.LPtoDP(&ptMouseNew);

	deltaX = ptMouseNew.x-ptMouse.x;
	deltaY = ptMouseNew.y-ptMouse.y;

	gSet.setDispStartX(gSet.getDispStartX()+deltaX);
	gSet.setDispStartY(gSet.getDispStartY()+deltaY);
	OnRedraw();
}

void CMCADView::MCADViewZoomOut(CPoint ptMouse) 
{
	CPoint ptMouseNew;
	CPoint ptHpgl;
	int deltaX,deltaY;

// 	CClientDC dc(this);
// 	dc.DPtoLP(&ptMouse);

	ptHpgl = ptScrToHpgl(ptMouse);
	gSet.setDispScale(gSet.getDispScale()*0.8);
	ptMouseNew = ptHpglToScr(ptHpgl);
	deltaX = ptMouseNew.x-ptMouse.x;
	deltaY = ptMouseNew.y-ptMouse.y;
	
	gSet.setDispStartX(gSet.getDispStartX()+deltaX);
	gSet.setDispStartY(gSet.getDispStartY()+deltaY);
	OnRedraw();
}

CPiece* CMCADView::GetPieceClicked(CPoint point)
{
	CPiece *pPiece;
	int nPieceSum;
	int i;
	nPieceSum = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetSize();
	
	CClientDC dc(this);

	for (i=0;i<nPieceSum;i++)
	{
		pPiece = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetAt(i);
		if(pPiece->PtScrInPiece(point,gSet.getDispScale()))
		{
			return pPiece;
		}
		if (pPiece->m_nPieceSelStat == 2)
		{
			return pPiece;
		}
	}
	return NULL;
}

CPiece* CMCADView::GetMovingPiece()
{
	CPiece *pPiece;
	int nPieceSum = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetSize();
	for (int i=0;i<nPieceSum;i++)
	{
		pPiece = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetAt(i);
		if (2 == pPiece->m_nPieceSelStat )
		{
			return pPiece;
		}
	}
	return NULL;
}

CPiece* CMCADView::GetSelPiece()
{
	CPiece *pPiece;
	int nPieceSum = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetSize();
	for (int i=0;i<nPieceSum;i++)
	{
		pPiece = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetAt(i);
		if (1 == pPiece->m_nPieceSelStat )
		{
			return pPiece;
		}
	}
	return NULL;
}

void CMCADView::UpdatePieceDisplay(CDC *pDrawDC)
{
	int i;
	CMCADDoc* pDoc = GetDocument();
	int nCurveSum;
	CPiece* pPiece;
	
	nCurveSum = pDoc->m_PiecesInMain[pDoc->m_nCurPage].GetSize();
	for(i=0;i<nCurveSum;i++)
	{
		pPiece=pDoc->m_PiecesInMain[pDoc->m_nCurPage].GetAt(i);
		if ( pPiece->m_bRefreshDisplay ) //状态发生了改变
		{
			pPiece->DrawPieceInMCADView(pDrawDC);
			pPiece->m_bRefreshDisplay = FALSE;
		}
	}
}

#define COPY_DELTA_X 12000
#define COPY_DELTA_Y 0
void CMCADView::OnPieceMakeACopy() //复制裁片
{
	CPiece* pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		GetDocument()->BackUp();//复制裁片之前

		CPiece* pNewPiece = new CPiece;
		*pNewPiece = *pPiece;
		pPiece->m_nPieceSelStat = 1;

		pNewPiece->OnMove(COPY_DELTA_X,COPY_DELTA_Y);
		pNewPiece->ReCalcPieceSize();
		pNewPiece->m_nPieceSelStat = 1;
		GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].Add(pNewPiece);
	}
	OnRedraw();
}

void CMCADView::OnPieceMirror() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		GetDocument()->BackUp();  //裁片镜像之前
		pPiece->PieceMirror();
		pPiece->ReCalcPieceSize();
		OnRedraw();
	}
}

void CMCADView::OnCopyMirror() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		GetDocument()->BackUp();  //裁片镜像之前
		CPiece* pNewPiece = new CPiece;
		*pNewPiece = *pPiece;
		pPiece->m_nPieceSelStat = 1;
		
		pNewPiece->OnMove(COPY_DELTA_X,COPY_DELTA_Y);
		pNewPiece->ReCalcPieceSize();
		pNewPiece->m_nPieceSelStat = 1;
		GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].Add(pNewPiece);

		pNewPiece->PieceMirror();
		pNewPiece->ReCalcPieceSize();
		OnRedraw();
	}	
}

void CMCADView::OnPieceRemove() 
{
	GetDocument()->BackUp(); //删除裁片之前
 	GetDocument()->SelPieceRemove();
 	OnRedraw();
}

void CMCADView::OnClear() 
{
	while (GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetSize() > 0)
	{
		CPiece* pPiece = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetAt(0);
		delete pPiece;
		GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].RemoveAt(0);
	}
	OnRedraw();
}

void CMCADView::OnSelToList() 
{
	GetDocument()->BackUp(); //返回列表之前
	GetDocument()->SelPieceToList();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_pListView->UpdateDisplay();
	OnRedraw();
}

void CMCADView::OnAllTolist() 
{
	GetDocument()->BackUp(); //全部返回列表之前
	GetDocument()->AllPieceToList();
	CMainFrame* pMainFrame = (CMainFrame*)AfxGetMainWnd();
	pMainFrame->m_pListView->UpdateDisplay();
	OnRedraw();	
}

void CMCADView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPiece *pPiece;
	CDC* pDC = GetDC();
	if (TOOL_SEL_PIECE == GetDocument()->m_nCurTool ) //当前为选择裁片模式
	{
		pPiece = GetPieceClicked(point);
		GetDocument()->OnResetPieceSelState();
		if (pPiece != NULL)
		{
			pPiece->m_nPieceSelStat = 2;
			OnRedraw();
			pPiece->m_bRefreshDisplay = TRUE;
			m_ptHpglPMLastP = ptScrToHpgl(point);
			UpdatePieceDisplay(pDC);
			pPiece->m_bRefreshDisplay = TRUE;
		}
		UpdatePieceDisplay(pDC);
	}
	CView::OnLButtonDblClk(nFlags, point);
}

void CMCADView::OnOutlineType1()
{
	GetDocument()->BackUp(); //增加外框1之前
	GetDocument()->OnAddOutLine1();
	OnRedraw();
}

void CMCADView::OnOutlineType2() 
{
	GetDocument()->BackUp(); //增加外框2之前
	GetDocument()->OnAddOutLine2();
	OnRedraw();
}

void CMCADView::OnCurveRemove() 
{
	GetDocument()->BackUp(); //删除曲线之前
	GetDocument()->OnSelCurveDel();
	OnRedraw();
}

void CMCADView::OnCurveToPen() 
{
	GetDocument()->BackUp(); //变笔之前
	GetDocument()->OnSelCurveToPen();
	GetDocument()->OnClearCurveSelState();
	OnRedraw();
}

void CMCADView::OnCurveToMm() 
{
	GetDocument()->BackUp(); //变铣之前
	GetDocument()->OnSelCurveToMM();
	GetDocument()->OnClearCurveSelState();
	OnRedraw();
}

void CMCADView::OnCurveReverse() 
{
	GetDocument()->BackUp(); //反向之前
	GetDocument()->OnSelCurveReverse();
	OnRedraw();
}

#include "DlgPropSlot.h"
void CMCADView::OnCurveToSlot() 
{
	CDlgPropSlot dlg;
	double dInflateDist;

	dlg.m_dSlotWidth = gSet.getSlotWidth();
	if (IDOK == dlg.DoModal())
	{
		GetDocument()->BackUp(); //开槽之前
		gSet.setSlotWidth(dlg.m_dSlotWidth);

		dInflateDist = gSet.getSlotWidth() - gSet.getDrillerDimension()*0.01;
		if (dInflateDist > 0.19)//槽宽和钻头之差大于0.2mm
		{
			GetDocument()->OnSelCurveToSlot(dInflateDist);
		}
		OnRedraw();
	}
}

void CMCADView::OnViewPolyInflate() 
{
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();

	if (NULL == pCurve)
	{
		return;
	}
	pCurve->GetCurveStyle();
	if(FALSE == pCurve->m_bIsPolygon)
	{
		AfxMessageBox("操作失败：\n请选择封闭曲线");
		return;
	}
	CDlgGetInput dlg;
	dlg.m_dInput = gSet.getInflateDist();

	dlg.m_strTips.Format("输入外扩距离(mm):");
	if (IDOK == dlg.DoModal())
	{
		gSet.setInflateDist(dlg.m_dInput);

		GetDocument()->BackUp(); //外扩之前
		pCurve->OnSmoothRpFile();
		pCurve->PolyInflate(dlg.m_dInput);
		OnRedraw();
	}
}

void CMCADView::OnSetCurveStart()
{
	// TODO: 在此添加命令处理程序代码
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();
	if (NULL == pCurve)
	{
		return;
	}
	pCurve->GetCurveStyle();
	if (FALSE == pCurve->m_bIsPolygon)
	{
		AfxMessageBox("操作失败：\n请选择封闭曲线");
		return;
	}
	pCurve->OnSetCurveStart(m_ptRBDown);
	OnRedraw();
}

void CMCADView::OnPolyShrink() 
{
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();
	if (NULL == pCurve)
	{
		return;
	}
	pCurve->GetCurveStyle();

	if(FALSE == pCurve->m_bIsPolygon)
	{
		AfxMessageBox("操作失败：\n请选择封闭曲线");
		return;
	}
	CDlgGetInput dlg;
//	dlg.m_dInput = gSet.getDrillerDimension()*0.005;
	dlg.m_dInput = gSet.getShrinkDist();

	dlg.m_strTips.Format("输入收缩距离(mm):");
	if (IDOK == dlg.DoModal())
	{
		gSet.setShrinkDist(dlg.m_dInput);
		GetDocument()->BackUp(); //收缩之前
		pCurve->OnSmoothRpFile();
		pCurve->GetCurveMaxMin();
		pCurve->PolyShrink(dlg.m_dInput);
		pCurve->CurveSmoothMethod1();
		pCurve->GetCurveMaxMin();
		OnRedraw();
	}
}

void CMCADView::OnCurveConnect()
{
	int i;
	GetDocument()->BackUp(); //首尾相连之前
	for (i=0;i<20;i++)
	{
		GetDocument()->OnSelCurveCombine12();
		GetDocument()->OnSelCurveCombine23();
	}
	OnRedraw();
}

#define MAX_BREAK_SEC_NUM 200
void CMCADView::OnCurveBreak() 
{
	CDlgGetInput dlg;
	dlg.m_dInput = gSet.getBreakAngle();
	dlg.m_strTips.Format("曲线分割角度(1~90度):");
	if (IDOK == dlg.DoModal())
	{
		gSet.setBreakAngle((int)(dlg.m_dInput));
		OnBreakCurveByAngle();
	}
}

void CMCADView::OnBreakCurveByAngle()
{
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();

	if (NULL == pCurve)
	{
		return;
	}
	CCurve* pCurveNew = new CCurve;
	*pCurveNew = *pCurve;

	int nCurveSize = pCurve->m_pointList->GetSize();
	int i,j;
	CPoint pt0,pt1,pt2;
	double dDeltaAngle,dAngle1,dAngle2;
	double absDeltaAnge;
	int nJJ[MAX_BREAK_SEC_NUM];
	for (j=0;j<MAX_BREAK_SEC_NUM;j++)
	{
		nJJ[j] = 0;
	}
	i=0;
	j=0;

	for (i=1;i<nCurveSize-1;i++)
	{
		pt0 = pCurve->m_pointList->GetAt(i-1);
		pt1 = pCurve->m_pointList->GetAt(i);
		pt2 = pCurve->m_pointList->GetAt(i+1);
		dAngle1 = atan2( (double)(pt1.y-pt0.y) , (double)(pt1.x-pt0.x) );
		dAngle2 = atan2( (double)(pt2.y-pt1.y) , (double)(pt2.x-pt1.x) );

		dDeltaAngle = dAngle2-dAngle1;
		if (dDeltaAngle>PI)
		{
			dDeltaAngle = dDeltaAngle - 2.0*PI;
		}
		else if (dDeltaAngle <= -PI)
		{
			dDeltaAngle = dDeltaAngle + 2.0*PI;
		}
		
		absDeltaAnge = fabs(dDeltaAngle);
		if ( absDeltaAnge > gSet.getBreakAngle() * PI /180.0)
		{
			nJJ[j++] = i; //记录需要打断的位置
		}
		if (j >= (MAX_BREAK_SEC_NUM-1))
		{
			break;
		}
	}

	if (nJJ[0] == 0)
	{
		return;
	}
	GetDocument()->BackUp(); //曲线合并之前

	while (pCurve->m_pointList->GetSize() > nJJ[0]+1)
	{
		pCurve->m_pointList->RemoveAt(nJJ[0]+1);
	}
	pCurve->OnCheckCurveIsPolygon();
	pCurve->GetCurveMaxMin();

	for (j=1;j<MAX_BREAK_SEC_NUM;j++)
	{
		if (0 == nJJ[j])
		{
			break;
		}
		pCurve = new CCurve;
		*pCurve = *pCurveNew;
		while (pCurve->m_pointList->GetSize() > nJJ[j]+1)
		{
			pCurve->m_pointList->RemoveAt(nJJ[j]+1);
		}
		for (i=0;i<nJJ[j-1];i++)
		{
			pCurve->m_pointList->RemoveAt(0);
		}
		pCurve->GetCurveMaxMin();
		pCurve->OnCheckCurveIsPolygon();
		GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
	}

	pCurve = new CCurve;
	*pCurve = *pCurveNew;
	for (i=0;i<nJJ[j-1];i++)
	{
		pCurve->m_pointList->RemoveAt(0);
	}
	pCurve->GetCurveMaxMin();
	pCurve->OnCheckCurveIsPolygon();
	GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
	OnRedraw();
}

#include "DlgPropLineExpand.h"
void CMCADView::OnCurveLonger() 
{
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();
	if (NULL != pCurve)
	{
		CDlgPropLineExpand dlg;
		dlg.m_nExpandBefore = gSet.getExpandBefore();
		dlg.m_nExpandAfter = gSet.getExpandBefore();
		if (IDOK == dlg.DoModal())
		{
			gSet.setExpandBefore(dlg.m_nExpandBefore);
			gSet.setExpandAfter(dlg.m_nExpandAfter);
			
			while(pCurve != NULL)
			{
				pCurve->OnExpand(dlg.m_nExpandBefore,dlg.m_nExpandAfter);
				pCurve->m_bIsSelected = FALSE;
				pCurve = GetDocument()->GetSelCurve();
			}

			GetDocument()->BackUp(); //曲线延伸之后
			OnRedraw();
		}
	}
}

void CMCADView::OnRedraw()
{
	CClientDC dc(this);
	CBitmap *Oldmap;
	CBitmap Bitmap;
	CRect rect;
	CDC memDC;

	GetWindowRect(rect);
	Bitmap.CreateCompatibleBitmap(&dc, rect.Width(), rect.Height());
	memDC.CreateCompatibleDC(&dc);
	Oldmap = memDC.SelectObject(&Bitmap);
 	
	CRect rectClient;
	GetClientRect(rectClient);
	dc.DPtoLP(&rectClient);
	memDC.SetMapMode(dc.GetMapMode());
	memDC.PatBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), WHITENESS);

	OnDrawPage(&memDC);

	dc.BitBlt(rectClient.left, rectClient.top, rectClient.Width(), rectClient.Height(), &memDC,
	rectClient.left, rectClient.top, SRCCOPY);
	memDC.DeleteDC();
}

void CMCADView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	m_bRBDown = TRUE;
	m_ptRBDown = point;
	m_ptMouseMove = point;
// 	if (TOOL_SET_CUT_SEQ == GetDocument()->m_nCurTool)
// 	{
// 		GetDocument()->m_nCurTool = TOOL_SEL_PIECE;
// 		OnRedraw();
// 	}
	CView::OnRButtonDown(nFlags, point);
}

void CMCADView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	m_bRBDown = FALSE;
	if( (GetDocument()->m_nCurTool == TOOL_DRAW_LINE)
		||(GetDocument()->m_nCurTool == TOOL_DRAW_CURVE)
		||(GetDocument()->m_nCurTool == TOOL_DRAW_RECT)
		)
	{
		m_nDrawLineStep = 0;
		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		CView::OnRButtonDown(nFlags, point);
		return;
	}

	int deltax,deltay;
	deltax = abs(point.x-m_ptRBDown.x);
	deltay = abs(point.y-m_ptRBDown.y);
	if( (deltax > 5) || (deltay > 5) )
	{
		CView::OnRButtonDown(nFlags, point);
		return;
	}
	CMenu menu;
	if (TOOL_SEL_PIECE == GetDocument()->m_nCurTool)
	{
		CPiece *pPiece;
		pPiece = GetPieceClicked(point);
		GetDocument()->OnResetPieceSelState();
		if (pPiece != NULL)
		{
			pPiece->m_nPieceSelStat = 1;
			pPiece->m_bRefreshDisplay = TRUE;
		}
		
		CPoint pt;
		CDC* pDC = GetDC();

		UpdatePieceDisplay(pDC); //更新裁片在视图中的显示，哪个状态改变了就重绘哪个
		menu.LoadMenu(IDR_MENU_PIECE);
		GetCursorPos(&pt);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);
		CView::OnRButtonDown(nFlags, point);
		return;
	}
	
	if (TOOL_SEL_CURVE == GetDocument()->m_nCurTool)
	{
		CPoint pt;
		menu.LoadMenu(IDR_MENU_LINE);
		GetCursorPos(&pt);
		menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, pt.x, pt.y, this);
	}
	CView::OnRButtonUp(nFlags, point);
}

void CMCADView::OnForceToPoly() 
{
	GetDocument()->OnSelCurveToPoly();
	GetDocument()->BackUp();
	OnRedraw();
}

void CMCADView::OnUpdateToPen(CCmdUI* pCmdUI) 
{
	if(TOOL_SEL_CURVE == GetDocument()->m_nCurTool)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateToMm(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateCurveToSlot(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool)  ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateCurveConnect(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateCurveReverse(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateViewPolyInflate(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdatePolyShrink(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateCurveExpand(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateCurveRemove(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}


void CMCADView::OnUpdateCurveBreak(CCmdUI* pCmdUI) 
{
	if ( (TOOL_SEL_CURVE == GetDocument()->m_nCurTool) ) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateForceToPoly(CCmdUI* pCmdUI) 
{
	if (TOOL_SEL_CURVE == GetDocument()->m_nCurTool)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateMakeacopy(CCmdUI* pCmdUI) 
{
	if(TOOL_SEL_PIECE == GetDocument()->m_nCurTool) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateMirror(CCmdUI* pCmdUI) 
{
	if(TOOL_SEL_PIECE == GetDocument()->m_nCurTool) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateAddoutline1(CCmdUI* pCmdUI) 
{
	if(TOOL_SEL_PIECE == GetDocument()->m_nCurTool) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateAddoutline2(CCmdUI* pCmdUI) 
{
	if(TOOL_SEL_PIECE == GetDocument()->m_nCurTool) {
		pCmdUI->Enable(TRUE);
	}else{
		pCmdUI->Enable(FALSE);
	}
}

void CMCADView::OnUpdateAddDrill(CCmdUI* pCmdUI) 
{
	if (TOOL_ADD_DRILL == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

CPoint CMCADView::FindNearestHpglpt(CPoint ptScr)
{
	CCurve* pCurve;
	CMCADDoc* pDoc = GetDocument();
	int i;
	int nCurveSum = pDoc->m_CurveInMain[pDoc->m_nCurPage].GetSize();
	CPoint ptHpgl;

	for (i=0;i<nCurveSum;i++)
	{
		pCurve = pDoc->m_CurveInMain[pDoc->m_nCurPage].GetAt(i);
		if (pCurve->m_bIsPolygon)
		{
			continue;
		}
		int nCurveSize = pCurve->m_pointList->GetSize();
		if (nCurveSize <= 1)
		{
			continue;
		}


		ptHpgl = pCurve->m_pointList->GetAt(0);
		if(getdistance(ptHpglToScr(ptHpgl),ptScr) < 12.0)
		{
			return ptHpgl;
		}

		ptHpgl = pCurve->m_pointList->GetAt(nCurveSize-1);
		if(getdistance(ptHpglToScr(ptHpgl),ptScr) < 12.0)
		{
			return ptHpgl;
		}
	}
	ptHpgl.x= -10;
	ptHpgl.y= -10;
	return ptHpgl;
}

void CMCADView::OnPieceRot1ccw() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		pPiece->PieceRotate(2.0*PI-PI/180.0);
		pPiece->ReCalcPieceSize();
		GetDocument()->BackUp(); //旋转裁片之后
		OnRedraw();
	}
}

void CMCADView::OnPieceRot1cw() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		pPiece->PieceRotate(PI/180.0);
		pPiece->ReCalcPieceSize();
		GetDocument()->BackUp(); //旋转裁片之后
		OnRedraw();
	}
}

void CMCADView::OnPieceRot90cw() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		pPiece->PieceRotate(PI*0.50);
		pPiece->ReCalcPieceSize();
		GetDocument()->BackUp(); //裁片旋转后
		OnRedraw();
	}
}

void CMCADView::OnPieceRot90ccw() 
{
	CPiece *pPiece = GetSelPiece();
	if (pPiece != NULL)
	{
		pPiece->PieceRotate(PI*1.50);
		pPiece->ReCalcPieceSize();
		GetDocument()->BackUp(); //裁片旋转后
		OnRedraw();
	}
}

void CMCADView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	OnCalcDispProportion();
}

void CMCADView::OnLButtonDown(UINT nFlags, CPoint point) //鼠标左键单击
{
	gSet.setDemoEndFlag(TRUE);
	switch(GetDocument()->m_nCurTool)
	{
		case TOOL_SET_CUT_SEQ:
		OnLBtnDownSetCutSeq(point);
		break;

		case TOOL_DRAW_LINE:	//当前为画线段模式
		OnLBtnDownAddLine(point);
		break;

		case TOOL_DRAW_CURVE:
		OnLBtnDownAddCurve(point);
		break;

		case TOOL_DRAW_RECT:
		OnLBtnDownAddRect(point);
		break;
		
		case TOOL_SEL_CURVE:
		OnLBtnDownSelCurve(point);
		break;

		case TOOL_SEL_PIECE:
		OnLBtnDownSelPiece(point);
		break;

		case TOOL_ADD_DRILL:
		OnLbtnDownAddDrill(point);
		break;

		case TOOL_ADD_TEXT:	//当前为添加文字模式
		OnLbtnDownAddText(point);
		break;
	}
	CView::OnLButtonDown(nFlags, point);
	return;
}

void CMCADView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	switch(GetDocument()->m_nCurTool)
	{
		case TOOL_SEL_CURVE:
		OnLBtnUpSelCurve(point);
		break;

		case TOOL_DRAW_RECT:
		OnLBtnUpDrawRect(point);
		break;
	}
	CView::OnLButtonUp(nFlags, point);
}

#include "DlgPropTextAdd.h"
void CMCADView::OnLbtnDownAddText(CPoint point)
{
	CDlgPropTextAdd dlg;
	CPoint ptSecond;
	double dTextX,dTextY,dAngle;
	switch(m_nDrawLineStep)
	{
		case 0: //寻找起点
		m_ptAddLineFirstP = ptScrToHpgl(point);
		m_ptMouseMove = m_ptLBDown = ptHpglToScr(m_ptAddLineFirstP);
		m_nDrawLineStep = 1;
		break;

		case 1:
		dTextX = m_ptAddLineFirstP.x/40.0;
		dTextY = m_ptAddLineFirstP.y/40.0;
		ptSecond = ptScrToHpgl(point);
		dAngle = atan2((double)(ptSecond.y-m_ptAddLineFirstP.y),(double)(ptSecond.x-m_ptAddLineFirstP.x));
		dAngle = dAngle*180.0/PI;

		gTextProp.setTextPosX(dTextX);
		gTextProp.setTextPosY(dTextY);
		gTextProp.setTextAngle(dAngle);

		m_nDrawLineStep = 0;

		if(IDOK == dlg.DoModal() )
		{
			OnAddText();
		}
		OnRedraw();
		break;
	}
}

#include "DlgPropLineAdd.h"
void CMCADView::OnLBtnDownAddLine(CPoint point)
{
	CDlgPropLineAdd dlg;
	CPoint AddLineSecondP;
	CPoint ptFind;
	ptFind = FindNearestHpglpt(point);
	switch(m_nDrawLineStep)
	{
		case 0: //寻找起点
		if ((ptFind.x == -10)&&((ptFind.y == -10)))
		{
			m_ptAddLineFirstP = ptScrToHpgl(point);
		}
		else
		{
			m_ptAddLineFirstP = ptFind;
		}
		m_ptMouseMove = m_ptLBDown = ptHpglToScr(m_ptAddLineFirstP);
		m_nDrawLineStep = 1;
		break;
		
		case 1: //寻找终点
		if ((ptFind.x == -10)&&((ptFind.y == -10)))
		{
			AddLineSecondP = ptScrToHpgl(point);
			int iRadius100 = (int)(getdistance(m_ptAddLineFirstP,AddLineSecondP)*0.25);
			int iAngle100 = (int)(atan2((double)(AddLineSecondP.y-m_ptAddLineFirstP.y),(double)(AddLineSecondP.x-m_ptAddLineFirstP.x))*100.0*180.0/PI);
			dlg.m_dLineAddRadius = iRadius100/100.0;
			dlg.m_dLineAddAngle = iAngle100/100.0;
			
			if (IDOK == dlg.DoModal())
			{
				GetDocument()->BackUp(); //画线之前

				CPoint ptAdd;
				ptAdd.x = (int)(m_ptAddLineFirstP.x+dlg.m_dLineAddRadius*cos(dlg.m_dLineAddAngle*PI/180.0)*400.0);
				ptAdd.y = (int)(m_ptAddLineFirstP.y+dlg.m_dLineAddRadius*sin(dlg.m_dLineAddAngle*PI/180.0)*400.0);
				CCurve* pCurve = new CCurve;
				pCurve->m_nCurveType = CURVE_TYPE_MM;
				pCurve->m_bIsSelected = TRUE;
				pCurve->m_bIsPolygon = FALSE;
				pCurve->m_bIsOutline = FALSE;
				pCurve->m_pointList->Add(m_ptAddLineFirstP);
				pCurve->m_pointList->Add(ptAdd);
				pCurve->GetCurveMaxMin();
				GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
			}
		}
		else
		{
			GetDocument()->BackUp(); //画线之前
			CPoint ptAdd;
			ptAdd = ptFind;
			CCurve* pCurve = new CCurve;
			pCurve->m_nCurveType = CURVE_TYPE_MM;
			pCurve->m_bIsSelected = TRUE;
			pCurve->m_bIsPolygon = FALSE;
			pCurve->m_bIsOutline = FALSE;
			pCurve->m_pointList->Add(m_ptAddLineFirstP);
			pCurve->m_pointList->Add(ptAdd);
			pCurve->GetCurveMaxMin();
			GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
		}
		m_nDrawLineStep = 0;
		OnRedraw();
		break;
	}
}

void CMCADView::OnLBtnDownAddCurve(CPoint point)
{
	CPoint AddLineSecondP;
	CPoint ptFind;
	ptFind = FindNearestHpglpt(point);
	switch(m_nDrawLineStep)
	{
		case 0: //寻找起点
		if ((ptFind.x == -10)&&((ptFind.y == -10)))
		{
			m_ptAddLineFirstP = ptScrToHpgl(point);
		}
		else
		{
			m_ptAddLineFirstP = ptFind;
		}
		m_ptMouseMove = m_ptLBDown = ptHpglToScr(m_ptAddLineFirstP);
		m_nDrawLineStep = 1;
		break;
		
		case 1: //寻找终点
		if ((ptFind.x == -10)&&((ptFind.y == -10)))
		{
			GetDocument()->BackUp(); //画线之前
			CCurve* pCurve = new CCurve;
			pCurve->m_nCurveType = CURVE_TYPE_MM;
			pCurve->m_bIsSelected = TRUE;
			pCurve->m_bIsPolygon = FALSE;
			pCurve->m_bIsOutline = FALSE;
			pCurve->m_pointList->Add(m_ptAddLineFirstP);

			AddLineSecondP = ptScrToHpgl(point);
			pCurve->m_pointList->Add(AddLineSecondP);
			GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
			m_ptAddLineFirstP = AddLineSecondP;
			m_ptMouseMove = m_ptLBDown = ptHpglToScr(m_ptAddLineFirstP);
		}
		else
		{
			GetDocument()->BackUp(); //画线之前
			CPoint ptAdd = ptFind;
			CCurve* pCurve;
			pCurve = new CCurve;
			pCurve->m_nCurveType = CURVE_TYPE_MM;
			pCurve->m_bIsSelected = TRUE;
			pCurve->m_bIsPolygon = FALSE;
			pCurve->m_bIsOutline = FALSE;
			pCurve->m_pointList->Add(m_ptAddLineFirstP);

			pCurve->m_pointList->Add(ptAdd);
			GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);

			m_nDrawLineStep = 0;
		}
		OnCurveConnect();
		OnRedraw();
		break;
	}
}

void CMCADView::OnLBtnDownSetCutSeq(CPoint point)
{
	int nPieceSum = GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].GetSize();
	if (m_nNextCutSequence >= nPieceSum)
	{
//		m_nNextCutSequence = 0;
//		GetDocument()->m_nCurTool = TOOL_SEL_PIECE;
		OnRedraw();
		return;
	}
	GetDocument()->OnResetPieceSelState();
	CPiece *pPiece = GetPieceClicked(point);

 	if (NULL == pPiece)
 		return;
 	pPiece->m_nPieceSelStat = 1;

	GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].RemoveAt(pPiece->m_nCutSequence);
	GetDocument()->m_PiecesInMain[GetDocument()->m_nCurPage].InsertAt(m_nNextCutSequence,pPiece);
	GetDocument()->OnResetCutSeq();
	m_nNextCutSequence++;
	OnRedraw();
	return;
}

void CMCADView::OnLBtnDownAddRect(CPoint point)
{
	m_ptLBDown = point;
	m_ptMouseMove = point;
	m_bLBDown = TRUE;
	return;
}

void CMCADView::OnLBtnDownSelCurve(CPoint point)
{
	m_ptLBDown = point;
	m_ptMouseMove = point;
	m_bLBDown = TRUE;
	return;
}

#include "DlgPropRect.h"
void CMCADView::OnLBtnUpDrawRect(CPoint point)
{
	m_bLBDown = FALSE;
	int deltaX = point.x-m_ptLBDown.x;
	int deltaY = point.y-m_ptLBDown.y;
	if ( (abs(deltaX) < 10)&&(abs(deltaY) < 10) )
	{
		return;
	}
	CPoint ptHpgl1,ptHpgl2;
	CDlgPropRect dlg;
	ptHpgl1 = ptScrToHpgl(m_ptLBDown);
	ptHpgl2 = ptScrToHpgl(point);
	dlg.m_nRectSizeX = (int)fabs( (ptHpgl2.x-ptHpgl1.x)/40.0 );
	dlg.m_nRectSizeY = (int)fabs( (ptHpgl2.y-ptHpgl1.y)/40.0 );
	int xSign = (ptHpgl2.x>ptHpgl1.x)?1:-1;
	int ySign = (ptHpgl2.y>ptHpgl1.y)?1:-1;;
	if(IDOK == dlg.DoModal())
	{
		GetDocument()->BackUp(); //画方框之前

		CPoint ptHpgl3,ptHpgl4;
		ptHpgl2.x = ptHpgl1.x+dlg.m_nRectSizeX*40*xSign;
		ptHpgl2.y = ptHpgl1.y+dlg.m_nRectSizeY*40*ySign;
		
		ptHpgl3.x = ptHpgl2.x;
		ptHpgl3.y = ptHpgl1.y;

		ptHpgl4.x = ptHpgl1.x;
		ptHpgl4.y = ptHpgl2.y;

		CCurve* pCurve = new CCurve;
		pCurve->m_nCurveType = CURVE_TYPE_MM;
		pCurve->m_bIsSelected = TRUE;
		pCurve->m_bIsPolygon = TRUE;
		pCurve->m_bIsOutline = FALSE;
		pCurve->m_pointList->Add(ptHpgl1);
		pCurve->m_pointList->Add(ptHpgl3);
		pCurve->m_pointList->Add(ptHpgl2);
		pCurve->m_pointList->Add(ptHpgl4);
		pCurve->m_pointList->Add(ptHpgl1);
		GetDocument()->m_CurveInMain[GetDocument()->m_nCurPage].Add(pCurve);
		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		OnRedraw();
	}
	return;
}

void CMCADView::OnLBtnUpSelCurve(CPoint point)
{
	CPoint pt1,pt2;
	if(m_ptLBDown.x < point.x)
	{
		pt1.x = m_ptLBDown.x;
		pt2.x = point.x;
	}
	else
	{
		pt1.x = point.x;
		pt2.x = m_ptLBDown.x;
	}
	if(m_ptLBDown.y < point.y)
	{
		pt1.y = m_ptLBDown.y;
		pt2.y = point.y;
	}
	else
	{
		pt1.y = point.y;
		pt2.y = m_ptLBDown.y;
	}
	
	if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
	{
		GetDocument()->OnClearCurveSelState();
	}
	
	CRect rect(pt1,pt2);
	if( (rect.Width()>10)&&(rect.Height()>10) )
	{
		GetDocument()->FindCurvesInRect(rect);
	}
	else
	{
		GetDocument()->FindCurveOnPoint(point);
	}
	m_bLBDown = FALSE;
	OnRedraw();
}

void CMCADView::OnLBtnDownSelPiece(CPoint point)
{
	CPiece *pPiece;
	CDC* pDC = GetDC();
	pPiece = GetPieceClicked(point);
	if (NULL == pPiece)
	{
		if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
		{
			GetDocument()->OnResetPieceSelState();
			OnRedraw();
		}
		return;
	}
	if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
	{
		if (pPiece->m_nPieceSelStat != 2)
		{
			GetDocument()->OnResetPieceSelState();
		}
	}

	switch (pPiece->m_nPieceSelStat)
	{
		case 0:
		pPiece->m_nPieceSelStat = 1;
		break;
		case 1:
		pPiece->m_nPieceSelStat = 0;
		break;
		case 2:
		pPiece->m_nPieceSelStat = 1;
		GetDocument()->BackUp();
		break;
	}
	pPiece->m_bRefreshDisplay = TRUE;
	UpdatePieceDisplay(pDC); //更新裁片在视图中的显示，哪个状态改变了就重绘哪个
	return;
}

void CMCADView::OnLbtnDownAddDrill(CPoint point)
{
	GetDocument()->BackUp(); //添加钻孔之前

	GetDocument()->OnResetPieceSelState();
	GetDocument()->OnGetLineFromPieces(); //将裁片打散为线条

	GetDocument()->OnAddDrill(point);
	OnRedraw();
}

#define FR_DIST_PIXEL_X 20
#define FR_DIST_PIXEL_Y 40
void CMCADView::OnCalcDispProportion()
{
	CRect rect;
	double d,dX,dY;
	GetWindowRect(rect);
	gSet.setScrXPixel(rect.Width());
	gSet.setScrYPixel(rect.Height());
	gSet.setDispStartX(0);
	gSet.setDispStartY(0);
	
	dX = (double)(rect.Width()-2*FR_DIST_PIXEL_X)/(double)(gSet.getDispSizeX()*40);
	dY = (double)(rect.Height()-2*FR_DIST_PIXEL_Y)/(double)(gSet.getDispSizeY()*40);
	
	d = (dX < dY)?dX:dY;
	gSet.setDispScale(d);
}

BOOL CMCADView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if(m_nDrawLineStep == 0)
	{
		if (zDelta > 0)
		{
			MCADViewZoomIn(pt);
		}
		else
		{
			MCADViewZoomOut(pt);
		}
	}
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMCADView::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPoint HpglPt =	ptScrToHpgl(point);
	CMainFrame*pMainFrame=(CMainFrame*)::AfxGetMainWnd();
	pMainFrame->SetMousePosText(HpglPt);

	if (m_bRBDown) //移动视图
	{
		OnMMoveRbDown(point);
		OnRedraw();
		CView::OnMouseMove(nFlags, point);
		return;
	}

	switch(GetDocument()->m_nCurTool)
	{
		case TOOL_DRAW_LINE:  //画线段
		case TOOL_DRAW_CURVE: //画曲线
		case TOOL_ADD_TEXT:   //增加文字
		OnMMoveDrawLineCurve(point);
		break;

		case TOOL_DRAW_RECT:
		OnMMoveAddRect(point);
		break;

		case TOOL_SEL_PIECE:
		OnMMoveSelPiece(point);
		break;

		case TOOL_SEL_CURVE:
		OnMMoveSelCurve(point);
		break;
	}
	CView::OnMouseMove(nFlags, point);
	return;
}

void CMCADView::OnMMoveRbDown(CPoint point)
{
	int nDispStartX = gSet.getDispStartX();
	int nDispStartY = gSet.getDispStartY();
	int deltaX = m_ptMouseMove.x-point.x;
	int deltaY = m_ptMouseMove.y-point.y;
	gSet.setDispStartX(nDispStartX+deltaX);
	gSet.setDispStartY(nDispStartY+deltaY);
	m_ptMouseMove = point;
}

void CMCADView::OnMMoveDrawLineCurve(CPoint point)
{
	if(m_nDrawLineStep == 1)
	{
		CPen pen;
		CPen *pOldPen;
		CClientDC dc(this);
		
		pen.CreatePen(2, 1, RGB(0, 0, 0));
		pOldPen = dc.SelectObject(&pen);
		
		dc.SetROP2(R2_XORPEN);
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(m_ptMouseMove);
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(point);
		m_ptMouseMove = point;
		dc.SelectObject(pOldPen);
		pen.DeleteObject();
	}
}

void CMCADView::OnMMoveSelPiece(CPoint point)
{
	int deltaX,deltaY;
	CPoint ptHpgl = ptScrToHpgl(point);
	deltaX = ptHpgl.x - m_ptHpglPMLastP.x;
	deltaY = ptHpgl.y - m_ptHpglPMLastP.y;
	m_ptHpglPMLastP = ptHpgl;
	
	CPiece *pPiece = GetMovingPiece();
	if (pPiece != NULL)
	{
		pPiece->m_bRefreshDisplay = TRUE;
		CDC* pDC = GetDC();
		UpdatePieceDisplay(pDC);
		
		pPiece->OnMove(deltaX,deltaY);
		pPiece->OnCheckIfBelowZero();
		pPiece->ReCalcPieceSize();
		pPiece->m_bRefreshDisplay = TRUE;
		UpdatePieceDisplay(pDC);
	}
	return;
}

void CMCADView::OnMMoveSelCurve(CPoint point)
{
	CClientDC dc(this);
	CPen pen;
	CPen *pOldPen;
	
	pen.CreatePen(2, 1, RGB(0, 0, 0));
	pOldPen = dc.SelectObject(&pen);
	
	if (m_bLBDown)
	{
		dc.SetROP2(R2_XORPEN);
		
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(m_ptLBDown.x, m_ptMouseMove.y);
		dc.LineTo(m_ptMouseMove);
		dc.LineTo(m_ptMouseMove.x, m_ptLBDown.y);
		dc.LineTo(m_ptLBDown);
		
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(m_ptLBDown.x, point.y);
		dc.LineTo(point);
		dc.LineTo(point.x, m_ptLBDown.y);
		dc.LineTo(m_ptLBDown);
		m_ptMouseMove = point;
	}
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	return;
}

void CMCADView::OnMMoveAddRect(CPoint point)
{
	CClientDC dc(this);
	CPen pen;
	CPen *pOldPen;
	
	pen.CreatePen(LT_SOLID, 1, gColorSet.getClrPen());
	pOldPen = dc.SelectObject(&pen);
	
	if (m_bLBDown)
	{
		dc.SetROP2(R2_XORPEN);
		
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(m_ptLBDown.x, m_ptMouseMove.y);
		dc.LineTo(m_ptMouseMove);
		dc.LineTo(m_ptMouseMove.x, m_ptLBDown.y);
		dc.LineTo(m_ptLBDown);
		
		dc.MoveTo(m_ptLBDown);
		dc.LineTo(m_ptLBDown.x, point.y);
		dc.LineTo(point);
		dc.LineTo(point.x, m_ptLBDown.y);
		dc.LineTo(m_ptLBDown);
		m_ptMouseMove = point;
	}
	dc.SelectObject(pOldPen);
	pen.DeleteObject();
	return;
}

void CMCADView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	switch (nChar)
	{
		case VK_UP:
		if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
		{
			gSet.setDispStartY(gSet.getDispStartY()+40);
			OnRedraw();
		}
		break;

		case VK_DOWN:
		if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
		{
			gSet.setDispStartY(gSet.getDispStartY()-40);
			OnRedraw();
		}
		break;

		case VK_LEFT:
		if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
		{
			gSet.setDispStartX(gSet.getDispStartX()+40);
			OnRedraw();
		}
		break;

		case VK_RIGHT:
		if( (!( GetKeyState(VK_CONTROL) < 0 ) ) && (!( GetKeyState(VK_SHIFT) < 0 ) ) ) //按住Ctrl键可多选
		{
			gSet.setDispStartX(gSet.getDispStartX()-40);
			OnRedraw();
		}
		break;

		case VK_DELETE:
		if (TOOL_SEL_PIECE == GetDocument()->m_nCurTool)
		{
			OnPieceRemove();
		}
		else
		{
			OnCurveRemove();
		}
		break;
	}
	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}


void CMCADView::OnFindOutline() 
{
	GetDocument()->OnSearchCutData();
	OnRedraw();	
}

void CMCADView::OnLineToDot() 
{
	// TODO: Add your command handler code here
	GetDocument()->BackUp(); //曲线变点之前
	CCurve* pCurve;
	pCurve = GetDocument()->GetSelCurve();
	if (NULL != pCurve)
	{
		CDlgGetInput dlg;
		dlg.m_dInput = gSet.getDrillDist()/10.0;
		dlg.m_strTips.Format("请输入孔距(mm):");
		if (IDOK == dlg.DoModal())
		{
			gSet.setDrillDist((int)(dlg.m_dInput*10.0));

			GetDocument()->OnSelCurveToDrill();
			GetDocument()->BackUp(); //曲线变点之后
			OnRedraw();
		}
	}
}


void CMCADView::OnAddText(void)
{
//	AfxMessageBox(gTextProp.m_strTextAdd);
	if (gTextProp.m_strTextAdd.GetLength() <= 0)
	{
		AfxMessageBox("没有输入文字内容");
		return;
	}
	GetDocument()->OnAddString();
}

extern CPoint g_ptStart;
extern CPoint g_ptEnd;


void CMCADView::OnDemoRunDrawLine(int iDrawType)
{
	CClientDC dc(this);
	CPoint ptStart,ptEnd;
	CPen penNew, *pPenOld;
	if (iDrawType == 1)
	{
		penNew.CreatePen(LT_SOLID, 0, RGB(0x0f, 0xff, 0x0)); //
	}
	else
	{
		penNew.CreatePen(LT_SOLID, 0, RGB(0, 0, 0xff)); //
	}

	pPenOld = dc.SelectObject(&penNew);

	ptStart = ptHpglToScr(g_ptStart);
	ptEnd = ptHpglToScr(g_ptEnd);

	dc.DPtoLP(&ptStart);
	dc.DPtoLP(&ptEnd);

	dc.MoveTo(ptStart);
	dc.LineTo(ptEnd);
	dc.SelectObject(pPenOld);
}

