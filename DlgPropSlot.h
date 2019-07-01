#if !defined(AFX_DLGPROPSLOT_H__122CC087_1C64_437F_A31C_99E9C8BD2F07__INCLUDED_)
#define AFX_DLGPROPSLOT_H__122CC087_1C64_437F_A31C_99E9C8BD2F07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DlgPropSlot.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDlgPropSlot dialog

class CDlgPropSlot : public CDialog
{
// Construction
public:
	CDlgPropSlot(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDlgPropSlot)
	enum { IDD = IDDLG_PROP_SLOT };
	double	m_dSlotWidth;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDlgPropSlot)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDlgPropSlot)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DLGPROPSLOT_H__122CC087_1C64_437F_A31C_99E9C8BD2F07__INCLUDED_)
