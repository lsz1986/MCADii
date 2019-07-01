// DlgSetRotAngle.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgGetInput.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgGetInput dialog


CDlgGetInput::CDlgGetInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgGetInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgGetInput)
	m_dInput = 0.0;
	m_strTips = _T("");
	//}}AFX_DATA_INIT
}


void CDlgGetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgGetInput)
	DDX_Text(pDX, IDCE_D_INPUT, m_dInput);
	DDX_Text(pDX, IDTXT_INPUT, m_strTips);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgGetInput, CDialog)
	//{{AFX_MSG_MAP(CDlgGetInput)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgGetInput message handlers
void CDlgGetInput::OnOK() 
{
	UpdateData(TRUE);
	CDialog::OnOK();
}
