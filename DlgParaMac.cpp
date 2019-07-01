// DlgSetup.cpp : implementation file
//

#include "stdafx.h"
#include "MCAD.h"
#include "DlgParaMac.h"
#include "Global.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgParaMac dialog


CDlgParaMac::CDlgParaMac(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgParaMac::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgParaMac)
	m_nMpDistX = 0;
	m_nMpDistY = 0;
	m_nZPosMove = 0;
	m_nZPosWork = 1500;
	m_nPwmPlot = 0;
	m_nSpdMilling = 0;
	m_nSpdMove = 0;
	m_nSpdPlot = 0;
	m_n10000X = 0;
	m_n10000Y = 0;
	m_nKpDistX = 0;
	m_nKpDistY = 0;
	m_nPwmCut = 0;
	m_nSpdCut = 0;
	m_bVMEnable = FALSE;
	m_iAngleAdjust = 100;
	m_nMMotorDelay = 0;
//	m_bCutAllInnerLineFirst = false;
	//}}AFX_DATA_INIT
}

void CDlgParaMac::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgParaMac)
	DDX_Text(pDX, IDCE_MPDIST_X, m_nMpDistX);
	DDV_MinMaxInt(pDX, m_nMpDistX, 0, 100);
	DDX_Text(pDX, IDCE_MPDIST_Y, m_nMpDistY);
	DDV_MinMaxInt(pDX, m_nMpDistY, 0, 100);
	DDX_Text(pDX, IDCE_POS_MOVE, m_nZPosMove);
	DDV_MinMaxInt(pDX, m_nZPosMove, 0, 300);
	DDX_Text(pDX, IDCE_POS_WORK, m_nZPosWork);
	DDV_MinMaxInt(pDX, m_nZPosWork, 50, 600);
	DDX_Text(pDX, IDCE_PWM_PLOT, m_nPwmPlot);
	DDX_Text(pDX, IDCE_SPD_MILLING, m_nSpdMilling);
	DDV_MinMaxInt(pDX, m_nSpdMilling, 1, 100);
	DDX_Text(pDX, IDCE_SPD_MOVE, m_nSpdMove);
	DDX_Text(pDX, IDCE_SPD_PLOT, m_nSpdPlot);
	DDX_Text(pDX, IDCE_10000X, m_n10000X);
	DDV_MinMaxUInt(pDX, m_n10000X, 9000, 11000);
	DDX_Text(pDX, IDCE_10000Y, m_n10000Y);
	DDV_MinMaxUInt(pDX, m_n10000Y, 9000, 11000);
	DDX_Text(pDX, IDCE_KPDIST_X, m_nKpDistX);
	DDV_MinMaxInt(pDX, m_nKpDistX, 0, 100);
	DDX_Text(pDX, IDCE_KPDIST_Y, m_nKpDistY);
	DDV_MinMaxInt(pDX, m_nKpDistY, 0, 100);
	DDX_Text(pDX, IDCE_PWM_CUT, m_nPwmCut);
	DDX_Text(pDX, IDCE_SPD_CUT, m_nSpdCut);
	DDX_Check(pDX, IDCHK_VMEN, m_bVMEnable);
	DDX_Text(pDX, IDCE_ANGLE_ADJ, m_iAngleAdjust);
	DDV_MinMaxInt(pDX, m_iAngleAdjust, 50, 150);
	DDX_Text(pDX, IDCE_MMOTOR_DELAY, m_nMMotorDelay);
	DDV_MinMaxInt(pDX, m_nMMotorDelay, 0, 20);
	//}}AFX_DATA_MAP
	//  DDX_Check(pDX, IDCHK_INNERLINE_FIRST, m_bCutAllInnerLineFirst);
}

BEGIN_MESSAGE_MAP(CDlgParaMac, CDialog)
	//{{AFX_MSG_MAP(CDlgParaMac)
	ON_BN_CLICKED(IDBTN_PROCPARA, OnProcpara)
	ON_BN_CLICKED(IDBTN_PARA_SAVE, OnParaSave)
	ON_BN_CLICKED(IDBTN_PARA_RESTORE, OnParaRestore)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, &CDlgParaMac::OnBnClickedOk)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgParaMac message handlers

BOOL CDlgParaMac::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	ReadMacPara();
	if (FALSE == g_bParaRead) //参数读取成功
	{
		AfxMessageBox("读取参数失败");
		GetDlgItem(IDCE_10000X)->EnableWindow(FALSE);
		GetDlgItem(IDCE_10000Y)->EnableWindow(FALSE);
		GetDlgItem(IDCE_SPD_PLOT)->EnableWindow(FALSE);
		GetDlgItem(IDCE_SPD_CUT)->EnableWindow(FALSE);
		GetDlgItem(IDCE_SPD_MILLING)->EnableWindow(FALSE);
		GetDlgItem(IDCE_SPD_MOVE)->EnableWindow(FALSE);

		GetDlgItem(IDCE_PWM_PLOT)->EnableWindow(FALSE);
		GetDlgItem(IDCE_PWM_CUT)->EnableWindow(FALSE);
		GetDlgItem(IDCE_POS_MOVE)->EnableWindow(FALSE);
		GetDlgItem(IDCE_POS_WORK)->EnableWindow(FALSE);
		
		GetDlgItem(IDCE_KPDIST_X)->EnableWindow(FALSE);
		GetDlgItem(IDCE_KPDIST_Y)->EnableWindow(FALSE);

		GetDlgItem(IDCE_MPDIST_X)->EnableWindow(FALSE);
		GetDlgItem(IDCE_MPDIST_Y)->EnableWindow(FALSE);
		GetDlgItem(IDCE_ANGLE_ADJ)->EnableWindow(FALSE);
		GetDlgItem(IDCE_MMOTOR_DELAY)->EnableWindow(FALSE);

		GetDlgItem(IDBTN_PROCPARA)->EnableWindow(FALSE);
		GetDlgItem(IDBTN_PARA_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDBTN_PARA_RESTORE)->EnableWindow(FALSE);
		GetDlgItem(IDCHK_VMEN)->EnableWindow(FALSE);
	}
	OnUpdateParaToCtrl();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

#include "DlgPwdInput.h"
#include "DlgProtectPara.h"
void CDlgParaMac::OnProcpara() 
{
	CDlgPwdInput dlg;
	if (IDOK == dlg.DoModal())
	{
		if (dlg.m_strPwd == "gm")
		{
			CDlgProtectPara dlg;
			dlg.DoModal();
		}
		else
		{
			AfxMessageBox("密码错误");
		}
	}
}

void CDlgParaMac::OnParaSave() 
{
	char Custer[80] = "MCAD_V2参数文件(*.mv2)\0*.mv2\0\0\0";
	CFileDialog dlg(FALSE);
	dlg.m_ofn.lpstrFilter = Custer;
	
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		CFile scpFile;
		
		int nFileNameLen = strFileName.GetLength();
		if (nFileNameLen<5)
		{
			strFileName += ".mv2";
		}
		else
		{
			if( (strFileName.GetAt(nFileNameLen-4) == '.' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-3)) == 'M' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-2)) == 'V' ) &&
				(toupper(strFileName.GetAt(nFileNameLen-1)) == '2' ) 
				)
			{
			}
			else
			{
				strFileName += ".mv2";
			}
		}
		
		scpFile.Open(strFileName,CFile::modeCreate|CFile::modeReadWrite|CFile::typeBinary);
		UNION_MAC_PARA tepmPara;
		tepmPara.stBuff = gPara;
		scpFile.Write(&tepmPara.charBuff[0],MAC_PARA_SIZE);
		scpFile.Close();
	}	
}

void CDlgParaMac::OnParaRestore() 
{
	char Custer[80] = "MCAD_V2参数文件(*.mv2)\0*.mv2\0\0\0";
	CFileDialog dlg(TRUE);
	dlg.m_ofn.lpstrFilter = Custer;
	
	if(dlg.DoModal() == IDOK)
	{
		CString strFileName = dlg.GetPathName();
		CFile scpFile;
		scpFile.Open(strFileName,CFile::modeRead|CFile::typeBinary);
		UNION_MAC_PARA tepmPara;
		scpFile.Read(&tepmPara.charBuff[0],MAC_PARA_SIZE);
		scpFile.Close();
		gPara = tepmPara.stBuff;
		OnUpdateParaGlobal();
		OnUpdateParaToCtrl();
	}
}

void CDlgParaMac::OnUpdateParaGlobal()
{
	gSet.setPPMMX(gPara.m_dPPMMX);
	gSet.setPPMMY(gPara.m_dPPMMY);
	gSet.setPPMMZ(gPara.m_dPPMMZ);

	gSet.setMacSizeX(gPara.m_sXSizeMm);
	gSet.setMacSizeY(gPara.m_sYSizeMm);
	
	gSet.set10000X(gPara.m_s10000X);
	gSet.set10000Y(gPara.m_s10000Y);
	
	gSet.setMPDistX(gPara.m_cMPDistX);
	gSet.setMPDistY(gPara.m_cMPDistY);
	
	gSet.setKPDistX(gPara.m_cKPDistX);
	gSet.setKPDistY(gPara.m_cKPDistY);
	
	gSet.setSpdPlot(gPara.m_cSpdPlot);
	gSet.setSpdMilling(gPara.m_cSpdMilling);
	gSet.setSpdCut(gPara.m_cSpdCut);
	gSet.setSpdMove(gPara.m_cSpdMove);
	
	gSet.setPwmPen(gPara.m_cPwmPen);
	gSet.setPwmCut(gPara.m_cPwmCut);
	
	gSet.setZPosMove(gPara.m_sZPosMove);
	gSet.setZPosWork(gPara.m_sZPosWork);
	gSet.setAngleAdjust(gPara.m_cAngleAdjust);

	gSet.setVmEnable(gPara.m_cVMEnable);
	gSet.setMMotorOnDelay(gPara.m_cMMOnDelay);
}

void CDlgParaMac::OnUpdateParaToCtrl()
{
	m_n10000X = gSet.get10000X();
	m_n10000Y = gSet.get10000Y();
	
	m_nSpdPlot = gSet.getSpdPlot();
	m_nSpdCut = gSet.getSpdCut();
	m_nSpdMilling = gSet.getSpdMilling();
	m_nSpdMove = gSet.getSpdMove();
	
	m_nPwmPlot =  gSet.getPwmPen();
	m_nPwmCut =  gSet.getPwmCut();

	m_nZPosMove = gSet.getZPosMove();
	m_nZPosWork = gSet.getZPosWork();
	m_iAngleAdjust = gSet.getAngleAdjust();

	m_nKpDistX = gSet.getKPDistX();
	m_nKpDistY = gSet.getKPDistY();
	m_nMpDistX = gSet.getMPDistX();
	m_nMpDistY = gSet.getMPDistY();
	m_bVMEnable = gSet.getVmEnable();
	m_nMMotorDelay = gSet.getMMotorOnDelay();

//	m_bCutAllInnerLineFirst = gSet.getCutAllInnerLineFirst();
	UpdateData(FALSE);	
}


void CDlgParaMac::OnBnClickedOk()
{
	UpdateData(TRUE);
	gSet.set10000X(m_n10000X);
	gSet.set10000Y(m_n10000Y);

	gSet.setSpdPlot(m_nSpdPlot);
	gSet.setSpdCut(m_nSpdCut);
	gSet.setSpdMilling(m_nSpdMilling);
	gSet.setSpdMove(m_nSpdMove);

	gSet.setPwmPen(m_nPwmPlot);
	gSet.setPwmCut(m_nPwmCut);

	gSet.setZPosMove(m_nZPosMove);
	gSet.setZPosWork(m_nZPosWork);

	gSet.setMPDistX(m_nMpDistX);
	gSet.setMPDistY(m_nMpDistY);

	gSet.setKPDistX(m_nKpDistX);
	gSet.setKPDistY(m_nKpDistY);

	gSet.setAngleAdjust(m_iAngleAdjust);
	gSet.setVmEnable(m_bVMEnable);
	gSet.setMMotorOnDelay(m_nMMotorDelay);
//	gSet.setCutAllInnerLineFirst(m_bCutAllInnerLineFirst);

	if(g_bParaRead == TRUE)
	{
		WriteMacPara();
		Sleep(10);
	}
	CDialog::OnOK();
}
