// NonFSDialog.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "NonFSDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNonFSDialog dialog

void CNonFSDialog::init()
{
	m_bFullScreen = FALSE;
}

CNonFSDialog::CNonFSDialog()
{
	init();
}

CNonFSDialog::CNonFSDialog(UINT nIDTemplate, CWnd* pParent)
:CDialog(nIDTemplate, pParent)
{
	init();
}

CNonFSDialog::CNonFSDialog(LPCTSTR lpszTemplateName, CWnd* pParent)
:CDialog(lpszTemplateName, pParent)
{
	init();
}

CNonFSDialog::~CNonFSDialog()
{

}

void CNonFSDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNonFSDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNonFSDialog, CDialog)
	//{{AFX_MSG_MAP(CNonFSDialog)
	// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
	ON_WM_ACTIVATE()
	ON_WM_SETTINGCHANGE()

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNonFSDialog message handlers

void CNonFSDialog::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CWnd::OnActivate(nState, pWndOther, bMinimized);
}

void CNonFSDialog::OnSettingChange(UINT uFlags, LPCTSTR lpszSection)
{
	CWnd::OnSettingChange(uFlags, lpszSection);
}