// DlgProtectPara.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgProtectPara.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProtectPara dialog


CDlgProtectPara::CDlgProtectPara(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProtectPara::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProtectPara)
	m_sXSizeMm = 0;
	m_sYSizeMm = 0;
	m_dPPMMX = 0.0;
	m_dPPMMY = 0.0;
	m_dPPMMZ = 0.0;
	m_nWorkStartX = 0;
	m_nWorkStartY = 0;
	//}}AFX_DATA_INIT
}


void CDlgProtectPara::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProtectPara)
	DDX_Text(pDX, IDCE_MAXPP_X, m_sXSizeMm);
	DDV_MinMaxUInt(pDX, m_sXSizeMm, 900, 3000);
	DDX_Text(pDX, IDCE_MAXPP_Y, m_sYSizeMm);
	DDV_MinMaxUInt(pDX, m_sYSizeMm, 900, 3000);
	DDX_Text(pDX, IDCE_PPMMX, m_dPPMMX);
	DDV_MinMaxDouble(pDX, m_dPPMMX, 50., 500.);
	DDX_Text(pDX, IDCE_PPMMY, m_dPPMMY);
	DDV_MinMaxDouble(pDX, m_dPPMMY, 50., 500.);
	DDX_Text(pDX, IDCE_PPMMZ, m_dPPMMZ);
	DDV_MinMaxDouble(pDX, m_dPPMMZ, 500., 5000.);
	DDX_Text(pDX, IDCE_WORK_STARTX, m_nWorkStartX);
	DDV_MinMaxInt(pDX, m_nWorkStartX, 0, 30);
	DDX_Text(pDX, IDCE_WORK_STARTY, m_nWorkStartY);
	DDV_MinMaxInt(pDX, m_nWorkStartY, 0, 50);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProtectPara, CDialog)
	//{{AFX_MSG_MAP(CDlgProtectPara)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProtectPara message handlers

void CDlgProtectPara::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	gSet.setMacSizeX(m_sXSizeMm);
	gSet.setMacSizeY(m_sYSizeMm);

	gSet.setWorkStartX(m_nWorkStartX);
	gSet.setWorkStartY(m_nWorkStartY);

	gSet.setPPMMX(m_dPPMMX);
	gSet.setPPMMY(m_dPPMMY);
	gSet.setPPMMZ(m_dPPMMZ);
	CDialog::OnOK();
}

BOOL CDlgProtectPara::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_sXSizeMm = gSet.getMacSizeX();
	m_sYSizeMm = gSet.getMacSizeY();
	m_nWorkStartX = gSet.getWorkStartX();
	m_nWorkStartY = gSet.getWorkStartY();

	m_dPPMMX = gSet.getPPMMX();
	m_dPPMMY = gSet.getPPMMY();
	m_dPPMMZ = gSet.getPPMMZ();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
