// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MCAD.h"
#include "HpglFile.h"
#include "MainFrm.h"
#include "Cnc.h"
#include "Splash.h"

UINT ThreadTimeDetect(LPVOID pParam);
UINT ThreadAutoConnect(LPVOID pParam); //自动连接机器并开始服务
UINT ThreadDemoDisplay(LPVOID pParam);
CWnd* g_pMainfrm;
CPoint g_ptStart;
CPoint g_ptEnd;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_COMMAND(IDTB_PAGE1, OnTbPage1)
	ON_COMMAND(IDTB_PAGE2, OnTbPage2)
	ON_COMMAND(IDTB_PAGE3, OnTbPage3)
	ON_COMMAND(IDTB_PAGE4, OnTbPage4)
	ON_COMMAND(IDTB_PAGE5, OnTbPage5)
	ON_COMMAND(IDM_PLT_IMPORT, OnPltOpen)
	ON_COMMAND(TDTB_ZOOMALL, OnZoomAll)
	ON_COMMAND(IDCB_START, OnWorkStart)
	ON_UPDATE_COMMAND_UI(IDTB_PAGE1, OnUpdateTbPage1)
	ON_UPDATE_COMMAND_UI(IDTB_PAGE2, OnUpdateTbPage2)
	ON_UPDATE_COMMAND_UI(IDTB_PAGE3, OnUpdateTbPage3)
	ON_UPDATE_COMMAND_UI(IDTB_PAGE4, OnUpdateTbPage4)
	ON_UPDATE_COMMAND_UI(IDTB_PAGE5, OnUpdateTbPage5)
	ON_COMMAND(IDTB_MAC_PARA, OnMacPara)
	ON_COMMAND(IDTB_MAN_CTRL, OnMacTest)
	ON_COMMAND(IDM_PARA_SOFT, OnParaSoft)
	ON_COMMAND(IDM_APP_VIDEO, OnAppVideo)
	ON_WM_PAINT()
	ON_COMMAND(IDM_AUTH_UPDATE, OnAuthUpdate)
	ON_COMMAND(IDM_SUPER_USER, OnSuperUser)
	ON_COMMAND(IDM_APP_MANUAL, OnAppManual)
	ON_COMMAND(IDTBS_SCURVE, OnTbSelCurve)
	ON_COMMAND(IDTBS_SPIECE, OnSelPiece)
	ON_UPDATE_COMMAND_UI(IDTBS_SPIECE, OnUpdatePiece)
	ON_UPDATE_COMMAND_UI(IDTBS_SCURVE, OnUpdateLine)
	ON_UPDATE_COMMAND_UI(IDTB_MAN_CTRL, OnUpdateManCtrl)
	ON_UPDATE_COMMAND_UI(IDTB_MAC_PARA, OnUpdateMacPara)
	ON_COMMAND(ID_FILE_ADD_SNAKE_LINE, OnFileAddSnakeLine)
	ON_UPDATE_COMMAND_UI(IDCB_START, OnUpdateStart)
	ON_COMMAND(IDCB_PAUSE, OnWorkPause)
	ON_UPDATE_COMMAND_UI(IDCB_PAUSE, OnUpdatePause)
	ON_COMMAND(IDCB_CANCEL, OnWorkCancel)
	ON_UPDATE_COMMAND_UI(IDCB_CANCEL, OnUpdateCancel)
	ON_COMMAND(ID_FILE_ADD_DRILL_TABLE, OnFileAddDrillTable)
	ON_COMMAND(ID_FILE_EXPORT_PLT, OnFileExportPlt)
	ON_COMMAND(IDTBS_DRAW_LINE, OnDrawLine)
	ON_UPDATE_COMMAND_UI(IDTBS_DRAW_LINE, OnUpdateDrawLine)
	ON_COMMAND(IDTBS_ADRILL, OnAdrill)
	ON_UPDATE_COMMAND_UI(IDTBS_ADRILL, OnUpdateAdrill)
	ON_COMMAND(IDTBS_DRAW_CURVE, OnDrawCurve)
	ON_UPDATE_COMMAND_UI(IDTBS_DRAW_CURVE, OnUpdateDrawCurve)
	ON_COMMAND(IDTBS_DRAW_RECT, OnDrawRect)
	ON_UPDATE_COMMAND_UI(IDTBS_DRAW_RECT, OnUpdateDrawRect)
	ON_COMMAND(IDTBS_CUT_SEQ, OnCutSeq)
	ON_UPDATE_COMMAND_UI(IDTBS_CUT_SEQ, OnUpdateCutSeq)
	ON_COMMAND(ID_FILE_ADD_MP_TEST, OnFileAddMpTest)
	ON_MESSAGE(USER_DISP_STAT, OnDispSysStatus)
	ON_MESSAGE(USER_END_DEMO_DISPLAY, OnEndDemoDisplay)
	ON_MESSAGE(USER_DEMO_DRAW_LINE, OnDemoDrawLine)
	ON_COMMAND(IDCB_SEL_ALL, OnSelAll)
	//}}AFX_MSG_MAP
	ON_COMMAND(IDTBS_ADD_TEXT, &CMainFrame::OnIdtbsAddText)
	ON_UPDATE_COMMAND_UI(IDTBS_ADD_TEXT, &CMainFrame::OnUpdateIdtbsAddText)
	ON_COMMAND(IDCB_DEMO_START, &CMainFrame::OnIdcbDemoStart)
	ON_UPDATE_COMMAND_UI(IDCB_DEMO_START, &CMainFrame::OnUpdateIdcbDemoStart)
	ON_WM_TIMER()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	IDS_PT_MM,
	IDS_SYS_STATE,
	IDS_PROGRESS
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction
CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	::SetProp(m_hWnd, "MCAD_BY_YHZ", (HANDLE)1);
	CSplashWnd::ShowSplashScreen(this);

	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}


	if (!m_wndTbLine.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndTbLine.LoadToolBar(IDR_TB_LINE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndTbPage.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_BOTTOM
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndTbPage.LoadToolBar(IDR_TB_PAGE))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	m_wndTbLine.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndTbLine);

	m_wndTbPage.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndTbPage);

	gSet.setAppExitFlag(FALSE);
	g_pMainfrm = this;

	gThreadTimeDetect = AfxBeginThread(ThreadTimeDetect,NULL);
	gThreadAutoConnect = AfxBeginThread(ThreadAutoConnect,NULL);
	gSet.setQueryEnable(TRUE);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnTbPage1() 
{
	GetDocument()->m_nCurPage = 0;
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnTbPage2() 
{
	GetDocument()->m_nCurPage = 1;
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnTbPage3() 
{
	GetDocument()->m_nCurPage = 2;
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnTbPage4() 
{
	GetDocument()->m_nCurPage = 3;
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnTbPage5() 
{
	GetDocument()->m_nCurPage = 4;
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnUpdateTbPage1(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_nCurPage == 0)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateTbPage2(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_nCurPage == 1)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateTbPage3(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_nCurPage == 2)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateTbPage4(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_nCurPage == 3)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateTbPage5(CCmdUI* pCmdUI) 
{
	if (GetDocument()->m_nCurPage == 4)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

CMCADDoc* CMainFrame::GetDocument()
{
	return (CMCADDoc*)(GetActiveView()->GetDocument());
}

void CMainFrame::OnPltOpen() 
{
	char Custer[80] = "HP-GL(*.plt)\0*.plt\0All file(*.*)\0*.*\0\0";
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = Custer;

	if(dlg.DoModal() == IDOK)
	{
		CHpglFile* pHpglFile = new CHpglFile;
		CString strFileName = dlg.GetPathName();
		pHpglFile->m_strPathAndName = strFileName;
		if (0 != pHpglFile->OpenPltFile())
		{
			AfxMessageBox("文件打开失败!");
			return;
		}
		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		GetDocument()->OnImportCurveFromPlt(pHpglFile);
		GetDocument()->OnRemoveDupPoints();
		GetDocument()->OnSetCurveTypeFromPenNum();

		GetDocument()->OnSearchOutLine();
		GetDocument()->OnRemoveCurveNotOutline();//20161013

		GetDocument()->BackUp(); //导入PLT之后
		Invalidate(); //刷新显示
		delete pHpglFile;
	}
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	CRect cr;
	GetDesktopWindow()->GetWindowRect(&cr);
//	GetWindowRect( &cr );
	
	// Create the main splitter with 1 row and 2 columns
	if( !m_wndSplitter.CreateStatic(this,1,2) )
	{
		MessageBox( "Error setting up m_mainSplitter", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	// The views for each pane must be created 
	if( !m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CMCADView),	
		CSize(cr.Width()-160, cr.Height()), pContext) )
	{
		MessageBox( "创建主视图失败", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}
	
	if ( !m_wndSplitter.CreateView( 0, 1, RUNTIME_CLASS(CPieceListView),
		CSize(160, cr.Height()), pContext ) )
	{
		MessageBox( "创建列表视图失败", "ERROR", MB_OK | MB_ICONERROR );
		return FALSE;
	}

	m_pMainView = (CMCADView*)m_wndSplitter.GetPane(0,0);
	m_pListView = (CPieceListView*)m_wndSplitter.GetPane(0,1);
	m_pListView->m_pMainView = m_pMainView;
	return TRUE;
}

void CMainFrame::SetMousePosText(CPoint ptHpgl)
{
	CString strText;
	int nIndex;
	CWindowDC dc(&m_wndStatusBar);
	CSize sizeText;

	strText.Format("坐标:(%dmm,%dmm)",ptHpgl.x/40,ptHpgl.y/40);
	nIndex=m_wndStatusBar.CommandToIndex(IDS_PT_MM);
	m_wndStatusBar.SetPaneText(nIndex,strText);
	
	sizeText=dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,IDS_PT_MM,SBPS_NORMAL,sizeText.cx);
}

void CMainFrame::OnZoomAll() 
{
	m_pMainView->MCADViewZoomAll();	
}

#include "DlgParaMac.h"
void CMainFrame::OnMacPara() 
{
	gSet.setQueryEnable(FALSE);
	CDlgParaMac dlg;
	dlg.DoModal();
	gSet.setQueryEnable(TRUE);
}

#include "DlgParaSoft.h"
void CMainFrame::OnParaSoft() 
{
	CDlgParaSoft dlg;
	if (IDOK == dlg.DoModal())
	{
		m_pMainView->OnRedraw();
	}
}

void CMainFrame::OnAppVideo() 
{
	CString str;
	str = gSet.m_strAppDir+"操作演示.exe";
	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
}

LRESULT CMainFrame::OnDispSysStatus(WPARAM wParam, LPARAM lParam)
{
	CString strText;
	int nIndex;
	CSize sizeText;
	CWindowDC dc(&m_wndStatusBar);
	if (g_bParaRead)
	{
		switch(gSysState.macState)
		{
		case SYS_INIT:
			m_wndProgress.SetPos(0);
			strText.Format("系统正在初始化...");
			break;

		case SEEK_ZP:
		case MOVE_TO_DEFZP:
			m_wndProgress.SetPos(0);
			strText.Format("正在寻找原点...");
			break;

		case READY:
			m_wndProgress.SetPos(0);
			strText.Format("准备就绪");
			break;

		case READY_PAUSE:
			strText.Format("暂停按下");
			break;

		case WORK_PAUSE: //有文件开始打印后的暂停
			if (gSysState.CncSum <= 0)
			{
				m_wndProgress.SetPos(0);
			}
			else
			{
				m_wndProgress.SetPos(gSysState.CncNo*100/gSysState.CncSum);
			}
			strText.Format("暂停工作%d/%d",gSysState.CncNo,gSysState.CncSum);
			break;

		case WORKING:
			if (gSysState.CncSum <= 0)
			{
				m_wndProgress.SetPos(0);
			}
			else
			{
				m_wndProgress.SetPos(gSysState.CncNo*100/gSysState.CncSum);
			}
			strText.Format("工作中%d/%d",gSysState.CncNo,gSysState.CncSum);
			break;

		default:
			m_wndProgress.SetPos(0);
			strText.Format("Error %d",gSysState.macState);
			break;
		}
	}
	else
	{
		m_wndProgress.SetPos(0);
		strText.Format("USB未连接");
	}

	nIndex= m_wndStatusBar.CommandToIndex(IDS_SYS_STATE);
	m_wndStatusBar.SetPaneText(nIndex,strText);
	sizeText = dc.GetTextExtent(strText);
	m_wndStatusBar.SetPaneInfo(nIndex,IDS_SYS_STATE,SBPS_NORMAL,sizeText.cx);
	return 0;
}

void CMainFrame::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect;
	int nIndex;
	
	nIndex= m_wndStatusBar.CommandToIndex(IDS_PROGRESS);
	m_wndStatusBar.SetPaneInfo(nIndex,IDS_PROGRESS,SBPS_NORMAL,600);
	m_wndStatusBar.GetItemRect(nIndex,&rect);
	
	if(!m_wndProgress.m_hWnd)
	{
		m_wndProgress.Create(WS_CHILD | WS_VISIBLE | PBS_SMOOTH, rect,&m_wndStatusBar,123);
	}
	else
		m_wndProgress.MoveWindow(rect);
}

#include "DlgRegInfo.h"
void CMainFrame::OnAuthUpdate() 
{
	if(g_bParaRead != TRUE)
	{
		AfxMessageBox("请先联机然后进行此操作");
		return;
	}
	
	u32 McuCode = gUSB.OnGetChipID(); //进入授权更新界面时
	if (McuCode > 0x70000000)
	{
		gSet.setMcuCode(McuCode);
		CString str;
		str.Format("%x",McuCode ^ YHZIntArray[11]);
		gSet.setMcuSn(str);
		CDlgRegInfo dlg;
		dlg.DoModal();
	}
	else
	{
		AfxMessageBox("联机失败!");
	}	
}

#include "DlgSuperUser.h"
void CMainFrame::OnSuperUser() 
{
	u32 McuCode = gUSB.OnGetChipID(); //进入超级用户界面时
	if (McuCode > 0x70000000)
	{
		gSet.setMcuCode(McuCode);
		CDlgSuperUser dlg;
		dlg.DoModal();
	}
	else
	{
		AfxMessageBox("联机失败!");
	}
}

int CMainFrame::OnCheckReg()
{
	if (FALSE == g_bParaRead)//没有数据
	{
		AfxMessageBox("连接机器失败!");
		return -1;
	}
	if(!(  (READY == gSysState.macState) || (READY_PAUSE == gSysState.macState) ) )
	{
		AfxMessageBox("系统没有准备好!");
		return -2;
	}
	if( 0 != gReg.OnCheckRegCode(gSet.getAuthCode()) )
	{
		return -3;
	}
	return 100;
}

void CMainFrame::OnAppManual() 
{
	CString str;
	str = gSet.m_strAppDir+"使用说明.txt";
	ShellExecute(NULL, "open", str, NULL, NULL, SW_SHOWNORMAL);
}

void CMainFrame::OnSelPiece() 
{
	GetDocument()->m_nCurTool = TOOL_SEL_PIECE;
	GetDocument()->OnClearCurveSelState(); //所有裁片中的所有曲线全部设为未选中状态

	GetDocument()->OnSearchOutLine(); //检查是否封闭等
	GetDocument()->OnRemoveCurveNotOutline();//20161013

	GetDocument()->OnSearchPieceStep1(); //从曲线中识别裁片外轮廓 
	GetDocument()->OnSearchPieceStep2(); //识别裁片中的内线
	m_pMainView->OnRedraw();
}

void CMainFrame::OnCutSeq() 
{
	GetDocument()->m_nCurTool = TOOL_SET_CUT_SEQ;
	GetDocument()->OnClearCurveSelState(); //所有裁片中的所有曲线全部设为未选中状态
	
	GetDocument()->OnSearchOutLine(); //检查是否封闭等
	GetDocument()->OnRemoveCurveNotOutline();//20161013

	GetDocument()->OnSearchPieceStep1(); //从曲线中识别裁片外轮廓 
	GetDocument()->OnSearchPieceStep2(); //识别裁片中的内线

	GetDocument()->OnResetCutSeq();
	m_pMainView->m_nNextCutSequence = 0;
	m_pMainView->OnRedraw();
}


void CMainFrame::OnTbSelCurve() 
{
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnUpdatePiece(CCmdUI* pCmdUI) 
{
	if (TOOL_SEL_PIECE == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateCutSeq(CCmdUI* pCmdUI) 
{
	if (TOOL_SET_CUT_SEQ == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}


void CMainFrame::OnUpdateLine(CCmdUI* pCmdUI) 
{
	if (TOOL_SEL_CURVE == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}	
}

void CMainFrame::OnUpdateManCtrl(CCmdUI* pCmdUI) 
{
	if( (READY == gSysState.macState) || (READY_PAUSE == gSysState.macState) )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateMacPara(CCmdUI* pCmdUI) 
{
	if( (WORKING == gSysState.macState) || (WORK_PAUSE == gSysState.macState) )
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

#include "DlgTest.h"
void CMainFrame::OnMacTest() 
{
	gSet.setQueryEnable(FALSE);
	CDlgTest dlg;
	dlg.m_nTestPwmPlot = gSet.getPwmPen();
	dlg.m_nTestPwmCut = gSet.getPwmCut();
	dlg.m_nTestZPos = gSet.getZPosWork();
	dlg.DoModal();
	gSet.setQueryEnable(TRUE);
}

void CMainFrame::OnWorkPause() 
{
	gSet.setQueryEnable(FALSE);
	gUSB.OnWorkPause();
	gSet.setQueryEnable(TRUE);
}

void CMainFrame::OnWorkCancel() 
{
	gSet.setQueryEnable(FALSE);
	gUSB.OnWorkCancel();	
	gSet.setQueryEnable(TRUE);
}

void CMainFrame::OnUpdateStart(CCmdUI* pCmdUI) 
{
	if( (gSysState.macState == READY) || (WORK_PAUSE == gSysState.macState) )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdatePause(CCmdUI* pCmdUI) 
{
	if (gSysState.macState == WORKING)
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

void CMainFrame::OnUpdateCancel(CCmdUI* pCmdUI) 
{
	if ( (gSysState.macState == WORKING)||(gSysState.macState == WORK_PAUSE) )
	{
		pCmdUI->Enable(TRUE);
	}
	else
	{
		pCmdUI->Enable(FALSE);
	}
}

#include "DlgPlatformProp.h"
void CMainFrame::OnFileAddSnakeLine() 
{
	CDlgPlatformProp dlg;
	int XSize,YSize,Gap,MmDir;
	if (IDOK == dlg.DoModal())
	{
		XSize = dlg.m_nPlatformXSize;
		YSize = dlg.m_nPlatformYSize;
		Gap = dlg.m_nGapX;
		MmDir = dlg.m_nPlatformDir;
		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
		GetDocument()->OnGetLineFromPieces();
		if (MmDir == 0)
		{
			GetDocument()->OnAddSnakeLineByY(XSize,YSize,Gap);
		}
		else
		{
			GetDocument()->OnAddSnakeLineByX(XSize,YSize,Gap);
		}
		m_pMainView->OnRedraw();
	}
}

#include "DlgAddDrillTable.h"
void CMainFrame::OnFileAddDrillTable() 
{
	CDlgAddDrillTable dlg;
	int StartX,StartY,DotNumX,DotNumY,GapX,GapY;
	if (IDOK == dlg.DoModal())
	{
		StartX = dlg.m_nStartX;
		StartY = dlg.m_nStartY;
		DotNumX = dlg.m_nDotNumX;
		DotNumY = dlg.m_nDotNumY;
		GapX = dlg.m_nGapXmm;
		GapY = dlg.m_nGapYmm;
		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
		GetDocument()->OnGetLineFromPieces();
 		GetDocument()->OnAddDrillTable(StartX,StartY,DotNumX,DotNumY,GapX,GapY);
		m_pMainView->OnRedraw();
	}
}

void CMainFrame::OnFileExportPlt() 
{
	CTime timeNow = CTime::GetCurrentTime();
	srand((u32)(timeNow.GetTime()));
	int nRandNum1 = rand();
	if(nRandNum1 < 0)
	{
		nRandNum1 = nRandNum1*(-1);
	}
	if(nRandNum1 < 100)
	{
		nRandNum1 += 1123;
	}
	int nRandNum2 = rand();
	if(nRandNum2 < 0)
	{
		nRandNum2 = nRandNum2*(-1);
	}
	nRandNum2 = nRandNum2%10;

#if 1
	if (FALSE == OnCheckSoftDog(99,nRandNum2) )
	{
		MessageBox("操作失败!\n请插入加密狗。","未检测到加密狗",MB_ICONERROR);
		return;
	}
	
	if(FALSE == OnCheckSoftDog(nRandNum1,nRandNum2) )
	{
		MessageBox("操作失败!\n请插入加密狗。","未检测到加密狗",MB_ICONERROR);
		return;
	}
#endif
	char Custer[80] = "HP-GL(*.plt)\0*.plt\0All file(*.*)\0*.*\0\0";
	CString str,str1;
	CFileDialog dlg(FALSE);

	dlg.m_ofn.lpstrFilter = Custer;
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		int nFileNameLen = strFileName.GetLength();
		if (nFileNameLen<5)
		{
			strFileName += ".plt";
		}
		else
		{
			if( (strFileName.GetAt(nFileNameLen-4) == '.' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-3)) == 'P' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-2)) == 'L' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-1)) == 'T' )
				)
			{
			}
			else
			{
				strFileName += ".plt";
			}
		}

		GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
		GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
		GetDocument()->OnGetLineFromPieces();
		m_pMainView->OnRedraw();
		GetDocument()->OnExportToPlt(strFileName);
	}
}

void CMainFrame::OnDrawLine() 
{
	GetDocument()->m_nCurTool = TOOL_DRAW_LINE;
}

void CMainFrame::OnDrawCurve() 
{
	GetDocument()->m_nCurTool = TOOL_DRAW_CURVE;
}

void CMainFrame::OnAdrill() 
{
	GetDocument()->m_nCurTool = TOOL_ADD_DRILL;	
}

void CMainFrame::OnUpdateAdrill(CCmdUI* pCmdUI) 
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


void CMainFrame::OnIdtbsAddText()
{
	GetDocument()->m_nCurTool = TOOL_ADD_TEXT;	
}


void CMainFrame::OnUpdateIdtbsAddText(CCmdUI *pCmdUI)
{
	if (TOOL_ADD_TEXT == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateDrawLine(CCmdUI* pCmdUI) 
{
	if (TOOL_DRAW_LINE == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnUpdateDrawCurve(CCmdUI* pCmdUI) 
{
	if (TOOL_DRAW_CURVE == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

void CMainFrame::OnDrawRect() 
{
	GetDocument()->m_nCurTool = TOOL_DRAW_RECT;
}

void CMainFrame::OnUpdateDrawRect(CCmdUI* pCmdUI) 
{
	if (TOOL_DRAW_RECT == GetDocument()->m_nCurTool)
	{
		pCmdUI->SetCheck(TRUE);
	}
	else
	{
		pCmdUI->SetCheck(FALSE);
	}
}

const int SDIntArray[100] = 
{
	4072011,8416531,8892317,1229619,1427756,3654321,3219805,1122060,1513272,1876538,/*0~9*/
	6922761,1788886,1513182,1513678,4156598,8978266,5978626,4782626,5389862,3332626,/*10~19*/
	6922721,1698886,1723182,1517678,1156528,5978266,5878626,5582626,5380626,5444626,/*20~29*/
	6922361,1680886,1291182,1537678,1456508,5878266,5898626,5862626,5387626,5855526,/*30~39*/
	6924761,1688186,1213382,1513778,1412598,5898266,5897626,5877626,7389626,5836666,/*40~49*/
	6952761,1688826,1721312,1513768,1715598,5897266,5897826,5878826,5089826,5838777,/*50~59*/
	6622761,1688883,1201318,1513767,1415638,5897866,5897866,5878296,5389726,8882626,/*60~69*/
	7577776,3858888,9259999,1212212,6566626,5555575,5897862,5882261,5389826,1583990,/*70~79*/
	2021222,3031323,4041424,5051525,6162636,7273747,8182838,9192929,4849504,3322110,/*80~89*/
	1111111,2222222,3333333,4444444,8888888,9999999,5555555,6666666,1010101,7777777/*90~99*/
};

BOOL CMainFrame::OnCheckSoftDog(int nCmd1,int nCmd2)
{
	int nCheckNum;
	nCheckNum = (nCmd1%12345+SDIntArray[nCmd1%100] )*nCmd2;
	if(nCheckNum < 0x6000)
		nCheckNum += 0x6000;

	int i;
	gMySoftDog.SendCmdToSoftDog(nCmd1,nCmd2);
	if (FALSE == gMySoftDog.m_SoftDogDetected)
	{
		return FALSE;
	}
	gMySoftDog.ReadHidData();

	int nRcvCmd1,nRcvCmd2;
	int nRcvCheckNum;
	UNION_U832 u832Temp;
	for (i=0;i<4;i++)
	{
		u832Temp.u8buff[i] = gMySoftDog.m_cHidRdBuf[3+i];
	}
	nRcvCmd1 = u832Temp.u32buff;
	if (nRcvCmd1 != nCmd1)
	{
		return FALSE;
	}
		
	for (i=0;i<4;i++)
	{
		u832Temp.u8buff[i] = gMySoftDog.m_cHidRdBuf[7+i];
	}
	nRcvCmd2 = u832Temp.u32buff;
	if (nRcvCmd2 != nCmd2)
	{
		return FALSE;
	}

	for (i=0;i<4;i++)
	{
		u832Temp.u8buff[i] = gMySoftDog.m_cHidRdBuf[11+i];
	}
	nRcvCheckNum = u832Temp.u32buff;
	if (nCmd1 == 99)
	{
		for (i=0;i<100;i++)
		{
			if (nRcvCheckNum == SDIntArray[i])
			{
				return TRUE;
			}
		}
		return FALSE;
	}

	if (nRcvCheckNum != nCheckNum)
	{
		return FALSE;
	}
	return TRUE;
}

void CMainFrame::OnFileAddMpTest() 
{
	GetDocument()->m_nCurTool = TOOL_SEL_CURVE;
	GetDocument()->OnResetPieceSelState(); //所有裁片全部设为未选中状态
	GetDocument()->OnGetLineFromPieces();
	GetDocument()->OnAddMpTestFile();
	m_pMainView->OnRedraw();
}

void CMainFrame::OnSelAll() 
{
	if(GetDocument()->m_nCurTool != TOOL_SEL_CURVE)
		return;
	GetDocument()->OnSelAllCurve();
	m_pMainView->OnRedraw();	
}


void CMainFrame::OnWorkStart()
{
	if (gSysState.macState == WORK_PAUSE)
	{
		gSet.setQueryEnable(FALSE);
		gUSB.OnWorkResume();
		gSet.setQueryEnable(TRUE);
		return;
	}
	CString strTemp;
	int rev = GetDocument()->CreateCncList();
	if (rev <= 0)
	{
		AfxMessageBox("当前页没有数据！");
		return;
	}

#if 1
	gSet.setQueryEnable(FALSE);
	if (OnCheckReg() != 100)
	{
		gSet.setQueryEnable(TRUE);
		return;
	}
#endif
	if (IDOK == MessageBox("按下确定按钮后机器即将动作，是否继续？", "注意", MB_OKCANCEL))
	{
		rev = SendCncData();
		switch (rev)
		{
		case ERROR_NONE:
			break;
		case ERROR_XYSIZE:
			MessageBox("文件大小超出范围", "警告", MB_ICONERROR);
			break;
		default:
			strTemp.Format("发送文件失败，error %d", rev);
			MessageBox(strTemp, "警告", MB_ICONERROR);
			break;
		}
		gSet.setQueryEnable(TRUE);
	}
}


void CMainFrame::OnUpdateIdcbDemoStart(CCmdUI *pCmdUI)
{
	if ((gSysState.macState == WORKING) || (gSysState.macState == WORK_PAUSE))
	{
		pCmdUI->Enable(FALSE);
	}
	else
	{
		pCmdUI->Enable(TRUE);
	}
}

CWinThread* gThreadDemoDisplay;
void CMainFrame::OnIdcbDemoStart()
{
	CString strTemp;
	int rev = GetDocument()->CreateCncList();
	if (rev <= 0)
	{
		AfxMessageBox("当前页没有数据！");
		return;
	}
	 
	if (NULL == gThreadDemoDisplay)
	{
		gSet.setDemoEndFlag(FALSE);
		gThreadDemoDisplay = AfxBeginThread(ThreadDemoDisplay,NULL);
	}
	else
	{
		gSet.setDemoEndFlag(TRUE);
	}
	return;
}

LRESULT CMainFrame::OnEndDemoDisplay(WPARAM wParam, LPARAM lParam)
{
	gThreadDemoDisplay = NULL;
	return 0;
}

LRESULT CMainFrame::OnDemoDrawLine(WPARAM wParam, LPARAM lParam)
{
	m_pMainView->OnDemoRunDrawLine(wParam);
	return 0;
}

#define DISP_SEC_LEN (1.0) //每次显示2mm长度
UINT ThreadDemoDisplay(LPVOID pParam)
{
	int i, j;
	double dLineLen;
	int iLineSecNum;
	double dLineStartX, dLineStartY, dLineEndX, dLineEndY;
	double dDrawStartX, dDrawStartY, dDrawEndX, dDrawEndY;
	double dAngle;
	ST_CNC_DATA_ALL* pCutData0;
	ST_CNC_DATA_ALL* pCutData1;

	for (i=0;i<g_ptrCncList.GetSize()-1;i++)
	{
		if (gSet.getAppExitFlag()||(gSet.getDemoEndFlag()))
		{
			g_pMainfrm->SendMessage(USER_END_DEMO_DISPLAY, 0, 0);
			return 0;
		}
		Sleep(1);

		if (gSet.getAppExitFlag() || (gSet.getDemoEndFlag()))
		{
			g_pMainfrm->SendMessage(USER_END_DEMO_DISPLAY, 0, 0);
			return 0;
		}

		pCutData0 = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i);
		pCutData1 = (ST_CNC_DATA_ALL*)g_ptrCncList.GetAt(i+1);
		if ( (pCutData1->m_nCmdType == TYPE_PLOT) || (pCutData1->m_nCmdType == TYPE_MM) )
		{
			if (pCutData1->m_nCmdType == TYPE_PLOT)
			{
				dLineStartX = pCutData0->m_iX / gSet.getPPMMX()*40.0 - gSet.getMPDistX()*4.0 - 40.0*START_PENX_MM;
				dLineStartY = pCutData0->m_iY / gSet.getPPMMY()*40.0 - gSet.getMPDistY()*4.0 - 40.0*START_PENY_MM;

				dLineEndX = pCutData1->m_iX / gSet.getPPMMX()*40.0 - gSet.getMPDistX()*4.0 - 40.0*START_PENX_MM;
				dLineEndY = pCutData1->m_iY / gSet.getPPMMY()*40.0 - gSet.getMPDistY()*4.0 - 40.0*START_PENY_MM;
			}
			else
			{
				dLineStartX = pCutData0->m_iX / gSet.getPPMMX()*40.0 - 40.0*START_MX_MM;
				dLineStartY = pCutData0->m_iY / gSet.getPPMMY()*40.0 - 40.0*START_MY_MM;

				dLineEndX = pCutData1->m_iX / gSet.getPPMMX()*40.0 - 40.0*START_MX_MM;
				dLineEndY = pCutData1->m_iY / gSet.getPPMMY()*40.0 - 40.0*START_MY_MM;
			}

			dLineLen = sqrt( (dLineEndX - dLineStartX)*(dLineEndX - dLineStartX) + (dLineEndY - dLineStartY)*(dLineEndY - dLineStartY));
			dAngle = atan2( (dLineEndY - dLineStartY), (dLineEndX - dLineStartX) );
			iLineSecNum = (int)(dLineLen / DISP_SEC_LEN);
			if (iLineSecNum <= 1)
			{
				dDrawStartX = dLineStartX;
				dDrawStartY = dLineStartY;

				dDrawEndX = dLineEndX;
				dDrawEndY = dLineEndY;
//-----------------------------------------------------------------
				g_ptStart.x = (int)dDrawStartX;
				g_ptStart.y = (int)dDrawStartY;

				g_ptEnd.x = (int)dDrawEndX;
				g_ptEnd.y = (int)dDrawEndY;
				if (pCutData1->m_nCmdType == TYPE_PLOT)
				{
					g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 0, 0);
				}
				else
				{

					g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 1, 0);
				}
				continue;
			}

			for (j=0;j<iLineSecNum;j++)
			{
				dDrawStartX = dLineStartX + j*DISP_SEC_LEN*cos(dAngle);
				dDrawStartY = dLineStartY + j*DISP_SEC_LEN*sin(dAngle);

				dDrawEndX = dLineStartX + (j+1)*DISP_SEC_LEN*cos(dAngle);
				dDrawEndY = dLineStartY + (j+1)*DISP_SEC_LEN*sin(dAngle);
//-----------------------------------------------------------------
				g_ptStart.x = (int)dDrawStartX;
				g_ptStart.y = (int)dDrawStartY;

				g_ptEnd.x = (int)dDrawEndX;
				g_ptEnd.y = (int)dDrawEndY;
				if (pCutData1->m_nCmdType == TYPE_PLOT)
				{
					g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 0, 0);
				}
				else
				{
					g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 1, 0);
				}
				if (gSet.getAppExitFlag() || (gSet.getDemoEndFlag()))
				{
					g_pMainfrm->SendMessage(USER_END_DEMO_DISPLAY, 0, 0);
					return 0;
				}
			}
			dDrawStartX = dLineStartX + j*DISP_SEC_LEN*cos(dAngle);
			dDrawStartY = dLineStartY + j*DISP_SEC_LEN*sin(dAngle);

			dDrawEndX = dLineEndX;
			dDrawEndY = dLineEndY;
//-----------------------------------------------------------------
			g_ptStart.x = (int)dDrawStartX;
			g_ptStart.y = (int)dDrawStartY;

			g_ptEnd.x = (int)dDrawEndX;
			g_ptEnd.y = (int)dDrawEndY;
			if (pCutData1->m_nCmdType == TYPE_PLOT)
			{
				g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 0, 0);
			}
			else
			{
				g_pMainfrm->SendMessage(USER_DEMO_DRAW_LINE, 1, 0);
			}
		}
	}
	g_pMainfrm->SendMessage(USER_END_DEMO_DISPLAY,0 ,0 );
	return 0;
}

