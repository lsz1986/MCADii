// MainFrm.h : interface of the CMainFrame class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MAINFRM_H__D9FD893B_8D6A_4DAC_97B4_AC893513A007__INCLUDED_)
#define AFX_MAINFRM_H__D9FD893B_8D6A_4DAC_97B4_AC893513A007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#include "PieceListView.h"
#include "MCADView.h"
#include "MCADDoc.h"
#endif // _MSC_VER > 1000

#include "Global.h"

extern CWinThread* gThreadTimeDetect;
extern CWinThread* gThreadAutoConnect;
extern CWinThread* gThreadDemoDisplay;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
public:
	CMCADView* m_pMainView;
	CPieceListView* m_pListView;

// Operations
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL OnCheckSoftDog(int nCmd1,int nCmd2);
	int OnCheckReg();
	void SetMousePosText(CPoint ptmm);
	CMCADDoc* GetDocument();
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CToolBar    m_wndTbLine;
	CToolBar    m_wndTbPage;
	CSplitterWnd m_wndSplitter;
	CProgressCtrl m_wndProgress;
	
	afx_msg LRESULT OnDispSysStatus(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnEndDemoDisplay(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDemoDrawLine(WPARAM wParam, LPARAM lParam);
// Generated message map functions
protected:
	//{{AFX_MSG(CMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTbPage1();
	afx_msg void OnTbPage2();
	afx_msg void OnTbPage3();
	afx_msg void OnTbPage4();
	afx_msg void OnTbPage5();
	afx_msg void OnPltOpen();
	afx_msg void OnZoomAll();
	afx_msg void OnWorkStart();
	afx_msg void OnUpdateTbPage1(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbPage2(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbPage3(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbPage4(CCmdUI* pCmdUI);
	afx_msg void OnUpdateTbPage5(CCmdUI* pCmdUI);
	afx_msg void OnMacPara();
	afx_msg void OnMacTest();
	afx_msg void OnParaSoft();
	afx_msg void OnAppVideo();
	afx_msg void OnPaint();
	afx_msg void OnAuthUpdate();
	afx_msg void OnSuperUser();
	afx_msg void OnAppManual();
	afx_msg void OnTbSelCurve();
	afx_msg void OnSelPiece();
	afx_msg void OnUpdatePiece(CCmdUI* pCmdUI);
	afx_msg void OnUpdateLine(CCmdUI* pCmdUI);
	afx_msg void OnUpdateManCtrl(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMacPara(CCmdUI* pCmdUI);
	afx_msg void OnFileAddSnakeLine();
	afx_msg void OnUpdateStart(CCmdUI* pCmdUI);
	afx_msg void OnWorkPause();
	afx_msg void OnUpdatePause(CCmdUI* pCmdUI);
	afx_msg void OnWorkCancel();
	afx_msg void OnUpdateCancel(CCmdUI* pCmdUI);
	afx_msg void OnFileAddDrillTable();
	afx_msg void OnFileExportPlt();
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI* pCmdUI);
	afx_msg void OnAdrill();
	afx_msg void OnUpdateAdrill(CCmdUI* pCmdUI);
	afx_msg void OnDrawCurve();
	afx_msg void OnUpdateDrawCurve(CCmdUI* pCmdUI);
	afx_msg void OnDrawRect();
	afx_msg void OnUpdateDrawRect(CCmdUI* pCmdUI);
	afx_msg void OnCutSeq();
	afx_msg void OnUpdateCutSeq(CCmdUI* pCmdUI);
	afx_msg void OnFileAddMpTest();
	afx_msg void OnSelAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnIdtbsAddText();
	afx_msg void OnUpdateIdtbsAddText(CCmdUI *pCmdUI);
	afx_msg void OnIdcbDemoStart();
	afx_msg void OnUpdateIdcbDemoStart(CCmdUI *pCmdUI);

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MAINFRM_H__D9FD893B_8D6A_4DAC_97B4_AC893513A007__INCLUDED_)
