#if !defined(AFX_DLGPROPRECT_H__A955F418_16EF_4990_AF39_4D408D16268F__INCLUDED_)
#define AFX_DLGPROPRECT_H__A955F418_16EF_4990_AF39_4D408D16268F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropRect.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropRect dialog

class CDlgPropRect : public CDialog
{
// Construction
public:
	CDlgPropRect(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropRect)
	enum { IDD = IDDLG_PROP_RECT };
	int	m_nRectSizeX;
	int	m_nRectSizeY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropRect)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropRect)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPRECT_H__A955F418_16EF_4990_AF39_4D408D16268F__INCLUDED_)
