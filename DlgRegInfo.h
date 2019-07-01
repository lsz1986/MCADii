#if !defined(AFX_DLGREGINFO_H__A2579A39_0A26_4C3C_8F40_4DAC24314A3A__INCLUDED_)
#define AFX_DLGREGINFO_H__A2579A39_0A26_4C3C_8F40_4DAC24314A3A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgRegInfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgRegInfo dialog

class CDlgRegInfo : public CDialog
{
// Construction
public:

	CDlgRegInfo(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgRegInfo)
	enum { IDD = IDDLG_REG };
	CString	m_strPasswd;
	CString	m_strAuthCode;
	CString	m_strAuthSn;
	CString	m_strEndDate;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgRegInfo)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgRegInfo)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	afx_msg void OnInputRegCode();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGREGINFO_H__A2579A39_0A26_4C3C_8F40_4DAC24314A3A__INCLUDED_)
