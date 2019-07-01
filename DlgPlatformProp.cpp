// DlgPlatformProp.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPlatformProp.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPlatformProp dialog


CDlgPlatformProp::CDlgPlatformProp(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPlatformProp::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPlatformProp)
	m_nGapX = 8;
	m_nPlatformXSize = 1200;
	m_nPlatformYSize = 900;
	m_nPlatformDir = 0;
	//}}AFX_DATA_INIT
}


void CDlgPlatformProp::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPlatformProp)
	DDX_Text(pDX, IDCE_GAP_X, m_nGapX);
	DDX_Text(pDX, IDCE_PLATFORM_XSIZE, m_nPlatformXSize);
	DDX_Text(pDX, IDCE_PLATFORM_YSIZE, m_nPlatformYSize);
	DDX_Text(pDX, IDCE_MMDIR, m_nPlatformDir);
	DDV_MinMaxUInt(pDX, m_nPlatformDir, 0, 1);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPlatformProp, CDialog)
	//{{AFX_MSG_MAP(CDlgPlatformProp)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPlatformProp message handlers

void CDlgPlatformProp::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
