// MCAD.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MCAD.h"
#include "AboutAll.h"

#include "MainFrm.h"
#include "MCADDoc.h"
#include "MCADView.h"
#include "Global.h"
#include "Splash.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMCADApp

BEGIN_MESSAGE_MAP(CMCADApp, CWinApp)
	//{{AFX_MSG_MAP(CMCADApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMCADApp construction

CMCADApp::CMCADApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMCADApp object

CMCADApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMCADApp initialization
BOOL CheckFileRelation(const char *strExt, const char *strAppKey);
void RegisterFileRelation(char *strExt, char *strAppName, char *strAppKey, char *strDefaultIcon, char *strDescribe);

BOOL CMCADApp::InitInstance()
{
    HANDLE hMutex = CreateMutex(NULL, FALSE, "YHZ_MUTEX");
    // 如果本程序已有实例在运行，则GetLastError()返回ERROR_ALREADY_EXISTS
    if(GetLastError() == ERROR_ALREADY_EXISTS)
    {
        CloseHandle(hMutex); //关闭互斥体对象句柄
        //获取桌面窗口的一个子窗口。
        HWND hWndPrev =::GetWindow(::GetDesktopWindow(), GW_CHILD);
        // 枚举当前窗口，查找本程序已运行的实例
        while(::IsWindow(hWndPrev))
        {
            //如有，则是要本程序已运行实例的窗口，并将它激活。
//            if(::GetProp(hWndPrev, AfxGetApp()->m_pszExeName))
            if(::GetProp(hWndPrev, "MCAD_BY_YHZ"))
            {
                //如果主窗口已最小化，则恢复其大小。
                if (::IsIconic(hWndPrev))
                    ::ShowWindow(hWndPrev, SW_RESTORE);
				::ShowWindow(hWndPrev,SW_SHOW);
                //激活应用程序的主窗口
                ::SetForegroundWindow(hWndPrev);
                return FALSE; //退出本实例
            }
            if(::GetProp(hWndPrev, "MCC_BY_YHZ"))
            {
				AfxMessageBox("关闭切割中心才能运行本程序");
                return FALSE; //退出本实例
            }
            //继续寻找下一个窗口。
            hWndPrev = ::GetWindow(hWndPrev, GW_HWNDNEXT);
        }
        return FALSE; //退出本实例
    }	

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif


	ReadSettings(); //从ini文件中读取系统设置
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	if(FALSE == CheckFileRelation(".mprj2","模版工程"))
	{
//		AfxMessageBox("mprj File not associate");
		char strAppName[MAX_PATH + 1];
		char strDefaultIcon[MAX_PATH + 1];
		sprintf(strAppName,"%s", gSet.m_strAppName.GetBuffer(0));
		sprintf(strDefaultIcon,"%s,1", gSet.m_strAppName.GetBuffer(0));
	    RegisterFileRelation(".mprj2", strAppName, "MCADII.exe", strDefaultIcon, "模版工程文件");
	}

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CMCADDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CMCADView));
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// Dispatch commands specified on the command line
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.
//	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->ShowWindow(SW_SHOWMAXIMIZED);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About
#include "HyperLink.h"
class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	CHyperLink	m_hyperLinkWebSite;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	DDX_Control(pDX, ID_HyperLinkWebSite, m_hyperLinkWebSite);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CMCADApp::OnAppAbout()
{
#if (CUSTOMER == 0)
	CAboutDlg aboutDlg;
#else
	CAboutAll aboutDlg;
#endif
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CMCADApp message handlers

void CMCADApp::WriteSettings()
{
	WriteProfileInt(_T("Para"), _T("X向显示大小"), (int)gSet.getDispSizeX());
	WriteProfileInt(_T("Para"), _T("Y向显示大小"), (int)gSet.getDispSizeY());
	WriteProfileInt(_T("Para"), _T("模板X向起点微调"), (int)gSet.getWorkStartX());
	WriteProfileInt(_T("Para"), _T("模板Y向起点微调"), (int)gSet.getWorkStartY());

	WriteProfileString(_T("Para"), _T("注册码"), gSet.getAuthCode());

	WriteProfileInt(_T("Para"), _T("DirllerDimension"), (int)gSet.getDrillerDimension());//铣刀直径
	WriteProfileInt(_T("Para"), _T("BreakAngle"), (int)gSet.getBreakAngle());
	
	WriteProfileInt(_T("Para"), _T("输出绘图笔号"), (int)gSet.getPltExportPenNum());
	WriteProfileInt(_T("Para"), _T("输出切割笔号"), (int)gSet.getPltExportCutNum());
	WriteProfileInt(_T("Para"), _T("输出钻孔笔号"), (int)gSet.getPltExportDrillNum());

	WriteProfileInt(_T("Para"), _T("先切所有内线"), (int)gSet.getCutAllInnerLineFirst());

	DeleteFile(gSet.m_strFonHz);
	DeleteFile(gSet.m_strFonEn);
	DeleteDirectory(gSet.m_strTempDir.GetBuffer(0));
}

void CMCADApp::ReadSettings()
{
	gSet.getDirectorys();
	
	free((void*)m_pszProfileName);
	m_pszProfileName = _tcsdup(gSet.m_strAppDir + "setting.ini");
	
	gSet.setDispSizeX(GetProfileInt(_T("Para"), _T("X向显示大小"),1200));
	gSet.setDispSizeY(GetProfileInt(_T("Para"), _T("Y向显示大小"),900));
	gSet.setWorkStartX(GetProfileInt(_T("Para"), _T("模板X向起点微调"),0));
	gSet.setWorkStartY(GetProfileInt(_T("Para"), _T("模板Y向起点微调"),0));

	gSet.setAuthCode(GetProfileString(_T("Para"), _T("注册码")));

	gSet.setDrillerDimension(GetProfileInt(_T("Para"), _T("DirllerDimension"),300));
	gSet.setBreakAngle(GetProfileInt(_T("Para"), _T("BreakAngle"),15));

	gSet.setPltExportPenNum(GetProfileInt(_T("Para"), _T("输出绘图笔号"),1));
	gSet.setPltExportCutNum(GetProfileInt(_T("Para"), _T("输出切割笔号"),2));
	gSet.setPltExportDrillNum(GetProfileInt(_T("Para"), _T("输出钻孔笔号"),2));

	gSet.setCutAllInnerLineFirst(GetProfileInt(_T("Para"), _T("先切所有内线"),0));
	gSet.setInflateDist((double)gSet.getDrillerDimension()*0.005);
	gSet.setShrinkDist((double)gSet.getDrillerDimension()*0.005);
}

int CMCADApp::ExitInstance() 
{
	gSet.setAppExitFlag(TRUE);
	WriteSettings();
	Sleep(800);
	return CWinApp::ExitInstance();
}

BOOL CMCADApp::DeleteDirectory(char *psDirName)
{
  CFileFind tempFind; 
    char sTempFileFind[ _MAX_PATH ] = { 0 }; 
    sprintf(sTempFileFind,"%s//*.*",psDirName); 
    BOOL IsFinded = tempFind.FindFile(sTempFileFind); 
    while (IsFinded) 
    { 
        IsFinded = tempFind.FindNextFile(); 
        if (!tempFind.IsDots())
        { 
            char sFoundFileName[ _MAX_PATH ] = { 0 }; 
            strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 
            if (tempFind.IsDirectory()) 
            { 
                char sTempDir[ _MAX_PATH ] = { 0 }; 
                sprintf(sTempDir,"%s//%s",psDirName,sFoundFileName); 
                DeleteDirectory(sTempDir); 
            } 
            else 
            { 
                char sTempFileName[ _MAX_PATH ] = { 0 };
                sprintf(sTempFileName,"%s//%s",psDirName,sFoundFileName); 
                DeleteFile(sTempFileName); 
            } 
        } 
    } 
    tempFind.Close(); 
    if(!RemoveDirectory(psDirName)) 
    { 
        return FALSE; 
    } 
    return TRUE; 
}

/****************************************************
* 检测文件关联情况
* strExt: 要检测的扩展名(例如: ".txt")
* strAppKey: ExeName扩展名在注册表中的键值(例如: "txtfile")
* 返回TRUE: 表示已关联，FALSE: 表示未关联

******************************************************/

BOOL CheckFileRelation(const char *strExt, const char *strAppKey)
{
    int nRet=FALSE;
    HKEY hExtKey;
    char szPath[_MAX_PATH]; 
    DWORD dwSize=sizeof(szPath); 
    if(RegOpenKey(HKEY_CLASSES_ROOT,strExt,&hExtKey)==ERROR_SUCCESS)
    {
        RegQueryValueEx(hExtKey,NULL,NULL,NULL,(LPBYTE)szPath,&dwSize);
        if(_stricmp(szPath,strAppKey)==0)
        {
            nRet=TRUE;
        }
        RegCloseKey(hExtKey);
        return nRet;
    }
    return nRet;
}

void RegisterFileRelation(char *strExt, char *strAppName, char *strAppKey, char *strDefaultIcon, char *strDescribe)
{
    char strTemp[_MAX_PATH];
    HKEY hKey;
    RegCreateKey(HKEY_CLASSES_ROOT,strExt,&hKey);
    RegSetValue(hKey,"",REG_SZ,strAppKey,strlen(strAppKey)+1);
    RegCloseKey(hKey);   
    RegCreateKey(HKEY_CLASSES_ROOT,strAppKey,&hKey);
    RegSetValue(hKey,"",REG_SZ,strDescribe,strlen(strDescribe)+1);
    RegCloseKey(hKey);
    sprintf(strTemp,"%s\\DefaultIcon",strAppKey);
    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    RegSetValue(hKey,"",REG_SZ,strDefaultIcon,strlen(strDefaultIcon)+1);
    RegCloseKey(hKey);
    sprintf(strTemp,"%s\\Shell",strAppKey);
    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    RegSetValue(hKey,"",REG_SZ,"Open",strlen("Open")+1);
    RegCloseKey(hKey);
    sprintf(strTemp,"%s\\Shell\\Open\\Command",strAppKey);
    RegCreateKey(HKEY_CLASSES_ROOT,strTemp,&hKey);
    sprintf(strTemp,"%s \"%%1\"",strAppName);
    RegSetValue(hKey,"",REG_SZ,strTemp,strlen(strTemp)+1);
    RegCloseKey(hKey);
	
}


BOOL CMCADApp::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;

	return CWinApp::PreTranslateMessage(pMsg);
}
