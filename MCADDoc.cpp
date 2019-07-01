// MCADDoc.cpp : implementation of the CMCADDoc class
//

#include "stdafx.h"
#include "MCAD.h"
#include "Cnc.h"
#include "MCADDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int gNoMeaning=0;
/////////////////////////////////////////////////////////////////////////////
// CMCADDoc

IMPLEMENT_DYNCREATE(CMCADDoc, CDocument)

BEGIN_MESSAGE_MAP(CMCADDoc, CDocument)
	//{{AFX_MSG_MAP(CMCADDoc)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCADDoc construction/destruction

CMCADDoc::CMCADDoc()
{
	m_nCurPage = 0;
	m_nCurTool = TOOL_SEL_CURVE;
	m_backup_index = 0;
//	BackUp();
	SetModifiedFlag(FALSE);
}

CMCADDoc::~CMCADDoc()
{
	//backup information
	for(int i = 0; i < m_backup.GetSize(); i ++){
		CFile::Remove(m_backup.GetAt(i));
	}
	m_backup.RemoveAll();
	DeleteContents();
}

void CMCADDoc::DeleteContents() 
{
	int i;
	CPiece *pPiece;
	CCurve *pCurve;
	for(i=0;i<5;i++)
	{
		while(m_PiecesInMain[i].GetSize() > 0)
		{
			pPiece = m_PiecesInMain[i].GetAt(0);
			delete pPiece;
			m_PiecesInMain[i].RemoveAt(0);
		}
		while(m_CurveInMain[i].GetSize() > 0)
		{
			pCurve = m_CurveInMain[i].GetAt(0);
			delete pCurve;
			m_CurveInMain[i].RemoveAt(0);
		}
	}
	CDocument::DeleteContents();
}

BOOL CMCADDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)
	return TRUE;
}

#define MAX_BACKUP_NUM 40
void CMCADDoc::BackUp()
{
	CString fileName;

	int i;
	fileName.Format("MCADV2temp.%02d", m_backup.GetSize());
	fileName = gSet.m_strTempDir+fileName;
	
	OnSaveDocument(fileName);
	m_backup.InsertAt(m_backup_index, fileName);
	SetModifiedFlag(TRUE);
	m_backup_index++;

	if (m_backup_index > MAX_BACKUP_NUM)
	{
		CString fileNameOld;
		CString fileNameNew;
		DeleteFile(m_backup.GetAt(0));
		m_backup.RemoveAll();

		for(i=0;i<MAX_BACKUP_NUM;i++)
		{
			fileNameNew.Format("MCADV2temp.%02d", i);
			fileNameOld.Format("MCADV2temp.%02d", i+1);
			fileNameNew = gSet.m_strTempDir+fileNameNew;
			fileNameOld = gSet.m_strTempDir+fileNameOld;
			rename(fileNameOld,fileNameNew);
			m_backup.Add(fileNameNew);
		}
		m_backup_index = MAX_BACKUP_NUM;
	}
}

void CMCADDoc::ReStore(BOOL backward)
{
	m_backup_index -= backward ? 1 : -1;
	OnOpenDocument(m_backup.GetAt(m_backup_index-1));
}

/////////////////////////////////////////////////////////////////////////////
// CMCADDoc serialization

void CMCADDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		ar<<m_nCurPage<<m_nCurTool;
	}
	else
	{
		ar>>m_nCurPage>>m_nCurTool;
	}
	m_PiecesInList.Serialize(ar);
	for(int i=0;i<5;i++)
	{
		m_CurveInMain[i].Serialize(ar);
		m_PiecesInMain[i].Serialize(ar);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CMCADDoc diagnostics

#ifdef _DEBUG
void CMCADDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMCADDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMCADDoc commands

void CMCADDoc::OnImportCurveFromText(CHpglFile* pHpglFile,double dStartX,double dStartY,int nScale)
{
	CPoint pt,lastPt;
	int i,j;
	int nPenNum;
	int nCmdParaSize;
	CPlotCmd *pPlotCmd;
	int nOrgPageNum=1;
	int nStartX;
	CCurve *pCurve;
	pCurve = new CCurve;
	nPenNum = 1;
	nStartX = 0;
	pCurve->m_nPenNumInPlt = nPenNum;

	for (i=0;i<pHpglFile->m_pCmdList_ORG.GetSize();i++)
	{
		pPlotCmd = (CPlotCmd*)pHpglFile->m_pCmdList_ORG.GetAt(i);
		switch(pPlotCmd->m_nType)
		{
		case PLOT_CMD_PD:
			nCmdParaSize = pPlotCmd->m_anData.GetSize();
			if ( nCmdParaSize >=2 )
			{
				for (j=0;j<nCmdParaSize;j+=2)
				{
					pt.x = nStartX + pPlotCmd->m_anData.GetAt(j);
					pt.y = pPlotCmd->m_anData.GetAt(j+1);

					pt.x = (int)(dStartX+(pt.x-dStartX)*(1.0+nScale));
					pt.y = (int)(dStartY+(pt.y-dStartY)*(1.0+nScale));

					pCurve->m_pointList->Add(pt);
				}
			}
			break;

		case PLOT_CMD_PU:
			nCmdParaSize = (pPlotCmd->m_anData).GetSize();
			if (nCmdParaSize >= 2) //指令有效
			{
				pt.x = nStartX + pPlotCmd->m_anData.GetAt(0);
				pt.y = pPlotCmd->m_anData.GetAt(1);

				if( pCurve->m_pointList->GetSize() > 0)
				{
					lastPt = pCurve->m_pointList->GetAt(pCurve->m_pointList->GetSize()-1);

					if ( (getdistance(lastPt,pt) > 4.0) || //移动的目标点和曲线的最后一个点坐标不同
						(pCurve->m_nPenNumInPlt != nPenNum)) //或者笔号发生了变化
					{
						m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
						pCurve = new CCurve;
						pCurve->m_nPenNumInPlt = nPenNum;
					}
					else
					{
						pCurve->m_pointList->RemoveAt(pCurve->m_pointList->GetSize()-1);
					}
				}
				pt.x = (int)(dStartX+(pt.x-dStartX)*(1.0+nScale));
				pt.y = (int)(dStartY+(pt.y-dStartY)*(1.0+nScale));
				pCurve->m_pointList->Add(pt);
			}
			break;
		}
	}
	if(pCurve->m_pointList->GetSize()>0)
	{
		m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
	}
	else
	{
		delete pCurve;
	}
}

void CMCADDoc::OnImportCurveFromPlt(CHpglFile* pHpglFile)
{
	CPoint pt,lastPt;
	int i,j;
	int nPenNum;
	int nCmdParaSize;
	CPlotCmd *pPlotCmd;
	int nOrgPageNum=1;
	int nStartX;
	CCurve *pCurve;
	pCurve = new CCurve;
	nPenNum = 1;
	nStartX = 0;
	pCurve->m_nPenNumInPlt = nPenNum;

	for (i=0;i<pHpglFile->m_pCmdList_ORG.GetSize();i++)
	{
		pPlotCmd = (CPlotCmd*)pHpglFile->m_pCmdList_ORG.GetAt(i);
		switch(pPlotCmd->m_nType)
		{
			case PLOT_CMD_PD:
			nCmdParaSize = pPlotCmd->m_anData.GetSize();
			if ( nCmdParaSize >=2 )
			{
				for (j=0;j<nCmdParaSize;j+=2)
				{
					pt.x = nStartX + pPlotCmd->m_anData.GetAt(j);
					pt.y = pPlotCmd->m_anData.GetAt(j+1);
					pCurve->m_pointList->Add(pt);
				}
			}
			break;

			case PLOT_CMD_PU:
			nCmdParaSize = (pPlotCmd->m_anData).GetSize();
			if (nCmdParaSize >= 2) //指令有效
			{
				pt.x = nStartX + pPlotCmd->m_anData.GetAt(0);
				pt.y = pPlotCmd->m_anData.GetAt(1);
				
				if( pCurve->m_pointList->GetSize() > 0)
				{
					lastPt = pCurve->m_pointList->GetAt(pCurve->m_pointList->GetSize()-1);

					if ( (getdistance(lastPt,pt) > 4.0) || //移动的目标点和曲线的最后一个点坐标不同
						(pCurve->m_nPenNumInPlt != nPenNum)) //或者笔号发生了变化
					{
						m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
						pCurve = new CCurve;
						pCurve->m_nPenNumInPlt = nPenNum;
					}
					else
					{
						pCurve->m_pointList->RemoveAt(pCurve->m_pointList->GetSize()-1);
					}
				}
				pCurve->m_pointList->Add(pt);
			}
			break;

			case PLOT_CMD_SP:
			if (pPlotCmd->m_anData.GetSize() == 0)
			{
				nPenNum = 1;
			}
			else
			{
				nPenNum = pPlotCmd->m_anData.GetAt(0);
			}
			
			if (pCurve->m_nPenNumInPlt != nPenNum) //笔号发生了变化
			{
				if(pCurve->m_pointList->GetSize() > 0)
				{
					m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
					pCurve = new CCurve;
				}
				pCurve->m_nPenNumInPlt = nPenNum;
			}
			break;

			case PLOT_CMD_PG:
			if(pCurve->m_pointList->GetSize()>0)
			{
				m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
			}
			if (m_CurveInMain[m_nCurPage].GetSize()>0)
			{
				OnGetMaxX();
				nStartX = m_nMaxX;
			}
			pCurve = new CCurve;
			nPenNum = 1;
			pCurve->m_nPenNumInPlt = nPenNum;
			break;
		}
	}
	if(pCurve->m_pointList->GetSize()>0)
	{
		m_CurveInMain[m_nCurPage].Add((CCurve *)pCurve);
	}
	else
	{
		delete pCurve;
	}
}

void CMCADDoc::OnSetCurveTypeFromPenNum()
{
	int i;
	CCurve* pCurve;
	int nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if(pCurve->m_nPenNumInPlt <= 1)
		{
			pCurve->m_nCurveType = CURVE_TYPE_PEN;
		}
		else
		{
			pCurve->m_nCurveType = CURVE_TYPE_MM;
			if (pCurve->m_pointList->GetSize() < 2)
			{
				pCurve->m_nCurveType = CURVE_TYPE_DRILL;
			}
		}
	}
}

void CMCADDoc::OnRemoveDupPoints()
{
	CCurve *pCurve;
	int nCurveSum,nCurveIndex;
	int nCurveSize;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	if (nCurveSum == 0)
		return;
	
	for (nCurveIndex=0; nCurveIndex<nCurveSum; nCurveIndex++)
	{
		pCurve = (CCurve *)m_CurveInMain[m_nCurPage].GetAt(nCurveIndex);
		pCurve->OnRemoveDupPoints(); //检查移除重合的点
		nCurveSize = pCurve->m_pointList->GetSize();
		if ( nCurveSize <= 0)
		{
			delete pCurve;
			m_CurveInMain[m_nCurPage].RemoveAt(nCurveIndex);
			nCurveSum--;
			nCurveIndex--;
		}
		else if (nCurveSize <= 1)
		{
			if (pCurve->m_nPenNumInPlt <= 1 )
			{
				delete pCurve;
				m_CurveInMain[m_nCurPage].RemoveAt(nCurveIndex);
				nCurveSum--;
				nCurveIndex--;
			}
		}
	}
}

void CMCADDoc::OnSearchOutLine()
{
	CCurve *pCurve;
	int nCurveSum,nCurveIndex;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	if (nCurveSum == 0)
		return;
	
	for (nCurveIndex=0; nCurveIndex<nCurveSum; nCurveIndex++)
	{
		pCurve = (CCurve *)m_CurveInMain[m_nCurPage].GetAt(nCurveIndex);
		pCurve->OnCheckCurveIsPolygon();
		pCurve->GetCurveMaxMin();
		pCurve->CheckIsPieceOutLine(); //滤除小片
	}
}

void CMCADDoc::PieceGetInnerLine(CPiece* pPiece)
{
	int i;
	int nCurveSum; //未识别的曲线总数
	CCurve* pCurve;

	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = (CCurve*)m_CurveInMain[m_nCurPage].GetAt(i);
		if (pPiece->CurveInPiece(pCurve))
		{
			pPiece->m_CurveList.Add(pCurve);
			m_CurveInMain[m_nCurPage].RemoveAt(i);
			i--;
			nCurveSum--;
		}
	}
}

void CMCADDoc::OnSearchPieceStep2()
{
	int i;
	CPiece *pPiece;
	for (i=0; i<m_PiecesInMain[m_nCurPage].GetSize(); i++)
	{
		pPiece = (CPiece *)m_PiecesInMain[m_nCurPage].GetAt(i);
		PieceGetInnerLine(pPiece);
	}
}

#include "DlgPropOutline.h"
void CMCADDoc::OnPieceCombine()
{
	CPiece *pPiece0;
	CPiece *pPiece1;
	int nPieceNum,nPieceSum;
	int nCurveNum,nCurveSum;
	nPieceSum =  m_PiecesInMain[m_nCurPage].GetSize();
	for (nPieceNum=0;nPieceNum < nPieceSum;nPieceNum++)
	{
		pPiece0 = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
		if (1 == pPiece0->m_nPieceSelStat)
		{
			break;
		}
	}

	nPieceNum++;
	for (; nPieceNum<nPieceSum; nPieceNum++)
	{
		pPiece1 = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
		if (1 == pPiece1->m_nPieceSelStat ) //裁片选中
		{
			nCurveSum = (pPiece1->m_CurveList).GetSize();
			for (nCurveNum=0; nCurveNum<nCurveSum ; nCurveNum++)
			{
				CCurve* pCurve = new CCurve;
				*pCurve = *( (pPiece1->m_CurveList).GetAt(nCurveNum) );

				pPiece0->m_CurveList.Add(pCurve);
			}
			delete pPiece1;
			m_PiecesInMain[m_nCurPage].RemoveAt(nPieceNum);
			nPieceNum--;
			nPieceSum--;
		}
	}	
}

void CMCADDoc::OnAddOutLine1()
{
	int nPieceNum,nPieceSum;
	BOOL bPieceSeled;
	CPiece *pPiece;

	bPieceSeled = FALSE;
	nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	for (nPieceNum=0;nPieceNum < nPieceSum;nPieceNum++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
		if (1 == pPiece->m_nPieceSelStat ) //有裁片选中
		{
			bPieceSeled = TRUE;
		}
	}

	if(FALSE == bPieceSeled) //无裁片被选中
		return;

	CDlgPropOutline dlg;
	if (IDOK == dlg.DoModal() ) //确定按钮
	{
		OnPieceCombine();

		nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
		for (nPieceNum=0;nPieceNum < nPieceSum;nPieceNum++)
		{
			pPiece = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
			if (1 == pPiece->m_nPieceSelStat ) //有裁片选中
			{
				pPiece->ReCalcPieceSize();
				pPiece->AddOutLineType1();
				pPiece->ReCalcPieceSize();
				break;
			}
		}
	}
}

#include "DlgGetInput.h"
void CMCADDoc::OnAddOutLine2()
{
	int nPieceNum,nPieceSum;
	BOOL bPieceSeled;
	CPiece *pPiece;
	
	bPieceSeled = FALSE;
	nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	for (nPieceNum=0;nPieceNum < nPieceSum;nPieceNum++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
		if (1 == pPiece->m_nPieceSelStat ) //有裁片选中
		{
			bPieceSeled = TRUE;
		}
	}
	
	if(FALSE == bPieceSeled) //无裁片被选中
		return;
	
	CDlgGetInput dlg;
	dlg.m_dInput = 20;
	dlg.m_strTips.Format("外框与边界距离(mm):");

	if (IDOK == dlg.DoModal() ) //确定按钮
	{
		nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
		for (nPieceNum=0; nPieceNum < nPieceSum; nPieceNum++)
		{
			pPiece = m_PiecesInMain[m_nCurPage].GetAt(nPieceNum);
			if (1 == pPiece->m_nPieceSelStat ) //有裁片选中
			{
				pPiece->OnGetPieceOutLine();
				pPiece->AddOutLineType2(dlg.m_dInput);
				pPiece->ReCalcPieceSize();
				break;
			}
		}
	}
}

void CMCADDoc::OnSearchPieceStep1()
{
	int nSizeCurveList;
	CCurve* pCurve;
	CPiece* pPiece;
	nSizeCurveList = m_CurveInMain[m_nCurPage].GetSize();
	for (int i=0;i<nSizeCurveList;i++)
	{
		pCurve = (CCurve*)m_CurveInMain[m_nCurPage].GetAt(i);
		if(pCurve->m_bIsOutline)
		{
			pPiece=new CPiece;
			pPiece->m_CurveList.Add(pCurve);
			m_PiecesInMain[m_nCurPage].Add(pPiece);
			pPiece->m_nXmin = pCurve->m_nXmin;
			pPiece->m_nXmax = pCurve->m_nXmax;
			pPiece->m_nYmin = pCurve->m_nYmin;
			pPiece->m_nYmax = pCurve->m_nYmax;

			m_CurveInMain[m_nCurPage].RemoveAt(i);
			i--;
			nSizeCurveList--;
		}
	}
}

void CMCADDoc::SelPieceRemove()
{
	int i;
	CPiece* pPiece;
	int nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	for (i=0;i<nPieceSum;i++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(i);
		if (1 == pPiece->m_nPieceSelStat )
		{
			delete pPiece;
			m_PiecesInMain[m_nCurPage].RemoveAt(i);
			i--;
			nPieceSum--;
		}
	}
}

void CMCADDoc::SelPieceToList()
{
	int i;
	CPiece* pPiece;
	int nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();

	for (i=0;i<nPieceSum;i++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(i);
		if (1 == pPiece->m_nPieceSelStat )
		{
			pPiece->m_nPieceSelStat = 0;
			m_PiecesInList.Add(pPiece);
			m_PiecesInMain[m_nCurPage].RemoveAt(i);
			i--;
			nPieceSum--;
		}
	}
}

void CMCADDoc::OnResetPieceSelState()
{
	CPiece* pPiece;
	int i;
	int nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	if (nPieceSum <= 0)
	{
		return;
	}
	for (i=0;i<nPieceSum;i++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(i);
		if (pPiece->m_nPieceSelStat != 0)
		{
			pPiece->m_nPieceSelStat = 0;
			pPiece->m_bRefreshDisplay = TRUE;
		}
	}
}

void CMCADDoc::OnClearCurveSelState()
{
	CCurve* pCurve;
	for (int i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		pCurve->m_bIsSelected = FALSE;
	}
}

void CMCADDoc::FindCurvesInRect(CRect rect)
{
	CCurve* pCurve;
	int i;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	if (nCurveSum <= 0)
	{
		return;
	}

	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		pCurve->GetCurveMaxMin();
		if (pCurve->CurveInRect(rect))
		{
			pCurve->m_bIsSelected = TRUE;
		}
	}
}

void CMCADDoc::OnAddString(void)
{
	CFile fileTempPlt;
	CString strTempFileName;
	CString strTemp;
	double dAngleX,dAngleY;
	double dStartX,dStartY;

	strTempFileName = gSet.m_strTempDir+_T("addString.plt");
	fileTempPlt.Open(strTempFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);

	dAngleX = 10.0*cos( gTextProp.getTextAngle() * PI / 180.0 );
	dAngleY = 10.0*sin( gTextProp.getTextAngle() * PI / 180.0 );
	dStartX = 40.0*gTextProp.getTextPosX();
	dStartY = 40.0*gTextProp.getTextPosY();

	strTemp.Format("IN;PU%f,%f;DI%.3f,%.3f;LB%s",dStartX,dStartY,dAngleX,dAngleY,gTextProp.m_strTextAdd);
	fileTempPlt.Write(strTemp.GetBuffer(0),strTemp.GetLength());
	fileTempPlt.Close();

	CHpglFile* pHpglFile = new CHpglFile;
	pHpglFile->m_strPathAndName = strTempFileName;
	if (0 != pHpglFile->OpenPltFile())
	{
		return;
	}
	OnImportCurveFromText(pHpglFile,
		gTextProp.getTextPosX()*40.0,
		gTextProp.getTextPosY()*40.0,
		gTextProp.getTextInflate());
//	OnRemoveDupPoints();
//	OnSetCurveTypeFromPenNum();

	m_nCurTool = TOOL_SEL_CURVE;
	BackUp(); //导入PLT之后
	delete pHpglFile;
}

void CMCADDoc::FindCurveOnPoint(CPoint ptScr)
{
	CCurve* pCurve;
	int i;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	if (nCurveSum <= 0)
	{
		return;
	}
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->PointOnCurve(ptScr))
		{
			pCurve->m_bIsSelected = !(pCurve->m_bIsSelected);
			break;
		}
	}
}

void CMCADDoc::OnSelCurveToPoly()
{
	CCurve* pCurve;
	int nCurveSum;
	int i;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected)
		{
			CPoint pt = pCurve->m_pointList->GetAt(0);
			pCurve->m_pointList->Add(pt);
		}
	}
}

void CMCADDoc::OnSelCurveDel()
{
	int i;
	CCurve* pCurve;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected)
		{
			delete pCurve;
			m_CurveInMain[m_nCurPage].RemoveAt(i);
			i--;
			nCurveSum--;
		}
	}
}

void CMCADDoc::OnSelCurveToPen()
{
	int i;
	CCurve* pCurve;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if( (pCurve->m_bIsSelected)&&(CURVE_TYPE_DRILL != pCurve->m_nCurveType) )
		{
			pCurve->m_nCurveType = CURVE_TYPE_PEN;
		}
	}
}

void CMCADDoc::OnSelCurveToMM()
{
	int i;
	CCurve* pCurve;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if ( (pCurve->m_bIsSelected)&&(CURVE_TYPE_DRILL != pCurve->m_nCurveType) )
		{
			pCurve->m_nCurveType = CURVE_TYPE_MM;
		}
	}
}

void CMCADDoc::OnSelCurveReverse()
{
	int i;
	CCurve* pCurve;
	int nCurveSum;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected)
		{
			pCurve->OnSmoothRpFile(); //20161206 解决富怡文件反向时显示错误的问题
			pCurve->OnReverse();
		}
	}
}

CCurve* CMCADDoc::GetSelCurve()
{
	CCurve* pCurve;
	int i;
	int nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected)
		{
			return pCurve;
		}
	}
	return NULL;
}

CPiece* CMCADDoc::GetPieceWhichCurveSeled()
{
	CCurve* pCurve;
	CPiece* pPiece;
	int nPieceSum;
	int i;
	nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	for (i=0;i<nPieceSum;i++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(i);
		int nCurveSum = pPiece->m_CurveList.GetSize(); 
		
		for (int j=0;j<nCurveSum;j++)
		{
			pCurve = pPiece->m_CurveList.GetAt(j);
			if (pCurve->m_bIsSelected)
			{
				return pPiece;
			}
		}
	}
	return NULL;
}

void CMCADDoc::OnSelCurveCombine12()
{
	int nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	CCurve *pCurve1;
	CCurve *pCurve2;
	int i;
	int n1,n2;
	for (i=0;i<nCurveSum;i++)
	{
		pCurve1 = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve1->m_bIsSelected)
		{
			n1 = i;
			i++;
			break;
		}
		else
		{
			pCurve1=NULL;
		}
	}

	if (i<nCurveSum)
	{
		for (;i<nCurveSum;i++)
		{
			pCurve2 = m_CurveInMain[m_nCurPage].GetAt(i);
			if (pCurve2->m_bIsSelected)
			{
				n2 = i;
				break;
			}
			else
			{
				pCurve2=NULL;
			}
		}
	}
	else
	{
		pCurve2 = NULL;
	}

	if(NULL == pCurve1)
	{
		return;
	}

	if (NULL == pCurve2)
	{
		return;
	}

	int nTempSize;
	CPoint pt1,pt2;
	nTempSize = pCurve1->m_pointList->GetSize()-1;
	pt1 = pCurve1->m_pointList->GetAt(nTempSize); //第一条线段的结尾
	pt2 = pCurve2->m_pointList->GetAt(0);

	int k;
	if( ((int)getdistance(pt1,pt2)) < 260)
	{
		int nCurveSize = pCurve2->m_pointList->GetSize();
		if (pt1 == pt2)
		{
			k=1;
		}
		else
		{
			k=0;
		}
		for (;k<nCurveSize;k++)
		{
			CPoint ptA;
			ptA = pCurve2->m_pointList->GetAt(k);
			pCurve1->m_pointList->Add(ptA);
		}
		pCurve1->GetCurveMaxMin();
		m_CurveInMain[m_nCurPage].RemoveAt(n2);
		delete pCurve2;
		return;
	}

	nTempSize = pCurve2->m_pointList->GetSize()-1;
	pt1 = pCurve2->m_pointList->GetAt(nTempSize);
	pt2 = pCurve1->m_pointList->GetAt(0);

	if( ((int)getdistance(pt1,pt2)) < 260)
	{
		int nCurveSize = pCurve1->m_pointList->GetSize();
		if (pt1 == pt2)
		{
			k=1;
		}
		else
		{
			k=0;
		}

		for (;k<nCurveSize;k++)
		{
			CPoint ptB;
			ptB = pCurve1->m_pointList->GetAt(k);
			pCurve2->m_pointList->Add(ptB);
		}
		pCurve2->GetCurveMaxMin();
		m_CurveInMain[m_nCurPage].RemoveAt(n1);
		delete pCurve1;
		return;
	}
}

void CMCADDoc::OnSelCurveCombine23()
{
	int nCurveSum = m_CurveInMain[m_nCurPage].GetSize(); 
	CCurve *pCurve0;
	CCurve *pCurve1;
	CCurve *pCurve2;
	int i;
	int n1,n2;
	for (i=0;i<nCurveSum;i++)
	{
		pCurve0 = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve0->m_bIsSelected)
		{
			n1 = i;
			i++;
			break;
		}
		else
		{
			pCurve0=NULL;
		}
	}
	if (i>=nCurveSum)
		return;

	for (;i<nCurveSum;i++)
	{
		pCurve1 = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve1->m_bIsSelected)
		{
			n1 = i;
			i++;
			break;
		}
		else
		{
			pCurve1=NULL;
		}
	}

	if (i<nCurveSum)
	{
		for (;i<nCurveSum;i++)
		{
			pCurve2 = m_CurveInMain[m_nCurPage].GetAt(i);
			if (pCurve2->m_bIsSelected)
			{
				n2 = i;
				break;
			}
			else
			{
				pCurve2=NULL;
			}
		}
	}
	else
	{
		pCurve2 = NULL;
	}

	if(NULL == pCurve1)
	{
		return;
	}

	CPoint pt1,pt2;
	if (NULL == pCurve2)
	{
		return;
	}

	int nTempSize;
	nTempSize = pCurve1->m_pointList->GetSize()-1;
	pt1 = pCurve1->m_pointList->GetAt(nTempSize); //第一条线段的结尾
	pt2 = pCurve2->m_pointList->GetAt(0);

	int k;
	if( ((int)getdistance(pt1,pt2)) < 260)
	{
		int nCurveSize = pCurve2->m_pointList->GetSize();
		if (pt1 == pt2)
		{
			k=1;
		}
		else
		{
			k=0;
		}
		for (;k<nCurveSize;k++)
		{
			CPoint ptA;
			ptA = pCurve2->m_pointList->GetAt(k);
			pCurve1->m_pointList->Add(ptA);
		}
		pCurve1->GetCurveMaxMin();
		m_CurveInMain[m_nCurPage].RemoveAt(n2);
		delete pCurve2;
		return;
	}

	nTempSize = pCurve2->m_pointList->GetSize()-1;
	pt1 = pCurve2->m_pointList->GetAt(nTempSize);
	pt2 = pCurve1->m_pointList->GetAt(0);

	if( ((int)getdistance(pt1,pt2)) < 260)
	{
		int nCurveSize = pCurve1->m_pointList->GetSize();
		if (pt1 == pt2)
		{
			k=1;
		}
		else
		{
			k=0;
		}

		for (;k<nCurveSize;k++)
		{
			CPoint ptB;
			ptB = pCurve1->m_pointList->GetAt(k);
			pCurve2->m_pointList->Add(ptB);
		}
		pCurve2->GetCurveMaxMin();
		m_CurveInMain[m_nCurPage].RemoveAt(n1);
		delete pCurve1;
		return;
	}
}

void CMCADDoc::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(m_backup_index < m_backup.GetSize());
}

void CMCADDoc::OnUpdateEditUndo(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_backup_index > 1);
}

void CMCADDoc::OnEditRedo() 
{
	ReStore(FALSE);
	UpdateAllViews(NULL);
}

void CMCADDoc::OnEditUndo() 
{
	ReStore(TRUE);
	UpdateAllViews(NULL);
}

void CMCADDoc::OnCopyWorkPieces()
{
	CCurve* pCurve;
	CCurve* pCurveWork;
	while(m_CurvesWork.GetSize() > 0)
	{
		pCurve = m_CurvesWork.GetAt(0);
		delete pCurve;
		m_CurvesWork.RemoveAt(0);
	}

	m_nCurTool = TOOL_SEL_PIECE;
	OnClearCurveSelState(); //所有裁片中的所有曲线全部设为未选中状态

	OnSearchOutLine(); //检查是否封闭等
	OnRemoveCurveNotOutline();//20161013

	OnSearchPieceStep1(); //从曲线中识别裁片外轮廓 
	OnSearchPieceStep2(); //识别裁片中的内线

	m_nCurTool = TOOL_SEL_CURVE;
	OnResetPieceSelState(); //所有裁片全部设为未选中状态
	OnGetLineFromPieces();
	UpdateAllViews(NULL);

	int nCurveSum;
	int i;
	nCurveSum = m_CurveInMain[m_nCurPage].GetSize();
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		pCurve->GetCurveMaxMin();
		pCurve->GetCurveStyle();
		pCurve->OnCheckCurveIsPolygon();
		pCurveWork = new CCurve;
		*pCurveWork = *pCurve;
		m_CurvesWork.Add(pCurveWork);
	}
	return;
}

int CMCADDoc::CreateCncList()
{
	int nSize;
	ReleaseCncDataBuffer();
	OnCopyWorkPieces();
	OnWorkPageAngleAdjust();
	GetCncData();
	nSize = g_ptrCncList.GetSize();
	if (nSize <= 0)
	{
		return 0;
	}
	CalculateAngles();
	GetLongAxialSteps();
	CalcCurvature();
	CalcCncSpeed();
	CalcSdPercent();
	GetCncMaxPulse();

#ifdef _DEBUG
	DumpDataToFile();
#endif
	return g_ptrCncList.GetSize();
}

void CMCADDoc::OnWorkPageAngleAdjust()
{
	if (gSet.getAngleAdjust() == 100)
	{
		return;
	}

	int i;
	CCurve* pCurve;
	double dAngle =  ( 100.0-gSet.getAngleAdjust() )*0.01*PI/180.0;
	int nCurveSum = m_CurvesWork.GetSize();
	int xMax;
	if (nCurveSum <= 0)
	{
		return;
	}
	pCurve = m_CurvesWork.GetAt(0);
	xMax = pCurve->m_nXmax;	
	if (nCurveSum > 1)
	{
		for (i=1;i<nCurveSum;i++)
		{
			pCurve = m_CurvesWork.GetAt(i);
			xMax = (xMax>pCurve->m_nXmax)? xMax:pCurve->m_nXmax;
		}
	}
//----------------寻找最大X坐标完毕----------------------------
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		pCurve->OnAngleAdjust(dAngle,xMax);
	}
	return;
}

void CMCADDoc::GetCncData()
{
	int i;
	CCurve *pCurve;
	int nCurveSum;
//笔指令------------------------------------------------------
	nCurveSum = m_CurvesWork.GetSize(); 
	if (nCurveSum <= 0 )
	{
		return;
	}

	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		if (CURVE_TYPE_PEN == pCurve->m_nCurveType)
		{
			pCurve->OnToPlotData();
		}
	}
	
//钻孔指令------------------------------------------------------
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		if(CURVE_TYPE_DRILL == pCurve->m_nCurveType)
		{
			pCurve->OnToDrillData();
		}
	}

//切割指令-----------------------------飞-------------------------
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		if( (CURVE_TYPE_MM == pCurve->m_nCurveType)
			&&(FALSE == pCurve->m_bIsPolygon) //非闭合曲线
			&& (FALSE == pCurve->m_bIsOutline) )
		{
			pCurve->OnToMMData();
		}
	}
	
	//切割指令------------------------------------------------------
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		if( (CURVE_TYPE_MM == pCurve->m_nCurveType)
			&&(TRUE == pCurve->m_bIsPolygon) //闭合曲线
			&& (FALSE == pCurve->m_bIsOutline) )
		{
			pCurve->OnToMMData();
		}
	}

//------------------------------------------------------------ 外框
	for (i=0;i<nCurveSum;i++)
	{
		pCurve = m_CurvesWork.GetAt(i);
		if( (CURVE_TYPE_MM == pCurve->m_nCurveType) && (pCurve->m_bIsOutline))
		{
			pCurve->OnToMMData();
		}
	}
	return;
}

void CMCADDoc::OnGetMaxX()
{
	int i;
	CCurve *pCurve;
	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		pCurve->GetCurveMaxMin();
		if (m_nMaxX < pCurve->m_nXmax)
		{
			m_nMaxX = pCurve->m_nXmax;
		}
	}
}

void CMCADDoc::OnExportToPlt(CString strFileName)
{
	CCurve* pCurve;
	int i,j;
	int nCurveType = CURVE_TYPE_PEN;
	CString str,str1;
	CFile pltFile;
	pltFile.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
	
	str.Format("IN;SP%d;",gSet.getPltExportPenNum());
	pltFile.Write(str.GetBuffer(0),str.GetLength());
	
	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (CURVE_TYPE_PEN == pCurve->m_nCurveType)
		{
			int nPLSize = pCurve->m_pointList->GetSize();
			if (nPLSize >= 2)
			{
				str.Format("PU%d,%d;PD",pCurve->m_pointList->GetAt(0).x,pCurve->m_pointList->GetAt(0).y);
				for (j=0;j<nPLSize;j++)
				{
					str1.Format("%d,%d,",pCurve->m_pointList->GetAt(j).x,pCurve->m_pointList->GetAt(j).y);
					str+=str1;
				}
				pltFile.Write(str.GetBuffer(0),str.GetLength()-1);
				str = ";";
				pltFile.Write(str.GetBuffer(0),1);
			}
			
		}
	}

	str.Format("SP%d;",gSet.getPltExportDrillNum());
	pltFile.Write(str.GetBuffer(0),str.GetLength());
	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (CURVE_TYPE_DRILL == pCurve->m_nCurveType)
		{
			str.Format("PU%d,%d;PD%d,%d;",
				pCurve->m_pointList->GetAt(0).x,
				pCurve->m_pointList->GetAt(0).y,
				pCurve->m_pointList->GetAt(0).x,
				pCurve->m_pointList->GetAt(0).y);
			pltFile.Write(str.GetBuffer(0),str.GetLength());
		}
	}	

	str.Format("SP%d;",gSet.getPltExportCutNum());
	pltFile.Write(str.GetBuffer(0),str.GetLength());
	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if ((CURVE_TYPE_MM == pCurve->m_nCurveType)&&(FALSE == pCurve->m_bIsOutline))
		{
			int nPLSize = pCurve->m_pointList->GetSize();
			if (nPLSize >= 2)
			{
				str.Format("PU%d,%d;PD",pCurve->m_pointList->GetAt(0).x,pCurve->m_pointList->GetAt(0).y);
				for (j=0;j<nPLSize;j++)
				{
					str1.Format("%d,%d,",pCurve->m_pointList->GetAt(j).x,pCurve->m_pointList->GetAt(j).y);
					str+=str1;
				}
				pltFile.Write(str.GetBuffer(0),str.GetLength()-1);
				str = ";";
				pltFile.Write(str.GetBuffer(0),1);
			}
		}
	}

	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if ( (CURVE_TYPE_MM == pCurve->m_nCurveType)&&(pCurve->m_bIsOutline) )
		{
			int nPLSize = pCurve->m_pointList->GetSize();
			if (nPLSize >= 2)
			{
				str.Format("PU%d,%d;PD",pCurve->m_pointList->GetAt(0).x,pCurve->m_pointList->GetAt(0).y);
				for (j=0;j<nPLSize;j++)
				{
					str1.Format("%d,%d,",pCurve->m_pointList->GetAt(j).x,pCurve->m_pointList->GetAt(j).y);
					str+=str1;
				}
				pltFile.Write(str.GetBuffer(0),str.GetLength()-1);
				str = ";";
				pltFile.Write(str.GetBuffer(0),1);
			}
		}
	}
	str.Format("PG;");
	pltFile.Close();
}

void CMCADDoc::OnGetLineFromPieces()
{
	int i,j;
	CPiece* pPiece;
	CCurve* pCurve;
	for (i=0;i<m_PiecesInMain[m_nCurPage].GetSize();i++)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(i);
		for (j=0;j<pPiece->m_CurveList.GetSize();j++)
		{
			pCurve = pPiece->m_CurveList.GetAt(j);
			m_CurveInMain[m_nCurPage].Add(pCurve);
		}
	}

	while (m_PiecesInMain[m_nCurPage].GetSize() > 0)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(0);
		while(pPiece->m_CurveList.GetSize() > 0)
		{
			pPiece->m_CurveList.RemoveAt(0);
		}
		m_PiecesInMain[m_nCurPage].RemoveAt(0);
	}
}

void CMCADDoc::OnAddDrill(CPoint ptScr)
{
 	int i,j;
 	CCurve* pCurve0;
 	CPoint pt0;
	CPoint ptScr0;
 
 	for (i=0;i<m_CurveInMain[m_nCurPage].GetSize();i++)
 	{
		pCurve0 = m_CurveInMain[m_nCurPage].GetAt(i);
		for (j=0;j<pCurve0->m_pointList->GetSize();j++)
		{
			pt0 = pCurve0->m_pointList->GetAt(j);
			ptScr0 = ptHpglToScr(pt0);
			if( getdistance(ptScr,ptScr0) < 20.0 ) //少于20个像素
			{
				CCurve* pCurve = new CCurve;

				CPoint ptHpgl = pt0;
				pCurve->m_nCurveType = CURVE_TYPE_DRILL;
				pCurve->m_pointList->RemoveAll();
				pCurve->m_pointList->Add(ptHpgl);
				pCurve->m_nXmax = ptHpgl.x;
				pCurve->m_nXmin = ptHpgl.x;
				pCurve->m_nYmax = ptHpgl.y;
				pCurve->m_nYmin = ptHpgl.y;
				pCurve->m_bIsOutline = FALSE;
				pCurve->m_bIsPolygon = FALSE;
				pCurve->m_dxxx = 0.0;
				m_CurveInMain[m_nCurPage].Add(pCurve);
				return;
			}

		}
	}


	CCurve* pCurve = new CCurve;

	CPoint ptHpgl = ptScrToHpgl(ptScr);
	pCurve->m_nCurveType = CURVE_TYPE_DRILL;
	pCurve->m_pointList->RemoveAll();
	pCurve->m_pointList->Add(ptHpgl);
	pCurve->m_nXmax = ptHpgl.x;
	pCurve->m_nXmin = ptHpgl.x;
	pCurve->m_nYmax = ptHpgl.y;
	pCurve->m_nYmin = ptHpgl.y;
	pCurve->m_bIsOutline = FALSE;
	pCurve->m_bIsPolygon = FALSE;
	pCurve->m_dxxx = 0.0;
	m_CurveInMain[m_nCurPage].Add(pCurve);


}

void CMCADDoc::AllPieceToList()
{
	CPiece* pPiece;
	while(m_PiecesInMain[m_nCurPage].GetSize() > 0)
	{
		pPiece = m_PiecesInMain[m_nCurPage].GetAt(0);
		pPiece->m_nPieceSelStat = 0;
		m_PiecesInList.Add(pPiece);
		m_PiecesInMain[m_nCurPage].RemoveAt(0);
	}
}

void CMCADDoc::OnResetCutSeq()
{
	int i;
	CPiece* pPiece;
	int nPieceSum = m_PiecesInMain[m_nCurPage].GetSize();
	if (nPieceSum<=0)
		return;
	for (i=0;i<nPieceSum;i++)
	{
		pPiece=m_PiecesInMain[m_nCurPage].GetAt(i);
		pPiece->m_nCutSequence = i;
	}
}

void CMCADDoc::OnAddDrillTable(int StartX,int StartY,int XNum,int YNum,int GapX,int GapY)
{
	int i,j;
	CPoint pt;
	for (i=0;i<XNum;i++)
	{
		for (j=0;j<YNum;j++)
		{
			CCurve* pCurve = new CCurve;
			pCurve->m_nCurveType = CURVE_TYPE_DRILL;
			pt.x = StartX+GapX*40*i;
			if (i%2 == 0)
			{
				pt.y = StartY+GapY*40*j;
			}
			else
			{
				pt.y = StartY+GapY*40*j+GapY*20;
			}
			pCurve->m_pointList->Add(pt);
			m_CurveInMain[m_nCurPage].Add(pCurve);
		}
	}
}


void CMCADDoc::OnAddSnakeLineByX(int nXSize,int nYSize,int nXGap)
{
	CCurve* pCurve = new CCurve;
	pCurve->m_nCurveType = CURVE_TYPE_MM;
	int i;
	CPoint pt1,pt2,pt3,pt4;
	
	int nXLineNum;
	if (nXGap <= 0)
	{
		return;
	}
	nXLineNum = nXSize / (2*nXGap);
	if (nXLineNum <= 0)
	{
		return;
	}
	if (nYSize <= 0)
	{
		return;
	}
	for (i=0;i<nXLineNum;i++)
	{
		pt1.x = i*nXGap*80;
		pt1.y = 0;
		pt2.x = i*nXGap*80;
		pt2.y = nYSize*40;
		
		pt3.x = i*nXGap*80+nXGap*40;
		pt3.y = nYSize*40;
		pt4.x = i*nXGap*80+nXGap*40;
		pt4.y = 0;
		pCurve->m_pointList->Add(pt1);
		pCurve->m_pointList->Add(pt2);
		pCurve->m_pointList->Add(pt3);
		pCurve->m_pointList->Add(pt4);
	}
	
	m_CurveInMain[m_nCurPage].Add(pCurve);
}

void CMCADDoc::OnAddSnakeLineByY(int nXSize,int nYSize,int nYGap)
{
	CCurve* pCurve = new CCurve;
	pCurve->m_nCurveType = CURVE_TYPE_MM;
	int i;
	CPoint pt1,pt2,pt3,pt4;
	
	int nYLineNum;
	if (nYGap <= 0)
	{
		return;
	}
	nYLineNum = nYSize / (2*nYGap);
	if (nYLineNum <= 0)
	{
		return;
	}
	if (nYSize <= 0)
	{
		return;
	}
	for (i=0;i<nYLineNum;i++)
	{
		pt1.x = 0;
		pt1.y = i*nYGap*80;
		pt2.x = nXSize*40;
		pt2.y = i*nYGap*80;
		
		pt3.x = nXSize*40;
		pt3.y = i*nYGap*80+nYGap*40;
		pt4.x = 0;
		pt4.y = i*nYGap*80+nYGap*40;
		pCurve->m_pointList->Add(pt1);
		pCurve->m_pointList->Add(pt2);
		pCurve->m_pointList->Add(pt3);
		pCurve->m_pointList->Add(pt4);
	}
	
	m_CurveInMain[m_nCurPage].Add(pCurve);
}

void CMCADDoc::OnAddMpTestFile()
{
	int i;
	CPoint pt[4];
	CCurve* pCurve;

	pt[0].x = pt[3].x = 0;
	pt[1].x = pt[2].x = 8000;
	pt[0].y = pt[1].y = 0;
	pt[2].y = pt[3].y = 8000;

	for (i=0;i<4;i++)
	{
		pCurve = new CCurve;
		pCurve->m_nCurveType = CURVE_TYPE_DRILL;
		pCurve->m_pointList->Add(pt[i]);

		m_CurveInMain[m_nCurPage].Add(pCurve);
		pCurve->GetCurveMaxMin();
	}

	pCurve = new CCurve;
	pCurve->m_nCurveType = CURVE_TYPE_PEN;
	for(i=0;i<4;i++)
	{
		pCurve->m_pointList->Add(pt[i]);
	}
	pCurve->m_pointList->Add(pt[0]);
	m_CurveInMain[m_nCurPage].Add(pCurve);
	pCurve->GetCurveMaxMin();
}

void CMCADDoc::OnSearchCutData()
{
	int i;
	CCurve* pCurve;
	double dXSize,dYSize;
	int nPageSize = m_CurveInMain[m_nCurPage].GetSize();
	if (nPageSize == 0)
	{
		return;
	}
	for (i=0;i<nPageSize;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_nCurveType != CURVE_TYPE_DRILL)
		{
			pCurve->m_nCurveType = CURVE_TYPE_PEN;
		}
	}	
	for (i=0;i<nPageSize;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_pointList->GetSize() < 4)
		{
			continue;
		}
		dXSize = (pCurve->m_nXmax-pCurve->m_nXmin)/40.0;
		dYSize = (pCurve->m_nYmax-pCurve->m_nYmin)/40.0;
		if( (dXSize*dYSize<900.0)|| (dYSize<10.0) || (dXSize<10.0 ) )
		{
			continue;
		}
		int nCurveSize = pCurve->m_pointList->GetSize();
		CPoint ptS,ptE;
		ptS = pCurve->m_pointList->GetAt(0);
		ptE = pCurve->m_pointList->GetAt(nCurveSize-1);
		if ( fabs((double)(ptS.x-ptE.x)) > 12 )
		{
			continue;
		}
		if ( fabs((double)(ptS.y-ptE.y)) > 12 )
		{
			continue;
		}
		pCurve->m_nCurveType = CURVE_TYPE_MM;
	}
	
	OnRemoveCurveNotMM();
}

void CMCADDoc::OnRemoveCurveNotOutline()
{
	CPoint pt10,pt11,pt12;
	CPoint pt20,pt21,pt22;
	CCurve* pCurve1;
	CCurve* pCurve2;
	int CurveSize1,CurveSize2;

	int i,j;
	int nPageSize = m_CurveInMain[m_nCurPage].GetSize();
	if (nPageSize<=2)
	{
		return;
	}

	for (i=0;i<nPageSize;i++)
	{
		pCurve1 = m_CurveInMain[m_nCurPage].GetAt(i);
		CurveSize1 = pCurve1->m_pointList->GetSize();
		if (CurveSize1 < 4)
		{
			continue;
		}

		pt10 = pCurve1->m_pointList->GetAt(0);
		pt11 = pCurve1->m_pointList->GetAt(1);
		pt12 = pCurve1->m_pointList->GetAt(2);

		if (pCurve1->m_bIsOutline)
		{
			for (j=0;j<nPageSize;j++)
			{
				pCurve2 = m_CurveInMain[m_nCurPage].GetAt(j);
				CurveSize2 = pCurve2->m_pointList->GetSize();

				if( (j==i) || (!pCurve2->m_bIsOutline) || (CurveSize2<4) )
				{
					continue;
				}
				pt20 = pCurve2->m_pointList->GetAt(0);
				pt21 = pCurve2->m_pointList->GetAt(1);
				pt22 = pCurve2->m_pointList->GetAt(2);
				if( pCurve1->PtInPolygon(pt20) && pCurve1->PtInPolygon(pt21) && pCurve1->PtInPolygon(pt22) )
				{
					pCurve2->m_bIsOutline = FALSE;
				}

				if( pCurve2->PtInPolygon(pt10) && pCurve2->PtInPolygon(pt11) && pCurve2->PtInPolygon(pt12) )
				{
					pCurve1->m_bIsOutline = FALSE;
				}
			}
		}
	}
}

void CMCADDoc::OnRemoveCurveNotMM()
{
	CPoint pt10,pt11,pt12;
	CPoint pt20,pt21,pt22;
	CCurve* pCurve1;
	CCurve* pCurve2;
	int CurveSize1,CurveSize2;

	int i,j;
	int nPageSize = m_CurveInMain[m_nCurPage].GetSize();
	if (nPageSize<=2)
	{
		return;
	}

	for (i=0;i<nPageSize;i++)
	{
		pCurve1 = m_CurveInMain[m_nCurPage].GetAt(i);
		CurveSize1 = pCurve1->m_pointList->GetSize();
		if (CurveSize1 < 4)
		{
			continue;
		}

		pt10 = pCurve1->m_pointList->GetAt(0);
		pt11 = pCurve1->m_pointList->GetAt(1);
		pt12 = pCurve1->m_pointList->GetAt(2);

		if (pCurve1->m_nCurveType == CURVE_TYPE_MM)
		{
			for (j=0;j<nPageSize;j++)
			{
				pCurve2 = m_CurveInMain[m_nCurPage].GetAt(j);
				CurveSize2 = pCurve2->m_pointList->GetSize();

				if( (j==i) || (pCurve2->m_nCurveType != CURVE_TYPE_MM) || (CurveSize2<4) )
				{
					continue;
				}
				pt20 = pCurve2->m_pointList->GetAt(0);
				pt21 = pCurve2->m_pointList->GetAt(1);
				pt22 = pCurve2->m_pointList->GetAt(2);
				if( pCurve1->PtInPolygon(pt20) && pCurve1->PtInPolygon(pt21) && pCurve1->PtInPolygon(pt22) )
				{
					pCurve2->m_nCurveType = CURVE_TYPE_PEN;
				}
				
				if( pCurve2->PtInPolygon(pt10) && pCurve2->PtInPolygon(pt11) && pCurve2->PtInPolygon(pt12) )
				{
					pCurve1->m_nCurveType = CURVE_TYPE_PEN;
				}
			}
		}
	}
}

void CMCADDoc::OnSelCurveToDrill()
{
	int i,j;
	CCurve* pCurve;
	int nPageSize,nCurveSize;
	double dDrillDistHpgl;
	CPoint pt,ptNext;
	double dDist,dDistRemain;
	double dAngle;

	nPageSize = m_CurveInMain[m_nCurPage].GetSize();
	
	dDrillDistHpgl = gSet.getDrillDist()*4;//单位变成HPGL了

	for (i=0;i<nPageSize;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected)
		{
			nCurveSize = pCurve->m_pointList->GetSize();
			pt = pCurve->m_pointList->GetAt(0);

			CCurve *pCurveAdd = new CCurve;
			pCurveAdd->m_nCurveType = CURVE_TYPE_DRILL;
			pCurveAdd->m_pointList->Add(pt);
			m_CurveInMain[m_nCurPage].Add(pCurveAdd);
			dDistRemain = 0.0;

			for (j=1;j<nCurveSize;)
			{
				ptNext = pCurve->m_pointList->GetAt(j);
				while(1)
				{
					dDist = getdistance(pt,ptNext);
					if ( dDist > (dDrillDistHpgl-dDistRemain) ) //插入一点
					{
						dAngle = atan2( (double)(ptNext.y-pt.y),(double)(ptNext.x-pt.x));
						pt.x += (int)( (dDrillDistHpgl-dDistRemain)*cos(dAngle) );
						pt.y += (int)( (dDrillDistHpgl-dDistRemain)*sin(dAngle) );
						
						CCurve *pCurveAdd = new CCurve;
						pCurveAdd->m_nCurveType = CURVE_TYPE_DRILL;
						pCurveAdd->m_pointList->Add(pt);
						m_CurveInMain[m_nCurPage].Add(pCurveAdd);
						dDistRemain = 0.0;
					}
					else
					{
						dDistRemain += dDist; //计入上一段的余量
						pt = ptNext;
						j++;
						break;
					}
				}
			}

			delete pCurve;
			m_CurveInMain[m_nCurPage].RemoveAt(i);
			return;
		}
	}
}

void CMCADDoc::OnSelAllCurve()
{
	CCurve* pCurve;
	int i;
	int nPageSize = m_CurveInMain[m_nCurPage].GetSize();
	if (nPageSize<=0)
	{
		return;
	}
	for (i=0;i<nPageSize;i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		pCurve->m_bIsSelected = TRUE;
	}
}

void CMCADDoc::OnSelCurveToSlot(double dInflateDist)
{
	int i;
	CCurve* pCurve;
	for (i = 0; i < m_CurveInMain[m_nCurPage].GetSize(); i++)
	{
		pCurve = m_CurveInMain[m_nCurPage].GetAt(i);
		if (pCurve->m_bIsSelected == TRUE)
		{
			pCurve->GetCurveStyle();
			if ( pCurve->m_bIsPolygon == FALSE)
			{
				pCurve->CurveInflate(dInflateDist*40.0*0.5);
				pCurve->CurveSmoothMethod1();
				pCurve->m_nCurveType = CURVE_TYPE_MM;
			}
		}
	}
}
