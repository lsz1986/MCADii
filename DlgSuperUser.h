#if !defined(AFX_DLGSUPERUSER_H__381E0E26_9AA0_49B2_9080_C77FB8814DC5__INCLUDED_)
#define AFX_DLGSUPERUSER_H__381E0E26_9AA0_49B2_9080_C77FB8814DC5__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgSuperUser.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgSuperUser dialog

class CDlgSuperUser : public CDialog
{
// Construction
public:
	int m_nRandSN;
	CDlgSuperUser(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgSuperUser)
	enum { IDD = IDDLG_SUPER_USER };
	CString	m_strRandCode;
	int		m_nRandPw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgSuperUser)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgSuperUser)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGSUPERUSER_H__381E0E26_9AA0_49B2_9080_C77FB8814DC5__INCLUDED_)
