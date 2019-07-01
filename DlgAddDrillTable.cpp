// DlgAddDrillTable.cpp : implementation file
//

#include "stdafx.h"
#include "mcad.h"
#include "DlgAddDrillTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDrillTable dialog


CDlgAddDrillTable::CDlgAddDrillTable(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgAddDrillTable::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgAddDrillTable)
	m_nDotNumX = 50;
	m_nDotNumY = 50;
	m_nGapXmm = 16;
	m_nGapYmm = 16;
	m_nStartX = 0;
	m_nStartY = 0;
	//}}AFX_DATA_INIT
}


void CDlgAddDrillTable::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgAddDrillTable)
	DDX_Text(pDX, IDCE_DOTNUM_X, m_nDotNumX);
	DDV_MinMaxInt(pDX, m_nDotNumX, 5, 200);
	DDX_Text(pDX, IDCE_GAP_X, m_nGapXmm);
	DDV_MinMaxInt(pDX, m_nGapXmm, 10, 200);
	DDX_Text(pDX, IDCE_GAP_Y, m_nGapYmm);
	DDV_MinMaxInt(pDX, m_nGapYmm, 10, 200);
	DDX_Text(pDX, IDCE_DOTNUM_Y, m_nDotNumY);
	DDV_MinMaxInt(pDX, m_nDotNumY, 5, 200);
	DDX_Text(pDX, IDCE_START_X, m_nStartX);
	DDV_MinMaxInt(pDX, m_nStartX, 0, 100);
	DDX_Text(pDX, IDCE_START_Y, m_nStartY);
	DDV_MinMaxInt(pDX, m_nStartY, 0, 100);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgAddDrillTable, CDialog)
	//{{AFX_MSG_MAP(CDlgAddDrillTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDrillTable message handlers

void CDlgAddDrillTable::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
