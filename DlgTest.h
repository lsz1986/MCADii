#if !defined(AFX_DLGTEST_H__D0F6E6A4_4698_44EA_9E1C_90FF32364A3E__INCLUDED_)
#define AFX_DLGTEST_H__D0F6E6A4_4698_44EA_9E1C_90FF32364A3E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgTest.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgTest dialog

class CDlgTest : public CDialog
{
// Construction
public:
	CDlgTest(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgTest)
	enum { IDD = IDDLG_TEST };
	int		m_nTestPwmPlot;
	int		m_nTestPwmCut;
	int		m_nTestZPos;
	BOOL	m_bTestPD;
	BOOL	m_bTestCD;
	BOOL	m_bTestMD;
	BOOL	m_bTestMmOn;
	BOOL	m_bTestVmOn;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgTest)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgTest)
	afx_msg void OnTestCd();
	afx_msg void OnTestMd();
	afx_msg void OnTestMmon();
	afx_msg void OnTestPd();
	afx_msg void OnTestVmOn();
	virtual void OnCancel();
	afx_msg void OnToDefzp();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedSavepp();
	afx_msg void OnBnClickedSavecp();
	afx_msg void OnBnClickedSaveMm();
	afx_msg void OnBnClickedSaveWm();
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGTEST_H__D0F6E6A4_4698_44EA_9E1C_90FF32364A3E__INCLUDED_)
