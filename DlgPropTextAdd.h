#pragma once


// CDlgPropTextAdd �Ի���

class CDlgPropTextAdd : public CDialog
{
	DECLARE_DYNAMIC(CDlgPropTextAdd)

public:
	CDlgPropTextAdd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CDlgPropTextAdd();

// �Ի�������
	enum { IDD = IDDLG_PROP_TEXT_ADD };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	double m_dTextX;
	double m_dTextY;
	double m_dTextAngle;
	int m_nTextInflate;

	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	CString m_strTextAdd;
};
