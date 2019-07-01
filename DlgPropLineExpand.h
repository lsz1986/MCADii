#if !defined(AFX_DLGPROPLINEEXPAND_H__9A871575_88AE_494E_8183_67674F03D678__INCLUDED_)
#define AFX_DLGPROPLINEEXPAND_H__9A871575_88AE_494E_8183_67674F03D678__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropLineExpand.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropLineExpand dialog

class CDlgPropLineExpand : public CDialog
{
// Construction
public:
	CDlgPropLineExpand(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropLineExpand)
	enum { IDD = IDDLG_PROP_LINE_INFLATE };
	int		m_nExpandBefore;
	int		m_nExpandAfter;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropLineExpand)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropLineExpand)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPLINEEXPAND_H__9A871575_88AE_494E_8183_67674F03D678__INCLUDED_)
