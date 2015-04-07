#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterCacheListsDlg.h"
#include "CFilterCacheLists.h"
#include "CFilterIgnoredCachesDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterCacheListsDlg::CFilterCacheListsDlg(CWnd* pParent) : CNonFSDialog(CFilterCacheListsDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterCacheListsDlg)
	m_ExclusiveShow = -1;
	m_HideFound = FALSE;
	m_ShowIgnored = FALSE;
	m_UseShowOnly = FALSE;
	m_HideDisabled = FALSE;
	//}}AFX_DATA_INIT

	m_pCacheLists = 0;
}

void CFilterCacheListsDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterCacheListsDlg)
	DDX_Control(pDX, IDC_EXCLUSIVE_OPTIONS, m_ExclusiveShowCtrl);
	DDX_Radio(pDX, IDC_EXCLUSIVE_OPTIONS, m_ExclusiveShow);
	DDX_Check(pDX, IDC_HIDE_FOUND, m_HideFound);
	DDX_Check(pDX, IDC_HIDE_IGNORED, m_ShowIgnored);
	DDX_Check(pDX, IDC_SHOW_ONLY, m_UseShowOnly);
	DDX_Check(pDX, IDC_HIDE_DISABLED, m_HideDisabled);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterCacheListsDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterCacheListsDlg)
	ON_BN_CLICKED(IDC_EDIT_IGNORED, OnEditIgnored)
	ON_BN_CLICKED(IDC_SHOW_ONLY, OnShowOnly)
	ON_BN_CLICKED(IDC_HIDE_FOUND, OnHideFound)
	ON_BN_CLICKED(IDC_HIDE_IGNORED, OnHideIgnored)
	ON_BN_CLICKED(IDC_HIDE_DISABLED, OnHideDisabled)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterCacheListsDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	m_ExclusiveShow = m_pCacheLists->m_ExclusiveOptions;
	m_HideFound = m_pCacheLists->m_HideFoundCaches;
	m_HideDisabled = m_pCacheLists->m_HideDisabledCaches;
	m_ShowIgnored = m_pCacheLists->m_ShowIgnoredCaches;
	m_UseShowOnly = m_pCacheLists->m_UseShowOnly;
	
	UpdateData(false);

	OnShowOnly();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterCacheListsDlg::OnEditIgnored() 
{
	CFilterIgnoredCachesDlg	Dlg;

	Dlg.DoModal();
}

void CFilterCacheListsDlg::OnShowOnly() 
{
	UpdateData(true);

	CWnd* pCheck = GetDlgItem(IDC_EXCLUSIVE_OPTIONS2);

	if (m_UseShowOnly == TRUE)
	{
		m_HideFound = FALSE;
		m_ShowIgnored = FALSE;
		m_HideDisabled = FALSE;

		m_ExclusiveShowCtrl.EnableWindow(true);
		pCheck->EnableWindow(true);

		UpdateData(false);
	}
	else
	{
		m_ExclusiveShowCtrl.EnableWindow(false);
		pCheck->EnableWindow(false);
	}
}

void CFilterCacheListsDlg::OnCancel() 
{
	UpdateData(true);

	m_pCacheLists->m_ExclusiveOptions = m_ExclusiveShow;
	m_pCacheLists->m_HideFoundCaches = m_HideFound;
	m_pCacheLists->m_HideDisabledCaches = m_HideDisabled;
	m_pCacheLists->m_ShowIgnoredCaches = m_ShowIgnored;
	m_pCacheLists->m_UseShowOnly = m_UseShowOnly;
	
	CNonFSDialog::OnCancel();
}


void CFilterCacheListsDlg::OnHideFound() 
{
	OnHideIgnored();
}

void CFilterCacheListsDlg::OnHideIgnored() 
{
	UpdateData(true);

	m_UseShowOnly = FALSE;

	CWnd* pCheck = GetDlgItem(IDC_EXCLUSIVE_OPTIONS2);

	m_ExclusiveShowCtrl.EnableWindow(false);
	pCheck->EnableWindow(false);

	UpdateData(false);
}

void CFilterCacheListsDlg::OnHideDisabled() 
{
	OnHideIgnored();
}
