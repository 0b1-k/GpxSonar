#include "stdafx.h"
#include "GpxSonar.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define PSPC_TOOLBAR_HEIGHT 24

const DWORD dwAdornmentFlags = 0; // exit button

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_NOTIFY(DLN_CE_CREATE, AFXCE_ID_DOCLIST, OnCreateDocList)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	m_wndCommandBar.m_bShowSharedNewButton = FALSE;

	/*
	m_ToolTipsTable[0] = MakeString(IDS_NEW);
	m_ToolTipsTable[1] = MakeString(IDS_FILE);
	m_ToolTipsTable[2] = MakeString(IDS_MHELP);
	m_ToolTipsTable[3] = MakeString(IDS_CUT);
	m_ToolTipsTable[4] = MakeString(IDS_COPY);
	m_ToolTipsTable[5] = MakeString(IDS_PASTE);
	*/

	if(!m_wndCommandBar.Create(this) ||
	   !m_wndCommandBar.InsertMenuBar(IDR_MAINFRAME) ||
	   !m_wndCommandBar.AddAdornments() ||
	   !m_wndCommandBar.LoadToolBar(IDR_MAINFRAME) /* ||
		!m_wndCommandBar.SendMessage(TB_SETTOOLTIPS, (WPARAM)(7), (LPARAM)(m_ToolTipsTable))*/)
	{
		TRACE0("Failed to create CommandBar\n");
		return -1;      // fail to create
	}

	m_wndCommandBar.SetBarStyle(m_wndCommandBar.GetBarStyle() |
		/*CBRS_TOOLTIPS | CBRS_FLYBY |*/ CBRS_SIZE_FIXED);

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


void CMainFrame::OnCreateDocList(DLNHDR* pNotifyStruct, LRESULT* result)
{
	CCeDocList* pDocList = (CCeDocList*)FromHandle(pNotifyStruct->nmhdr.hwndFrom);
	ASSERT_KINDOF(CCeDocList, pDocList);

	CCeCommandBar* pDocListCB = pDocList->GetCommandBar();
	ASSERT(pDocListCB != NULL);

	pDocListCB->InsertMenuBar(IDM_DOCLIST);
	pDocListCB->SendMessage(TB_SETTOOLTIPS, (WPARAM)(1), (LPARAM)(m_ToolTipsTable));
	CFrameWnd::OnCreateDocList(pNotifyStruct, result);	
}


void CMainFrame::OnDestroy()
{
	for(int i = 0; i < NUM_TOOL_TIPS; i++)
		delete m_ToolTipsTable[i];

	CFrameWnd::OnDestroy();
}

LPTSTR CMainFrame::MakeString(UINT stringID)
{
	TCHAR buffer[255];
	TCHAR* theString;

	::LoadString(AfxGetInstanceHandle(), stringID, buffer, 255);
	theString = new TCHAR[lstrlen(buffer) + 1];
	lstrcpy(theString, buffer);
	return theString;
}   

 #ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG
