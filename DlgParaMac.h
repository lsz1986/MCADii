#if !defined(AFX_DLGSETUP_H__4630DFA4_B85F_4849_B24D_85D4ABD86488__INCLUDED_)
#define AFX_DLGSETUP_H__4630DFA4_B85F_4849_B24D_85D4ABD86488__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSetup.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParaMac dialog

class CDlgParaMac : public CDialog
{
// Construction
public:
	void OnUpdateParaGlobal();
	void OnUpdateParaToCtrl();
	CDlgParaMac(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParaMac)
	enum { IDD = IDDLG_PARA_MAC };
	int		m_nMpDistX;
	int		m_nMpDistY;
	int		m_nZPosMove;
	int		m_nZPosWork;
	int		m_nPwmPlot;
	int		m_nSpdMilling;
	int		m_nSpdMove;
	int		m_nSpdPlot;
	UINT	m_n10000X;
	UINT	m_n10000Y;
	int		m_nKpDistX;
	int		m_nKpDistY;
	int		m_nPwmCut;
	int		m_nSpdCut;
	BOOL	m_bVMEnable;
	int		m_iAngleAdjust;
	int		m_nMMotorDelay;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParaMac)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParaMac)
	virtual BOOL OnInitDialog();
	afx_msg void OnProcpara();
	afx_msg void OnParaSave();
	afx_msg void OnParaRestore();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
//	BOOL m_bCutAllInnerLineFirst;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETUP_H__4630DFA4_B85F_4849_B24D_85D4ABD86488__INCLUDED_)
