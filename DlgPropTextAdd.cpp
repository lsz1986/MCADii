// DlgPropTextAdd.cpp : 实现文件
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgPropTextAdd.h"
#include "afxdialogex.h"
#include "Global.h"

// CDlgPropTextAdd 对话框

IMPLEMENT_DYNAMIC(CDlgPropTextAdd, CDialog)

CDlgPropTextAdd::CDlgPropTextAdd(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgPropTextAdd::IDD, pParent)
	, m_strTextAdd(_T(""))
{
	m_dTextX = 0.0;
	m_dTextY = 0.0;
	m_dTextAngle = 0.0;
	m_nTextInflate = 5;
}

CDlgPropTextAdd::~CDlgPropTextAdd()
{
}

void CDlgPropTextAdd::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDCE_TEXT_X, m_dTextX);
	DDX_Text(pDX, IDCE_TEXT_Y, m_dTextY);
	DDX_Text(pDX, IDCE_TEXT_ANGLE, m_dTextAngle);
	DDV_MinMaxDouble(pDX, m_dTextAngle, -179.99, 180.0);
	DDX_Text(pDX, IDCE_TEXT_INFLATE, m_nTextInflate);
	DDV_MinMaxInt(pDX, m_nTextInflate, 0, 9);
	DDX_Text(pDX, IDCE_TEXT_CONTENT, m_strTextAdd);
}


BEGIN_MESSAGE_MAP(CDlgPropTextAdd, CDialog)
	ON_BN_CLICKED(IDOK, &CDlgPropTextAdd::OnBnClickedOk)
END_MESSAGE_MAP()


// CDlgPropTextAdd 消息处理程序
BOOL CDlgPropTextAdd::OnInitDialog()
{
	CDialog::OnInitDialog();
	// TODO:  在此添加额外的初始化
	m_dTextX = gTextProp.getTextPosX();
	m_dTextY = gTextProp.getTextPosY();
	m_dTextAngle = gTextProp.getTextAngle();
	m_nTextInflate = gTextProp.getTextInflate();
	m_strTextAdd = gTextProp.m_strTextAdd;
	GetDlgItem(IDCE_TEXT_CONTENT)->SetFocus();
	UpdateData(FALSE);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDlgPropTextAdd::OnBnClickedOk()
{
	UpdateData();
	gTextProp.setTextAngle(m_dTextAngle);
	gTextProp.setTextPosX(m_dTextX);
	gTextProp.setTextPosY(m_dTextY);
	gTextProp.setTextInflate(m_nTextInflate);
	gTextProp.m_strTextAdd = m_strTextAdd;
	CDialog::OnOK();
}
