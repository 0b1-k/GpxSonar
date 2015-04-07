#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterMgrDlg.h"
#include "CFilterContainersDlg.h"
#include "CFilterCacheTypesDlg.h"
#include "CFilterBearingDistanceDlg.h"
#include "CFilterCacheListsDlg.h"
#include "CFilterStringsDlg.h"
#include "CFilterRatingsDlg.h"
#include "CFilterOnStrings.h"
#include "CSearchDlg.h"
#include "CGpxParser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterMgrDlg::CFilterMgrDlg(CWnd* pParent) : CNonFSDialog(CFilterMgrDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterMgrDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFilterMgrDlg::~CFilterMgrDlg()
{
	m_ImageList.DeleteImageList();
}

void CFilterMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterMgrDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterMgrDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterMgrDlg)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE, OnDblclkTree)
	ON_BN_CLICKED(IDC_CONFIG, OnConfig)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterMgrDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	#define	BMP_FILTERS_HAMMER	0

	// Setup the icon list for the tree control
	m_ImageList.Create(IDB_FILTERS, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_Tree.SetImageList(&m_ImageList, TVSIL_NORMAL);

	// Fill the tree with the filtering options
	SetupFilteringOptions();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFilterMgrDlg::SetupFilteringOptions()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	itFilter	it;
	HTREEITEM	Item;

	CFilterBase* pFilt = pFilterMgr->First(it);
	
	while (!pFilterMgr->EndOfFilters(it))
	{
		Item = InsertFilter(TVI_ROOT, pFilt->GetName().c_str(), pFilt->GetType());
		
		m_Tree.SetCheck(Item, pFilt->IsEnabled());

		pFilt->SetData((void*) Item);

		pFilt = pFilterMgr->Next(it);
	}
}

HTREEITEM CFilterMgrDlg::InsertFilter(HTREEITEM ParentItem, const TCHAR* pText, GcFilter FilterType)
{
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = ParentItem;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = (TCHAR*) pText;
	tvInsert.item.lParam = (LPARAM) FilterType;
	tvInsert.item.iImage = BMP_FILTERS_HAMMER;
	tvInsert.item.iSelectedImage = BMP_FILTERS_HAMMER;

	return m_Tree.InsertItem(&tvInsert);
}

void CFilterMgrDlg::OnDblclkTree(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM Item = m_Tree.GetSelectedItem();

	GcFilter FilterType;
	
	FilterType = (GcFilter) m_Tree.GetItemData(Item);
	
	switch(FilterType)
	{
	case FilterCacheTypes:
		OnCacheTypes();
		break;
	case FilterContainerTypes:
		OnContainerTypes();
		break;
	case FilterBearingDistance:
		OnBearingDistance();
		break;
	case FilterCacheLists:
		OnCacheLists();
		break;
	case FilterStateList:
		OnStateList();
		break;
	case FilterCountryList:
		OnCountryList();
		break;
	case FilterSearch:
		OnSearch();
		break;
	case FilterRatings:
		OnRatings();
		break;
	case FilterTravelBugs:
		MessageBox(_T("This filter has no configurable parameters."), _T("Toggle Filter"), MB_OK | MB_ICONINFORMATION);
		break;
	}

	*pResult = 0;
}

void CFilterMgrDlg::OnCacheTypes()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CFilterCacheTypesDlg Dlg;

	Dlg.m_pTypes = (CFilterCacheTypes*) pFilterMgr->Find(FilterCacheTypes);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnContainerTypes()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CFilterContainersDlg Dlg;

	Dlg.m_pConts = (CFilterCacheContainers*) pFilterMgr->Find(FilterContainerTypes);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnBearingDistance()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CFilterBearingDistanceDlg	Dlg;

	Dlg.m_pBearDist = (CFilterBearingDistance*) pFilterMgr->Find(FilterBearingDistance);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnCacheLists()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CFilterCacheListsDlg	Dlg;

	Dlg.m_pCacheLists = (CFilterCacheLists*) pFilterMgr->Find(FilterCacheLists);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnStateList()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;
	CGpxParser* pGP = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;

	CFilterStringsDlg Dlg;

	Dlg.m_pFiltStrs = (CFilterOnStrings*) pFilterMgr->Find(FilterStateList);

	Dlg.m_pFiltStrs->Update(pGP->m_StateList);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnCountryList()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;
	CGpxParser* pGP = ((CGpxSonarApp*) AfxGetApp())->m_pGpxParser;

	CFilterStringsDlg Dlg;

	Dlg.m_pFiltStrs = (CFilterOnStrings*) pFilterMgr->Find(FilterCountryList);

	Dlg.m_pFiltStrs->Update(pGP->m_CountryList);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnSearch()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CSearchDlg	Dlg;

	Dlg.m_pSearch = (CFilterSearch*) pFilterMgr->Find(FilterSearch);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnRatings()
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	CFilterRatingsDlg Dlg;

	Dlg.m_pRatings = (CFilterCacheRatings*) pFilterMgr->Find(FilterRatings);

	Dlg.DoModal();
}

void CFilterMgrDlg::OnOK() 
{
	CFilterMgr* pFilterMgr = ((CGpxSonarApp*) AfxGetApp())->m_pFilterMgr;

	itFilter	it;

	CFilterBase* pFilt = pFilterMgr->First(it);
	
	while (!pFilterMgr->EndOfFilters(it))
	{		
		HTREEITEM Item = (HTREEITEM) pFilt->GetData();

		pFilt->Enable(m_Tree.GetCheck(Item));

		pFilt = pFilterMgr->Next(it);
	}

	CNonFSDialog::OnOK();
}

void CFilterMgrDlg::OnConfig() 
{
	NMHDR Hdr;
	LRESULT	Res;

	OnDblclkTree(&Hdr, &Res);
}
