// MCADDoc.h : interface of the CMCADDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_MCADDOC_H__E63786AF_0D6F_42DB_BA14_9F52FF1D5480__INCLUDED_)
#define AFX_MCADDOC_H__E63786AF_0D6F_42DB_BA14_9F52FF1D5480__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "HpglFile.h"
#include "Piece.h"

class CMCADDoc : public CDocument
{
protected: // create from serialization only
	CMCADDoc();
	DECLARE_DYNCREATE(CMCADDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMCADDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual void DeleteContents();
	//}}AFX_VIRTUAL

// Implementation
public:
	void OnAddString();
	void OnRemoveDupPoints();
	void OnSearchOutLine();
	void OnSearchPieceStep1();
	void OnSearchPieceStep2();
	void PieceGetInnerLine(CPiece* pPiece);
	void OnImportCurveFromPlt(CHpglFile* pHpglFile);
	void OnImportCurveFromText(CHpglFile* pHpglFile,double dStartX,double dStartY,int nScale);
	virtual ~CMCADDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

private:

public:
	void OnSelAllCurve();
	void OnSelCurveToDrill();
	void OnRemoveCurveNotMM();
	void OnRemoveCurveNotOutline();
	void OnSearchCutData();
	void OnAddMpTestFile();
	void OnResetCutSeq();
	void AllPieceToList();
	void OnSetCurveTypeFromPenNum();
	void OnAddDrill(CPoint ptScr);
	void OnGetLineFromPieces();
	void OnExportToPlt(CString strFileName);
	void OnAddSnakeLineByX(int nXSize,int nYSize,int nXGap);
	void OnAddSnakeLineByY(int nXSize,int nYSize,int nYGap);
	void OnAddDrillTable(int StartX,int StartY,int XNum,int YNum,int GapX,int GapY);

	void OnGetMaxX();
	void OnWorkPageAngleAdjust();
	void OnPieceCombine();

	void OnAddOutLine1();
	void OnAddOutLine2();

	void GetCncData();
	void OnCopyWorkPieces();
	int CreateCncList();
	CPiece* GetPieceWhichCurveSeled();
	void OnSelCurveCombine12();
	void OnSelCurveCombine23();
	void OnSelCurveDel();
	void OnSelCurveToPoly();
	void OnSelCurveToPen();
	void OnSelCurveToMM();
	void OnSelCurveReverse();
	CCurve* GetSelCurve();
	void FindCurveOnPoint(CPoint pt);
	void FindCurvesInRect(CRect rect);
	void OnClearCurveSelState();
	void OnResetPieceSelState();
	void SelPieceToList();
	void SelPieceRemove();
	int m_nCurPage;
	int m_nCurTool;
	int m_nMaxX;
//--------------------备份恢复用-------------------
	int m_backup_index;
	CStringArray m_backup;
	void ReStore(BOOL backward);
	void BackUp();
//-------------------------------------------------
	CTypedPtrArray <CObArray, CPiece *> m_PiecesInList;    //列表中的裁片
	CTypedPtrArray <CObArray, CPiece *> m_PiecesInMain[5]; //
	CTypedPtrArray <CObArray, CCurve *> m_CurveInMain[5];   //未识别的曲线
	CTypedPtrArray <CObArray, CCurve *> m_CurvesWork;   //工作数据缓存

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CMCADDoc)
	afx_msg void OnEditRedo();
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditRedo(CCmdUI* pCmdUI);
	afx_msg void OnUpdateEditUndo(CCmdUI* pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	void OnSelCurveToSlot(double d);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MCADDOC_H__E63786AF_0D6F_42DB_BA14_9F52FF1D5480__INCLUDED_)
