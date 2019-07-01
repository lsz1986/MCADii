#if !defined(AFX_DLGPROPLINEADD_H__A8EF7A56_45E9_4477_92E9_DC321377DE80__INCLUDED_)
#define AFX_DLGPROPLINEADD_H__A8EF7A56_45E9_4477_92E9_DC321377DE80__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropLineAdd.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineAdd dialog

class CDlgPropLineAdd : public CDialog
{
// Construction
public:
	CDlgPropLineAdd(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropLineAdd)
	enum { IDD = IDDLG_PROP_LINE_ADD };
	double	m_dLineAddAngle;
	double	m_dLineAddRadius;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropLineAdd)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropLineAdd)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPLINEADD_H__A8EF7A56_45E9_4477_92E9_DC321377DE80__INCLUDED_)
