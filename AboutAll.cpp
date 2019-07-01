// AboutAll.cpp : 实现文件
//

#include "stdafx.h"
#include "MCAD.h"
#include "AboutAll.h"
#include "afxdialogex.h"


// CAboutAll 对话框

IMPLEMENT_DYNAMIC(CAboutAll, CDialog)

CAboutAll::CAboutAll(CWnd* pParent /*=NULL*/)
	: CDialog(IDD_ABOUT_ALL, pParent)
{

}

CAboutAll::~CAboutAll()
{
}

void CAboutAll::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutAll, CDialog)
END_MESSAGE_MAP()


// CAboutAll 消息处理程序
