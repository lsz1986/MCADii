#if !defined(AFX_DLGOUTLINEPROP_H__250EABDB_567F_4636_BB6F_CD61BD758622__INCLUDED_)
#define AFX_DLGOUTLINEPROP_H__250EABDB_567F_4636_BB6F_CD61BD758622__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgPropOutline.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropOutline dialog

class CDlgPropOutline : public CDialog
{
// Construction
public:
	CDlgPropOutline(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropOutline)
	enum { IDD = IDDLG_PROP_OUTLINE1 };
	CComboBox	m_ctrlCutPos;
	double	m_dMmTop;
	double	m_dMmBot;
	double	m_dMmLeft;
	double	m_dMmRight;
	double	m_dMmRadius;
	BOOL	m_bLB;
	BOOL	m_bRB;
	BOOL	m_bRT;
	BOOL	m_bClockWise;
	BOOL	m_bLT;
	double	m_dCutDist;
	double	m_dCutDepth;
	double	m_dCutWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropOutline)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropOutline)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGOUTLINEPROP_H__250EABDB_567F_4636_BB6F_CD61BD758622__INCLUDED_)
