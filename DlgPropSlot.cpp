// DlgPropSlot.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPropSlot.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropSlot dialog


CDlgPropSlot::CDlgPropSlot(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropSlot::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropSlot)
	m_dSlotWidth = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgPropSlot::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropSlot)
	DDX_Text(pDX, IDCE_SLOT_WIDTH, m_dSlotWidth);
	DDV_MinMaxDouble(pDX, m_dSlotWidth, 0., 50.);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropSlot, CDialog)
	//{{AFX_MSG_MAP(CDlgPropSlot)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropSlot message handlers
