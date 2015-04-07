#include "stdafx.h"
#include "GpxSonar.h"
#include "MainFrm.h"
#include "GpxSonarDoc.h"
#include "GpxSonarView.h"
#include "CAboutDlg.h"
#include "CPath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

BEGIN_MESSAGE_MAP(CGpxSonarApp, CWinApp)
	//{{AFX_MSG_MAP(CGpxSonarApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_LOAD_GPX, OnLoadGpx)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CGpxSonarApp::CGpxSonarApp() : CWinApp()
{
	m_pCachePageWriter = 0;
	m_pFilterMgr = 0;
	m_pGpxParser = 0;
	m_pCurrCache = 0;
	m_pTBMgr = 0;
	m_pCacheMgr = 0;
	m_pFieldNoteMgr = 0;
}

CGpxSonarApp theApp;

BOOL CGpxSonarApp::InitInstance()
{
	CCommandLineInfo cmdInfo;
	
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;

	// Standard initialization
	
	// Change the registry key under which our settings are stored.
	//SetRegistryKey(_T("Local AppWizard-Generated Applications"));

	// Register document templates
	m_pDocTemplate = new CCeDocListDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGpxSonarDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CGpxSonarView),
		CString(_T("GPX Files|*.zip")),
		CString(_T("All Folders")) ); 

	AddDocTemplate(m_pDocTemplate);

	CPath	Path;
	String	GpxFile;
	
	GpxFile = Path.BuildPath(DEFAULT_AUTOLOAD_FILE);
	
	// Check if a default file needs to be opened
	if (GetFileAttributes(GpxFile.c_str()) != -1)
	{
		CFile F;

		if (F.Open(GpxFile.c_str(), CFile::modeRead, NULL))
		{
			TCHAR FilenameBuffer[_MAX_PATH];

			ZeroMemory(FilenameBuffer, sizeof(FilenameBuffer));

			try
			{
				// Read the name of the Gpx file to process
				F.Read((void*) &FilenameBuffer, sizeof(FilenameBuffer) - 1);

				// Check that the file exists before constructing a command line
				if (GetFileAttributes(FilenameBuffer) != -1)
				{
					cmdInfo.m_nShellCommand = CCommandLineInfo::FileOpen;
					cmdInfo.m_strFileName = FilenameBuffer;
				}
			}
			catch(...)
			{
				cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
			}
		}

		F.Close();
	}
	else
	{
		ParseCommandLine(cmdInfo);
	}

	// Dispatch commands specified on the command line
	BOOL bResult = ProcessShellCommand(cmdInfo);

	if (!bResult)
	{
		m_pDocTemplate->ShowDocList();
	
		// if there is no file in the doclist, we will create a new one.
		CCeDocList* pDL = m_pDocTemplate->m_pWndDocList;

		if (pDL->GetItemCount() == 0) 
		{
			pDL->OnClose();

			OnFileOpen();
		}
	}

	return TRUE;
}

// App command to run the dialog
void CGpxSonarApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

void CGpxSonarApp::OnFileNew() 
{
	m_pDocTemplate->ShowDocList();

	// if there is no file in the doclist, we will create a new one.
	CCeDocList* pDL = m_pDocTemplate->m_pWndDocList;

	if (pDL->GetItemCount() == 0) 
	{
		pDL->OnClose();

		OnFileOpen();
	}
}

void CGpxSonarApp::OnLoadGpx() 
{
	m_pDocTemplate->CloseAllDocuments(FALSE);
	CGpxSonarApp::OnFileNew();
}
