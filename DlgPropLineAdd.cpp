// DlgPropLineAdd.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPropLineAdd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineAdd dialog


CDlgPropLineAdd::CDlgPropLineAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropLineAdd::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropLineAdd)
	m_dLineAddAngle = 0.0;
	m_dLineAddRadius = 0.0;
	//}}AFX_DATA_INIT
}


void CDlgPropLineAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropLineAdd)
	DDX_Text(pDX, IDCE_LINE_ANGLE, m_dLineAddAngle);
	DDX_Text(pDX, IDCE_LINE_LEN, m_dLineAddRadius);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropLineAdd, CDialog)
	//{{AFX_MSG_MAP(CDlgPropLineAdd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineAdd message handlers

void CDlgPropLineAdd::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
