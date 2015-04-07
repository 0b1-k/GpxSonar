#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterIgnoredCachesDlg.h"
#include "CGpxParser.h"
#include "CFilterMgr.h"
#include "CFilterCacheLists.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterIgnoredCachesDlg::CFilterIgnoredCachesDlg(CWnd* pParent) : CNonFSDialog(CFilterIgnoredCachesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterIgnoredCachesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CFilterIgnoredCachesDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterIgnoredCachesDlg)
	DDX_Control(pDX, IDC_LIST, m_List);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterIgnoredCachesDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterIgnoredCachesDlg)
	ON_BN_CLICKED(IDC_UNIGNORE, OnUnignore)
	ON_BN_CLICKED(IDC_UNIGNOREALL, OnUnignoreall)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterIgnoredCachesDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	CGpxParser* pGP = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;
	
	itGC	C;
	CString Name;

	CGeoCache* pGc = pGP->First(C);

	while (!pGP->EndOfCacheList(C))
	{
		if (pGc->m_Ignored)
		{
			Name = _T("(");
			Name += pGc->m_Shortname.c_str();
			Name += _T(") ");
			Name += pGc->m_GsCacheName.c_str();

			int Item = m_List.AddString(Name);

			m_List.SetItemDataPtr(Item, (void*) pGc);
		}

		pGc = pGP->Next(C);
	}

	m_List.SetCurSel(0);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterIgnoredCachesDlg::OnUnignore() 
{
	int Item = m_List.GetCurSel();
	
	if (Item == LB_ERR)
	{
		return;
	}

	CFilterMgr* pFiltMgr = (CFilterMgr*) ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;
	CFilterCacheLists* pFCL = (CFilterCacheLists*) pFiltMgr->Find(FilterCacheLists);

	CGeoCache* pGc = (CGeoCache*) m_List.GetItemDataPtr(Item);

	pFCL->UnIgnore(pGc);

	m_List.DeleteString(Item);

	m_List.SetCurSel(0);
}

void CFilterIgnoredCachesDlg::OnUnignoreall() 
{
	CGpxParser* pGP = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;
	CFilterMgr* pFiltMgr = (CFilterMgr*) ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;
	CFilterCacheLists* pFCL = (CFilterCacheLists*) pFiltMgr->Find(FilterCacheLists);
	
	itGC	C;

	CGeoCache* pGc = pGP->First(C);

	while (!pGP->EndOfCacheList(C))
	{
		if (pGc->m_Ignored)
		{
			pFCL->UnIgnore(pGc);
		}

		pGc = pGP->Next(C);
	}

	m_List.ResetContent();

	CNonFSDialog::OnCancel();
}
