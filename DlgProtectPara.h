#if !defined(AFX_DLGPROTECTPARA_H__2981D9F5_888D_4533_9072_45FC4C57D76F__INCLUDED_)
#define AFX_DLGPROTECTPARA_H__2981D9F5_888D_4533_9072_45FC4C57D76F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgProtectPara.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgProtectPara dialog

class CDlgProtectPara : public CDialog
{
// Construction
public:
	CDlgProtectPara(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgProtectPara)
	enum { IDD = IDDLG_PARA_PROTECT };
	UINT	m_sXSizeMm;
	UINT	m_sYSizeMm;
	double	m_dPPMMX;
	double	m_dPPMMY;
	double	m_dPPMMZ;
	int		m_nWorkStartX;
	int		m_nWorkStartY;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgProtectPara)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgProtectPara)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROTECTPARA_H__2981D9F5_888D_4533_9072_45FC4C57D76F__INCLUDED_)
