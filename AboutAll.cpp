// AboutAll.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MCAD.h"
#include "AboutAll.h"
#include "afxdialogex.h"


// CAboutAll �Ի���

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


// CAboutAll ��Ϣ�������
