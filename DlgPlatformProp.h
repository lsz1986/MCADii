#if !defined(AFX_DLGPLATFORMPROP_H__1B729B1D_BD1D_46B4_A57D_7B8B49554356__INCLUDED_)
#define AFX_DLGPLATFORMPROP_H__1B729B1D_BD1D_46B4_A57D_7B8B49554356__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPlatformProp.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPlatformProp dialog

class CDlgPlatformProp : public CDialog
{
// Construction
public:
	CDlgPlatformProp(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPlatformProp)
	enum { IDD = IDDLG_PROP_MEANDERLINE };
	UINT	m_nGapX;
	UINT	m_nPlatformXSize;
	UINT	m_nPlatformYSize;
	UINT	m_nPlatformDir;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPlatformProp)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPlatformProp)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPLATFORMPROP_H__1B729B1D_BD1D_46B4_A57D_7B8B49554356__INCLUDED_)
