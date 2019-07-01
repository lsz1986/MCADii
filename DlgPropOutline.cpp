// DlgOutLineProp.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPropOutline.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropOutline dialog
CDlgPropOutline::CDlgPropOutline(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropOutline::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropOutline)
	m_dCutDist = 0.0;
	m_dCutDepth = 0.0;
	m_dCutWidth = 0.0;
	//}}AFX_DATA_INIT
}

void CDlgPropOutline::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropOutline)
	DDX_Control(pDX, IDCOMBO_CUT_POS, m_ctrlCutPos);
	DDX_Text(pDX, IDCE_TOP, m_dMmTop);
	DDX_Text(pDX, IDCE_BOTTOM, m_dMmBot);
	DDX_Text(pDX, IDCE_LEFT, m_dMmLeft);
	DDX_Text(pDX, IDCE_RIGHT, m_dMmRight);
	DDX_Text(pDX, IDCE_RADIUS, m_dMmRadius);
	DDX_Check(pDX, IDCHK_LB, m_bLB);
	DDX_Check(pDX, IDCHK_RB, m_bRB);
	DDX_Check(pDX, IDCHK_RT, m_bRT);
	DDX_Check(pDX, IDCHK_CLOCKWISE, m_bClockWise);
	DDX_Check(pDX, IDCHK_LT, m_bLT);
	DDX_Text(pDX, IDCE_CUT_DIST, m_dCutDist);
	DDX_Text(pDX, IDCE_CUT_DEPTH, m_dCutDepth);
	DDX_Text(pDX, IDCE_CUT_WIDTH, m_dCutWidth);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgPropOutline, CDialog)
	//{{AFX_MSG_MAP(CDlgPropOutline)
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropOutline message handlers

void CDlgPropOutline::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	gOutLineProp.setMmTop(m_dMmTop);
	gOutLineProp.setMmBot(m_dMmBot);
	gOutLineProp.setMmLeft(m_dMmLeft);
	gOutLineProp.setMmRight(m_dMmRight);
	gOutLineProp.setMmOutlineRadius(m_dMmRadius);

	gOutLineProp.setClockWise(m_bClockWise);
	gOutLineProp.setRoundLT(m_bLT);
	gOutLineProp.setRoundLB(m_bLB);
	gOutLineProp.setRoundRT(m_bRT);
	gOutLineProp.setRoundRB(m_bRB);

	gOutLineProp.setCutDepth(m_dCutDepth);
	gOutLineProp.setCutWidth(m_dCutWidth);
	gOutLineProp.setCutDist(m_dCutDist);
	gOutLineProp.setCutPos(m_ctrlCutPos.GetCurSel());

	CDialog::OnOK();
}

BOOL CDlgPropOutline::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_dMmTop = gOutLineProp.getMmTop();
	m_dMmBot = gOutLineProp.getMmBot();
	m_dMmLeft = gOutLineProp.getMmLeft();
	m_dMmRight = gOutLineProp.getMmRight();
	m_dMmRadius = gOutLineProp.getMmOutlineRadius();
	m_bLB = gOutLineProp.getRoundLB();
	m_bRB = gOutLineProp.getRoundRB();
	m_bRT = gOutLineProp.getRoundRT();
	m_bClockWise = gOutLineProp.getClockWise();
	m_bLT = gOutLineProp.getRoundLT();

	m_dCutDist = gOutLineProp.getCutDist(); //剪口间距
	m_dCutDepth = gOutLineProp.getCutDepth(); //剪口深度
	m_dCutWidth = gOutLineProp.getCutWidth(); //剪口宽度

	m_ctrlCutPos.AddString("0不添加");
	m_ctrlCutPos.AddString("1在左边添加");
	m_ctrlCutPos.AddString("2在下边添加");
	m_ctrlCutPos.AddString("3在右边添加");
	m_ctrlCutPos.AddString("4在上边添加");
	m_ctrlCutPos.SetCurSel(gOutLineProp.getCutPos());
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
