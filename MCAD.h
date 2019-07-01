// MCAD.h : main header file for the MCAD application
//

#if !defined(AFX_MCAD_H__CE2A312B_6A6F_4071_A29A_B32F5A99423A__INCLUDED_)
#define AFX_MCAD_H__CE2A312B_6A6F_4071_A29A_B32F5A99423A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CMCADApp:
// See MCAD.cpp for the implementation of this class
//

class CMCADApp : public CWinApp
{
public:
	BOOL DeleteDirectory(char* psDirName);
	void WriteSettings();
	void ReadSettings();
	CMCADApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCADApp)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CMCADApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCAD_H__CE2A312B_6A6F_4071_A29A_B32F5A99423A__INCLUDED_)
