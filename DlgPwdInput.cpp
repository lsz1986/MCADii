// DlgPwdInput.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPwdInput.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPwdInput dialog


CDlgPwdInput::CDlgPwdInput(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPwdInput::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPwdInput)
	m_strPwd = _T("");
	//}}AFX_DATA_INIT
}


void CDlgPwdInput::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPwdInput)
	DDX_Text(pDX, IDCE_PWD, m_strPwd);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPwdInput, CDialog)
	//{{AFX_MSG_MAP(CDlgPwdInput)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPwdInput message handlers
