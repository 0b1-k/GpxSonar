#include "stdafx.h"
#include "GpxSonar.h"
#include "CMyCachesDlg.h"
#include "CCacheMgr.h"
#include "Literals.h"
#include "CCachePropertiesDlg.h"
#include "CCacheMgr.h"
#include "CCacheImportDlg.h"
#include "CFieldNoteMgr.h"
#include "CCacheCategoryEditor.h"
#include "CExportLocationMgr.h"
#include "CExportLocationDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMyCachesDlg::CMyCachesDlg(CWnd* pParent): CNonFSDialog(CMyCachesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMyCachesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pExpLocMgr = 0;
	m_pCacheMgr = 0;
	m_CurrSortCol = 0;
	m_NeedToSaveChanges = false;
}

CMyCachesDlg::~CMyCachesDlg()
{
	m_ImageList.DeleteImageList();

	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		delete (*I);
	}

	m_Headings.clear();
}

void CMyCachesDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMyCachesDlg)
	DDX_Control(pDX, IDC_COMMAND, m_CommandList);
	DDX_Control(pDX, IDC_CATEGORY, m_Categories);
	DDX_Control(pDX, IDC_CACHES, m_CacheList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMyCachesDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CMyCachesDlg)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_CACHES, OnColumnclickCaches)
	ON_NOTIFY(NM_DBLCLK, IDC_CACHES, OnDblclkCaches)
	ON_BN_CLICKED(IDC_CAT_EDIT, OnCatEdit)
	ON_CBN_SELCHANGE(IDC_CATEGORY, OnSelchangeCategory)
	ON_BN_CLICKED(IDC_GO, OnGo)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CMyCachesDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	// Default column widths expressed in pixels
	#define CACHE_NAME_COL_WIDTH 240
	
	m_Headings.push_back(new CHeading(0, _T("Name"), CACHE_NAME_COL_WIDTH, CompareName));

	m_CacheList.SetExtendedStyle(m_CacheList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_SUBITEMIMAGES);

	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	m_ImageList.Create(IDB_CACHES, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA);
	m_CacheList.SetImageList(&m_ImageList, LVSIL_SMALL);

	int Col = 0;

	// Add the headers to the list control
	for (itHeading I = m_Headings.begin(); I != m_Headings.end(); I++)
	{
		m_CacheList.InsertColumn(Col++, (*I)->m_Name.c_str(), LVCFMT_LEFT, (*I)->m_Length);
	}

	m_CommandList.SetCurSel(0);

	EnableCategories();

	UpdateCacheList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

// Enable / disable the cache category selection if the list of selections is empty
void CMyCachesDlg::EnableCategories()
{
	m_Categories.ResetContent();

	for (itStr S = m_pCacheMgr->m_CacheCategories.begin(); S != m_pCacheMgr->m_CacheCategories.end(); S++)
	{
		m_Categories.AddString((*S).c_str());
	}

	m_Categories.SetCurSel(0);

	m_Categories.EnableWindow(true);
}

void CMyCachesDlg::UpdateCacheList()
{
	LVITEM	Item;
	int		Count = 0;
	CString	Cat;

	// Retrieve the category
	m_Categories.GetLBText(m_Categories.GetCurSel(), Cat);

	m_CacheList.DeleteAllItems();

	for (itGC C = m_pCacheMgr->m_Caches.begin(); C != m_pCacheMgr->m_Caches.end(); C++)
	{
		CGeoCache* pCache = *C;

		bool Proceed = true;
		
		// Compare the category of the cache against the currently selected category
		if (Cat != pCache->m_Category.c_str())
		{
			// No match! Don't insert the cache in the list
			Proceed = false;

			// Since the cache is not visible, take it out of the scope
			pCache->m_InScope = false;
		}

		if (Proceed)
		{
			pCache->m_InScope = true;

			// Image of cache type & cache name
			Item.mask =  LVIF_IMAGE | LVIF_TEXT;
			Item.iItem = Count;
			Item.iSubItem = 0;
			Item.iImage = pCache->TypeLookup();
			Item.pszText = (TCHAR*) pCache->m_GsCacheName.c_str();

			// Preserve the index of the cache object in the list
			pCache->m_ListIndex = m_CacheList.InsertItem(&Item);

			m_CacheList.SetItemData(pCache->m_ListIndex, (DWORD) pCache);

			Count++;
		}
	}
}

int CMyCachesDlg::ShowCacheProperties(CGeoCache* pGc)
{
	CCachePropertiesDlg	Dlg;

	Dlg.m_pGc = pGc;

	Dlg.DoModal();

	return IDOK;
}

CGeoCache* CMyCachesDlg::GetSelectedItem()
{
	POSITION Pos = m_CacheList.GetFirstSelectedItemPosition();

	long Item = m_CacheList.GetNextSelectedItem(Pos);

	if (Item != -1)
	{
		return (CGeoCache*) m_CacheList.GetItemData(Item);
	}

	return 0;
}

void CMyCachesDlg::OnDblclkCaches(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnEdit();
	
	*pResult = 0;
}

int CALLBACK CMyCachesDlg::CompareName(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CMyCachesDlg* pThis = (CMyCachesDlg*) lParamSort;
	CGeoCache* pGC1 = (CGeoCache*) lParam1;
	CGeoCache* pGC2 = (CGeoCache*) lParam2;
	
	if (pGC1->m_GsCacheName < pGC2->m_GsCacheName)
	{
		return (pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle * -1);
	}
	else if (pGC1->m_GsCacheName > pGC2->m_GsCacheName)
	{
		return pThis->m_Headings[pThis->m_CurrSortCol]->m_SortToggle;
	}
	
	return 0;
}

void CMyCachesDlg::OnColumnclickCaches(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	m_CurrSortCol = pNMListView->iSubItem;

	m_Headings[m_CurrSortCol]->m_SortToggle *= -1;

	m_CacheList.SortItems(m_Headings[m_CurrSortCol]->m_pSortFunc, (LPARAM) this);

	*pResult = 0;
}

void CMyCachesDlg::OnCatEdit() 
{
	CCacheCategoryEditor	Dlg;

	Dlg.DoModal();

	EnableCategories();
}

void CMyCachesDlg::OnSelchangeCategory() 
{
	UpdateCacheList();
}

void CMyCachesDlg::OnGo() 
{
	int Sel = m_CommandList.GetCurSel();

	if (Sel != LB_ERR)
	{
		CString	Cmd;

		m_CommandList.GetLBText(Sel, Cmd);

		if (Cmd.Find(_T("Create")) != -1)
		{
			OnNew();
		}
		else if (Cmd.Find(_T("Delete")) != -1)
		{
			OnDelete();
		}
		else if (Cmd.Find(_T("Edit")) != -1)
		{
			OnEdit();
		}
		else if (Cmd.Find(_T("Export")) != -1)
		{
			OnExport();
		}
		else // Import
		{
			OnImport();
		}
	}
}

void CMyCachesDlg::OnNew() 
{
	CGeoCache* pGc = m_pCacheMgr->New(m_Owner);

	if (!pGc)
	{
		return;
	}

	if (ShowCacheProperties(pGc) == IDOK)
	{
		m_NeedToSaveChanges = true;

		UpdateCacheList();
	}
	else
	{
		m_pCacheMgr->Delete(pGc);
	}
}

void CMyCachesDlg::OnEdit() 
{
	CGeoCache* pGc = GetSelectedItem();

	if (pGc && ShowCacheProperties(pGc) == IDOK)
	{
		m_NeedToSaveChanges = true;

		UpdateCacheList();
	}
}

void CMyCachesDlg::OnDelete() 
{
	CGeoCache* pGc = GetSelectedItem();

	if (pGc)
	{
		if (MessageBox( _T("Do you really want to\r\ndelete this cache?"), _T("Cache Obliterator"), MB_OKCANCEL | MB_ICONWARNING ) == IDOK)
		{
			m_pCacheMgr->Delete(pGc);

			UpdateCacheList();

			m_NeedToSaveChanges = true;
		}
	}
}

void CMyCachesDlg::OnImport() 
{
	CCacheImportDlg	Dlg;

	Dlg.m_Owner = m_Owner;

	if (Dlg.DoModal() == IDOK)
	{
		if (m_pCacheMgr->IsAlreadyDefined(Dlg.m_pCache))
		{
			CString	Msg;

			Msg.Format(_T("'%s' already exists in your cache list."), Dlg.m_pCache->m_GsCacheName.c_str());

			MessageBox(Msg, _T("Cache Already Imported"), MB_OK | MB_ICONINFORMATION);
			
			return;
		}

		CGeoCache* pGc = m_pCacheMgr->New(m_Owner);

		if (!pGc)
		{
			return;
		}

		CGpxParser* pParser = (CGpxParser*) ((CGpxSonarApp*)AfxGetApp())->m_pGpxParser;

		m_pCacheMgr->Copy(pParser, pGc, Dlg.m_pCache);

		m_NeedToSaveChanges = true;

		CString	Cat;

		// Retrieve the category
		m_Categories.GetLBText(m_Categories.GetCurSel(), Cat);

		pGc->m_Category = (LPCTSTR) Cat;

		UpdateCacheList();
	}
}

void CMyCachesDlg::OnExport() 
{
	CGpxParser*		pParser = (CGpxParser*) ((CGpxSonarApp*)AfxGetApp())->m_pGpxParser;
	CFieldNoteMgr*	pFieldNoteMgr = (CFieldNoteMgr*) ((CGpxSonarApp*)AfxGetApp())->m_pFieldNoteMgr;

	GCCont* pOld = pParser->SetCacheContainer(&m_pCacheMgr->m_Caches);

	CExportLocationDlg	Dlg;

	Dlg.m_pExpLocMgr = m_pExpLocMgr;
	Dlg.m_pExpLocMgr->SetGpxParser(pParser);
	Dlg.m_pExpLocMgr->SetFieldNoteMgr(pFieldNoteMgr);

	Dlg.DoModal();

	pParser->SetCacheContainer(pOld);
}
