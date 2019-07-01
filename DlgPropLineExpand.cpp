// DlgPropLineExpand.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPropLineExpand.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineExpand dialog


CDlgPropLineExpand::CDlgPropLineExpand(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropLineExpand::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropLineExpand)
	m_nExpandBefore = 0;
	m_nExpandAfter = 0;
	//}}AFX_DATA_INIT
}


void CDlgPropLineExpand::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropLineExpand)
	DDX_Text(pDX, IDCE_BEFORE, m_nExpandBefore);
	DDX_Text(pDX, IDCE_AFTER, m_nExpandAfter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropLineExpand, CDialog)
	//{{AFX_MSG_MAP(CDlgPropLineExpand)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineExpand message handlers

void CDlgPropLineExpand::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
