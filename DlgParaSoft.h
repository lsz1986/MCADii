#if !defined(AFX_DLGPARASOFT_H__AEFB7602_871F_4BFE_A8E6_A12B93618608__INCLUDED_)
#define AFX_DLGPARASOFT_H__AEFB7602_871F_4BFE_A8E6_A12B93618608__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgParaSoft.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgParaSoft dialog

class CDlgParaSoft : public CDialog
{
// Construction
public:
	CDlgParaSoft(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgParaSoft)
	enum { IDD = IDDLG_PARA_SOFT };
	int		m_nSizeX;
	int		m_nSizeY;
	int		m_nDrillerDimension;
	int		m_nExpCutNum;
	int		m_nExpDrillNum;
	int		m_nExpPenNum;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgParaSoft)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgParaSoft)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPARASOFT_H__AEFB7602_871F_4BFE_A8E6_A12B93618608__INCLUDED_)
