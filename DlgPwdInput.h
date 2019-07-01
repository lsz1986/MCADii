#if !defined(AFX_DLGPWDINPUT_H__D3921CA3_D07D_4390_8924_6025750E0ACF__INCLUDED_)
#define AFX_DLGPWDINPUT_H__D3921CA3_D07D_4390_8924_6025750E0ACF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPwdInput.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPwdInput dialog

class CDlgPwdInput : public CDialog
{
// Construction
public:
	CDlgPwdInput(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPwdInput)
	enum { IDD = IDDLG_PWD };
	CString	m_strPwd;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPwdInput)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPwdInput)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPWDINPUT_H__D3921CA3_D07D_4390_8924_6025750E0ACF__INCLUDED_)
