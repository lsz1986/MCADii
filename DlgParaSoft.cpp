// DlgParaSoft.cpp : implementation file
//
#include "stdafx.h"
#include "MCAD.h"
#include "DlgParaSoft.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSoft dialog

CDlgParaSoft::CDlgParaSoft(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParaSoft::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParaSoft)
	m_nSizeX = gSet.getDispSizeX();
	m_nSizeY = gSet.getDispSizeY();
	m_nDrillerDimension = gSet.getDrillerDimension();
	m_nExpCutNum = gSet.getPltExportCutNum();
	m_nExpDrillNum = gSet.getPltExportDrillNum();
	m_nExpPenNum = gSet.getPltExportPenNum();
	//}}AFX_DATA_INIT
}

void CDlgParaSoft::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParaSoft)
	DDX_Text(pDX, IDCE_SIZEX, m_nSizeX);
	DDV_MinMaxInt(pDX, m_nSizeX, 200, 2000);
	DDX_Text(pDX, IDCE_SIZEY, m_nSizeY);
	DDV_MinMaxInt(pDX, m_nSizeY, 200, 2000);
	DDX_Text(pDX, IDCE_DIRLL_DIMENSION, m_nDrillerDimension);
	DDV_MinMaxInt(pDX, m_nDrillerDimension, 50, 800);
	DDX_Text(pDX, IDCE_EXPORT_CUTNUM, m_nExpCutNum);
	DDV_MinMaxInt(pDX, m_nExpCutNum, 0, 100);
	DDX_Text(pDX, IDCE_EXPORT_DRILLNUM, m_nExpDrillNum);
	DDV_MinMaxInt(pDX, m_nExpDrillNum, 0, 100);
	DDX_Text(pDX, IDCE_EXPORT_PENNUM, m_nExpPenNum);
	DDV_MinMaxInt(pDX, m_nExpPenNum, 0, 100);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDlgParaSoft, CDialog)
	//{{AFX_MSG_MAP(CDlgParaSoft)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSoft message handlers

void CDlgParaSoft::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);
	gSet.setDispSizeX(m_nSizeX);
	gSet.setDispSizeY(m_nSizeY);
	gSet.setDrillerDimension(m_nDrillerDimension);
	gSet.setPltExportPenNum(m_nExpPenNum);
	gSet.setPltExportCutNum(m_nExpCutNum);
	gSet.setPltExportDrillNum(m_nExpDrillNum);
	CDialog::OnOK();
}
