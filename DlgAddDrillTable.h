#if !defined(AFX_DLGADDDRILLTABLE_H__FC585B27_A1C9_4179_AC36_466C00E34CE0__INCLUDED_)
#define AFX_DLGADDDRILLTABLE_H__FC585B27_A1C9_4179_AC36_466C00E34CE0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgAddDrillTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgAddDrillTable dialog

class CDlgAddDrillTable : public CDialog
{
// Construction
public:
	CDlgAddDrillTable(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgAddDrillTable)
	enum { IDD = IDDLG_PROP_DRILLTABLE };
	int		m_nDotNumX;
	int		m_nGapXmm;
	int		m_nGapYmm;
	int		m_nDotNumY;
	int		m_nStartX;
	int		m_nStartY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgAddDrillTable)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgAddDrillTable)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGADDDRILLTABLE_H__FC585B27_A1C9_4179_AC36_466C00E34CE0__INCLUDED_)
