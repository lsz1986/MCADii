#pragma once


// CAboutAll �Ի���

class CAboutAll : public CDialog
{
	DECLARE_DYNAMIC(CAboutAll)

public:
	CAboutAll(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CAboutAll();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_ALL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
