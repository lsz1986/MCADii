// DlgTest.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgTest.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgTest dialog


CDlgTest::CDlgTest(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTest::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgTest)
	m_nTestPwmPlot = 50;
	m_nTestPwmCut = 50;
	m_nTestZPos = 50;
	m_bTestPD = FALSE;
	m_bTestCD = FALSE;
	m_bTestMD = FALSE;
	m_bTestMmOn = FALSE;
	m_bTestVmOn = FALSE;
	//}}AFX_DATA_INIT
}


void CDlgTest::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgTest)
	DDX_Text(pDX, IDTCE_PWM_PLOT, m_nTestPwmPlot);
	DDV_MinMaxInt(pDX, m_nTestPwmPlot, 10, 90);
	DDX_Text(pDX, IDTCE_PWM_CUT, m_nTestPwmCut);
	DDV_MinMaxInt(pDX, m_nTestPwmCut, 10, 90);
	DDX_Text(pDX, IDTCE_Z_POS, m_nTestZPos);
	DDV_MinMaxInt(pDX, m_nTestZPos, 50, 400);
	DDX_Check(pDX, IDTCHK_PD, m_bTestPD);
	DDX_Check(pDX, IDTCHK_CD, m_bTestCD);
	DDX_Check(pDX, IDTCHK_MD, m_bTestMD);
	DDX_Check(pDX, IDTCHK_MMON, m_bTestMmOn);
	DDX_Check(pDX, IDTCHK_VMON, m_bTestVmOn);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgTest, CDialog)
	//{{AFX_MSG_MAP(CDlgTest)
	ON_BN_CLICKED(IDTCHK_CD, OnTestCd)
	ON_BN_CLICKED(IDTCHK_MD, OnTestMd)
	ON_BN_CLICKED(IDTCHK_MMON, OnTestMmon)
	ON_BN_CLICKED(IDTCHK_PD, OnTestPd)
	ON_BN_CLICKED(IDTCHK_VMON, OnTestVmOn)
	ON_BN_CLICKED(IDTBTN_TO_DEFZP, OnToDefzp)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDTBTN_SAVEPP, &CDlgTest::OnBnClickedSavepp)
	ON_BN_CLICKED(IDTBTN_SAVECP, &CDlgTest::OnBnClickedSavecp)
	ON_BN_CLICKED(IDTBTN_SAVE_WM, &CDlgTest::OnBnClickedSaveWm)
	ON_BN_CLICKED(IDTBTN_OK, &CDlgTest::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgTest message handlers
void CDlgTest::OnTestPd() 
{
	UpdateData();
	if (m_bTestPD)
	{
		gUSB.OnStartTest(TCMD_PEN_DOWN,m_nTestPwmPlot);
	}
	else
	{
		gUSB.OnStartTest(TCMD_PEN_DOWN,0);
	}
}

void CDlgTest::OnTestCd() 
{
	UpdateData();
	if (m_bTestCD)
	{
		gUSB.OnStartTest(TCMD_CUT_DOWM,m_nTestPwmCut);
	}
	else
	{
		gUSB.OnStartTest(TCMD_CUT_DOWM,0);
	}	
}

void CDlgTest::OnTestMd() 
{
	UpdateData();
	if (m_bTestMD)
	{
		gUSB.OnStartTest(TCMD_MM_DOWM,m_nTestZPos);
	}
	else
	{
		gUSB.OnStartTest(TCMD_MM_DOWM,0);
	}	
}

void CDlgTest::OnTestMmon() 
{
	UpdateData();
	if (m_bTestMmOn)
	{
		gUSB.OnStartTest(TCMD_MM_ON,1);
	}
	else
	{
		gUSB.OnStartTest(TCMD_MM_ON,0);
	}	
}

void CDlgTest::OnTestVmOn() 
{
	UpdateData();
	if (m_bTestVmOn)
	{
		gUSB.OnStartTest(TCMD_VM_ON,1);
	}
	else
	{
		gUSB.OnStartTest(TCMD_VM_ON,0);
	}
}

void CDlgTest::OnCancel() 
{
	gUSB.OnStartTest(TCMD_PEN_DOWN,0);
	gUSB.OnStartTest(TCMD_CUT_DOWM,0);
	gUSB.OnStartTest(TCMD_MM_DOWM,0);
	gUSB.OnStartTest(TCMD_VM_ON,0);
	gUSB.OnStartTest(TCMD_MM_ON,0);
	CDialog::OnCancel();
}

void CDlgTest::OnToDefzp() 
{
	gUSB.OnStartTest(TCMD_TO_DEFZP,0);
}

BOOL CDlgTest::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_nTestPwmPlot = gSet.getPwmPen();
	m_nTestPwmCut = gSet.getPwmCut();
	m_nTestZPos = gSet.getZPosWork();
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgTest::OnBnClickedSavepp()
{
	UpdateData();
	gSet.setPwmPen(m_nTestPwmPlot);
}

void CDlgTest::OnBnClickedSavecp()
{
	UpdateData();
	gSet.setPwmCut(m_nTestPwmCut);
}

void CDlgTest::OnBnClickedSaveMm()
{
	UpdateData();
	gSet.setZPosMove(m_nTestZPos);
}

void CDlgTest::OnBnClickedSaveWm()
{
	UpdateData();
	gSet.setZPosWork(m_nTestZPos);
}

void CDlgTest::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	WriteMacPara();
	CDialog::OnOK();
}
