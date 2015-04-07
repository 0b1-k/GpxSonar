#include "stdafx.h"
#include "GpxSonar.h"
#include "CFilterCacheTypesDlg.h"
#include "CFilterCacheTypes.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CFilterCacheTypesDlg::CFilterCacheTypesDlg(CWnd* pParent /*=NULL*/)
	: CNonFSDialog(CFilterCacheTypesDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFilterCacheTypesDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pTypes = 0;
}

CFilterCacheTypesDlg::~CFilterCacheTypesDlg()
{
	m_ImageList.DeleteImageList();
}

void CFilterCacheTypesDlg::DoDataExchange(CDataExchange* pDX)
{
	CNonFSDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFilterCacheTypesDlg)
	DDX_Control(pDX, IDC_TREE, m_Tree);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFilterCacheTypesDlg, CNonFSDialog)
	//{{AFX_MSG_MAP(CFilterCacheTypesDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CFilterCacheTypesDlg::OnInitDialog() 
{
	CNonFSDialog::OnInitDialog();
	
	#define	CACHE_ICON_X		16
	#define	IMAGELIST_GROW_BY	1
	#define	FUCIA				RGB(255,0,255)

	// Setup the icon list for the list control
	if (m_ImageList.Create(IDB_CACHES, CACHE_ICON_X, IMAGELIST_GROW_BY, FUCIA))
	{
		m_Tree.SetImageList(&m_ImageList, TVSIL_NORMAL);
		
		for (itFiltCacheTypes it = m_pTypes->m_Types.begin(); it != m_pTypes->m_Types.end(); it++)
		{
			CFiltCacheTypes* pFCT = *it;

			HTREEITEM Item = InsertItem(m_Tree, TVI_ROOT, Lookup(pFCT->m_Type), (void*) pFCT, (int) pFCT->m_Type);

			m_Tree.SetCheck(Item, pFCT->m_Enabled);
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

HTREEITEM CFilterCacheTypesDlg::InsertItem(CTreeCtrl& Tree, HTREEITEM ParentItem, const TCHAR* pText, void* pData, int Image)
{
	TVINSERTSTRUCT tvInsert;
	tvInsert.hParent = ParentItem;
	tvInsert.hInsertAfter = TVI_SORT;
	tvInsert.item.mask = TVIF_TEXT | TVIF_PARAM | TVIF_IMAGE | TVIF_SELECTEDIMAGE;
	tvInsert.item.pszText = (TCHAR*) pText;
	tvInsert.item.lParam = (LPARAM) pData;
	tvInsert.item.iImage = Image;
	tvInsert.item.iSelectedImage = Image;

	return Tree.InsertItem(&tvInsert);
}

const TCHAR* CFilterCacheTypesDlg::Lookup(GcType Type)
{
	switch(Type)
	{
	case GT_Traditional:
		return TYPE_TRADITIONAL;
		break;
	case GT_Multi:
		return TYPE_MULTI_DISP; // User friendlier name
		break;
	case GT_Virtual:
		return TYPE_VIRTUAL;
		break;
	case GT_Webcam:
		return TYPE_WEBCAM;
		break;
	case GT_Unknown:
		return TYPE_UNKNOWN_DISP; // User friendlier name
		break;
	case GT_LetterboxHybrid:
		return TYPE_LETTERBOXHYBRID_DISP; // User friendlier name
		break;
	case GT_Event:
		return TYPE_EVENT_DISP; // User friendlier name
		break;
	case GT_ProjectAPE:
		return TYPE_PROJECTAPE;
		break;
	case GT_Locationless:
		return TYPE_LOCATIONLESS;
		break;
	case GT_CITO:
		return TYPE_CITO;
		break;
	case GT_Earthcache:
		return TYPE_EARTHCACHE;
		break;
	}

	return _T("?");
}

void CFilterCacheTypesDlg::OnCancel() 
{
	HTREEITEM Item = m_Tree.GetRootItem();

	while (Item)
	{
		CFiltCacheTypes* pFCT = (CFiltCacheTypes*) m_Tree.GetItemData(Item);

		pFCT->m_Enabled = (bool) m_Tree.GetCheck(Item);

		Item = m_Tree.GetNextItem(Item, TVGN_NEXT);
	}
	
	CNonFSDialog::OnCancel();
}
