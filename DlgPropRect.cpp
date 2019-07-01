// DlgPropRect.cpp : implementation file
//

#include "stdafx.h"
#include "mcad.h"
#include "DlgPropRect.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRect dialog


CDlgPropRect::CDlgPropRect(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropRect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgPropRect)
	m_nRectSizeX = 0;
	m_nRectSizeY = 0;
	//}}AFX_DATA_INIT
}


void CDlgPropRect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgPropRect)
	DDX_Text(pDX, IDCE_RECT_SIZE_X, m_nRectSizeX);
	DDX_Text(pDX, IDCE_RECT_SIZE_Y, m_nRectSizeY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgPropRect, CDialog)
	//{{AFX_MSG_MAP(CDlgPropRect)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRect message handlers

void CDlgPropRect::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData();
	CDialog::OnOK();
}
