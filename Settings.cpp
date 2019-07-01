// Settings.cpp: implementation of the CSettings class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MCAD.h"
#include "Settings.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CColorSettings::CColorSettings()
{

}

CColorSettings::~CColorSettings()
{

}

CTextProp::CTextProp()
{
	m_dTextPosX = 0.0;
	m_dTextPosY = 0.0;
	m_dTextAngle = 0.0;
	m_nTextInflate = 5;
};

CTextProp::~CTextProp()
{

};

COutLineProp::COutLineProp()
{
	m_bLT = FALSE; //左上倒圆角
	m_bLB = FALSE; //左下
	m_bRT = TRUE; //右上
	m_bRB = TRUE; //右下
//	m_bClockWise = 1; //顺时针
	m_bClockWise = 0; //逆时针
	m_dMmTop = 30.0;
	m_dMmBot = 30.0;
	m_dMmLeft = 30.0;
	m_dMmRight = 30.0;
	m_dMmOutlineRadius = 30.0; //倒圆角半径

	m_dCutDepth = 16.8;
	m_dCutWidth = 4.8;
	m_dCutDist = 250.0; //剪口间距
	m_nCutPos = 0;
}

COutLineProp::~COutLineProp()
{
}

CSettings::CSettings()
{
	m_dSlotWidth = 4.0;
	m_nDrillDist = 80;
	m_dPPMMX = 160.0;
	m_dPPMMY = 160.0;
	m_dPPMMZ = 2000.0;
	m_nAngleAdjust = 100;
	m_nZPosMove = 0;
	m_nZPosWork = 150;

	m_nDrillerDimension = 300;

	m_nMPDistX = 50; //刀笔补偿
	m_nMPDistY = 50;
	m_nKPDistX = 50; //刀笔补偿
	m_nKPDistY = 50;

	m_n10000X = 10000; //比例调整
	m_n10000Y = 10000; //比例调整

	m_nBreakAngle = 15;

	m_nSpdPlot = 3;
	m_nSpdCut = 3;
	m_nSpdMilling = 3;
	m_nSpdMove = 3;

	m_nPwmPen = 30;
	m_nPwmCut = 30;
	m_nMMOnDelay = 10; //启动延时10秒

	m_nCurveLen = 2;

	m_nPltExportPenNum = 1;
	m_nPltExportCutNum = 2;
	m_nPltExportDrillNum = 3;

	m_nWorkStartY = 0;
	m_bVMEnable = TRUE;
	m_bQueryEnable = TRUE;
	m_bSuperUserOn = FALSE;

}

CSettings::~CSettings()
{
}

BOOL FreeResFile(DWORD dwResID, LPCTSTR lpszResType, LPCTSTR lpszFilePathName)  
{     
    HMODULE hInstance = ::GetModuleHandle(NULL);//得到自身实例句柄  
	
    HRSRC   hResID = ::FindResource(hInstance, MAKEINTRESOURCE(dwResID), lpszResType); //查找资源
    HGLOBAL hRes   = ::LoadResource(hInstance, hResID); //加载资源  
    LPVOID  pRes   = ::LockResource(hRes); //锁定资源  
	
    if (NULL == pRes) //锁定失败  
    {
        return FALSE;
    }
    DWORD  dwResSize = ::SizeofResource(hInstance, hResID); //得到待释放资源文件大小
    HANDLE hResFile  = CreateFile(
		lpszFilePathName,
		GENERIC_WRITE,
		0,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
    if (INVALID_HANDLE_VALUE == hResFile)
    {
        return FALSE;
    }
	DWORD dwWritten = 0; //写入文件的大小     
	WriteFile(hResFile, pRes, dwResSize, &dwWritten, NULL); //写入文件  
    CloseHandle(hResFile); //关闭文件句柄

	FreeResource(hRes);
    return (dwResSize == dwWritten); //若写入大小等于文件大小，返回成功，否则失败  
}

void CSettings::getDirectorys()
{
    TCHAR szFullPath[_MAX_PATH];
    TCHAR szDir[_MAX_DIR];
    TCHAR szDrive[_MAX_DRIVE];
    ::GetModuleFileName(NULL, szFullPath, _MAX_PATH);
	m_strAppName.Format(_T("%s"),szFullPath);

    _splitpath(szFullPath, szDrive, szDir, NULL, NULL);
	m_strAppDir.Format(_T("%s%s"), szDrive, szDir);

	checkTempDir();
	m_strFonHz.Format( _tcsdup(m_strTempDir + "fon_hz.bin") );
	m_strFonEn.Format( _tcsdup(m_strTempDir + "fon_en.bin") );

	DeleteFile(m_strFonHz);
	DeleteFile(m_strFonEn);
	FreeResFile(IDR_FON_EN, "bin", m_strFonEn);
	FreeResFile(IDR_FON_HZ, "bin", m_strFonHz);
}

void CSettings::checkTempDir()
{
	CFileFind fFind;
	SECURITY_ATTRIBUTES attrib;
	attrib.bInheritHandle = FALSE;
	attrib.lpSecurityDescriptor = NULL;
	attrib.nLength = sizeof(SECURITY_ATTRIBUTES); 
	
// 	TCHAR szTempPath[_MAX_PATH];
// 	CString strTempPath;
// 	GetTempPath(MAX_PATH, szTempPath);
// 	strTempPath.Format( _T("%s"), szTempPath );
// 	m_strTempDir = strTempPath+"MmTmp\\";

	m_strTempDir.Format( _tcsdup(m_strAppDir + "MmTmp\\") );
	if (FALSE == fFind.FindFile(m_strTempDir) )
	{
		CreateDirectory(m_strTempDir,&attrib);
	}
}
