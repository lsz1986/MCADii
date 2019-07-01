#pragma once


// CAboutAll 对话框

class CAboutAll : public CDialog
{
	DECLARE_DYNAMIC(CAboutAll)

public:
	CAboutAll(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CAboutAll();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUT_ALL };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
