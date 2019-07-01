#if !defined(AFX_DLGSETROTANGLE_H__BC7C24C6_0E3C_4197_8AA7_2472847AD248__INCLUDED_)
#define AFX_DLGSETROTANGLE_H__BC7C24C6_0E3C_4197_8AA7_2472847AD248__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CDlgGetInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgGetInput dialog

class CDlgGetInput : public CDialog
{
// Construction
public:
	CDlgGetInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgGetInput)
	enum { IDD = IDDLG_GET_INPUT };
	double		m_dInput;
	CString	m_strTips;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgGetInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgGetInput)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSETROTANGLE_H__BC7C24C6_0E3C_4197_8AA7_2472847AD248__INCLUDED_)
